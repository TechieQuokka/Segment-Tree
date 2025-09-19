#include "../include/segment_tree.h"
#include <assert.h>

void test_lazy_sum() {
    printf("Testing lazy propagation with sum...\n");

    int arr[] = {1, 2, 3, 4, 5, 6};
    int n = 6;

    segment_tree_t *st = segtree_create_lazy(arr, n, SEGTREE_SUM);
    assert(st != NULL);
    assert(st->lazy_enabled == true);

    int result;
    segtree_error_t error;

    error = segtree_query(st, 0, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 21);

    error = segtree_update_range(st, 1, 3, 5);
    assert(error == SEGTREE_OK);

    error = segtree_query(st, 0, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 36);

    error = segtree_query(st, 1, 3, &result);
    assert(error == SEGTREE_OK);
    assert(result == 24);

    error = segtree_query(st, 0, 0, &result);
    assert(error == SEGTREE_OK);
    assert(result == 1);

    error = segtree_query(st, 4, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 11);

    segtree_destroy(st);
    printf("Lazy sum test passed!\n\n");
}

void test_lazy_multiple_updates() {
    printf("Testing multiple lazy updates...\n");

    int arr[] = {0, 0, 0, 0, 0};
    int n = 5;

    segment_tree_t *st = segtree_create_lazy(arr, n, SEGTREE_SUM);
    assert(st != NULL);

    int result;
    segtree_error_t error;

    error = segtree_update_range(st, 0, 4, 1);
    assert(error == SEGTREE_OK);

    error = segtree_query(st, 0, 4, &result);
    assert(error == SEGTREE_OK);
    assert(result == 5);

    error = segtree_update_range(st, 1, 3, 2);
    assert(error == SEGTREE_OK);

    error = segtree_query(st, 0, 4, &result);
    assert(error == SEGTREE_OK);
    assert(result == 11);

    error = segtree_query(st, 1, 3, &result);
    assert(error == SEGTREE_OK);
    assert(result == 9);

    error = segtree_query(st, 0, 0, &result);
    assert(error == SEGTREE_OK);
    assert(result == 1);

    error = segtree_query(st, 4, 4, &result);
    assert(error == SEGTREE_OK);
    assert(result == 1);

    segtree_destroy(st);
    printf("Multiple lazy updates test passed!\n\n");
}

void test_lazy_mixed_operations() {
    printf("Testing mixed point and range operations...\n");

    int arr[] = {1, 1, 1, 1, 1};
    int n = 5;

    segment_tree_t *st = segtree_create_lazy(arr, n, SEGTREE_SUM);
    assert(st != NULL);

    int result;
    segtree_error_t error;

    error = segtree_query(st, 0, 4, &result);
    assert(error == SEGTREE_OK);
    assert(result == 5);

    error = segtree_update_point(st, 2, 10);
    assert(error == SEGTREE_OK);

    error = segtree_query(st, 0, 4, &result);
    assert(error == SEGTREE_OK);
    assert(result == 14);

    error = segtree_update_range(st, 1, 3, 5);
    assert(error == SEGTREE_OK);

    error = segtree_query(st, 0, 4, &result);
    assert(error == SEGTREE_OK);
    assert(result == 29);

    error = segtree_query(st, 2, 2, &result);
    assert(error == SEGTREE_OK);
    assert(result == 15);

    error = segtree_update_point(st, 0, 100);
    assert(error == SEGTREE_OK);

    error = segtree_query(st, 0, 4, &result);
    assert(error == SEGTREE_OK);
    assert(result == 128);

    segtree_destroy(st);
    printf("Mixed operations test passed!\n\n");
}

void test_lazy_error_handling() {
    printf("Testing lazy error handling...\n");

    int arr[] = {1, 2, 3};
    int n = 3;

    segment_tree_t *st = segtree_create(arr, n, SEGTREE_SUM);
    assert(st != NULL);
    assert(st->lazy_enabled == false);

    segtree_error_t error;

    error = segtree_update_range(st, 0, 2, 5);
    assert(error == SEGTREE_ERROR_INVALID_OPERATION);

    segtree_destroy(st);

    st = segtree_create_lazy(arr, n, SEGTREE_SUM);
    assert(st != NULL);

    error = segtree_update_range(st, -1, 2, 5);
    assert(error == SEGTREE_ERROR_INVALID_RANGE);

    error = segtree_update_range(st, 0, 5, 5);
    assert(error == SEGTREE_ERROR_INVALID_RANGE);

    error = segtree_update_range(st, 2, 1, 5);
    assert(error == SEGTREE_ERROR_INVALID_RANGE);

    segtree_destroy(st);
    printf("Lazy error handling test passed!\n\n");
}

int main() {
    printf("=== Starting Lazy Propagation Tests ===\n\n");

    test_lazy_sum();
    test_lazy_multiple_updates();
    test_lazy_mixed_operations();
    test_lazy_error_handling();

    printf("=== All Lazy Propagation Tests Passed! ===\n");
    return 0;
}