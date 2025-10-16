# LIN Parser module for diagnostic/tuning
_Application project for RaceUp 2025-2026_

By Luca Domeneghetti (MSc. Cybersecurity @ UniPd)

The goal of this project is to develop a basic parser and simulator suitable for a LIN-based protocol exchange.
Its modularity allows for further extension and ease of debugging.

A **bottom-up** approach was adopted: starting from low level parsing of raw data bytes, it was possible to package a LIN frame data structure that could be dispatched to the proper high-level handler.

## Modules
- **Parser**: takes a raw array of bytes and performs PID, checksum and format checks to ensure it abides by the standards of LIN protocol
- **Logger**: allows for a uniform logging process; handles different logging modules and can both output to a logfile and to stdout. It can be extended to implement a log rotation logic
- **Router**: default "frame handler", thus acting subsequent to a successful parsing of a LIN frame. It checks against the PID and routes the frame according to a predefined criteria.
- **Diagnostic**: performs basic diagnostic of the system's sensors. Has different levels of severity and different sensor values. It builds the log messare before forwarding it to the logger.
- **Tuner**: allows for tuning of the system's settings/parameters. Reads a parameter identifier and a measure value. It also logs the update.
- **FIFO interface**: useful for the interaction between the _sender_ and the _receiver_. Initializes a FIFO named pipe and opens it in read/write mode. Also defines two functions to send a frame and receive raw data. 

## Applications
### Receiver
Core of the application logic, parsing and logging. It runs as a continuous cycle program reading on the FIFO pipe.

Every time raw data is received, it forwards it to the parser module. Should the parsing be successful, the routing handles the proper dispatch of the frame.

The logger module captures log events coming from either the tuning or diagnostic module.
### Sender
Simpler application deployed as a "testing" tool for the receiver.

Its invocation interface is the following:

`lin_send id data1 ... data8`

The ID need not be a parity-ID, nor the checksum needs to be present.

Both the `id` and the `data` shall be inputted as a two character hex value. The sender parses such values as a `uint8_t` byte and a LIN frame is constructed (PID and checksum are automatically computed).

Finally, the sender uses FIFO's module to send the frame to the same named pipe used by the receiver.
