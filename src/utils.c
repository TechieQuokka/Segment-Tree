#include "../include/segment_tree.h"

static void print_tree_recursive(segment_tree_t *st, int node, int start, int end, int depth) {
    if (node >= st->tree_size || start > end) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    printf("Node %d [%d-%d]: %d", node, start, end, st->tree[node]);

    if (st->lazy_enabled && st->lazy != NULL && st->lazy[node] != st->identity) {
        printf(" (lazy: %d)", st->lazy[node]);
    }

    printf("\n");

    if (start == end) {
        return;
    }

    int mid = (start + end) / 2;
    print_tree_recursive(st, 2 * node, start, mid, depth + 1);
    print_tree_recursive(st, 2 * node + 1, mid + 1, end, depth + 1);
}

void segtree_print(segment_tree_t *st) {
    if (st == NULL) {
        printf("Segment Tree is NULL\n");
        return;
    }

    printf("=== Segment Tree Debug Info ===\n");
    printf("Size: %d, Tree Size: %d\n", st->size, st->tree_size);
    printf("Operation Type: %d, Lazy Enabled: %s\n",
           st->op_type, st->lazy_enabled ? "Yes" : "No");
    printf("Identity: %d\n", st->identity);
    printf("Stats - Queries: %d, Updates: %d\n",
           st->stats.query_count, st->stats.update_count);
    printf("=== Tree Structure ===\n");

    if (st->size > 0) {
        print_tree_recursive(st, 1, 0, st->size - 1, 0);
    }

    printf("=== End Debug Info ===\n\n");
}

static bool validate_tree_recursive(segment_tree_t *st, int node, int start, int end) {
    if (node >= st->tree_size || start > end || start < 0 || end >= st->size) {
        return false;
    }

    if (start == end) {
        return true;
    }

    int mid = (start + end) / 2;
    int left_child = 2 * node;
    int right_child = 2 * node + 1;

    if (left_child >= st->tree_size || right_child >= st->tree_size) {
        return false;
    }

    bool left_valid = validate_tree_recursive(st, left_child, start, mid);
    bool right_valid = validate_tree_recursive(st, right_child, mid + 1, end);

    if (!left_valid || !right_valid) {
        return false;
    }

    if (st->lazy_enabled && st->propagate != NULL) {
        segment_tree_t temp_st = *st;
        temp_st.propagate(&temp_st, left_child, start, mid);
        temp_st.propagate(&temp_st, right_child, mid + 1, end);

        int expected = st->combine(temp_st.tree[left_child], temp_st.tree[right_child]);
        if (st->tree[node] != expected) {
            return false;
        }
    } else {
        int expected = st->combine(st->tree[left_child], st->tree[right_child]);
        if (st->tree[node] != expected) {
            return false;
        }
    }

    return true;
}

bool segtree_validate(segment_tree_t *st) {
    if (st == NULL || st->tree == NULL) {
        return false;
    }

    if (st->size <= 0 || st->tree_size <= 0) {
        return false;
    }

    if (st->combine == NULL) {
        return false;
    }

    if (st->lazy_enabled) {
        if (st->lazy == NULL || st->propagate == NULL || st->update_lazy == NULL) {
            return false;
        }
    }

    if (st->size == 1) {
        return true;
    }

    return validate_tree_recursive(st, 1, 0, st->size - 1);
}

segtree_stats_t segtree_get_stats(segment_tree_t *st) {
    segtree_stats_t empty_stats = {0, 0, 0.0, 0.0};

    if (st == NULL) {
        return empty_stats;
    }

    return st->stats;
}

void segtree_reset_stats(segment_tree_t *st) {
    if (st == NULL) {
        return;
    }

    st->stats.query_count = 0;
    st->stats.update_count = 0;
    st->stats.total_query_time = 0.0;
    st->stats.total_update_time = 0.0;
}

void segtree_print_stats(segment_tree_t *st) {
    if (st == NULL) {
        printf("Segment Tree is NULL\n");
        return;
    }

    printf("=== Segment Tree Statistics ===\n");
    printf("Total Queries: %d\n", st->stats.query_count);
    printf("Total Updates: %d\n", st->stats.update_count);
    printf("Total Query Time: %.6f seconds\n", st->stats.total_query_time);
    printf("Total Update Time: %.6f seconds\n", st->stats.total_update_time);

    if (st->stats.query_count > 0) {
        printf("Average Query Time: %.6f seconds\n",
               st->stats.total_query_time / st->stats.query_count);
    }

    if (st->stats.update_count > 0) {
        printf("Average Update Time: %.6f seconds\n",
               st->stats.total_update_time / st->stats.update_count);
    }

    printf("================================\n\n");
}