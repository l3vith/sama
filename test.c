#include "alloc.h"

int main() {
  struct heapchunk *heap = heap_init();

  int *num = (int *)heap_alloc(heap, sizeof(int));
  int size;

  printf("Enter value of num: ");
  scanf("%d", num);
  printf("\nNumber is: %d", *num);

  printf("\nEnter size of array: ");
  scanf("%d", &size);
  int *arr;
  arr = (int *)heap_alloc(heap, sizeof(int) * size);

  for (int i = 0; i < size; i++) {
    printf("Enter [%d] element: ", i);
    scanf("%d", &arr[i]);
  }

  printf("\nArray is: ");
  for (int k = 0; k < size; k++) {
    printf("%d ", arr[k]);
  }

  return 0;
}
