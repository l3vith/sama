#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

size_t page = 4096; // Minimum Allocation Size According To Page Size

struct heapchunk {
  uint32_t size;          // Size of chunk
  bool inUse;             // Chunk in use or not
  struct heapchunk *next; // Pointer to next chunk
};

struct heapinfo {
  struct heapchunk *start; // Start of heap
  uint32_t avail;          // Remaining memory available in heap
};

static void freeList(struct heapchunk *bp) {
  // Freeing Memory Mechanism ????
}

// Maybe change to struct with types ?
static struct heapchunk *heap_init(size_t memsize) {
  struct heapchunk *memloc;
  void *memadd = mmap(NULL, page, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (memadd == (void *)-1) {
    perror("Memory Allocation Failed");
    return errno; // Wrong implimentation but wtv
  }

  struct heapchunk *first = (struct heapchunk *)memadd;
  first->size = page - sizeof(struct heapchunk);
  first->inUse = false;

  return memloc;
}
