# EmbedKit_KhyathiKommareddy

## Candidate Details

Name: Khyathi Kommareddy

## Assignment

Embedded Developer Fresher Assessment

## Module

### Ring Buffer (ringbuf.c)

Implementation of a fixed-size circular buffer (8-byte capacity) for uint8_t data.

Features:
- Buffer initialization
- Write operation with overflow protection
- Read operation with underflow protection
- Count stored bytes
- Check full condition
- Check empty condition
- Circular wrap-around using bitwise AND optimization

## Build Instructions

Compile:

```bash
gcc -Wall -std=c99 ringbuf.c -o ringbuf
```

Run:

```bash
./ringbuf
```

## Expected Functionality

- FIFO data storage
- Full-buffer detection
- Empty-buffer detection
- Proper wrap-around behavior
- Demonstration sequence as specified in assignment
