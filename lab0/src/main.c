#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_SIZE 13
#define MAX_SYSTEM 16
#define MIN_SYSTEM 2

const char *kSystemAlphabet = "0123456789abcdef";

void PrintBadInput(void) {
    printf("bad input\n");
}

void InputSystemBase(int *sys_base, int *error) {
    if (scanf("%13d", sys_base) != 1) {
        *error += 1;
    }
    if (*sys_base < MIN_SYSTEM || *sys_base > MAX_SYSTEM) {
        *error += 1;
    }
}

void ScanInputNumber(char *number, int number_size, int *error) {
    if (scanf("%13s", number) == EOF) {
        *error += 1;
    }
    number[number_size] = '\0';
}

void PrintOutputNumber(char *number_in_system_base) {
    int len = (int) strlen(number_in_system_base);
    char *place = strchr(number_in_system_base, '.');

    if (place != NULL) {
        int count = 0;
        for (int i = len - 1; i > 0; i--) {
            if (number_in_system_base[i] - '0' != 0) {
                count = i;
                break;
            }
        }
        for (int i = 0; i <= count; i++) {
            printf("%c", number_in_system_base[i]);
        }
    } else {
        printf("%s", number_in_system_base);
    }
}

double Pow(long long number, int pow) {
    long long res = 1.0;
    if (pow < 0) {
        return (1.0 / number) * Pow(number, pow + 1);
    } else
        while (pow != 0) {
            if (pow & 1)
                res = res * number;
            number = number * number;
            pow >>= 1;
        }
    return res;
}

int TakeIntPart(char *number, int *error) {
    char *point_position = strchr(number, '.');

    int len_of_number = (int) strlen(number);

    if (point_position == NULL) {
        return len_of_number;
    }
    int int_part = (int) (point_position - number);

    if (int_part == 0) {
        PrintBadInput();
        *error += 1;
    } else if (len_of_number - 1 == int_part) {
        PrintBadInput();
        *error += 1;
    }
    return int_part;
}

int FromCharToInt(int system_base, char symbol_of_number, int *error) {
    for (int i = 0; i < system_base; i++) {
        if (tolower(symbol_of_number) == kSystemAlphabet[i]) {
            return i;
        }
    }
    PrintBadInput();
    *error += 1;
    return -1;
}

double ConvertToDecimal(char *number, int system_base, int *error) {
    int int_size = TakeIntPart(number, error);
    if (*error != 0) {
        return 0;
    }
    double sum = 0.0;
    for (int i = 0; i < int_size; i++) {
        sum += FromCharToInt(system_base, number[i], error) * Pow(system_base, int_size - i - 1);
    }

    int len = (int) strlen(number);

    for (int i = int_size + 1; i < len; i++) {
        sum += FromCharToInt(system_base, number[i], error) * Pow(system_base, int_size - i);
    }
    return sum;
}

char *ConvertFromDecimal(double number, int sys_base2, int number_size, int *error) {
    long long integer = (long long) number;
    double float_part = number - integer;
    int int_size = 1;

    while (integer >= Pow(sys_base2, int_size)) {
        int_size++;
    }

    int line = int_size + number_size + 1;
    char *result_number = calloc(line, sizeof(char));

    if (result_number == NULL) {
        PrintBadInput();
        *error += 1;
    }

    for (int i = int_size - 1; i >= 0; i--) {
        result_number[i] = kSystemAlphabet[integer % sys_base2];
        integer /= sys_base2;
    }

    if (float_part > 0) {
        result_number[int_size] = '.';
        for (int i = 0; i < number_size - 1; i++) {
            float_part *= sys_base2;
            result_number[int_size + i + 1] = kSystemAlphabet[(int) float_part];
            float_part -= (int) float_part;
        }
    }
    return result_number;
}

char *ConvertNumber(char *number, int system_base1, int system_base2, int number_size, int *error) {
    double number_in_ten = ConvertToDecimal(number, system_base1, error);
    char *result_number = ConvertFromDecimal(number_in_ten, system_base2, number_size, error);
    return result_number;
}

int main(void) {
    int system_base1 = 0;
    int system_base2 = 0;
    int has_error = 0;
    char number[NUMBER_SIZE + 1] = {0};

    InputSystemBase(&system_base1, &has_error);
    InputSystemBase(&system_base2, &has_error);
    ScanInputNumber(number, NUMBER_SIZE, &has_error);

    if (has_error != 0) {
        PrintBadInput();
        return 0;
    }
    char *modified_number = ConvertNumber(number, system_base1, system_base2, NUMBER_SIZE, &has_error);
    if (has_error == 0) {
        PrintOutputNumber(modified_number);
    }
    free(modified_number);
}
