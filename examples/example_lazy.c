#include "../include/segment_tree.h"

int main() {
    printf("=== Segment Tree Lazy Propagation Example ===\n\n");

    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf(" (sum = %d)\n\n", 1+2+3+4+5+6+7+8);

    segment_tree_t *st = segtree_create_lazy(arr, n, SEGTREE_SUM);
    if (st == NULL) {
        printf("Failed to create lazy segment tree!\n");
        return 1;
    }

    printf("Lazy segment tree created successfully!\n");
    printf("Lazy propagation enabled: %s\n\n", st->lazy_enabled ? "Yes" : "No");

    int result;
    segtree_error_t error;

    printf("=== Initial Queries ===\n");

    error = segtree_query(st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [0, 7]: %d\n", result);
    }

    error = segtree_query(st, 2, 5, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [2, 5]: %d\n", result);
    }

    printf("\n=== Range Update Operations ===\n");

    printf("Adding 10 to range [1, 4]\n");
    error = segtree_update_range(st, 1, 4, 10);
    if (error != SEGTREE_OK) {
        printf("Range update failed!\n");
    }

    error = segtree_query(st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [0, 7] after update: %d\n", result);
    }

    error = segtree_query(st, 1, 4, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [1, 4] after update: %d\n", result);
    }

    error = segtree_query(st, 0, 0, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [0, 0] (unchanged): %d\n", result);
    }

    error = segtree_query(st, 5, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [5, 7] (unchanged): %d\n", result);
    }

    printf("\n=== Multiple Range Updates ===\n");

    printf("Adding 5 to range [0, 2]\n");
    segtree_update_range(st, 0, 2, 5);

    printf("Adding 3 to range [6, 7]\n");
    segtree_update_range(st, 6, 7, 3);

    error = segtree_query(st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [0, 7] after multiple updates: %d\n", result);
    }

    printf("\n=== Overlapping Range Updates ===\n");

    printf("Adding 2 to range [1, 5] (overlaps with previous updates)\n");
    segtree_update_range(st, 1, 5, 2);

    error = segtree_query(st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [0, 7] after overlapping update: %d\n", result);
    }

    printf("\n=== Detailed Range Analysis ===\n");
    printf("Index\tExpected\tActual\n");
    printf("-----\t--------\t------\n");

    for (int i = 0; i < n; i++) {
        int single_result;
        segtree_query(st, i, i, &single_result);

        int expected = arr[i];
        if (i >= 0 && i <= 2) expected += 5;
        if (i >= 1 && i <= 4) expected += 10;
        if (i >= 1 && i <= 5) expected += 2;
        if (i >= 6 && i <= 7) expected += 3;

        printf("%d\t%d\t\t%d\n", i, expected, single_result);
    }

    printf("\n=== Mixed Point and Range Operations ===\n");

    printf("Point update: setting index 3 to 100\n");
    segtree_update_point(st, 3, 100);

    error = segtree_query(st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [0, 7] after point update: %d\n", result);
    }

    error = segtree_query(st, 3, 3, &result);
    if (error == SEGTREE_OK) {
        printf("Value at index 3 after point update: %d\n", result);
    }

    printf("Range update: adding 1 to range [2, 4]\n");
    segtree_update_range(st, 2, 4, 1);

    error = segtree_query(st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Sum of range [0, 7] after final update: %d\n", result);
    }

    printf("\n=== Performance Analysis ===\n");

    clock_t start = clock();
    for (int i = 0; i < 1000; i++) {
        segtree_update_range(st, 0, n-1, 1);
    }
    clock_t end = clock();

    double range_update_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time for 1000 full-range updates: %.6f seconds\n", range_update_time);

    start = clock();
    for (int i = 0; i < 1000; i++) {
        int dummy_result;
        segtree_query(st, 0, n-1, &dummy_result);
    }
    end = clock();

    double query_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time for 1000 full-range queries: %.6f seconds\n", query_time);

    printf("\n=== Final Statistics ===\n");
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
    printf("\nLazy segment tree destroyed. Example completed!\n");

    return 0;
}