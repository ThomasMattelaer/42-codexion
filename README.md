*This project has been created as part of the 42 curriculum by tmattela.*

# Codexion

**Master the race for resources before the deadline masters you**

## Description

Codexion is a concurrent programming project written in C, modeled on the
classic dining philosophers problem. Instead of philosophers and forks, it
simulates a pool of **coders** who need to acquire **two dongles** (a pair of
shared, limited resources) at the same time in order to work — compiling,
debugging, and refactoring — before a **burnout deadline** forces them to stop.

The goal of the project is to design and implement a correct, deadlock-free,
starvation-free concurrent simulation using POSIX threads, mutexes, and
condition variables, while also exposing the trade-offs between two different
resource-scheduling strategies:

- **FIFO** — dongles are granted to coders strictly in the order they requested
  them.
- **EDF (Earliest Deadline First)** — dongles are granted to the coder whose
  burnout deadline is closest, in order to minimize the number of coders lost
  to burnout under contention.

A dedicated **monitor thread** continuously watches the state of the
simulation (coder activity, burnout, overall completion) and reports it,
without interfering with the correctness of the simulation itself.

## Instructions

### Compilation

```bash
make        # builds the codexion binary
make clean  # removes object files
make fclean # removes object files and the binary
make re     # rebuilds from scratch
```

### Execution

```bash
./codexion <num_coders> <burnout_time> <compile_time> <debug_time> <refactor_time> <num_dongles> <cooldown> <mode>
```

| Argument         | Description                                                        |
|------------------|---------------------------------------------------------------------|
| `num_coders`     | Number of coder threads in the simulation                          |
| `burnout_time`   | Time (ms) a coder can go without acquiring a dongle before burnout |
| `compile_time`   | Time (ms) spent compiling while holding both dongles                |
| `debug_time`     | Time (ms) spent debugging after releasing the dongles               |
| `refactor_time`  | Time (ms) spent refactoring before requesting dongles again         |
| `num_dongles`    | Number of dongles available in the pool                             |
| `cooldown`       | Time (ms) a dongle must rest after being released before reuse      |
| `mode`           | Scheduling strategy for dongle allocation: `fifo` or `edf`          |

Example:

```bash
./codexion 5 2000000000 200 200 200 2 200 fifo
```

This runs a simulation with 5 coders, an effectively disabled burnout
timer, compile/debug/refactor phases of 200 ms each, a pool of 2 dongles
with a 200 ms cooldown, scheduled in FIFO order.

### Testing for concurrency issues

The project was validated with Helgrind (Valgrind's thread error detector):

```bash
valgrind --tool=helgrind ./codexion <args>
```

A suppression file (`codexion.supp`) is provided to filter out a known false
positive triggered by Helgrind 3.18.1 misinterpreting the internal
`pthread_cond_timedwait` implementation shipped with glibc ≥ 2.34. To run
with suppressions applied:

```bash
valgrind --tool=helgrind --suppressions=codexion.supp ./codexion <args>
```

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions):** a coder never holds one
  dongle while blocking indefinitely on the other. Dongle acquisition is
  ordered and bounded by a burnout check on every wait iteration, so a coder
  can never wait forever while holding a partial set of resources — breaking
  the *hold and wait* condition.
- **Starvation prevention:** requests are tracked in a per-dongle wait queue.
  In FIFO mode, coders are served strictly in arrival order. In EDF mode,
  coders closest to their burnout deadline are prioritized, which prevents
  a slower or unlucky coder from being perpetually starved while others cycle
  through faster.
- **Cooldown handling:** a released dongle records its release timestamp
  (`last_release`) and cannot be reacquired until its cooldown period has
  elapsed. This is enforced by computing a bounded wait deadline
  (`get_timeout`) before each `pthread_cond_timedwait`, rather than relying on
  a plain `pthread_cond_wait`, so cooldown expiry always wakes waiting
  threads even in the absence of a signal.
- **Precise burnout detection:** every waiting coder re-checks
  `burnout_detected()` on each wake-up (spurious, timeout, or signaled),
  ensuring burnout is detected as soon as it becomes true rather than only
  when a dongle happens to become available.
