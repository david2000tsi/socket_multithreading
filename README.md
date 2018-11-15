
# Sockets and Threads in C++

This is a basic implementation of sockets and threads in C++ language.

To compile project, in command line (linux environment), run the following commands:

Create build path:

```
cd <project_path>
mkdir build && cd build
```

Run cmake to generate makefiles annd make to compile:

```
cmake ..
make
```

Run main file:

```
cd <project_path>
./bin/<bin_file>
```

Use netcat to receive/send data to socket test (run main file before to start socket listening, you can adjust socket timeout in main file):

```
nc localhost 8888
```
