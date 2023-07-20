Hi I am Aditya Singh and this is my C++, Pthreads project.
## Description
This is a simple P2P (peer-to-peer) file sharing application that supports concurrent downloading. Peers can download different parts of a shared file simultaneously, making the download process faster.

## Features
- Concurrent downloading from multiple peers
- Simulated network latency and random data transmission delays
- Combined downloaded parts into a single file

## Dependencies
- C++ Compiler (Supporting C++11 or later)

## Installation
1. Clone the repository: `git clone https://github.com/yourusername/mini-torrent.git`
2. Navigate to the project directory: `cd mini-torrent`
3. Compile the code: `g++ -o main main.cpp -pthread`

## Usage
1. Start the mini-torrent: `./main`
2. Enter the number of peers when prompted.
3. Observe the downloading progress.
4. After the download completes, check the `downloaded_file.txt` for the combined content.

## Code Example
```cpp
// Your main.cpp code example here
