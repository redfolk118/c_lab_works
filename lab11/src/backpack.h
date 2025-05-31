#ifndef BACKPACK_H
#define BACKPACK_H
int CalculateMaximumBackpackValue(int **best_value_table, int *weights,
                                  int *values, int number_of_items,
                                  int maximum_capacity,
                                  int *selected_item_indices,
                                  int *selected_items_count);
void PrintSelectedItems(int *selected_item_indices, int selected_items_count,
                        int *weights, int *values);
#endif