- **Log serialization:** all simulation output is funneled through a single
  synchronized logging path so that interleaved actions from concurrent
  coder threads never produce corrupted or interleaved lines in the output.

## Thread synchronization mechanisms

Codexion relies on standard POSIX primitives, used consistently to protect
each shared resource:

- **`pthread_mutex_t` per dongle:** each dongle has its own mutex protecting
  its internal state (`is_taken`, `last_release`, wait queue). This fine
  grained locking (one mutex per dongle rather than one global lock) allows
  coders competing for different dongles to progress independently.
- **`pthread_cond_t` per dongle:** coders that cannot immediately acquire a
  dongle wait on its condition variable via `pthread_cond_timedwait`, bounded
  by the cooldown deadline. This avoids busy-waiting while still guaranteeing
  periodic re-evaluation of the burnout condition.
- **Monitor state protection:** the monitor thread reads shared simulation
  state (coder status, dongle status) under the same locking discipline as
  the coder threads, so it never observes a torn or partially updated view of
  a coder's or dongle's state.
- **Logging synchronization:** a dedicated lock guards the output stream,
  ensuring that a coder's or the monitor's log line is written atomically
  relative to every other thread's output.

### How race conditions are prevented

Every read or write to shared fields (`is_taken`, `last_release`, the wait
queue, burnout state) happens only while the corresponding mutex is held.
The classic acquire/check/release pattern is used throughout:

```c
pthread_mutex_lock(&dongle->mutex);
/* ... read or mutate dongle->is_taken, dongle->last_release, queue ... */
pthread_mutex_unlock(&dongle->mutex);
```

`pthread_cond_timedwait` is always called with the associated mutex held, and
the mutex is re-acquired atomically on wake-up, guaranteeing that no other
thread can slip in and mutate the dongle's state between the wake-up and the
next check.

### Thread-safe communication between coders and the monitor

The monitor thread never receives direct signals from coder threads; instead
it periodically samples shared state under the appropriate locks. This
decouples the monitor's reporting cadence from the coders' scheduling logic,
avoiding any risk of the monitor observing an inconsistent intermediate state
or introducing extra contention on the dongle locks used by the simulation's
critical path.

## Resources

- *The Little Book of Semaphores*, Allen B. Downey — foundational reference
  for classic synchronization problems (dining philosophers, producer
  consumer, readers writers) and the reasoning patterns behind deadlock and
  starvation prevention.
- `man pthread_mutex_lock`, `man pthread_cond_timedwait`, `man pthread_create`
  — POSIX threads reference documentation.
- [Helgrind manual](https://valgrind.org/docs/manual/hg-manual.html) — Valgrind's
  thread error detector, used to validate the absence of data races and lock
  order violations.
- Coffman's conditions for deadlock (mutual exclusion, hold and wait, no
  preemption, circular wait) — used as a checklist when designing the
  dongle-acquisition logic.
- Earliest Deadline First (EDF) scheduling — classic real-time scheduling
  algorithm, adapted here to prioritize dongle allocation for coders closest
  to burnout.
- https://dev.to/yel-bakk/codexion-4fk8DEV CommunityCodexion
- Oceano - Multi-Threading (https://www.youtube.com/results?search_query=ocenao+multi+threads)

### AI usage

Claude (Anthropic) was used exclusively as a debugging and conceptual
sounding board, not for writing code or the report itself:

- **Debugging a Helgrind false positive:** analyzing a
  `pthread_cond_{signal,broadcast}: associated lock is not held` warning
  reported by Helgrind on `pthread_cond_timedwait` calls. Claude helped trace
  the exact call stack, identify it as a known false positive caused by the
  glibc ≥ 2.34 condition-variable reimplementation combined with an outdated
  Valgrind version (3.18.1), and generate/validate the corresponding
  suppression file (`codexion.supp`).
- **Conceptual discussion:** clarifying how Helgrind associates a mutex with
  a condition variable and how to distinguish an actual synchronization bug
  from a tooling artifact, through guided questions rather than direct
  answers.

- **Creation of the README**

All synchronization logic, scheduling implementation, and project code were
designed and written independently.
