# tiny
`tiny` is a heapless embedded development library optimized for flexible, reusable, and enjoyable development with minimal RAM and ROM usage. `tiny` can be used bare metal, with an RTOS, or integrated into an existing framework like Arduino or Mbed.

need to mention something about being portable?

fixme rough RAM/ROM requirements?

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

## Examples
### Bare Metal
link to samd21-tiny, stm8-tiny, avr-tiny, stm32-tiny, etc.

### RTOS Integration
stm32f050 freertos thing?

### Arduino Integration
link to pio-tiny-starter-kit
