#include "../include/segment_tree.h"

int main() {
    printf("=== Segment Tree Sum Example ===\n\n");

    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    segment_tree_t *st = segtree_create(arr, n, SEGTREE_SUM);
    if (st == NULL) {
        printf("Failed to create segment tree!\n");
        return 1;
    }

    printf("Segment tree created successfully!\n");
    printf("Tree size: %d, Original array size: %d\n\n", st->tree_size, st->size);

    int result;
    segtree_error_t error;

    printf("=== Query Operations ===\n");

    error = segtree_query(st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [0, 7]: %d\n", result);
    }

    error = segtree_query(st, 2, 5, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [2, 5]: %d\n", result);
    }

    error = segtree_query(st, 3, 3, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [3, 3]: %d\n", result);
    }

    printf("\n=== Update Operations ===\n");

    printf("Updating index 3 from %d to 20\n", arr[3]);
    error = segtree_update_point(st, 3, 20);
    if (error != SEGTREE_OK) {
        printf("Update failed!\n");
    }

    error = segtree_query(st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [0, 7] after update: %d\n", result);
    }

    error = segtree_query(st, 3, 3, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [3, 3] after update: %d\n", result);
    }

    printf("\n=== Multiple Updates ===\n");

    printf("Updating index 0 to 10\n");
    segtree_update_point(st, 0, 10);

    printf("Updating index 7 to 25\n");
    segtree_update_point(st, 7, 25);

    error = segtree_query(st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Final sum of range [0, 7]: %d\n", result);
    }

    printf("\n=== Statistics ===\n");
    segtree_stats_t stats = segtree_get_stats(st);
    printf("Total queries performed: %d\n", stats.query_count);
    printf("Total updates performed: %d\n", stats.update_count);
    printf("Total query time: %.6f seconds\n", stats.total_query_time);
    printf("Total update time: %.6f seconds\n", stats.total_update_time);

    if (stats.query_count > 0) {
        printf("Average query time: %.9f seconds\n",
               stats.total_query_time / stats.query_count);
    }

    if (stats.update_count > 0) {
        printf("Average update time: %.9f seconds\n",
               stats.total_update_time / stats.update_count);
    }

    segtree_destroy(st);
    printf("\nSegment tree destroyed. Example completed!\n");

    return 0;
}