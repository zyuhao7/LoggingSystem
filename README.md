# LoggingSystem

A powerful, flexible logging library for C++ applications with support for multiple sinks, log levels, and custom formatting.

## Overview
LoggingSystem provides a comprehensive logging solution that allows developers to easily integrate logging into their C++ applications. The library supports various logging destinations (sinks), different log levels, and customizable log message formatting.

## Features
- **Multiple Log Levels**: Supports standard log levels (DEBUG, INFO, WARN, ERROR, FATAL)
- **Flexible Sinks**: Configure log output to different destinations (console, file, etc.)
- **Custom Formatting**: Customize log message format with timestamps, log levels, module names, etc.
- **Thread Safety**: Safe for use in multi-threaded environments
- **Performance Optimizations**: Efficient logging with minimal overhead

## Project Structure
```
LoggingSystem-master/
├── example/
│   ├── args.cc          # Example of command-line argument logging
│   ├── builder.cc       # Example of logger builder pattern usage
│   ├── proxy.cc         # Example of logging proxy usage
│   └── singleton.cc     # Example of singleton logger usage
├── logs/
│   ├── bench.h          # Benchmark utilities
│   ├── buffer.hpp       # Log message buffer implementation
│   ├── formatter.hpp    # Log message formatting
│   ├── level.hpp        # Log level definitions
│   ├── logMacro.h       # Logging macros
│   ├── logfile/         # File logging implementation
│   ├── logger.h         # Logger interface
│   ├── logger.hpp       # Logger implementation
│   ├── loggermanager.h  # Logger management
│   ├── looper.hpp       # Log processing loop
│   ├── message.hpp      # Log message structure
│   ├── sink.hpp         # Log sink interface
│   ├── test/            # Test cases
│   ├── test.cc          # Test entry point
│   └── util.hpp         # Utility functions
└── pic/
    └── compare.png      # Performance comparison chart
```

## Usage Examples

### Basic Logger Initialization
```cpp
#include "logs/logger.hpp"
#include "logs/level.hpp"
#include "logs/sink.hpp"

int main() {
    // Create a console sink
    auto console_sink = std::make_shared<ConsoleSink>();
    
    // Create logger with console sink and INFO level
    Logger logger("main");
    logger.addSink(console_sink);
    logger.setLevel(LogLevel::INFO);
    
    // Log messages
    logger.debug("Debug message");  // Won't be logged (below INFO level)
    logger.info("Program started");
    logger.warn("Low memory warning");
    logger.error("File not found");
    logger.fatal("Critical system failure");
    
    return 0;
}
```

### Using the Singleton Logger
```cpp
#include "logs/loggermanager.h"

int main() {
    // Get the global logger instance
    auto& logger = LoggerManager::getInstance().getLogger("singleton_example");
    
    // Configure logger
    logger.setLevel(LogLevel::DEBUG);
    
    // Log a message
    logger.debug("Singleton logger initialized");
    
    return 0;
}
```

## Building the Project
The project uses a Makefile for build configuration. To build the library and examples:

```bash
cd logs
make
```

## Running Tests
To run the test suite:

```bash
cd logs
./test_logger
```

## Performance
See the performance comparison chart in `pic/compare.png` for benchmark results against other logging libraries.

