#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// Remove
size_t block_size = 1024;

// Legacy Allocator
typedef struct Block {
  void *address;
  size_t size;
  int8_t inUse;
  struct heapchunk_t *next;
} Block;

Block allocate(size_t size, int prot, int flags, int fd, off_t off) {
  Block block;
  block.size = size;
  block.address = mmap(NULL, size, prot, flags, fd, off);
  block.inUse = 1;

  if (block.address == (void *)-1) {
    perror("Error: Memory Mapping Failed");
    exit(EXIT_FAILURE);
  }
  return block;
}

struct heapinfo_t {
  struct heapchunk_t *start;
  uint32_t avail;
};

struct heapchunk_t {
  uint32_t size;
  bool inuse;
  struct heapchunk_t *next;
}







int main() {
  Block mem = allocate(getpagesize(), PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  printf("Memory add: %p", mem.address);

  int *alpha = (int *)mem.address;
  alpha[0] = 100;
  printf("\nValue is %d", *alpha++);

  printf("\nSize of allocated: %lu", mem.size);
  printf("PID: %d\n", getpid());
  return 0;
}
