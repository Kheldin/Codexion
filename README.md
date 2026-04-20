*This project has been created as part of the 42 curriculum by \<kacherch\>.*

# Codexion
### Master the race for resources before the deadline masters you

---

## Description

**Codexion** is a concurrency simulation inspired by the classic Dining Philosophers problem, reimagined in a co-working coding environment.

Multiple **coders** sit around a circular table, each needing two **USB dongles** (one on their left, one on their right) to compile their quantum code. Since there are as many dongles as coders, and each compilation requires two dongles simultaneously, coders must compete for shared resources without causing deadlocks, starvation, or burnout.

Each coder cycles through three states:
1. **Compiling** — requires holding two dongles simultaneously
2. **Debugging** — after compiling, dongles are released
3. **Refactoring** — after debugging, then back to attempting to compile

If a coder fails to start compiling within `time_to_burnout` milliseconds since their last compile (or since the simulation started), they **burn out** and the simulation stops. The goal is to keep all coders alive until every coder has compiled at least `number_of_compiles_required` times.

Key features:
- POSIX threads (`pthread`) — one thread per coder + one monitor thread
- Mutex-protected dongles with a cooldown period after each release
- Two arbitration policies: **FIFO** and **EDF** (Earliest Deadline First)
- Priority queue (min-heap) for fair scheduling
- Precise burnout detection (within 10 ms)
- Serialized logging output

---

## Instructions

### Compilation

```bash
cd coders/
make
```

This will produce the `codexion` executable. The Makefile uses `-Wall -Wextra -Werror -pthread`.

Available Makefile rules: `all`, `clean`, `fclean`, `re`.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are **mandatory**. The program rejects invalid inputs (negative numbers, non-integers, unknown scheduler names).

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) |
| `time_to_burnout` | Max ms a coder can go without starting a compile |
| `time_to_compile` | Duration of the compile phase (ms) |
| `time_to_debug` | Duration of the debug phase (ms) |
| `time_to_refactor` | Duration of the refactor phase (ms) |
| `number_of_compiles_required` | Simulation ends when all coders reach this count |
| `dongle_cooldown` | Ms a dongle is unavailable after being released |
| `scheduler` | `fifo` or `edf` |

### Example

```bash
./codexion 4 800 200 200 200 5 50 edf
```

Expected output format:
```
0 1 has taken a dongle
2 1 has taken a dongle
2 1 is compiling
202 1 is debugging
402 1 is refactoring
...
1505 4 burned out
```

---

## Blocking Cases Handled

### Deadlock Prevention

The classic deadlock scenario in this problem occurs when every coder simultaneously picks up their left dongle and waits indefinitely for their right dongle — a circular dependency. This is addressed by:

- **Resource ordering**: dongles are always requested in a consistent global order (lower index first), breaking the circular wait condition (one of Coffman's four necessary conditions for deadlock).
- **Atomic pair acquisition**: a coder only logs "has taken a dongle" and proceeds when both dongles can be acquired safely, preventing partial holds that could cause circular waiting.

Coffman's conditions addressed:
| Condition | Mitigation |
|---|---|
| Mutual exclusion | Necessary — kept, dongles are exclusive |
| Hold and wait | Broken by ordered acquisition strategy |
| No preemption | Kept — coders cannot steal dongles |
| Circular wait | Broken by consistent dongle index ordering |

### Starvation Prevention

Under FIFO scheduling, requests are served strictly in arrival order, preventing any coder from being perpetually skipped.

Under EDF scheduling, the coder with the **earliest burnout deadline** (`last_compile_start + time_to_burnout`) is granted access first. This ensures that coders closest to burning out are prioritized, preventing starvation as long as the parameters are feasible.

### Cooldown Handling

After a coder releases a dongle, a timestamp is recorded. Any subsequent request for that dongle is blocked (via `pthread_cond_timedwait`) until `dongle_cooldown` milliseconds have elapsed since the release. The scheduler's waiting queue is only consulted after the cooldown window passes.

### Precise Burnout Detection

A dedicated **monitor thread** runs continuously throughout the simulation. It periodically checks each coder's `last_compile_start` timestamp against the current time. If the elapsed time exceeds `time_to_burnout`, the monitor immediately prints the burnout message and sets a global stop flag. The polling interval is kept well below 10 ms to satisfy the precision requirement.

### Log Serialization

All output is protected by a dedicated **print mutex** (`pthread_mutex_t`). Any thread (coder or monitor) must acquire this mutex before writing to stdout, and releases it immediately after. This guarantees that no two log lines ever interleave.

---

## Thread Synchronization Mechanisms

### `pthread_mutex_t` — Mutual Exclusion

Each dongle has its own mutex protecting its internal state (available, cooldown timestamp, waiting queue). Before reading or modifying a dongle's state, a thread must acquire the dongle's mutex. A separate global mutex protects the print output.

```c
// Example: protecting dongle state
pthread_mutex_lock(&dongle->mutex);
// ... inspect and modify dongle state ...
pthread_mutex_unlock(&dongle->mutex);
```

### `pthread_cond_t` — Condition Variables

Each dongle also has a condition variable used to wake threads waiting for it. When a coder cannot acquire a dongle (either because it's in use, in cooldown, or a higher-priority coder is waiting), it registers itself in the dongle's waiting queue and calls `pthread_cond_wait` or `pthread_cond_timedwait` (the timed variant is used to allow periodic burnout checks while waiting).

```c
// Coder waits for its turn on a dongle
pthread_mutex_lock(&dongle->mutex);
enqueue_request(dongle, coder);
while (!my_turn(dongle, coder) || in_cooldown(dongle))
    pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &deadline);
pthread_mutex_unlock(&dongle->mutex);
```

When a coder releases a dongle, it calls `pthread_cond_broadcast` to wake all waiters, which then re-evaluate their eligibility under the chosen scheduler.

### Priority Queue (Min-Heap)

A custom min-heap is implemented to manage the waiting queue for each dongle. Under **FIFO**, the key is the arrival timestamp. Under **EDF**, the key is the burnout deadline (`last_compile_start + time_to_burnout`). The heap always places the highest-priority requester at the root, so grant decisions are O(log n).

### Monitor Thread

The monitor thread shares access to coders' state (compile count, last compile timestamp, stop flag) through a dedicated mutex. Coders update their own `last_compile_start` atomically under this mutex. The monitor reads these values to detect burnout without race conditions.

```c
// Monitor checks for burnout
pthread_mutex_lock(&sim->state_mutex);
elapsed = now_ms() - coder->last_compile_start;
if (elapsed > sim->time_to_burnout)
    sim->stop = 1;
pthread_mutex_unlock(&sim->state_mutex);
```

The `stop` flag is also checked by all coder threads at each state transition, ensuring a clean and prompt shutdown.

### Race Condition Prevention Summary

| Shared Resource | Protected By |
|---|---|
| Dongle availability & queue | Per-dongle `pthread_mutex_t` + `pthread_cond_t` |
| Coder compile count & timestamp | Simulation-wide `state_mutex` |
| stdout output | Global `print_mutex` |
| Simulation stop flag | `state_mutex` (written by monitor, read by coders) |

---

## Resources

### References

- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [The Little Book of Semaphores — Allen B. Downey](https://greenteapress.com/wp/semaphores/)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [Coffman Conditions — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Coffman_conditions)
- `man pthread_create`, `man pthread_mutex_init`, `man pthread_cond_wait`, `man gettimeofday`

### AI Usage
readme
