#include "backpack.h"

#include <stdio.h>
#include <stdlib.h>

int CalculateMaximumBackpackValue(int **best_value_table, int *weights,
                                  int *values, int number_of_items,
                                  int maximum_capacity,
                                  int *selected_item_indices,
                                  int *selected_items_count) {
  for (int item_index = 1; item_index <= number_of_items; item_index++) {
    for (int current_capacity = 1; current_capacity <= maximum_capacity;
         current_capacity++) {
      if (current_capacity >= weights[item_index - 1]) {
        int value_without_current_item =
            best_value_table[item_index - 1][current_capacity];
        int value_with_current_item =
            best_value_table[item_index - 1]
                            [current_capacity - weights[item_index - 1]] +
            values[item_index - 1];
        best_value_table[item_index][current_capacity] =
            (value_with_current_item > value_without_current_item)
                ? value_with_current_item
                : value_without_current_item;
      } else {
        best_value_table[item_index][current_capacity] =
            best_value_table[item_index - 1][current_capacity];
      }
    }
  }
  for (int item_index = number_of_items, current_capacity = maximum_capacity;
       item_index > 0 && current_capacity > 0; item_index--) {
    if (best_value_table[item_index][current_capacity] !=
        best_value_table[item_index - 1][current_capacity]) {
      selected_item_indices[(*selected_items_count)++] = item_index - 1;
      current_capacity -= weights[item_index - 1];
    }
  }

  return best_value_table[number_of_items][maximum_capacity];
}

void PrintSelectedItems(int *selected_item_indices, int selected_items_count,
                        int *weights, int *values) {
  for (int i = selected_items_count - 1; i >= 0; i--) {
    const int original_index = selected_item_indices[i];
    printf("%d %d\n", weights[original_index], values[original_index]);
  }
}
