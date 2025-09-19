#include "../include/segment_tree.h"

int main() {
    printf("=== Segment Tree Min/Max Example ===\n\n");

    int arr[] = {8, 3, 12, 1, 6, 9, 15, 2};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    printf("=== Testing MIN operations ===\n");

    segment_tree_t *min_st = segtree_create(arr, n, SEGTREE_MIN);
    if (min_st == NULL) {
        printf("Failed to create min segment tree!\n");
        return 1;
    }

    int result;
    segtree_error_t error;

    error = segtree_query(min_st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Min of range [0, 7]: %d\n", result);
    }

    error = segtree_query(min_st, 2, 5, &result);
    if (error == SEGTREE_OK) {
        printf("Min of range [2, 5]: %d\n", result);
    }

    error = segtree_query(min_st, 0, 2, &result);
    if (error == SEGTREE_OK) {
        printf("Min of range [0, 2]: %d\n", result);
    }

    printf("Updating index 3 from %d to 20\n", arr[3]);
    segtree_update_point(min_st, 3, 20);

    error = segtree_query(min_st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Min of range [0, 7] after update: %d\n", result);
    }

    error = segtree_query(min_st, 2, 5, &result);
    if (error == SEGTREE_OK) {
        printf("Min of range [2, 5] after update: %d\n", result);
    }

    printf("\n=== Testing MAX operations ===\n");

    segment_tree_t *max_st = segtree_create(arr, n, SEGTREE_MAX);
    if (max_st == NULL) {
        printf("Failed to create max segment tree!\n");
        segtree_destroy(min_st);
        return 1;
    }

    error = segtree_query(max_st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Max of range [0, 7]: %d\n", result);
    }

    error = segtree_query(max_st, 2, 5, &result);
    if (error == SEGTREE_OK) {
        printf("Max of range [2, 5]: %d\n", result);
    }

    error = segtree_query(max_st, 0, 2, &result);
    if (error == SEGTREE_OK) {
        printf("Max of range [0, 2]: %d\n", result);
    }

    printf("Updating index 6 from %d to 1\n", arr[6]);
    segtree_update_point(max_st, 6, 1);

    error = segtree_query(max_st, 0, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Max of range [0, 7] after update: %d\n", result);
    }

    error = segtree_query(max_st, 5, 7, &result);
    if (error == SEGTREE_OK) {
        printf("Max of range [5, 7] after update: %d\n", result);
    }

    printf("\n=== Comparative Queries ===\n");

    printf("Index\tValue\tMin[0,i]\tMax[0,i]\n");
    printf("-----\t-----\t--------\t--------\n");

    for (int i = 0; i < n; i++) {
        int min_result, max_result;

        segtree_query(min_st, 0, i, &min_result);
        segtree_query(max_st, 0, i, &max_result);

        printf("%d\t%d\t%d\t\t%d\n", i, arr[i], min_result, max_result);
    }

    printf("\n=== Finding specific ranges ===\n");

    printf("Looking for ranges where min >= 5:\n");
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int min_val;
            segtree_query(min_st, i, j, &min_val);
            if (min_val >= 5) {
                printf("Range [%d, %d]: min = %d\n", i, j, min_val);
            }
        }
    }

    printf("\nLooking for ranges where max <= 10:\n");
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int max_val;
            segtree_query(max_st, i, j, &max_val);
            if (max_val <= 10) {
                printf("Range [%d, %d]: max = %d\n", i, j, max_val);
            }
        }
    }

    printf("\n=== Statistics ===\n");
    segtree_stats_t min_stats = segtree_get_stats(min_st);
    segtree_stats_t max_stats = segtree_get_stats(max_st);

    printf("MIN Tree - Queries: %d, Updates: %d\n",
           min_stats.query_count, min_stats.update_count);
    printf("MAX Tree - Queries: %d, Updates: %d\n",
           max_stats.query_count, max_stats.update_count);

    segtree_destroy(min_st);
    segtree_destroy(max_st);

    printf("\nBoth segment trees destroyed. Example completed!\n");

    return 0;
}