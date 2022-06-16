# tiny
[![Tests](https://github.com/ryanplusplus/tiny/actions/workflows/test.yml/badge.svg)](https://github.com/ryanplusplus/tiny/actions/workflows/test.yml)

`tiny` is an embedded development library designed to be enjoyable to use, even in a heapless environment. Its comopnents are designed to be flexible, reusable, and have RAM and ROM usage. `tiny` can be used bare metal, with an RTOS, or integrated into an existing framework like Arduino or Mbed and can be used on all but the smallest microcontrollers.

## Design Philosophy
### Simplicity
`tiny` favors simplicity over providing every possible feature. This makes it easier to learn, keeps code size and compile times down, and helps to ensure that the features that _are_ included are high impact.

### Event-Driven
`tiny` is designed for event-driven applications. It relies upon short RTCs (run-to-complete steps) and synchronous event propagation to balance power with the limitations inherent in C and in embedded development.

### Abstractions
`tiny` relies heavily upon run-time polymorphism provided by low-cost abstractions. These are akin to interfaces in other programming langauges and allow for a roughly object-oriented style, but without inheritance. Interfaces make `tiny` flexible, testable, and portable.

## Components
fixme for these link to the component headers and give a brief overview

### HAL
`tiny` provides a basic HAL (hardware abstraction layer) that includes interfaces for interacting with hardware like digital I/O, analog inputs, PWM channels, and serial communication protocols. Most of the core `tiny` components are hardware independent, but those that aren't use the HAL to be portable.

See [include/hal](include/hal) for a list of all interfaces.

### Data Structures

- list
- ring buffer

### Time Source
- abstract time source

### Software Timers
- timer/timer group

### Events
- abstract event
- event
- single subscriber event

### Event Queues
- abstract event queue
- event queue

### Messages Buses
- abstract message bus
- message bus

### Key Value Stores
- abstract key value store
- ram key value store

### State Machines
- fsm
- hsm

### Communication
- comm

### Utilities
- crc16

## Integration/Use
for target builds:
- pull in include
- build src

for test builds also do:
- pull in test/include
- build test/src

### Examples
#### Bare Metal
link to samd21-tiny, stm8-tiny, avr-tiny, stm32-tiny, etc.

#### FreeRTOS
stm32f050 freertos thing?

#### Arduino
link to pio-tiny-starter-kit

## Development
dependencies
- make
- clang/gcc
- cpputest

build and run tests with `make`

use `rerun` to watch with `rerun make` or `rerun "make -j8"`
