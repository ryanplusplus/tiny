# tiny
[![Tests](https://github.com/ryanplusplus/tiny/actions/workflows/test.yml/badge.svg)](https://github.com/ryanplusplus/tiny/actions/workflows/test.yml)

`tiny` is an embedded development library designed to be enjoyable to use, even in a heapless environment. Its components are designed to be flexible, reusable, and have RAM and ROM usage. `tiny` can be used bare metal, with an RTOS, or integrated into an existing framework like Arduino or Mbed and can be used on all but the smallest microcontrollers.

## Design
### Simple
`tiny` favors simplicity over providing every possible feature. This makes it easier to learn, keeps code size and compile times down, and helps to ensure that the features that _are_ included are high impact.

### Event-Driven
`tiny` is designed for event-driven applications. It relies upon short RTCs (Run-To-Complete steps) and synchronous event propagation to enable "multi-tasking" without threading. This is roughly analogous to the JavaScript programming model.

### Flexible
`tiny` relies upon run-time polymorphism provided by low-cost interfaces. This allows for a roughly object-oriented style without inheritance. Interfaces make `tiny` flexible, portable, and testable.

## What's in the Box?
### HAL
`tiny` provides a basic HAL (Hardware Abstraction Layer) that includes interfaces for interacting with hardware like digital I/O, analog inputs, PWM channels, and serial communication protocols. Most of the core `tiny` components are hardware independent, but those that aren't use the HAL to be portable.

See [include/hal](include/hal) for a list of all interfaces.

### Data Structures
`tiny` provides heapless, arbitrarily-sized (intrusive) list, queue, and ring buffer data structures. See:
- [`tiny_list`](include/tiny_list.h)
- [`tiny_queue`](include/tiny_queue.h)
- [`tiny_ring_buffer`](include/tiny_ring_buffer.h)

### Software Timers
Software timers allow applications to generate one-shot and periodic time-based events. See:
- [`timer`](include/tiny_timer.h)

### Events
`tiny` defines an abstract event type as well as some concrete event implementations. Events are basically fancy callbacks that event handlers to be registered dynamically. See:
- [`i_tiny_event`](include/i_tiny_event.h)
- [`tiny_event`](include/tiny_event.h)
- [`tiny_single_subscriber_event`](include/tiny_single_subscriber_event.h)

### Event Queues
Event queues allow work to be queued to another RTC or to move an between an interrupt context and a non-interrupt context. See:
- [`i_tiny_event_queue`](include/i_tiny_event_queue.h)
- [`tiny_event_queue`](include/tiny_event_queue.h)

### Message Buses
`tiny`'s message bus interface allows components to broadcast events within an application. See:
- [`i_tiny_message_bus`](include/i_tiny_message_bus.h)
- [`tiny_message_bus`](include/tiny_message_bus.h)

### Key Value Stores
`tiny`'s key value store allows for data to be shared within a component in a structured way. Changes to values within a key value store generate data events that enable components to "watch" shared data. See:
- [`i_tiny_key_value_store`](include/i_tiny_key_value_store.h)
- [`tiny_ram_key_value_store`](include/tiny_ram_key_value_store.h)

### State Machines
`tiny` provides lightweight FSM (Finite State Machine) and HSM (Hierarchical State Machine) libraries. These model states as functions that are invoked when the state machine is signaled. State machines allow event-driven programs to be written without spaghetti code. See:
- [`tiny_fsm`](include/tiny_fsm.h)
- [`tiny_hsm`](include/tiny_hsm.h)

### Communication
`tiny` provides a simple library for point-to-point communication. It allows payloads to be sent between nodes in a network with data integrity protected by a CRC16.
- [`i_tiny_comm`](include/i_tiny_comm.h)
- [`tiny_comm`](include/tiny_comm.h)

## How Can I Use It?
Integrating `tiny` is simple: just put `include/` on the include path and build everything in `src/`.

To include `tiny` in a test build, add `test/include` to the include path and build the contents of `test/src`. This brings useful test doubles into your tests.

### Examples
#### Bare Metal
See [`ryanplusplus/samd21-tiny`](https://github.com/ryanplusplus/samd21-tiny) for an example of a bare metal `tiny` project using the Microchip SAMD21. This is a simple project that sets up a blinking LED and includes several hardware drivers that use the `tiny` HAL.

See [`ryanplusplus/w1209`](https://github.com/ryanplusplus/w1209) for an example of a bare metal `tiny` project that is a more complete application built around a `tiny` key value store.

#### FreeRTOS
See [`ryanplusplus/stm32f051-freertos`](https://github.com/ryanplusplus/stm32f051-freertos) for an example of a `tiny` project that integrates with FreeRTOS.

#### Arduino
See [`ryanplusplus/pio-tiny-starter-kit`](https://github.com/ryanplusplus/pio-tiny-starter-kit) for an example of a `tiny` project using the Arduino framework with PlatformIO.

## Development
Building and running tests requires MacOS or Linux and:
- A toolchain based on [`gcc`](https://gcc.gnu.org/) or [`clang`](https://clang.llvm.org/)
- [GNU `make`](https://www.gnu.org/software/make/)
- [CppUTest](http://cpputest.github.io/)
- [`rerun`](https://rerun.github.io/rerun/) (optional)

Build and run tests with:

```shell
$ make
```

Or use `rerun` to automatically build and run tests when source files change with:

```shell
$ rerun make
```
