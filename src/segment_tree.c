#include "../include/segment_tree.h"

static int calculate_tree_size(int n) {
    int size = 1;
    while (size < n) {
        size *= 2;
    }
    return size * 2;
}

static void build_tree(segment_tree_t *st, int *arr, int node, int start, int end) {
    if (start == end) {
        st->tree[node] = arr[start];
    } else {
        int mid = (start + end) / 2;
        build_tree(st, arr, 2 * node, start, mid);
        build_tree(st, arr, 2 * node + 1, mid + 1, end);
        st->tree[node] = st->combine(st->tree[2 * node], st->tree[2 * node + 1]);
    }
}

static void setup_operations(segment_tree_t *st, segtree_operation_t op) {
    switch (op) {
        case SEGTREE_SUM:
            st->combine = combine_sum;
            st->update_lazy = update_lazy_sum;
            st->propagate = propagate_sum;
            st->identity = 0;
            break;
        case SEGTREE_MIN:
            st->combine = combine_min;
            st->update_lazy = update_lazy_min;
            st->propagate = propagate_min;
            st->identity = SEGTREE_INF;
            break;
        case SEGTREE_MAX:
            st->combine = combine_max;
            st->update_lazy = update_lazy_max;
            st->propagate = propagate_max;
            st->identity = SEGTREE_NEG_INF;
            break;
        default:
            st->combine = combine_sum;
            st->update_lazy = update_lazy_sum;
            st->propagate = propagate_sum;
            st->identity = 0;
            break;
    }
}

segment_tree_t* segtree_create(int *arr, int n, segtree_operation_t op) {
    if (arr == NULL || n <= 0 || n > SEGTREE_MAX_SIZE) {
        return NULL;
    }

    segment_tree_t *st = (segment_tree_t*)malloc(sizeof(segment_tree_t));
    if (st == NULL) {
        return NULL;
    }

    st->size = n;
    st->tree_size = calculate_tree_size(n);
    st->op_type = op;
    st->lazy_enabled = false;

    st->tree = (int*)calloc(st->tree_size, sizeof(int));
    if (st->tree == NULL) {
        free(st);
        return NULL;
    }

    st->lazy = NULL;

    setup_operations(st, op);

    memset(&st->stats, 0, sizeof(segtree_stats_t));

    build_tree(st, arr, 1, 0, n - 1);

    return st;
}

segment_tree_t* segtree_create_custom(int *arr, int n,
                                     int (*combine)(int, int),
                                     int identity) {
    if (arr == NULL || n <= 0 || n > SEGTREE_MAX_SIZE || combine == NULL) {
        return NULL;
    }

    segment_tree_t *st = (segment_tree_t*)malloc(sizeof(segment_tree_t));
    if (st == NULL) {
        return NULL;
    }

    st->size = n;
    st->tree_size = calculate_tree_size(n);
    st->op_type = SEGTREE_CUSTOM;
    st->lazy_enabled = false;

    st->tree = (int*)calloc(st->tree_size, sizeof(int));
    if (st->tree == NULL) {
        free(st);
        return NULL;
    }

    st->lazy = NULL;
    st->combine = combine;
    st->update_lazy = NULL;
    st->propagate = NULL;
    st->identity = identity;

    memset(&st->stats, 0, sizeof(segtree_stats_t));

    build_tree(st, arr, 1, 0, n - 1);

    return st;
}

segment_tree_t* segtree_create_lazy(int *arr, int n, segtree_operation_t op) {
    segment_tree_t *st = segtree_create(arr, n, op);
    if (st == NULL) {
        return NULL;
    }

    st->lazy_enabled = true;
    st->lazy = (int*)calloc(st->tree_size, sizeof(int));
    if (st->lazy == NULL) {
        free(st->tree);
        free(st);
        return NULL;
    }

    for (int i = 0; i < st->tree_size; i++) {
        st->lazy[i] = st->identity;
    }

    return st;
}

void segtree_destroy(segment_tree_t *st) {
    if (st == NULL) {
        return;
    }

    if (st->tree != NULL) {
        free(st->tree);
    }
    if (st->lazy != NULL) {
        free(st->lazy);
    }
    free(st);
}

static int query_recursive(segment_tree_t *st, int node, int start, int end, int l, int r) {
    if (r < start || end < l) {
        return st->identity;
    }

    if (st->lazy_enabled && st->propagate != NULL) {
        st->propagate(st, node, start, end);
    }

    if (l <= start && end <= r) {
        return st->tree[node];
    }

    int mid = (start + end) / 2;
    int left_result = query_recursive(st, 2 * node, start, mid, l, r);
    int right_result = query_recursive(st, 2 * node + 1, mid + 1, end, l, r);

    return st->combine(left_result, right_result);
}

