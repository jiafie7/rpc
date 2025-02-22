# rpc

This project is a high-performance **C++ RPC** framework designed for efficient communication between a client and server using epoll-based reactor networking model, data serialization/deserialization, and dynamic unpacking of variable-length arguments. It enables seamless function calls from clients to servers over the network, supporting efficient request handling and result return.

## 🚀 Features

- **Reactor-based Networking Model**: The server uses an epoll-based reactor pattern for highly efficient I/O multiplexing.
- **Data Serialization & Deserialization**: Supports serializing and deserializing complex data structures, enabling transparent communication between client and server.
- **Dynamic Function Invocation**: Functions are bound dynamically at runtime, allowing the server to handle remote function calls based on their names and arguments.
- **Variable-Length Argument Support**: The framework handles the packing and unpacking of variable-length arguments for flexible function calls.

## 📂 Core Components

### Function

The Function class handles the dynamic binding of function names to concrete function implementations. It also provides functionality to invoke the bound functions by deserializing the parameters and serializing the results.

- `bind(const std::string& name, F func)`: Binds a function name to a concrete function.
- `call(const std::string& name, DataStream& in, DataStream& out)`: Calls a bound function and handles data serialization/deserialization.

### Server

The Server class is the core server component that listens for incoming RPC calls and executes the requested functions. It can bind functions dynamically and handle incoming function calls by invoking the appropriate function.

- `start()`: Starts the server and begins accepting client requests.
- `bind(const std::string& name, F func)`: Binds a function to a name for remote calls.
- `call(const std::string& name, DataStream& in, DataStream& out)`: Invokes a function after deserializing the incoming data.

### Client

The Client class handles sending function calls to the server. It serializes the function name and arguments, sends them to the server, and then deserializes the results received from the server.

- `call(const std::string& name, const Args&... args)`: Serializes the function call and sends it to the server.

### DataStream

The DataStream class is responsible for serializing and deserializing data, ensuring compatibility between client and server. It supports a wide range of data types including primitive types, containers (e.g., vectors, lists), and custom types.

- `write()`: Serializes data to the stream.
- `read()`: Deserializes data from the stream.
- `show()`: Displays the current state of the data stream.
- `operator<<()` and `operator>>()`: Stream insertion (<<) and extraction (>>) operators for different data types.

## 🖥️ Getting Started

1. Clone the Repository

```sh
git clone https://github.com/jiafie7/rpc.git
cd rpc
```

2. Build the Project

```sh
# compile project
mkdir build
cd build
cmake ..
make

# start server
./server &

# start client
./client
```

3. Output

```sh
10
hello, Michael
NVIDIA
```

## 📂 Project Structure

```
storm/
├── config/ ─────────── configuration file
├── rpc/ ────────────── rpc server and client
├── serialize/ ──────── serialize and deserialize data
├── frame/ ──────────── encapsulate server
├── socket/ ─────────── epoll for IO multiplexing
├── thread/ ─────────── thread pool and task dispatcher
├── task/ ───────────── concrete task implementation
├── utility/ ────────── tool class
├── server.cpp ──────── server example usage
├── client.cpp ──────── client example usage
└── CMakeLists.txt ──── project compile and build file
```

## 📝 License

This project is licensed under the MIT License. See the LICENSE file for more information.

## 🤝 Contributing

Contributions, bug reports, and feature requests are welcome. Feel free to fork the repository, open issues, or submit pull requests.
