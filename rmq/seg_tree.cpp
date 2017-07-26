#include<iostream>
#include<vector>

using namespace std;

struct SegTree {
    vector<int> seg_tree_idx___value;
    int initial_list_start_in_seg_tree_idx;
    int seg_tree_size;
    int initial_list_size;
    int default_value;

    SegTree(int n) {
        default_value = 1e+9;
        initial_list_size = n;
        seg_tree_size = 1;
        while (seg_tree_size < n) {
            seg_tree_size *= 2;
        }
        seg_tree_size *= 2;
        seg_tree_size = max(seg_tree_size, 4);
        initial_list_start_in_seg_tree_idx = seg_tree_size / 2 - 1;
        seg_tree_idx___value.resize(seg_tree_size);
        for (int seg_tree_idx = 0; seg_tree_idx < seg_tree_idx___value.size(); seg_tree_idx++) {
            seg_tree_idx___value[seg_tree_idx] = default_value;
        }
    }

    void refresh(int curr_seg_tree_idx) {
        seg_tree_idx___value[curr_seg_tree_idx] = min(seg_tree_idx___value[2 * curr_seg_tree_idx + 1],
                                                          seg_tree_idx___value[2 * curr_seg_tree_idx + 2]
                                                         );
    }

    void change_one_value(int init_list_idx, int new_value) {
        int seg_tree_idx = init_list_idx + initial_list_start_in_seg_tree_idx;
        seg_tree_idx___value[seg_tree_idx] = new_value;
        int curr_seg_tree_idx = (seg_tree_idx - 1) / 2;
        while(curr_seg_tree_idx > 0) {
            refresh(curr_seg_tree_idx);
            curr_seg_tree_idx--;
            curr_seg_tree_idx /= 2;
        }
        refresh(0);
    }

    int get_global_min() {
        return seg_tree_idx___value[0];
    }

    int get_seg_min(int l_init_idx_closed, int r_init_idx_closed) {
        int l_seg_tree_idx_closed = l_init_idx_closed + initial_list_start_in_seg_tree_idx;
        int r_seg_tree_idx_closed = r_init_idx_closed + initial_list_start_in_seg_tree_idx;
        int ans = default_value;
        while (r_seg_tree_idx_closed > l_seg_tree_idx_closed) {
            if (l_seg_tree_idx_closed % 2 == 0) {
                ans = min(ans, seg_tree_idx___value[l_seg_tree_idx_closed]);
                l_seg_tree_idx_closed++;
            }
            if (r_seg_tree_idx_closed % 2 == 1) {
                ans = min(ans, seg_tree_idx___value[r_seg_tree_idx_closed]);
                r_seg_tree_idx_closed--;
            }
            r_seg_tree_idx_closed--;
            r_seg_tree_idx_closed /= 2;

            l_seg_tree_idx_closed--;
            l_seg_tree_idx_closed /= 2;
        }
        if (r_seg_tree_idx_closed == l_seg_tree_idx_closed) {
            ans = min(ans, seg_tree_idx___value[r_seg_tree_idx_closed]);
        }
        return ans;
    }
};

int main() {

}
