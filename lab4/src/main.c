#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_LEN 1500
#define EMPTY_STACK -1
#define TRUE 1
#define FALSE 0
#define ASCII_SIZE 256

typedef enum Errors {
  OK = 0,
  ERROR_DIVISION_BY_ZERO = 1
} Errors;

int IsValidChar(char current_char_number) {
  return isdigit(current_char_number) || current_char_number == '+' ||
         current_char_number == '-' || current_char_number == '*' ||
         current_char_number == '/' || current_char_number == '(' ||
         current_char_number == ')' || current_char_number == ' ';
}

void HandleOpenBracket(int *balance_number, int *prev_char_was_operator_number,
                       int *prev_char_was_bracket_number,
                       int *prev_char_was_space_number) {
  (*balance_number)++;
  *prev_char_was_operator_number = TRUE;
  *prev_char_was_bracket_number = TRUE;
  *prev_char_was_space_number = FALSE;
}

int HandleCloseBracket(int *balance_number,
                       int const *prev_char_was_bracket_number,
                       int *prev_char_was_operator_number,
                       int *prev_char_was_space_number) {
  (*balance_number)--;
  if (*balance_number < 0 || *prev_char_was_bracket_number) {
    return 0;
  }
  *prev_char_was_operator_number = FALSE;
  *prev_char_was_space_number = FALSE;
  return 1;
}

int HandleOperator(int *prev_char_was_operator_number,
                   int *prev_char_was_bracket_number,
                   int *prev_char_was_space_number) {
  *prev_char_was_bracket_number = FALSE;
  if (*prev_char_was_operator_number) {
    return 0;
  }
  *prev_char_was_operator_number = TRUE;
  *prev_char_was_space_number = FALSE;
  return 1;
}

int IsValidSyntax(const char *expression) {
  int balance_number = FALSE;
  int prev_char_was_operator_number = TRUE;
  int prev_char_was_bracket_number = FALSE;
  int prev_char_was_space_number = FALSE;

  for (int index_number = 0; expression[index_number] != '\0'; index_number++) {
    char current_char_number = expression[index_number];

    if (!IsValidChar(current_char_number)) {
      return 0;
    }

    if (current_char_number == ' ') {
      prev_char_was_space_number = TRUE;
    }

    if (current_char_number == '(') {
      HandleOpenBracket(&balance_number, &prev_char_was_operator_number,
                        &prev_char_was_bracket_number,
                        &prev_char_was_space_number);
    } else if (current_char_number == ')') {
      if (!HandleCloseBracket(&balance_number, &prev_char_was_bracket_number,
                              &prev_char_was_operator_number,
                              &prev_char_was_space_number)) {
        return 0;
      }
    }

    if (current_char_number == '+' || current_char_number == '-' ||
        current_char_number == '*' || current_char_number == '/') {
      if (!HandleOperator(&prev_char_was_operator_number,
                          &prev_char_was_bracket_number,
                          &prev_char_was_space_number)) {
        return 0;
      }
    } else if (isdigit(current_char_number)) {
      prev_char_was_bracket_number = FALSE;
      prev_char_was_operator_number = FALSE;
    }
  }

  if (prev_char_was_space_number != FALSE || balance_number != FALSE ||
      prev_char_was_operator_number) {
    return 0;
  }
  return 1;
}

void ProcessOperators(char operator_char_number, double *values_number,
                      int *top_number_index, int *has_errors) {
  if (*top_number_index < 1) {
    *top_number_index = EMPTY_STACK;
    return;
  }
  double operand_b_number = values_number[(*top_number_index)--];
  double operand_a_number = values_number[(*top_number_index)--];
  if (operator_char_number == '+') {
    values_number[++(*top_number_index)] = operand_a_number + operand_b_number;
  } else if (operator_char_number == '-') {
    values_number[++(*top_number_index)] = operand_a_number - operand_b_number;
  } else if (operator_char_number == '*') {
    values_number[++(*top_number_index)] = operand_a_number * operand_b_number;
  } else if (operator_char_number == '/') {
    if (operand_b_number == 0) {
      *has_errors = ERROR_DIVISION_BY_ZERO;
      *top_number_index = EMPTY_STACK;
      return;
    }
    values_number[++(*top_number_index)] = operand_a_number / operand_b_number;
  }
}

double CalculateExpression(const char *expression, double *calculation_result,
                           int *has_errors) {
  int index_number = 0;
  double values_number[MAX_LEN];
  char operators_number[MAX_LEN];
  int top_number_index = EMPTY_STACK, top_operator_index = EMPTY_STACK;

  while (expression[index_number] != '\0') {
    if (isspace(expression[index_number])) {
      index_number++;
      continue;
    }

    if (isdigit(expression[index_number])) {
      double value_number = 0;
      while (isdigit(expression[index_number])) {
        value_number = value_number * 10 + (expression[index_number] - '0');
        index_number++;
      }
      values_number[++top_number_index] = value_number;
    } else if (expression[index_number] == '(') {
      operators_number[++top_operator_index] = '(';
      index_number++;
    } else if (expression[index_number] == ')') {
      while (top_operator_index >= 0 &&
             operators_number[top_operator_index] != '(') {
        ProcessOperators(operators_number[top_operator_index--], values_number,
                         &top_number_index, has_errors);
      }
      top_operator_index--;
      index_number++;
    } else {
      while (top_operator_index >= 0 &&
             (((expression[index_number] == '+' ||
                expression[index_number] == '-') &&
               (operators_number[top_operator_index] == '*' ||
                operators_number[top_operator_index] == '/')) ||
              ((expression[index_number] == '+' ||
                expression[index_number] == '-') &&
               (operators_number[top_operator_index] == '+' ||
                operators_number[top_operator_index] == '-')))) {
        ProcessOperators(operators_number[top_operator_index--], values_number,
                         &top_number_index, has_errors);
      }
      operators_number[++top_operator_index] = expression[index_number++];
    }
  }

  while (top_operator_index >= 0) {
    if (top_number_index < 1) {
      return 0;
    }
    ProcessOperators(operators_number[top_operator_index--], values_number,
                     &top_number_index, has_errors);
  }

  if (top_number_index != 0) {
    return 0;
  }

  *calculation_result = values_number[top_number_index];
  return 1;
}

void RemoveIncorrectSymbolsFromExpression(char *expression,
                                          int *number_length) {
  bool symbols_to_remove[ASCII_SIZE] = {false};
  symbols_to_remove['\n'] = true;
  symbols_to_remove['"'] = true;

  int index_number = 0;
  for (int current_index_number = 0; current_index_number < *number_length;
       current_index_number++) {
    if (!symbols_to_remove[(unsigned char)expression[current_index_number]]) {
      expression[index_number++] = expression[current_index_number];
    }
  }
  expression[index_number] = '\0';
  *number_length = index_number;
}

int main() {
  char expression[MAX_LEN];
  double calculation_result = 0;
  int has_errors = OK;
  FILE *file = fopen("in.txt", "r");

  int number_length = fread(expression, sizeof(char), MAX_LEN, file);
  fclose(file);

  expression[number_length] = '\0';

  RemoveIncorrectSymbolsFromExpression(expression, &number_length);

  if (!IsValidSyntax(expression)) {
    printf("syntax error");
    return 0;
  }

  CalculateExpression(expression, &calculation_result, &has_errors);
  if (has_errors == ERROR_DIVISION_BY_ZERO) {
    printf("division by zero");
    return 0;
  }
  printf("%lf\n", calculation_result);

  return 0;
}
