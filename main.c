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
};

int init_heap(struct heapinfo_t *heap) {
  // Allocating Page Size Memory (4096 ????)
  void *start = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (start == (void *)-1) {
    perror("Memory Allocation Failed");
    return (errno);
  }

  // Subtracting Struct Size from Page Size
  struct heapchunk_t *first = (struct heapchunk_t *)start;
  first->size = getpagesize() - sizeof(struct heapchunk_t);
  first->inuse = false;

  heap->start = first;
  // heap->start->next =
  //(struct heapchunk_t *)(start + sizeof(struct heapchunk_t) + first->size);
  heap->avail = first->size;
  return 0;
}

struct heapinfo_t heap = {0};

int *heap_alloc(size_t size) {
  if (size > heap.avail) {
    printf("Insufficient Size\n");
    return (void *)-1;
  }
  return 0;
}

int main() {
  if (init_heap(&heap) != errno) {
    printf("Failed to initialized heap\n");
    return -1;
  }

  printf("Allocation: %d", *heap_alloc(10));
  return 0;
}
