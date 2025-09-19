#include "../include/segment_tree.h"

int combine_sum(int a, int b) {
    return a + b;
}

int combine_min(int a, int b) {
    return (a < b) ? a : b;
}

int combine_max(int a, int b) {
    return (a > b) ? a : b;
}

void update_lazy_sum(segment_tree_t *st, int node, int start, int end, int val) {
    if (st == NULL || st->tree == NULL || st->lazy == NULL) {
        return;
    }

    st->lazy[node] += val;
}

void update_lazy_min(segment_tree_t *st, int node, int start, int end, int val) {
    if (st == NULL || st->tree == NULL || st->lazy == NULL) {
        return;
    }

    if (st->lazy[node] == SEGTREE_INF || val < st->lazy[node]) {
        st->lazy[node] = val;
        st->tree[node] = val;
    }
}

void update_lazy_max(segment_tree_t *st, int node, int start, int end, int val) {
    if (st == NULL || st->tree == NULL || st->lazy == NULL) {
        return;
    }

    if (st->lazy[node] == SEGTREE_NEG_INF || val > st->lazy[node]) {
        st->lazy[node] = val;
        st->tree[node] = val;
    }
}

void propagate_sum(segment_tree_t *st, int node, int start, int end) {
    if (st == NULL || st->tree == NULL || st->lazy == NULL) {
        return;
    }

    if (st->lazy[node] != 0) {
        st->tree[node] += st->lazy[node] * (end - start + 1);

        if (start != end) {
            st->lazy[2 * node] += st->lazy[node];
            st->lazy[2 * node + 1] += st->lazy[node];
        }

        st->lazy[node] = 0;
    }
}

void propagate_min(segment_tree_t *st, int node, int start, int end) {
    if (st == NULL || st->tree == NULL || st->lazy == NULL) {
        return;
    }

    if (st->lazy[node] != SEGTREE_INF) {
        st->tree[node] = st->lazy[node];

        if (start != end) {
            st->lazy[2 * node] = st->lazy[node];
            st->lazy[2 * node + 1] = st->lazy[node];
        }

        st->lazy[node] = SEGTREE_INF;
    }
}

void propagate_max(segment_tree_t *st, int node, int start, int end) {
    if (st == NULL || st->tree == NULL || st->lazy == NULL) {
        return;
    }

    if (st->lazy[node] != SEGTREE_NEG_INF) {
        st->tree[node] = st->lazy[node];

        if (start != end) {
            st->lazy[2 * node] = st->lazy[node];
            st->lazy[2 * node + 1] = st->lazy[node];
        }

        st->lazy[node] = SEGTREE_NEG_INF;
    }
}