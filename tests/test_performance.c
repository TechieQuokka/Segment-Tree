#include "../include/segment_tree.h"
#include <assert.h>
#include <time.h>

#define PERFORMANCE_SIZE 100000
#define PERFORMANCE_QUERIES 10000

void generate_random_array(int *arr, int n, int max_val) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % max_val + 1;
    }
}

void test_construction_performance() {
    printf("Testing construction performance with %d elements...\n", PERFORMANCE_SIZE);

    int *arr = (int*)malloc(PERFORMANCE_SIZE * sizeof(int));
    generate_random_array(arr, PERFORMANCE_SIZE, 1000);

    clock_t start = clock();
    segment_tree_t *st = segtree_create(arr, PERFORMANCE_SIZE, SEGTREE_SUM);
    clock_t end = clock();

    assert(st != NULL);

    double construction_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Construction time: %.6f seconds\n", construction_time);

    segtree_destroy(st);
    free(arr);
    printf("Construction performance test passed!\n\n");
}

void test_query_performance() {
    printf("Testing query performance with %d queries...\n", PERFORMANCE_QUERIES);

    int arr[1000];
    generate_random_array(arr, 1000, 100);

    segment_tree_t *st = segtree_create(arr, 1000, SEGTREE_SUM);
    assert(st != NULL);

    clock_t start = clock();

    for (int i = 0; i < PERFORMANCE_QUERIES; i++) {
        int left = rand() % 1000;
        int right = rand() % 1000;
        if (left > right) {
            int temp = left;
            left = right;
            right = temp;
        }

        int result;
        segtree_error_t error = segtree_query(st, left, right, &result);
        assert(error == SEGTREE_OK);
    }

    clock_t end = clock();

    double query_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total query time: %.6f seconds\n", query_time);
    printf("Average query time: %.9f seconds\n", query_time / PERFORMANCE_QUERIES);

    segtree_stats_t stats = segtree_get_stats(st);
    printf("Statistics - Queries: %d, Total time: %.6f\n",
           stats.query_count, stats.total_query_time);

    segtree_destroy(st);
    printf("Query performance test passed!\n\n");
}

void test_update_performance() {
    printf("Testing update performance with %d updates...\n", PERFORMANCE_QUERIES);

    int arr[1000];
    generate_random_array(arr, 1000, 100);

    segment_tree_t *st = segtree_create(arr, 1000, SEGTREE_SUM);
    assert(st != NULL);

    clock_t start = clock();

    for (int i = 0; i < PERFORMANCE_QUERIES; i++) {
        int index = rand() % 1000;
        int value = rand() % 1000 + 1;

        segtree_error_t error = segtree_update_point(st, index, value);
        assert(error == SEGTREE_OK);
    }

    clock_t end = clock();

    double update_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total update time: %.6f seconds\n", update_time);
    printf("Average update time: %.9f seconds\n", update_time / PERFORMANCE_QUERIES);

    segtree_stats_t stats = segtree_get_stats(st);
    printf("Statistics - Updates: %d, Total time: %.6f\n",
           stats.update_count, stats.total_update_time);

    segtree_destroy(st);
    printf("Update performance test passed!\n\n");
}

void test_lazy_performance() {
    printf("Testing lazy propagation performance...\n");

    int arr[1000];
    generate_random_array(arr, 1000, 100);

    segment_tree_t *st = segtree_create_lazy(arr, 1000, SEGTREE_SUM);
    assert(st != NULL);

    clock_t start = clock();

    for (int i = 0; i < PERFORMANCE_QUERIES / 2; i++) {
        int left = rand() % 1000;
        int right = rand() % 1000;
        if (left > right) {
            int temp = left;
            left = right;
            right = temp;
        }

        int value = rand() % 100 + 1;
        segtree_error_t error = segtree_update_range(st, left, right, value);
        assert(error == SEGTREE_OK);

        left = rand() % 1000;
        right = rand() % 1000;
        if (left > right) {
            int temp = left;
            left = right;
            right = temp;
        }

        int result;
        error = segtree_query(st, left, right, &result);
        assert(error == SEGTREE_OK);
    }

    clock_t end = clock();

    double lazy_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total lazy operations time: %.6f seconds\n", lazy_time);

    segtree_stats_t stats = segtree_get_stats(st);
    printf("Statistics - Queries: %d, Updates: %d\n",
           stats.query_count, stats.update_count);

    segtree_destroy(st);
    printf("Lazy performance test passed!\n\n");
}

void test_memory_usage() {
    printf("Testing memory usage...\n");

    int sizes[] = {100, 1000, 10000, 100000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        int size = sizes[i];
        int *arr = (int*)malloc(size * sizeof(int));
        generate_random_array(arr, size, 1000);

        segment_tree_t *st = segtree_create(arr, size, SEGTREE_SUM);
        assert(st != NULL);

        size_t tree_memory = st->tree_size * sizeof(int);
        printf("Size: %d, Tree array size: %d, Memory usage: %zu bytes\n",
               size, st->tree_size, tree_memory);

        segtree_destroy(st);
        free(arr);
    }

    printf("Memory usage test passed!\n\n");
}

int main() {
    printf("=== Starting Performance Tests ===\n\n");

    test_construction_performance();
    test_query_performance();
    test_update_performance();
    test_lazy_performance();
    test_memory_usage();

    printf("=== All Performance Tests Completed! ===\n");
    return 0;
}