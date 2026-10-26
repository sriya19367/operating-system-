# Operating Systems Laboratory Programs

## About

This repository contains C programs implemented as part of my Operating Systems laboratory course. It covers important Operating Systems concepts, including multithreading, process synchronization, CPU scheduling, memory management, disk scheduling, and file allocation.

## Objectives

- Understand fundamental Operating Systems concepts.
- Implement Operating Systems algorithms using C.
- Practise process synchronization and resource management.
- Explore CPU scheduling and memory management techniques.
- Learn file allocation and directory organization methods.

## Experiments and Programs

| No. | Experiment / Topic                                 | Program File(s)                      |
| --- | -------------------------------------------------- | ------------------------------------ |
| 1   | Multithreading                                     | `oxyg.c`                             |
| 2   | Mutual Exclusion                                   | `network.c`                          |
| 3   | Reader–Writer Problem                              | `exp5I.c`                            |
| 4   | Dining Philosophers Problem                        | `exp6.c`                             |
| 5   | bankers algorithm and Deadlock avoidance           | `exp7d.c`, `exp7n.c`                 |
| 6   | CPU Scheduling: FCFS and SJF                       | `exp8F.c`, `exp8S.c`                 |
| 7   | CPU Scheduling: Priority and Round Robin           | `p9.c`, `r9.c`                       |
| 8   | FIFO Page Replacement                              | `exp10.c`                            |
| 9   | Page Replacement: LRU and LFU                      | `LRU.c`, `LFU.c`, `both11.c`         |
| 10  | Memory Allocation: Best Fit and Worst Fit          | `exp12.c`, `exp12W.c`, `exp12both.c` |
| 11  | Disk Scheduling                                    | `exp13.c`                            |
| 12  | File Allocation: Sequential and Indexed            | `exp14S.c`, `exp14I.c`               |
| 13  | Directory Organization: Single-Level and Two-Level | `exp15s.c`, `exp15t.c`               |

## Technologies Used

- **Programming Language:** C
- **Operating Environment:** Ubuntu Linux
- **Code Editor:** Visual Studio Code
- **Compiler:** GCC

## How to Compile and Run

1. Open a terminal in the folder containing the program.

2. Compile the required C file:

   ```bash
   gcc filename.c -o program
   ```

3. Run the compiled program:

   ```bash
   ./program
   ```

Replace `filename.c` with the required source filename.

## Purpose

This repository is maintained for academic practice, learning, and reference. It demonstrates practical implementations of fundamental Operating Systems concepts and algorithms.
