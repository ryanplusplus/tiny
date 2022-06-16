# tiny
[![Tests](https://github.com/ryanplusplus/tiny/actions/workflows/test.yml/badge.svg)](https://github.com/ryanplusplus/tiny/actions/workflows/test.yml)

`tiny` is an embedded development library designed to be enjoyable to use, even in a heapless environment. Its comopnents are designed to be flexible, reusable, and have RAM and ROM usage. `tiny` can be used bare metal, with an RTOS, or integrated into an existing framework like Arduino or Mbed and can be used on all but the smallest microcontrollers.

## Development
dependencies
- make
- clang/gcc
- cpputest

build and run tests with `make`

use `rerun` to watch with `rerun make` or `rerun "make -j8"`

## Design Philosophy/Concepts/???
### Simplicity

### Event-Driven
event-driven with synchronous event propagation and short RTCs

### Abstractions
something about interfaces, portability, testability, and flexibility

something about HAL using interfaces

## Components
fixme for these link to the component headers and give a brief overview

### HAL
- link to include/hal

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
