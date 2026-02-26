*This project was created as part of the 42 curriculum by rmarin-n.*

# Codexion

## Description
Codexion is a multithreaded simulation inspired by the Dining Philosophers problem, reimagined as a group of programmers (“coders”) competing for access to shared resources called dongles. Each coder repeatedly cycles through compiling, debugging, and refactoring, while respecting burnout limits, cooldown times, and scheduling rules.

Two scheduling algorithms are implemented:

- **FIFO (First-In First-Out)** — coders access dongles in the order they arrive.
- **EDF (Earliest Deadline First)** — coders with the most urgent deadlines (based on their last compile time) receive priority.

The simulation includes:
- Thread creation and synchronization using POSIX threads.
- Mutexes and condition variables to coordinate access to shared dongles.
- A monitoring thread that detects burnout or completion.
- A queueing system with priority logic depending on the scheduler.
- Safe cleanup mechanisms to handle partial thread creation or early termination.

The goal is to model a highly concurrent environment where limited resources and scheduling decisions directly influence system behavior.

---

## Instructions

### Compilation
The project includes a Makefile. To compile the program:

make

To remove object files:

make clean

To remove all generated files:

make fclean

To rebuild everything:

make re

---

### Execution
Codexion requires eight arguments in addition to the program name:

./codexion <coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <compiles_required> <dongle_cooldown> <scheduler>

Example:

./codexion 5 800 200 100 150 7 300 fifo

Argument overview:
- **coders** — number of coder threads.
- **time_to_burnout** — maximum time a coder can go without compiling.
- **time_to_compile** — duration of the compile phase.
- **time_to_debug** — duration of the debugging phase.
- **time_to_refactor** — duration of the refactoring phase.
- **compiles_required** — number of successful compilations required to finish.
- **dongle_cooldown** — cooldown time before a dongle can be reused.
- **scheduler** — either `fifo` or `edf`.

If arguments are missing, invalid, non‑positive, or the scheduler is not recognized, the program prints an error message and exits.

---

## Resources

### References
- *The Dining Philosophers Problem* — Edsger Dijkstra.
- *Programming with POSIX Threads* — David R. Butenhof.
- Official documentation for pthread mutexes, condition variables, and thread creation.
- Articles on FIFO and EDF scheduling in operating systems.

### Use of AI
Artificial intelligence was used for:
- Improving clarity of error messages.
- Helping reorganize and explain parts of the code without altering logic.
- Providing conceptual explanations of concurrency mechanisms.

AI was **not** used to generate full code solutions or replace the reasoning required to design and implement the project.
