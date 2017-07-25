#include<iostream>
#include<vector>
#include<stack>

using namespace std;

struct Tree {
    vector<int> node_idx___parent_idx;
    vector<vector<int> > node_idx___children_idxs;
    vector<int> node_idx___depth;
    vector<vector<int> > node_idx__power___parent_idx; /// calculates 2^power's ancestor of a node

    void calculate_depths() {
        node_idx___depth.resize(node_idx___parent_idx.size());
        stack<int> _stack;
        node_idx___depth[0] = 0;
        _stack.push(0);

        while (not _stack.empty()) {
            int curr_node_idx = _stack.top();
            _stack.pop();
            for (int child_idx : node_idx___children_idxs[curr_node_idx]) {
                node_idx___depth[child_idx] = node_idx___depth[curr_node_idx] + 1;
                _stack.push(child_idx);
            }
        }
    }

    void lca_precalc(){
        /// calculates 2^power's ancestor of a node for powers of 2.
        int nodes_nr = node_idx___parent_idx.size();
        node_idx__power___parent_idx.resize(nodes_nr);
        for (int node_idx = 0; node_idx < nodes_nr; node_idx++) {
            node_idx__power___parent_idx[node_idx].push_back(node_idx___parent_idx[node_idx]);
        }
        int two_power = 2;
        for (int power = 1; two_power < nodes_nr; power++) {
            for (int node_idx = 0; node_idx < nodes_nr; node_idx++) {
                int middle_ancestor_idx = node_idx__power___parent_idx[node_idx][power - 1];
                node_idx__power___parent_idx[node_idx].push_back(node_idx__power___parent_idx[middle_ancestor_idx][power - 1]);
            }
            two_power *= 2;
        }
    }

    Tree(vector<int> node_idx___parent_idx_) {
        node_idx___parent_idx = node_idx___parent_idx_;
        int nodes_nr = node_idx___parent_idx.size();
        node_idx___children_idxs.resize(nodes_nr);
        node_idx___parent_idx[0] = 0;
        for (int node_idx = 1; node_idx < node_idx___parent_idx.size(); node_idx++) {
            int parent_idx = node_idx___parent_idx[node_idx];
            node_idx___children_idxs[parent_idx].push_back(node_idx);
        }

        calculate_depths();
        lca_precalc();
    }

    int get_depth(int node_idx) {
        return node_idx___depth[node_idx];
    }

    int get_best_power_of_two(int n) {
        /// Gets max q so that 2^q <= n or returns 0 if n = 0
        int q = 0;
        int two_pow = 1;
        while (two_pow <= n) {
            two_pow *= 2;
            q++;
        }
        return q - 1;
    }

    int pull_node_to_depth(int node_idx, int depth_wished) {
        int curr_depth = node_idx___depth[node_idx];
        int delta = curr_depth - depth_wished;
        if (delta == 0) {
            return node_idx;
        }
        int first_jump = get_best_power_of_two(delta);
        node_idx = node_idx__power___parent_idx[node_idx][first_jump];
        int curr_two_pow = (1 << first_jump);
        delta -= curr_two_pow;
        curr_two_pow /= 2;
        int curr_jump = first_jump - 1;
        while (delta > 0) {
            if (curr_two_pow <= delta) {
                delta -= curr_two_pow;
                node_idx = node_idx__power___parent_idx[node_idx][curr_jump];
            }
            curr_two_pow /= 2;
            curr_jump--;
        }
        return node_idx;
    }

    int get_best_jump_pow(int node_idx1, int node_idx2) {
        //! Requires node1 and node2 to be on the same depth
        /// Returns best pow after which ancestors are still different
        int curr_shot_pow = 0;
        while (node_idx__power___parent_idx[node_idx1][curr_shot_pow] != node_idx__power___parent_idx[node_idx2][curr_shot_pow]) {
            curr_shot_pow++;
        }
        return curr_shot_pow - 1;
    }

    int get_lca_idx(int node_idx1, int node_idx2) {
        if (node_idx1 == node_idx2) {
            return node_idx1;
        }
        /// Calculates idx of least common ancester of node1 and node2
        /// Time: O(log(n))
        int depth1 = node_idx___depth[node_idx1];
        int depth2 = node_idx___depth[node_idx2];
        if (depth1 > depth2) {
            swap(depth1, depth2);
            swap(node_idx1, node_idx2);
        }
        // now depth1 <= depth2
        node_idx2 = pull_node_to_depth(node_idx2, depth1);
        // now depth1 = depth2
        if (node_idx1 == node_idx2) {
            return node_idx1;
        }
        // now we are sure they are different
        //! Invariant1: node1 and node2 are different
        //! Invariant2: we can not jump on (best_jump_pow + 1) and still get different nodes
        int jump_pow = get_best_jump_pow(node_idx1, node_idx2);
        while (jump_pow >= 0) {
            if (node_idx__power___parent_idx[node_idx1][jump_pow] != node_idx__power___parent_idx[node_idx2][jump_pow]) {
                node_idx1 = node_idx__power___parent_idx[node_idx1][jump_pow];
                node_idx2 = node_idx__power___parent_idx[node_idx2][jump_pow];
            }
            jump_pow -= 1;
        }
        return node_idx___parent_idx[node_idx1];
    }

    int get_distance(int node_idx1, int node_idx2) {
        int lca_idx = get_lca_idx(node_idx1, node_idx2);
        int lca_depth = node_idx___depth[lca_idx];
        int node1_depth = node_idx___depth[node_idx1];
        int node2_depth = node_idx___depth[node_idx2];
        int dist = node1_depth + node2_depth - 2 * lca_depth;
        return dist;
    }
};

int main() {
    int stations_nr, days_nr;
    cin >> stations_nr >> days_nr;
    vector<int> idx___parent_idx(stations_nr);
    for (int idx = 1; idx < stations_nr; idx++) {
        int parent_idx;
        cin >> parent_idx;
        idx___parent_idx[idx] = parent_idx - 1;
    }
    Tree tree(idx___parent_idx);


}

