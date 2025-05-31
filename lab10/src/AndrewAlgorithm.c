#include "AndrewAlgorithm.h"

#include <stdio.h>
#include <stdlib.h>

static int ComparePointsByXThenY(const void *a, const void *b) {
  const Point *first = a;
  const Point *second = b;
  if (first->x_coord < second->x_coord)
    return -1;
  if (first->x_coord > second->x_coord)
    return +1;
  if (first->y_coord < second->y_coord)
    return -1;
  if (first->y_coord > second->y_coord)
    return +1;
  return 0;
}

static int GetVectorProductSign(const Point *origin, const Point *first,
                                const Point *second) {
  long double delta_x1 = (long double)first->x_coord - origin->x_coord;
  long double delta_y1 = (long double)first->y_coord - origin->y_coord;
  long double delta_x2 = (long double)second->x_coord - origin->x_coord;
  long double delta_y2 = (long double)second->y_coord - origin->y_coord;

  long double vector_product = delta_x1 * delta_y2 - delta_y1 * delta_x2;
  if (vector_product > 0)
    return +1;
  if (vector_product < 0)
    return -1;
  return 0;
}

Point *BuildConvexHullByAndrew(const Point *input_points,
                               int number_of_input_points,
                               int *convex_hull_vertex_count) {
  qsort((void *)input_points, number_of_input_points, sizeof(Point),
        ComparePointsByXThenY);
  Point *convex_hull_vertices =
      malloc(sizeof(Point) * 2 * number_of_input_points);
  int current_hull_size = 0;

  for (int input_index = 0; input_index < number_of_input_points;
       input_index++) {
    const Point *current_point = &input_points[input_index];
    while (current_hull_size >= 2) {
      const Point *second_last_vertex =
          &convex_hull_vertices[current_hull_size - 2];
      const Point *last_vertex = &convex_hull_vertices[current_hull_size - 1];
      if (GetVectorProductSign(second_last_vertex, last_vertex,
                               current_point) <= 0) {
        current_hull_size--;
        continue;
      }
      break;
    }
    convex_hull_vertices[current_hull_size++] = *current_point;
  }

  const int already_in_hull_vertices_count = current_hull_size;

  for (int input_index = number_of_input_points - 2; input_index >= 0;
       input_index--) {
    const Point *current_point = &input_points[input_index];
    while (current_hull_size >= already_in_hull_vertices_count + 1) {
      const Point *second_last_vertex =
          &convex_hull_vertices[current_hull_size - 2];
      const Point *last_vertex = &convex_hull_vertices[current_hull_size - 1];
      if (GetVectorProductSign(second_last_vertex, last_vertex,
                               current_point) <= 0) {
        current_hull_size--;
        continue;
      }
      break;
    }
    convex_hull_vertices[current_hull_size++] = *current_point;
  }

  if (current_hull_size > 1) {
    current_hull_size--;
  }

  *convex_hull_vertex_count = current_hull_size;
  return convex_hull_vertices;
}

void PrintConvexHull(int hull_vertex_count, const Point *hull_vertices) {
  for (int output_index = 0; output_index < hull_vertex_count; output_index++) {
    printf("%d %d\n", hull_vertices[output_index].x_coord,
           hull_vertices[output_index].y_coord);
  }
}
