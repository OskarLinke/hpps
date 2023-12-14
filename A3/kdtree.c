#include "kdtree.h"
#include "sort.h"
#include "io.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

struct sort_env {
  int c;
  int d;
  double *points;
};

int cmp_indexes(const int *ip, const int *jp, struct sort_env* env) {
  int i = *ip;
  int j = *jp;
  double *x = &env->points[i*env->d];
  double *y = &env->points[j*env->d];
  int c = env->c;

  if (x[c] < y[c]) {
    return -1;
  } else if (x[c] == y[c]) {
    return 0;
  } else {
    return 1;
  }
}

struct node {
  // Index of this node's point in the corresponding 'indexes' array.
  int point_index;

  // Axis along which this node has been split.
  int axis;

  // The left child of the node; NULL if none.
  struct node *left;

  // The right child of the node; NULL if none.
  struct node *right;
};

struct kdtree {
  int d;
  const double *points;
  struct node* root;
};

struct node* kdtree_create_node(int d, const double *points, int depth, int n, int* indexes) {
  struct node* node = malloc(sizeof(struct node));
  int axis = (depth % d);
  node->axis = axis;

  // Create struct for sort indexes
  struct sort_env env;
  env.points = points;
  env.d = d;
  env.c = axis;

  if ( n > 2 ) {
  // Sort indexes on dim axis.
    hpps_quicksort(indexes, n, sizeof(int),
                     (int (*)(const void*, const void*, void*))cmp_indexes,
                     &env);
    //printf("Enteredd kdtree_create_node\n");

    int median = indexes[n/2];
    node->point_index = median;

    // Print node info
    //printf("Created node: Index=%d, Axis=%d\n", node->point_index, node->axis);

    // Recursively call left and right child
    node-> left = kdtree_create_node(d, points, depth +1, (n-n/2), &indexes[0]);
    node-> right = kdtree_create_node(d, points, depth +1, n/2, &indexes[n-n/2]);
    return node;
  }

  if ( n == 2 ) {
  // Sort indexes on dim axis.
      hpps_quicksort(indexes, n, sizeof(int),
                     (int (*)(const void*, const void*, void*))cmp_indexes,
                     &env);

    int median = indexes[n/2];
    node->point_index = median;

    // Print node info
    //printf("IN N == 2 ### Created node: Index=%d, Axis=%d\n", node->point_index, node->axis);

    node-> left = kdtree_create_node(d, points, depth +1, (n-n/2), &indexes[0]);
    node-> right = NULL;
    return node;
  }

  int median = indexes[0];
  node->point_index = median;

  // Print node info
  //printf("N SHOULD BE == 1 ### Created node: Index=%d, Axis=%d\n", node->point_index, node->axis);

  node-> left = NULL;
  node-> right = NULL;
  return node;
  
}

struct kdtree *kdtree_create(int d, int n, const double* points) {
  struct kdtree* tree = malloc(sizeof(struct kdtree));
  tree->d = d;
  tree->points = points;

  int* indexes = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    indexes[i] = i;
  }

  tree->root = kdtree_create_node(d, points, 0, n, indexes);
  free(indexes);

  return tree;
}

void kdtree_free_node(struct node *node) {
  if (node->left != NULL) {

  kdtree_free_node(node->left);
  }
  if (node->right != NULL) {
  kdtree_free_node(node->right);
  }
  free(node);
}

void kdtree_free(struct kdtree *tree) {
  kdtree_free_node(tree->root);
  free(tree);
}

//void kdtree_knn_node(const struct kdtree *tree, int k, const double* query,
//                     int *closest, double *radius,
//                     const struct node *node) {
//  assert(0);
//}
//
//int* kdtree_knn(const struct kdtree *tree, int k, const double* query) {
//  int* closest = malloc(k * sizeof(int));
//  double radius = INFINITY;
//
//  for (int i = 0; i < k; i++) {
//    closest[i] = -1;
//  }

//  kdtree_knn_node(tree, k, query, closest, &radius, tree->root);
//
//  return closest;
//}

void kdtree_svg_node(double scale, FILE *f, const struct kdtree *tree,
                     double x1, double y1, double x2, double y2,
                     const struct node *node) {
  if (node == NULL) {
    return;
  }

  double coord = tree->points[node->point_index*2+node->axis];
  if (node->axis == 0) {
    // Split the X axis, so vertical line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            coord*scale, y1*scale, coord*scale, y2*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, coord, y2,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    coord, y1, x2, y2,
                    node->right);
  } else {
    // Split the Y axis, so horizontal line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            x1*scale, coord*scale, x2*scale, coord*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, x2, coord,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    x1, coord, x2, y2,
                    node->right);
  }
}

void kdtree_svg(double scale, FILE* f, const struct kdtree *tree) {
  assert(tree->d == 2);
  kdtree_svg_node(scale, f, tree, 0, 0, 1, 1, tree->root);
}

//int main () {
  // PRINT VALUES OF THE TREES
//  return 0;
//}