segtree_error_t segtree_query(segment_tree_t *st, int left, int right, int *result) {
    if (st == NULL || result == NULL) {
        return SEGTREE_ERROR_NULL_POINTER;
    }

    if (left < 0 || right >= st->size || left > right) {
        return SEGTREE_ERROR_INVALID_RANGE;
    }

    clock_t start_time = clock();

    *result = query_recursive(st, 1, 0, st->size - 1, left, right);

    clock_t end_time = clock();
    st->stats.query_count++;
    st->stats.total_query_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;

    return SEGTREE_OK;
}

static void update_point_recursive(segment_tree_t *st, int node, int start, int end, int idx, int val) {
    if (start == end) {
        st->tree[node] = val;
    } else {
        int mid = (start + end) / 2;
        if (idx <= mid) {
            update_point_recursive(st, 2 * node, start, mid, idx, val);
        } else {
            update_point_recursive(st, 2 * node + 1, mid + 1, end, idx, val);
        }
        st->tree[node] = st->combine(st->tree[2 * node], st->tree[2 * node + 1]);
    }
}

segtree_error_t segtree_update_point(segment_tree_t *st, int index, int value) {
    if (st == NULL) {
        return SEGTREE_ERROR_NULL_POINTER;
    }

    if (index < 0 || index >= st->size) {
        return SEGTREE_ERROR_INVALID_RANGE;
    }

    clock_t start_time = clock();

    update_point_recursive(st, 1, 0, st->size - 1, index, value);

    clock_t end_time = clock();
    st->stats.update_count++;
    st->stats.total_update_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;

    return SEGTREE_OK;
}

static void update_range_recursive(segment_tree_t *st, int node, int start, int end, int l, int r, int val) {
    if (st->lazy_enabled && st->propagate != NULL) {
        st->propagate(st, node, start, end);
    }

    if (start > r || end < l) {
        return;
    }

    if (start >= l && end <= r) {
        if (st->lazy_enabled && st->update_lazy != NULL) {
            st->update_lazy(st, node, start, end, val);
        } else {
            st->tree[node] = val;
        }
        return;
    }

    int mid = (start + end) / 2;
    update_range_recursive(st, 2 * node, start, mid, l, r, val);
    update_range_recursive(st, 2 * node + 1, mid + 1, end, l, r, val);

    if (st->lazy_enabled && st->propagate != NULL) {
        st->propagate(st, 2 * node, start, mid);
        st->propagate(st, 2 * node + 1, mid + 1, end);
    }

    st->tree[node] = st->combine(st->tree[2 * node], st->tree[2 * node + 1]);
}

segtree_error_t segtree_update_range(segment_tree_t *st, int left, int right, int value) {
    if (st == NULL) {
        return SEGTREE_ERROR_NULL_POINTER;
    }

    if (!st->lazy_enabled) {
        return SEGTREE_ERROR_INVALID_OPERATION;
    }

    if (left < 0 || right >= st->size || left > right) {
        return SEGTREE_ERROR_INVALID_RANGE;
    }

    clock_t start_time = clock();

    update_range_recursive(st, 1, 0, st->size - 1, left, right, value);

    clock_t end_time = clock();
    st->stats.update_count++;
    st->stats.total_update_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;

    return SEGTREE_OK;
}

segtree_error_t segtree_rebuild(segment_tree_t *st, int *new_arr, int new_size) {
    if (st == NULL || new_arr == NULL) {
        return SEGTREE_ERROR_NULL_POINTER;
    }

    if (new_size <= 0 || new_size > SEGTREE_MAX_SIZE) {
        return SEGTREE_ERROR_INVALID_RANGE;
    }

    int new_tree_size = calculate_tree_size(new_size);

    if (new_tree_size != st->tree_size) {
        int *new_tree = (int*)realloc(st->tree, new_tree_size * sizeof(int));
        if (new_tree == NULL) {
            return SEGTREE_ERROR_MEMORY_ALLOCATION;
        }
        st->tree = new_tree;

        if (st->lazy_enabled) {
            int *new_lazy = (int*)realloc(st->lazy, new_tree_size * sizeof(int));
            if (new_lazy == NULL) {
                return SEGTREE_ERROR_MEMORY_ALLOCATION;
            }
            st->lazy = new_lazy;
            for (int i = 0; i < new_tree_size; i++) {
                st->lazy[i] = st->identity;
            }
        }

        st->tree_size = new_tree_size;
    }

    st->size = new_size;

    for (int i = 0; i < st->tree_size; i++) {
        st->tree[i] = st->identity;
    }

    build_tree(st, new_arr, 1, 0, new_size - 1);

    return SEGTREE_OK;
}