*This project has been created as part of the 42 curriculum by mboutte*

# Codexion

## Description

`codexion` is a concurrency simulation written in C with POSIX threads and mutexes.
It models a group of coders seated around a shared workspace where each coder needs
two USB dongles to compile, then alternates between debugging and refactoring.

The goal of the project is to coordinate concurrent access to shared resources while
avoiding common synchronization failures such as deadlocks, race conditions, unfair
resource attribution, mixed logs, and inaccurate burnout detection. The simulation
stops when one coder burns out or when every coder has compiled the required number
of times.

## Instructions

### Build

Requirements:

- `cc`
- `make`
- POSIX threads (`pthread`)

Compile the mandatory program:

```sh
make
```

The Makefile builds with `cc` and uses `-Wall -Wextra -Werror -pthread`.

Clean object files:

```sh
make clean
```

Remove object files and binaries:

```sh
make fclean
```

Rebuild from scratch:

```sh
make re
```

Extra targets present in this repository:

```sh
make tsan
make run_tsan
make val
```

### Run

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Arguments:

- `number_of_coders`: number of coder threads and number of dongles
- `time_to_burnout`: maximum delay in ms before a coder burns out if they do not start compiling again
- `time_to_compile`: compile duration in ms
- `time_to_debug`: debug duration in ms
- `time_to_refactor`: refactor duration in ms
- `number_of_compiles_required`: simulation ends successfully once every coder reaches this count
- `dongle_cooldown`: cooldown in ms before a released dongle becomes usable again
- `scheduler`: arbitration policy, either `fifo` or `edf`

Example:

```sh
./codexion 4 1000 100 100 100 2 10 edf
```

Expected log format:

```text
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

## Blocking Cases Handled

- **Deadlock prevention:** dongle mutexes are always locked in a deterministic order based on dongle id. This removes circular wait when a coder tries to acquire two dongles.
- **Mutual exclusion on shared resources:** each dongle has its own mutex and availability state, so two coders cannot claim the same dongle at the same time.
- **FIFO / EDF arbitration:** coders waiting to compile are inserted into a shared protected queue. In `fifo` mode, insertion order is preserved. In `edf` mode, the queue is ordered by the earliest burnout deadline.
- **Cooldown handling:** after a compile phase, both dongles are released with an `end_cooldown` timestamp. They remain unavailable until the cooldown expires.
- **Burnout detection:** a dedicated monitor thread continuously checks each coder deadline and stops the simulation as soon as a burnout is detected.
- **Serialized logs:** all printed state changes pass through a print mutex, so lines do not interleave.
- **Single-coder edge case:** when only one dongle exists, the worker thread exits immediately and the monitor eventually reports burnout, matching the expected behavior for an impossible compile.

## Thread Synchronization Mechanisms

This implementation uses `pthread_mutex_t` only. It does not rely on `pthread_cond_t`;
coordination is done with mutex-protected shared state plus short polling loops.

Shared synchronization points:

- **Dongles (`t_dongle.lock`):** protect dongle ownership, availability, and cooldown timestamps.
- **Per-coder state (`t_coder.lock`):** protect `burnout_time` and the compile counter when they are read or updated concurrently.
- **Global simulation state (`lock_state`):** controls the start barrier and the stop condition shared between workers and the monitor.
- **Logging (`lock_printf`):** ensures each message is printed atomically.
- **Queue (`queue.lock`):** protects the scheduler queue while coders are inserted, removed, or checked against current dongle availability.

Examples of race prevention:

- A coder cannot mark a dongle as taken without holding the queue lock and the two dongle mutexes.
- The monitor reads burnout deadlines through accessor helpers guarded by the coder mutex, preventing torn reads while workers update deadlines at the start of a compile.
- The global `state` flag is always read or written under `lock_state`, which allows all threads to stop consistently once the simulation ends.

## Resources

Classic references related to threads, mutexes, and scheduling:

- POSIX threads overview: <https://man7.org/linux/man-pages/man7/pthreads.7.html>
- Mutex API reference: <https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html>
- `gettimeofday(2)` reference: <https://man7.org/linux/man-pages/man2/gettimeofday.2.html>
- EDF scheduling background: <https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling>
- Deadlock conditions (Coffman conditions): <https://en.wikipedia.org/wiki/Coffman_conditions>

AI usage for this repository:

- AI was used to review the subject requirements and draft this `README.md`.
- The final content was cross-checked against the source files before being kept.
- Any code claims in this document are limited to what is currently implemented in the repository.
