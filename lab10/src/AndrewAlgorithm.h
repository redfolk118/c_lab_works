#ifndef ANDREWALGORITHM_H
#define ANDREWALGORITHM_H

typedef struct Point {
  int x_coord;
  int y_coord;
} Point;

Point *BuildConvexHullByAndrew(const Point *input_points,
                               int number_of_input_points,
                               int *convex_hull_vertex_count);
void PrintConvexHull(int hull_vertex_count, const Point *hull_vertices);
#endif // ANDREWALGORITHM_H
