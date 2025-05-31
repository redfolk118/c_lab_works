#include <stdlib.h>
#include <stdio.h>

#define MIN_ARRAY_SIZE 0

void PrintBadInput(void) {
    printf("bad input");
}

void ScanArraySize(int *array_size, int *errors) {
    if (scanf("%d", array_size) != 1) {
        *errors += 1;
        return;
    }
    if (*array_size < MIN_ARRAY_SIZE) {
        *errors += 1;
    }
}

void ScanArray(int *array, int array_size, int *errors) {
    for (int i = 0; i < array_size; i++)
        if (scanf("%d", &array[i]) == EOF) {
            *errors += 1;
        }
}

void Swap(int *first_element, int *second_element) {
    int temp = *first_element;
    *first_element = *second_element;
    *second_element = temp;
}

void Heapify(int array[], int array_size, int index_of_element) {
    int largest_element = index_of_element;
    int left_element = 2 * index_of_element + 1;
    int right_element = 2 * index_of_element + 2;

    if (left_element < array_size && array[left_element] > array[largest_element]) {
        largest_element = left_element;
    }
    if (right_element < array_size && array[right_element] > array[largest_element]) {
        largest_element = right_element;
    }
    if (largest_element != index_of_element) {
        Swap(&array[index_of_element], &array[largest_element]);
        Heapify(array, array_size, largest_element);
    }
}

void DoHeapSort(int array[], int array_size) {
    for (int i = array_size / 2 - 1; i >= 0; i--)
        Heapify(array, array_size, i);

    for (int i = array_size - 1; i >= 0; i--) {
        Swap(&array[0], &array[i]);
        Heapify(array, i, 0);
    }
}

void PrintOutputArray(int *array, int array_size) {
    for (int i = 0; i < array_size; i++) {
        printf("%d ", array[i]);
    }
}

int main(void) {
    int array_size = 0;
    int has_error = 0;
    ScanArraySize(&array_size, &has_error);
    int *array = (int *) calloc(array_size, sizeof(int));
    ScanArray(array, array_size, &has_error);

    if (has_error != 0) {
        PrintBadInput();
        free(array);
        return 0;
    }

    DoHeapSort(array, array_size);
    PrintOutputArray(array, array_size);

    free(array);
}
