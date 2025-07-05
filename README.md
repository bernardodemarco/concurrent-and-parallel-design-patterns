# Concurrent and Parallel Programming Design Patterns

This project is a simulation of an automatic triggering system for autonomous vehicles. It focuses on the practical implementation and usage of **classical concurrent and parallel design patterns** in C. The following design patterns were implemented manually using POSIX threads and synchronization primitives:

- Producer and Consumer;
- Fork-Join; and,
- Mutex-based Critical Sections.

The only external dependency used was a thread pool implementation, provided by the [C-Thread-Pool](https://github.com/Pithikos/C-Thread-Pool) library. It offers a simple and clean API, and uses a **queue-based task buffer**.

## Table of Contents

- [Application Design](#application-design)
- [Technologies Used](#technologies-used)

## Application Design

The application consists of `N` sensors (where `N` is configurable via input). Each sensor is a thread running an infinite loop that continuously generates random values and inserts them into a shared buffer, following the **producer/consumer pattern**.

At the core of the system lies the control center, referred to as the **orchestrator**. This is a single thread responsible for consuming values from the shared buffer and managing vehicle actuators via a fixed-size thread pool (`4` threads) and an actuator control table.

Every time the orchestrator consumes a value, it packages the value into a task and dispatches it to one of the threads in the thread pool. Each task is responsible for determining the target actuator and activity level, updating the control table, and generating a log message on the terminal.

The actuator update and log generation are performed in parallel using a **fork-join strategy**. If an error occurs during either of these subtasks, the task is responsible for logging the failure.

To update an actuator, its activity level in the control table is modified and held for 2 to 3 seconds. Instead of locking the entire table as a single critical section, the table is divided into segments, each representing a critical region with a granularity of `20`. This means only the relevant section of the table is locked during updates, not the whole structure.

While the actuator update is performed, a log is also printed. Each log message remains visible on the console for one second. After synchronizing the fork-join subtasks, the thread pool task checks for errors (each subtask has a 20% chance of failing). If any errors occurred, an error log is printed to the terminal.

## Technologies Used

- C programming language;
- POSIX library for multithreading and synchronization; and,
- [C-Thread-Pool](https://github.com/Pithikos/C-Thread-Pool).

## Setup Instructions

A `Makefile` was used to simplify the compilation process. The resulting binary is named `app` and is located in the `build` directory.

- Compile:
    ```bash
    make compile
    ```
- Run:
    ```bash
    make run
    ```
- Clean:
    ```bash
    make clean
    ```

## Execution Examples

### Number of actuators equal to twice the number of sensors

#### Input Parameters

| Number of sensors | Number of actuators |
| :----------------: | :-----------------: |
|        100         |        200          |

#### Output

```bash
Changing actuator [90] with value [89]
Changing actuator [22] with value [69]
Changing actuator [73] with value [7]
An error occurred in the actuator [73]
Changing actuator [63] with value [17]
Changing actuator [105] with value [38]
An error occurred in the actuator [105]
Changing actuator [180] with value [61]
Changing actuator [103] with value [27]
Changing actuator [136] with value [95]
An error occurred in the actuator [136]
Changing actuator [72] with value [36]
Changing actuator [115] with value [55]
An error occurred in the actuator [115]
Changing actuator [197] with value [8]
Changing actuator [19] with value [39]
An error occurred in the actuator [19]
Changing actuator [172] with value [32]
Changing actuator [59] with value [3]
Changing actuator [59] with value [7]
Changing actuator [165] with value [26]
Changing actuator [37] with value [93]
Changing actuator [104] with value [93]
Changing actuator [72] with value [37]
Changing actuator [124] with value [92]
Changing actuator [39] with value [32]
An error occurred in the actuator [104]
An error occurred in the actuator [39]
Changing actuator [61] with value [88]
Changing actuator [112] with value [38]
```

| Number of logs | Number of changes in the actuators | Number of Errors |
| :----------------: | :-----------------: | :-----------------: |
|        30         |        23          | 7 |

During this execution, the log generation flow was smooth, consistently meeting the requirement that each update log should hold the console for one second.

### Number of sensors equal to twice the number of actuators

#### Input Parameter

| Number of sensors | Number of actuators |
| :----------------: | :-----------------: |
|        200         |        100          |

#### Output

```bash
Changing actuator [73] with value [7]
Changing actuator [80] with value [84]
Changing actuator [3] with value [14]
An error occurred in the actuator [80]
Changing actuator [36] with value [45]
Changing actuator [72] with value [42]
An error occurred in the actuator [72]
Changing actuator [15] with value [38]
An error occurred in the actuator [15]
Changing actuator [97] with value [35]
An error occurred in the actuator [97]
Changing actuator [94] with value [77]
Changing actuator [72] with value [68]
Changing actuator [59] with value [45]
Changing actuator [20] with value [50]
Changing actuator [65] with value [59]
An error occurred in the actuator [65]
Changing actuator [37] with value [77]
Changing actuator [4] with value [4] # Change of 4th actuator's activity level
Changing actuator [72] with value [36]
An error occurred in the actuator [72]
Changing actuator [24] with value [38]
Changing actuator [39] with value [1]
An error occurred in the actuator [39]
Changing actuator [61] with value [12]
An error occurred in the actuator [4] # error in the 4th actuator
Changing actuator [49] with value [78]
Changing actuator [92] with value [22]
Changing actuator [97] with value [65]
An error occurred in the actuator [97]
```

| Number of logs | Number of changes in the actuators | Number of Errors |
| :----------------: | :-----------------: | :-----------------: |
|        30         |        24          | 6 |

In this scenario, it was observed that error messages were occasionally printed only after a significant number of update logs had already appeared, such as in the case of actuator `4`, as seen in the output. This behavior is due to the sensor count being twice the actuator count.

As a result, the number of values inserted into the producer/consumer buffer tends to double. At the same time, the number of actuators is reduced to `100`, leading to the actuator table being split into only `5` critical regions (`100 / 20`). In the previous scenario, there were `10` critical regions.

Consequently, more sensor data competes for fewer critical regions. This increases the frequency and duration of task queuing in a suspended state, delaying updates to the actuator table entries.
