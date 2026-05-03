*This project has been created as part of the 42 curriculum by kacherch.*

# Codexion

### Master the race for resources before the deadline masters you

---

## Description

**Codexion** is a concurrency simulation inspired by the classic Dining Philosophers problem, reimagined in a co-working coding environment.

Multiple **coders** sit around a circular table, each needing two **USB dongles** (one on their left, one on their right) to compile their quantum code. Since there are as many dongles as coders, and each compile requires two dongles simultaneously, coders must compete for shared resources without causing deadlocks, starvation, or burnout.

Each coder cycles through three states:

1. **Compiling** — acquires two dongles, holds them for `time_to_compile` ms, then releases them
2. **Debugging** — releases the dongles and spends `time_to_debug` ms debugging
3. **Refactoring** — spends `time_to_refactor` ms refactoring, then immediately attempts to compile again

If a coder fails to start compiling within `time_to_burnout` milliseconds since their last compile (or since the simulation started), they **burn out** and the simulation stops immediately. The goal is to keep all coders alive until every coder has compiled at least `number_of_compiles_required` times.

Key features:

- One POSIX thread per coder + one dedicated monitor thread
- Per-dongle mutex with cooldown enforcement via `pthread_cond_timedwait`
- Global queue (linked list) for fair arbitration, protected by a dedicated mutex
- Two scheduling policies: **FIFO** and **EDF** (Earliest Deadline First)
- Priority queue (linked list ordered on insertion) for FIFO/EDF dispatch
- Precise burnout detection by the monitor thread (within 10 ms)
- Serialized logging via a dedicated output mutex

---

## Instructions

### Compilation

```
make
```

This produces the `codexion` executable. The Makefile compiles with `-Wall -Wextra -Werror -pthread`.

Available rules: `all`, `clean`, `fclean`, `re`.

### Execution

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are **mandatory**. The program rejects invalid inputs: negative numbers, non-integers, or a scheduler value other than `fifo` or `edf`.

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) |
| `time_to_burnout` | Max ms without starting a compile before burnout |
| `time_to_compile` | Duration of the compile phase (ms) |
| `time_to_debug` | Duration of the debug phase (ms) |
| `time_to_refactor` | Duration of the refactor phase (ms) |
| `number_of_compiles_required` | Simulation ends when all coders reach this count |
| `dongle_cooldown` | Ms a dongle is unavailable after being released |
| `scheduler` | `fifo` or `edf` |

### Examples

```
# 5 coders, 800ms burnout, 200ms compile/debug/refactor, 5 compiles, 50ms cooldown, EDF
./codexion 5 800 200 200 200 5 50 edf

# Single coder (only one dongle on the table)
./codexion 1 400 100 100 100 3 0 fifo
```

Expected output format:

```
0 1 has taken a dongle
2 1 has taken a dongle
2 1 is compiling
202 1 is debugging
402 1 is refactoring
405 2 has taken a dongle
406 2 has taken a dongle
406 2 is compiling
...
1505 4 burned out
```

---

## Blocking Cases Handled

### Deadlock Prevention

The classic deadlock scenario occurs when every coder simultaneously holds their left dongle and waits indefinitely for their right — a circular dependency. Two mechanisms prevent this:

**Global queue / single-entry-point**: before touching any dongle, a coder enqueues itself in a global linked-list queue protected by `mutex_queue`. It then waits on `cond_top_prio` until it is the head of the queue (i.e., its turn to acquire). Only one coder at a time attempts to lock dongles, eliminating the possibility of a partial-hold cycle.

**Consistent dongle ordering**: once a coder leaves the queue, it always acquires the lower-index dongle first (`lock_dongle(right)` then `lock_dongle(left)` if `right->id < left->id`, and vice versa). This breaks the circular-wait Coffman condition.

Coffman's four conditions:

| Condition | Mitigation |
|---|---|
| Mutual exclusion | Kept — dongles are exclusive by design |
| Hold and wait | Broken — coders wait in queue *before* holding anything |
| No preemption | Kept — coders cannot steal dongles |
| Circular wait | Broken — consistent global ordering of dongle acquisition |

### Starvation Prevention

**FIFO**: the global queue is a linked list sorted by insertion order (`enqueue` appends to the tail). Requests are served strictly in arrival order, so no coder is ever skipped.

**EDF**: the queue is maintained sorted by burnout deadline (`enqueue_edf` inserts in ascending order of `last_compiled + time_to_burnout`). The coder closest to burning out is always at the head. Liveness is guaranteed as long as the simulation parameters are feasible: a coder with a very tight deadline will always be promoted above coders with slack.

