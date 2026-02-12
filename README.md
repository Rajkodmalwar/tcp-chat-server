# TCP Multi-Client Chat Server

A lightweight, real-time chat server built in C using TCP sockets, select() I/O multiplexing, and POSIX threads. Demonstrates concurrent client handling and low-level network programming.

## ✨ Features

- **Concurrent Handling**: Supports 10+ simultaneous clients using select() (no threads per client)
- **Message Broadcasting**: Real-time message relay to all connected users
- **Multi-threaded Client**: Non-blocking UI with separate send/receive threads
- **Clean Protocol**: Username registration and graceful connection handling

## 🚀 Quick Start

### Build
\\\ash
# Using Make
make

# Or manually
gcc -pthread -o server server.c
gcc -pthread -o client client.c
\\\

### Run
\\\ash
# Terminal 1 - Start server
./server

# Terminal 2, 3, 4... - Connect clients
./client
\\\

## 🏗️ How It Works

**Server Architecture:**
- Uses \select()\ to monitor multiple client sockets simultaneously
- Single-threaded event loop for efficient I/O handling
- Broadcasts messages to all clients except sender

**Client Architecture:**
- Main thread: Handles user input and sending
- Receive thread (pthread): Continuously listens for incoming messages

**Protocol:**
1. Client connects → sends username
2. Server adds client to active list
3. Messages broadcast to all other clients
4. Disconnect handled gracefully

## 🛠️ Technical Implementation

### Key System Calls
\\\c
socket()   // Create TCP socket
bind()     // Bind to port 8080
listen()   // Mark as passive socket
accept()   // Accept client connections
select()   // Monitor multiple file descriptors
recv()     // Receive data
send()     // Send data
\\\

### Core Concepts Demonstrated
- TCP/IP socket programming
- I/O multiplexing with \select()\
- POSIX thread management
- Client-server protocol design
- Error handling and resource cleanup

## 📊 Example Output

**Server:**
\\\
Chat server started on port 8080
Waiting for connections...
New client connected: Alice [fd: 4]
New client connected: Bob [fd: 5]
Alice: Hello everyone!
Bob: Hi Alice!
\\\

**Client (Alice):**
\\\
Enter your name: Alice
Connected to server. Start chatting!
Welcome to chat server, Alice! (Active users: 1)
Hello everyone!
[Bob]: Hi Alice!
\\\

## 🧩 File Structure

\\\
tcp-chat-server/
├── server.c          # TCP server implementation
├── client.c          # Multi-threaded client
├── Makefile          # Build automation
└── README.md         # This file
\\\

## 💡 Learning Outcomes

This project demonstrates:
- Low-level socket programming (\socket\, \ind\, \listen\, \ccept\, \connect\)
- Concurrent I/O handling with \select()\
- Multi-threading with pthread
- Network protocol design
- System programming best practices

## 🔍 Why select()?

**select() advantages:**
- Single-threaded → lower memory overhead
- No context switching between threads
- Easier to debug and maintain
- Perfect for I/O-bound operations

**Scalability note:** For >1000 clients, use \epoll()\ (Linux) or \kqueue()\ (BSD)

## 🚧 Potential Enhancements

- [ ] Private messaging between users
- [ ] Chat rooms/channels
- [ ] Message persistence
- [ ] TLS/SSL encryption
- [ ] Upgrade to \epoll()\ for better scalability

## 📝 Technical Specifications

- **Language:** C (C99)
- **Platform:** Linux/Unix/macOS
- **Protocol:** TCP/IPv4
- **Port:** 8080
- **Max Clients:** 10 (configurable)
- **Buffer Size:** 1024 bytes

## 🎯 Use Cases

- Understanding socket programming fundamentals
- Learning concurrent I/O patterns
- Building networked applications
- Embedded systems communication
- IoT device messaging

## 🤝 Author

**Raj Kodmalwar**
- GitHub: [@Rajkodmalwar](https://github.com/Rajkodmalwar)
- Email: rajkodmalwar.in@gmail.com

## 📄 License

MIT License - Feel free to use for learning and projects!

---

⭐ **Star this repo if you found it helpful!**
