#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define SEGTREE_MAX_SIZE 1000000
#define SEGTREE_INF 2147483647
#define SEGTREE_NEG_INF -2147483648

typedef enum {
    SEGTREE_OK = 0,
    SEGTREE_ERROR_NULL_POINTER,
    SEGTREE_ERROR_INVALID_RANGE,
    SEGTREE_ERROR_MEMORY_ALLOCATION,
    SEGTREE_ERROR_INVALID_OPERATION
} segtree_error_t;

typedef enum {
    SEGTREE_SUM,
    SEGTREE_MIN,
    SEGTREE_MAX,
    SEGTREE_CUSTOM
} segtree_operation_t;

typedef struct {
    int query_count;
    int update_count;
    double total_query_time;
    double total_update_time;
} segtree_stats_t;

typedef struct segment_tree {
    int *tree;
    int *lazy;
    int size;
    int tree_size;
    segtree_operation_t op_type;

    int (*combine)(int a, int b);
    void (*update_lazy)(struct segment_tree *st, int node, int start, int end, int val);
    void (*propagate)(struct segment_tree *st, int node, int start, int end);

    int identity;
    bool lazy_enabled;
    segtree_stats_t stats;
} segment_tree_t;

segment_tree_t* segtree_create(int *arr, int n, segtree_operation_t op);
segment_tree_t* segtree_create_custom(int *arr, int n,
                                     int (*combine)(int, int),
                                     int identity);
segment_tree_t* segtree_create_lazy(int *arr, int n, segtree_operation_t op);
void segtree_destroy(segment_tree_t *st);

segtree_error_t segtree_query(segment_tree_t *st, int left, int right, int *result);
segtree_error_t segtree_update_point(segment_tree_t *st, int index, int value);
segtree_error_t segtree_update_range(segment_tree_t *st, int left, int right, int value);
segtree_error_t segtree_rebuild(segment_tree_t *st, int *new_arr, int new_size);

void segtree_print(segment_tree_t *st);
bool segtree_validate(segment_tree_t *st);
segtree_stats_t segtree_get_stats(segment_tree_t *st);
void segtree_reset_stats(segment_tree_t *st);

int combine_sum(int a, int b);
int combine_min(int a, int b);
int combine_max(int a, int b);

void update_lazy_sum(segment_tree_t *st, int node, int start, int end, int val);
void update_lazy_min(segment_tree_t *st, int node, int start, int end, int val);
void update_lazy_max(segment_tree_t *st, int node, int start, int end, int val);

void propagate_sum(segment_tree_t *st, int node, int start, int end);
void propagate_min(segment_tree_t *st, int node, int start, int end);
void propagate_max(segment_tree_t *st, int node, int start, int end);

#endif