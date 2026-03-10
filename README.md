# Cache Server

A lightweight in-memory key-value cache server written in **C++**, inspired by the design of Redis.  
This project demonstrates systems programming concepts such as **TCP networking, concurrency, command parsing, persistence, and background workers**.

The server allows multiple clients to connect over TCP and execute simple commands to store, retrieve, and manage cached data.

---

## Features

- In-memory key-value store
- TCP server supporting multiple clients
- Redis-style text command interface
- Key expiration (TTL)
- Background expiration cleanup
- Disk persistence (snapshot storage)
- Configurable server settings
- Logging system
- Server metrics via INFO command
- Graceful client disconnect
- Modular architecture

---

## Supported Commands

|Command|Description|
|---|---|
|`SET key value`|Store a value|
|`GET key`|Retrieve a value|
|`DEL key`|Delete a key|
|`EXPIRE key seconds`|Set expiration time|
|`TTL key`|Check remaining lifetime|
|`PING`|Health check|
|`INFO`|Server statistics|
|`SAVE`|Manually persist data|
|`QUIT`|Close client connection|

---

## Architecture

```
Client
   │
   ▼
TCP Server
   │
   ▼
Command Parser
   │
   ▼
Database Engine
   │
   ▼
Persistence Layer
```

The server accepts client connections, parses commands, interacts with the in-memory database, and optionally persists data to disk.

---

## Project Structure

```
cache-server/
│
├── src/
│   ├── main.cpp
│   ├── server.cpp
│   ├── database.cpp
│   ├── command_parser.cpp
│   └── persistence.cpp
│
├── include/
│   ├── server.h
│   ├── database.h
│   ├── command_parser.h
│   └── persistence.h
│
├── data/
│   └── dump.rdb
│
├── logs/
│
├── build/
│
├── Makefile
└── README.md
```

---

## Building the Project

Requirements

- C++17 compatible compiler
- Linux or Unix-like system
- `make`

Compile the server:

```
make
```

Run the server:

```
make run
```

The server starts on port **6379** by default.

---

## Connecting to the Server

You can connect using **netcat**:

```
nc localhost 6379
```

Example session:

```
SET name Alen
OK

GET name
Alen

EXPIRE name 10
1

TTL name
9

QUIT
BYE
```

---

## Persistence

The server periodically saves the in-memory database to disk.

Data is stored in:

```
data/dump.rdb
```

You can manually trigger a save using:

```
SAVE
```

---

## Version History

|Version|Description|
|---|---|
|v0.1.0|Core in-memory key-value store|
|v0.2.0|TCP server implementation|
|v0.3.0|Key expiration and TTL|
|v0.4.0|Disk persistence|
|v0.5.0|Multi-client support|
|v0.6.0|Command parser improvements|
|v0.7.0|Background expiration worker|
|v0.8.0|Configuration system|
|v0.9.0|Logging and INFO command|
|v1.0.0|Stable release|

---

## Learning Goals

This project was built to explore:

- Socket programming
- Concurrent server design
- In-memory data structures
- Background worker threads
- Persistence mechanisms
- Command parsing
- Systems-level C++ development

---

## Future Improvements

- RESP protocol compatibility
- Replication support
- Cluster mode
- Memory eviction policies
- Benchmarking tools

---

## License

MIT License
