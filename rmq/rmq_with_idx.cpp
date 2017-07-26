#include<iostream>
#include<vector>

using namespace std;

struct SegTree {
    vector<int> seg_tree_idx___max_value;
    vector<int> seg_tree_idx___max_pos;
    int initial_list_start_in_seg_tree_idx;
    int seg_tree_size;
    int initial_list_size;
    int default_value;

    SegTree(int n) {
        //cout << n << endl;
        default_value = -(1e+9);
        initial_list_size = n;
        seg_tree_size = 1;
        while (seg_tree_size < n) {
            seg_tree_size *= 2;
        }
        seg_tree_size *= 2;
        seg_tree_size = max(seg_tree_size, 4);
        initial_list_start_in_seg_tree_idx = seg_tree_size / 2 - 1;
        seg_tree_idx___max_value.resize(seg_tree_size);
        seg_tree_idx___max_pos.resize(seg_tree_size);
        for (int seg_tree_idx = 0; seg_tree_idx < seg_tree_idx___max_value.size(); seg_tree_idx++) {
            seg_tree_idx___max_value[seg_tree_idx] = default_value;
        }
        //cout << n << endl;
        //cout << initial_list_start_in_seg_tree_idx << endl;
        for (int seg_tree_idx = initial_list_start_in_seg_tree_idx; seg_tree_idx < seg_tree_size; seg_tree_idx++) {
            seg_tree_idx___max_pos[seg_tree_idx] = seg_tree_idx;
        }
        for (int seg_tree_idx = initial_list_start_in_seg_tree_idx - 1; seg_tree_idx >= 0; seg_tree_idx--) {
            refresh(seg_tree_idx);
        }
        // for (auto max_pos : seg_tree_idx___max_pos) {
            // cout << max_pos << " ";
        // }
        // cout << endl;
    }

    void refresh(int curr_seg_tree_idx) {
        int left_child_idx = 2 * curr_seg_tree_idx + 1;
        int right_child_idx = 2 * curr_seg_tree_idx + 2;
        int left_child_max_value = seg_tree_idx___max_value[left_child_idx];
        int right_child_max_value = seg_tree_idx___max_value[right_child_idx];
        int left_child_max_pos = seg_tree_idx___max_pos[left_child_idx];
        int right_child_max_pos = seg_tree_idx___max_pos[right_child_idx];
        if (right_child_max_value > left_child_max_value) {
            seg_tree_idx___max_value[curr_seg_tree_idx] = right_child_max_value;
            seg_tree_idx___max_pos[curr_seg_tree_idx] = right_child_max_pos;
        }
        else {
            seg_tree_idx___max_value[curr_seg_tree_idx] = left_child_max_value;
            seg_tree_idx___max_pos[curr_seg_tree_idx] = left_child_max_pos;
        }
    }

    void change_one_value(int init_list_idx, int new_value) {
        int seg_tree_idx = init_list_idx + initial_list_start_in_seg_tree_idx;
        seg_tree_idx___max_value[seg_tree_idx] = new_value;
        int curr_seg_tree_idx = (seg_tree_idx - 1) / 2;
        while(curr_seg_tree_idx > 0) {
            refresh(curr_seg_tree_idx);
            curr_seg_tree_idx--;
            curr_seg_tree_idx /= 2;
        }
        refresh(0);
    }

    // int get_init_list_idx(int seg_tree_idx) {
        // int curr_value = seg_tree_idx___max_value[seg_tree_idx];
        // while (seg_tree_idx < initial_list_start_in_seg_tree_idx) {
            // int parent1_idx = seg_tree_idx * 2 + 1;
            // int parent2_idx = seg_tree_idx * 2 + 2;
            // if (seg_tree_idx___max_value[parent1_idx] == curr_value) {
                // seg_tree_idx = parent1_idx;
            // }
            // else {
                // seg_tree_idx = parent2_idx;
            // }
        // }
        // int init_list_idx = seg_tree_idx - initial_list_start_in_seg_tree_idx;
        // return init_list_idx;
    // }

    int get_global_max() {
        return seg_tree_idx___max_value[0];
    }

    int get_seg_max_idx(int l_init_idx_closed, int r_init_idx_closed) {
        int l_seg_tree_idx_closed = l_init_idx_closed + initial_list_start_in_seg_tree_idx;
        int r_seg_tree_idx_closed = r_init_idx_closed + initial_list_start_in_seg_tree_idx;
        // cout << "*** " << l_seg_tree_idx_closed << " " << r_seg_tree_idx_closed << endl;
        int ans_idx = seg_tree_idx___max_pos[l_seg_tree_idx_closed];
        // cout << "*" << ans_idx << endl;
        while (r_seg_tree_idx_closed > l_seg_tree_idx_closed) {
            if (l_seg_tree_idx_closed % 2 == 0) {
                if (seg_tree_idx___max_value[ans_idx] < seg_tree_idx___max_value[l_seg_tree_idx_closed]) {
                    ans_idx = seg_tree_idx___max_pos[l_seg_tree_idx_closed];
                }
                l_seg_tree_idx_closed++;
            }
            if (r_seg_tree_idx_closed % 2 == 1) {
                if (seg_tree_idx___max_value[ans_idx] < seg_tree_idx___max_value[r_seg_tree_idx_closed]) {
                    ans_idx = seg_tree_idx___max_pos[r_seg_tree_idx_closed];
                }
                r_seg_tree_idx_closed--;
            }
            r_seg_tree_idx_closed--;
            r_seg_tree_idx_closed /= 2;

            l_seg_tree_idx_closed--;
            l_seg_tree_idx_closed /= 2;
        }
        if (r_seg_tree_idx_closed == l_seg_tree_idx_closed) {
            if (seg_tree_idx___max_value[ans_idx] < seg_tree_idx___max_value[r_seg_tree_idx_closed]) {
                ans_idx = seg_tree_idx___max_pos[r_seg_tree_idx_closed];
            }
        }
        int init_list_ans_idx = ans_idx - initial_list_start_in_seg_tree_idx;
        return ans_idx;
    }
};

int main() {
    int n;
    cin >> n;
    SegTree rmq(n);
    for (int init_list_idx = 0; init_list_idx < n; init_list_idx++) {
        int value;
        cin >> value;
        rmq.change_one_value(init_list_idx, value);
    }
    // for (auto max_pos : rmq.seg_tree_idx___max_pos) {
        // cout << max_pos << " ";
    // }
    // cout << endl << rmq.initial_list_start_in_seg_tree_idx << endl;
    // cout << rmq.get_global_max() << endl;
    int requests_nr;
    cin >> requests_nr;
    for (int i = 0; i < requests_nr; i++) {
        int l, r;
        cin >> l >> r;
        int max_idx = rmq.get_seg_max_idx(l - 1, r - 1);
        cout << rmq.seg_tree_idx___max_value[max_idx] << " " << (max_idx - rmq.initial_list_start_in_seg_tree_idx + 1) << endl;
    }
}
