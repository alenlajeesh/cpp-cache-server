# Changelog

All notable changes to **Cache Server** will be documented in this file.

This project follows **semantic versioning**.

---

# v0.1.0 – Initial Release

### Added

* Basic in-memory key-value store
* `SET` command
* `GET` command
* `DEL` command

### Summary

Initial prototype of Cache Server with core caching functionality.

---

# v0.2.0 – TCP Server

### Added

* TCP socket server
* Client connection handling
* Basic request-response communication

### Improved

* Network communication between client and server

---

# v0.3.0 – Key Expiration and TTL

### Added

* `EXPIRE` command
* `TTL` command
* Key expiration tracking system

### Improved

* Cache lifecycle management

---

# v0.4.0 – Disk Persistence

### Added

* Database snapshot saving
* Database loading at startup
* `dump.rdb` persistence file

### Improved

* Data durability across server restarts

---

# v0.5.0 – Multi-Client Support

### Added

* Multi-client TCP handling
* Thread-based client processing

### Improved

* Concurrent request handling
* Server scalability

---

# v0.6.0 – Command Parser Improvements

### Added

* Dedicated command parser module
* Cleaner separation between networking and database logic

### Improved

* Code modularity and maintainability

---

# v0.7.0 – Background Expiration Worker

### Added

* Background thread for automatic removal of expired keys

### Improved

* TTL reliability
* Memory cleanup efficiency

---

# v0.8.0 – Configuration System

### Added

* Configuration file support (`cache-server.conf`)
* Configurable server port
* Configurable data directory

### Improved

* Server configuration without recompilation

---

# v0.9.0 – Logging and Server Metrics

### Added

* Logging system for client connections and commands
* `INFO` command for server statistics
* Request logging for debugging and monitoring

### Improved

* Server observability and diagnostics

---

# v1.0.0 – Stable Release

### Added

* `SAVE` command for manual database persistence
* `QUIT` command for graceful client disconnection

### Improved

* Stability improvements for client handling
* Finalized project architecture


