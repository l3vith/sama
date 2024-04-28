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
static struct heapchunk *heap_init() {
  struct heapchunk *memloc;
  void *memadd = mmap(NULL, page, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (memadd == (void *)-1) {
    perror("Memory Allocation Failed");
  }

  struct heapchunk *first = (struct heapchunk *)memadd;
  first->size = page - sizeof(struct heapchunk);
  first->inUse = false;

  first->next = NULL;

  return first;
}

struct heapchunk *next_chunk(struct heapchunk *current) {
  if (current == NULL)
    return NULL;

  return (struct heapchunk
              *)((char *)current +
                 (sizeof(struct heapchunk) +
                  current->size)); // Current Memory Location + (Size Of
                                   // Structure Metadata + Size Of Chunk)
}

struct heapchunk *new_chunk(struct heapchunk *current, size_t size) {
  return (struct heapchunk *)((char *)current + (sizeof(struct heapchunk)) +
                              size);
}

struct heapchunk *heap_alloc(struct heapchunk *heap, size_t size) {
  struct heapchunk *current = heap;
  while (current != NULL) {
    if (!current->inUse && current->size >= size) {
      if (current->size > size + sizeof(struct heapchunk)) {
        struct heapchunk *newChunk = new_chunk(current, size);
        newChunk->size = current->size - size - sizeof(struct heapchunk);
        newChunk->inUse = false;
        newChunk->next = current->next;
        current->next = newChunk;
        current->size = size;
      }
      current->inUse = true;
      return current;
    }
    current = current->next;
  }
  return NULL;
}

int main() {
  struct heapchunk *heap = heap_init();

  if (heap == NULL) {
    perror("Failued to initialized heap\n");
    return EXIT_FAILURE;
  }

  int *ptr = (int *)(heap_alloc(heap, 4));
  *ptr = 10;
  printf("Value of ptr is: %d\n", *ptr);

  char *ptr2 = (char *)(heap_alloc(heap, 1));
  *ptr2 = 'c';
  printf("Value of ptr2 is: %c\n", *ptr2);

  int *array;
  array = (int *)heap_alloc(heap, 5 * sizeof(int));
  printf("\nEnter Elements Of Array: ");
  for (int i = 0; i < 5; i++) {
    scanf("%d", &array[i]);
  }

  printf("\nArray: ");
  for (int j = 0; j < 5; j++) {
    printf("%d ", array[j]);
  }

  return EXIT_SUCCESS;
}
