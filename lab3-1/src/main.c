#include <stdio.h>
#include <stdlib.h>

void Swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int TakeMedianOfThree(int arr[], int start, int end) {
    int mid = start + (end - start) / 2;
    if (arr[start] > arr[mid]) {
        Swap(&arr[start], &arr[mid]);
    }
    if (arr[start] > arr[end]) {
        Swap(&arr[start], &arr[end]);
    }
    if (arr[mid] > arr[end]) {
        Swap(&arr[mid], &arr[end]);
    }
    return mid;
}

void SplitArrayByPivot(int arr[], int start, int end, int *left,
                           int *right) {
    int pivot_index = TakeMedianOfThree(arr, start, end);
    int pivot = arr[pivot_index];
    Swap(&arr[pivot_index], &arr[end]);
    *left = start;
    *right = end;

    int i = start;
    while (i <= *right) {
        if (arr[i] < pivot) {
            Swap(&arr[i], &arr[*left]);
            (*left)++;
            i++;
        } else if (arr[i] > pivot) {
            Swap(&arr[i], &arr[*right]);
            (*right)--;
        } else {
            i++;
        }
    }
}

void DoQuickSort(int arr[], int start, int end) {
    if (start < end) {
        int left_index = 0, right_index = 0;
        SplitArrayByPivot(arr, start, end, &left_index, &right_index);

        DoQuickSort(arr, start, left_index - 1);
        DoQuickSort(arr, right_index + 1, end);
    }
}

void ScanArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            free(arr);
            return;
        }
    }
}

void PrintArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int size;
    if (scanf("%d", &size) != 1 || size < 0) {
        return 0;
    }

    int *arr = (int *) malloc(size * sizeof(int));
    if (!arr) {
        free(arr);
        return 1;
    }

    ScanArray(arr, size);

    DoQuickSort(arr, 0, size - 1);

    PrintArray(arr, size);

    free(arr);
    return 0;
}