### Cooldown Handling

When a coder releases its two dongles, it sets `dongle->available_at = get_time() + dongle_cooldown` on each before unlocking the dongle mutex. Any subsequent requester entering `lock_dongle` finds `get_time() < dongle->available_at` and calls `pthread_cond_timedwait` in a loop with a short deadline until the cooldown window has passed. This ensures the cooldown is respected precisely without busy-waiting.

### Precise Burnout Detection

A dedicated **monitor thread** runs `monitor_routine` in a tight loop. At each iteration it inspects every coder's `last_compiled` timestamp (protected by `last_compiled_mutex`) and computes `get_time() - last_compiled`. If this exceeds `time_to_burnout`, the monitor acquires `mutex_output`, prints the burnout message, calls `set_exit`, and returns. Because the loop has no sleep, the burnout message is printed within a few microseconds of the actual deadline — well within the 10 ms requirement.

### Log Serialization

All output (coder state changes and the burnout message) is protected by `mutex_output`. Any thread must acquire this mutex before calling `printf` and releases it immediately after. Since the dongle-unlock broadcast is sent *after* the output mutex is released, log lines from different threads never interleave.

---

## Thread Synchronization Mechanisms

### Per-dongle `pthread_mutex_t` — exclusive access

Each dongle owns `dongle_mutex` which serializes all state reads and writes. A coder holds `dongle_mutex` for the entire duration of the compile phase: this is the resource lock that makes the dongle "in use". The cooldown check (`on_cd_mutex` + `cd_cond`) is a nested lock within `dongle_mutex`, used only to sleep during the cooldown window without blocking unrelated operations.

```c
// In lock_dongle():
pthread_mutex_lock(dongle->dongle_mutex);          // acquire the dongle
pthread_mutex_lock(dongle->on_cd_mutex);
while (get_time() < dongle->available_at)           // wait out cooldown
    pthread_cond_timedwait(dongle->cd_cond,
                           dongle->on_cd_mutex, &target);
pthread_mutex_unlock(dongle->on_cd_mutex);
// dongle_mutex stays locked — held during compile, released after
```

### Global queue `mutex_queue` + `cond_top_prio`

A single global linked list (`config->queue`) holds all coders waiting to acquire dongles. Access is serialized by `mutex_queue`. A coder blocks on `cond_top_prio` until `is_top_prio(coder)` returns true (it is at the head). After acquiring both dongles, it broadcasts on `cond_top_prio` to wake the next waiter.

```c
// In acquire_dongles():
pthread_mutex_lock(coder->config->mutex_queue);
enqueue(&coder->config->queue, ft_new_coder_node(coder)); // FIFO or EDF insert
while (check_exit(coder->config) == 0 && is_top_prio(coder) == 0)
    pthread_cond_wait(coder->config->cond_top_prio,
                      coder->config->mutex_queue);
dequeue(&coder->config->queue);
pthread_mutex_unlock(coder->config->mutex_queue);
```

### `last_compiled_mutex` — burnout timestamp

Each coder has its own `last_compiled_mutex` protecting the `last_compiled` field. The coder writes it right before starting `usleep(time_to_compile)`. The monitor reads it on every loop iteration. This fine-grained mutex avoids contention: coders from different threads never share this lock.

### `mutex_output` — serialized logging

All `printf` calls are wrapped by `mutex_output`. The monitor acquires it before printing the burnout message and sets the exit flag while holding it, so no coder can print a stale state message after the burnout has been announced.

### Race condition prevention summary

| Shared resource | Protected by |
|---|---|
| Dongle availability & cooldown | `dongle_mutex` + `on_cd_mutex` / `cd_cond` |
| Global dispatch queue | `mutex_queue` + `cond_top_prio` |
| Per-coder compile timestamp | `last_compiled_mutex` |
| Simulation exit flag | `config_mutex` |
| Total compilation counter | `mutex_total_comp` |
| stdout output | `mutex_output` |

---

## Resources

### References

- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [The Little Book of Semaphores — Allen B. Downey](https://greenteapress.com/wp/semaphores/)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [Coffman Conditions — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Coffman_conditions)
- `man pthread_create`, `man pthread_mutex_init`, `man pthread_cond_wait`, `man pthread_cond_timedwait`, `man gettimeofday`

### AI Usage

Claude (Anthropic) was used during this project for the following tasks:

- Generating the initial structure and sections of this README based on the actual source code (compile.c, monitor.c, coders.h) reviewed directly from the repository

All generated documentation was reviewed, tested, and validated before inclusion.