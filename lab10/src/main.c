#include "AndrewAlgorithm.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 100000

int main(void) {
  int number_of_points;
  if (scanf("%d", &number_of_points) != 1) {
    return 0;
  }

  if (number_of_points < 0 || number_of_points > MAX_POINTS) {
    printf("bad number of points\n");
    return 0;
  }

  Point *points = malloc(sizeof(Point) * number_of_points);

  for (int point_index = 0; point_index < number_of_points; point_index++) {
    if (scanf("%d %d", &points[point_index].x_coord,
              &points[point_index].y_coord) != 2) {
      printf("bad number of lines\n");
      free(points);
      return 0;
    }
  }

  if (number_of_points < 3) {
    PrintConvexHull(number_of_points, points);
    free(points);
    return 0;
  }

  int convex_hull_size = 0;
  Point *convex_hull =
      BuildConvexHullByAndrew(points, number_of_points, &convex_hull_size);

  PrintConvexHull(convex_hull_size, convex_hull);

  free(points);
  free(convex_hull);
  return 0;
}
