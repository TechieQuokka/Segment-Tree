#include "../include/segment_tree.h"
#include <assert.h>

void test_create_and_destroy() {
    printf("Testing create and destroy...\n");

    int arr[] = {1, 2, 3, 4, 5};
    int n = 5;

    segment_tree_t *st = segtree_create(arr, n, SEGTREE_SUM);
    assert(st != NULL);
    assert(st->size == n);
    assert(st->op_type == SEGTREE_SUM);
    assert(st->lazy_enabled == false);

    segtree_destroy(st);
    printf("Create and destroy test passed!\n\n");
}

void test_sum_operations() {
    printf("Testing sum operations...\n");

    int arr[] = {1, 3, 5, 7, 9, 11};
    int n = 6;

    segment_tree_t *st = segtree_create(arr, n, SEGTREE_SUM);
    assert(st != NULL);

    int result;
    segtree_error_t error;

    error = segtree_query(st, 0, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 36);

    error = segtree_query(st, 1, 4, &result);
    assert(error == SEGTREE_OK);
    assert(result == 24);

    error = segtree_query(st, 2, 2, &result);
    assert(error == SEGTREE_OK);
    assert(result == 5);

    error = segtree_update_point(st, 2, 10);
    assert(error == SEGTREE_OK);

    error = segtree_query(st, 0, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 41);

    error = segtree_query(st, 2, 2, &result);
    assert(error == SEGTREE_OK);
    assert(result == 10);

    segtree_destroy(st);
    printf("Sum operations test passed!\n\n");
}

void test_min_operations() {
    printf("Testing min operations...\n");

    int arr[] = {8, 3, 12, 1, 6, 9};
    int n = 6;

    segment_tree_t *st = segtree_create(arr, n, SEGTREE_MIN);
    assert(st != NULL);

    int result;
    segtree_error_t error;

    error = segtree_query(st, 0, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 1);

    error = segtree_query(st, 0, 2, &result);
    assert(error == SEGTREE_OK);
    assert(result == 3);

    error = segtree_query(st, 3, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 1);

    error = segtree_update_point(st, 3, 15);
    assert(error == SEGTREE_OK);

    error = segtree_query(st, 0, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 3);

    error = segtree_query(st, 3, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 6);

    segtree_destroy(st);
    printf("Min operations test passed!\n\n");
}

void test_max_operations() {
    printf("Testing max operations...\n");

    int arr[] = {2, 7, 1, 9, 4, 6};
    int n = 6;

    segment_tree_t *st = segtree_create(arr, n, SEGTREE_MAX);
    assert(st != NULL);

    int result;
    segtree_error_t error;

    error = segtree_query(st, 0, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 9);

    error = segtree_query(st, 0, 2, &result);
    assert(error == SEGTREE_OK);
    assert(result == 7);

    error = segtree_query(st, 3, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 9);

    error = segtree_update_point(st, 3, 1);
    assert(error == SEGTREE_OK);

    error = segtree_query(st, 0, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 7);

    error = segtree_query(st, 3, 5, &result);
    assert(error == SEGTREE_OK);
    assert(result == 6);

    segtree_destroy(st);
    printf("Max operations test passed!\n\n");
}

void test_error_handling() {
    printf("Testing error handling...\n");

    int arr[] = {1, 2, 3};
    int n = 3;

    segment_tree_t *st = segtree_create(arr, n, SEGTREE_SUM);
    assert(st != NULL);

    int result;
    segtree_error_t error;

    error = segtree_query(NULL, 0, 2, &result);
    assert(error == SEGTREE_ERROR_NULL_POINTER);

    error = segtree_query(st, 0, 2, NULL);
    assert(error == SEGTREE_ERROR_NULL_POINTER);

    error = segtree_query(st, -1, 2, &result);
    assert(error == SEGTREE_ERROR_INVALID_RANGE);

    error = segtree_query(st, 0, 5, &result);
    assert(error == SEGTREE_ERROR_INVALID_RANGE);

    error = segtree_query(st, 2, 1, &result);
    assert(error == SEGTREE_ERROR_INVALID_RANGE);

    error = segtree_update_point(st, -1, 10);
    assert(error == SEGTREE_ERROR_INVALID_RANGE);

    error = segtree_update_point(st, 5, 10);
    assert(error == SEGTREE_ERROR_INVALID_RANGE);

    segtree_destroy(st);
    printf("Error handling test passed!\n\n");
}

void test_custom_operations() {
    printf("Testing custom operations...\n");

    int custom_combine(int a, int b) {
        return a * b;
    }

    int arr[] = {2, 3, 4, 5};
    int n = 4;

    segment_tree_t *st = segtree_create_custom(arr, n, custom_combine, 1);
    assert(st != NULL);

    int result;
    segtree_error_t error;

    error = segtree_query(st, 0, 3, &result);
    assert(error == SEGTREE_OK);
    assert(result == 120);

    error = segtree_query(st, 1, 2, &result);
    assert(error == SEGTREE_OK);
    assert(result == 12);

    error = segtree_update_point(st, 1, 6);
    assert(error == SEGTREE_OK);

    error = segtree_query(st, 0, 3, &result);
    assert(error == SEGTREE_OK);
    assert(result == 240);

    segtree_destroy(st);
    printf("Custom operations test passed!\n\n");
}

int main() {
    printf("=== Starting Basic Tests ===\n\n");

    test_create_and_destroy();
    test_sum_operations();
    test_min_operations();
    test_max_operations();
    test_error_handling();
    test_custom_operations();

    printf("=== All Basic Tests Passed! ===\n");
    return 0;
}