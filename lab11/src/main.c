#include "backpack.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int number_of_items, maximum_capacity;
  if (scanf("%d %d", &number_of_items, &maximum_capacity) != 2)
    return 1;

  int *weights = malloc(number_of_items * sizeof(int));
  int *values = malloc(number_of_items * sizeof(int));

  for (int item_index = 0; item_index < number_of_items; item_index++) {
    if (scanf("%d %d", &weights[item_index], &values[item_index]) != 2) {
      free(weights);
      free(values);
      return 0;
    }
  }

  int **maximum_value_by_item_and_weight =
      malloc((number_of_items + 1) * sizeof(int *));
  for (int item_index = 0; item_index <= number_of_items; item_index++) {
    maximum_value_by_item_and_weight[item_index] =
        calloc((maximum_capacity + 1), sizeof(int));
  }
  int *selected_item_indices = malloc(number_of_items * sizeof(int));
  int selected_items_count = 0;
  const int maximum_total_value = CalculateMaximumBackpackValue(
      maximum_value_by_item_and_weight, weights, values, number_of_items,
      maximum_capacity, selected_item_indices, &selected_items_count);
  printf("%d\n", maximum_total_value);

  PrintSelectedItems(selected_item_indices, selected_items_count, weights,
                     values);

  free(selected_item_indices);
  for (int item_index = 0; item_index <= number_of_items; item_index++) {
    free(maximum_value_by_item_and_weight[item_index]);
  }
  free(maximum_value_by_item_and_weight);
  free(weights);
  free(values);

  return 0;
}
