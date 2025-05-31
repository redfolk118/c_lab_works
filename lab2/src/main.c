#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 100
#define ZERO 0
#define NUMBER_OF_DIGITS 10
#define NO_ERRORS 0
#define NO_MORE_PERMUTATIONS -1

void PrintBadInput() {
    printf("bad input");
}

void ScanPermutations(char *permutations, int *has_errors) {
    if (fgets(permutations, 100, stdin) == NULL) {
        *has_errors += 1;
        return;
    }

    size_t len = strlen(permutations);
    permutations[len - 1] = '\0';
}

void ScanNumberOfPermutations(int *number_of_permutations, int *has_error) {
    if (scanf("%d", number_of_permutations) != 1) {
        *has_error += 1;
    }
    if (*number_of_permutations < 0) {
        *has_error += 1;
    }
}

bool IsCorrectPermutation(const char *permutation, int len) {
    if (len > 10) {
        return false;
    }

    int digit_count[NUMBER_OF_DIGITS] = {ZERO};
    for (int i = 0; i < len; i++) {
        if (permutation[i] < '0' || permutation[i] > '9') {
            return false;
        }
        int digit = permutation[i] - '0';
        digit_count[digit]++;
        if (digit_count[digit] > 1) {
            return false;
        }
    }

    return true;
}

void SwapChar(char *permutations, int first_element, int second_element) {
    char temp = permutations[first_element];
    permutations[first_element] = permutations[second_element];
    permutations[second_element] = temp;
}

void ReverseString(char *permutations, int left, int right) {
    while (left < right) {
        SwapChar(permutations, left, right);
        left++;
        right--;
    }
}

bool FindRightmostAscendingPair(char *permutation, int len, int *pivot_index) {
    for (int i = 0; i < len - 1; i++) {
        if (permutation[i] < permutation[i + 1]) {
            *pivot_index = i;
        }
    }
    if (*pivot_index == NO_MORE_PERMUTATIONS) {
        return false;
    }
    return true;
}

void FindRightMostLargerElement(char *permutations, int len, int *pivot_index,
        int *replacement_index) {
    for (int i = *pivot_index + 1; i < len; i++) {
        if (permutations[*pivot_index] < permutations[i]) {
            *replacement_index = i;
        }
    }
}

bool GetPermutations(char *permutations, int len) {
    int pivot_index = NO_MORE_PERMUTATIONS;
    int replacement_index = NO_MORE_PERMUTATIONS;
    if (!FindRightmostAscendingPair(permutations, len, &pivot_index)) {
        return false;
    }
    FindRightMostLargerElement(permutations, len, &pivot_index, &replacement_index);

    SwapChar(permutations, replacement_index, pivot_index);

    ReverseString(permutations, pivot_index + 1, len - 1);

    return true;
}

int main() {
    char permutations[MAX_SIZE];
    int number_of_permutations;
    int has_error = NO_ERRORS;

    ScanPermutations(permutations, &has_error);
    if (has_error != NO_ERRORS) {
        PrintBadInput();
        return 0;
    }

    int len = strlen(permutations);

    ScanNumberOfPermutations(&number_of_permutations, &has_error);
    if (has_error != NO_ERRORS) {
        PrintBadInput();
        return 0;
    }

    if (!IsCorrectPermutation(permutations, len)) {
        PrintBadInput();
        return 0;
    }

    for (int i = 0; i < number_of_permutations; i++) {
        if (!GetPermutations(permutations, len)) {
            return 0;
        }
        printf("%s\n", permutations);
    }
}
