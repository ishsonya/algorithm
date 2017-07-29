#include<iostream>
#include<vector>
#include<algorithm>
#include<tuple>

using namespace std;

int MY_RAND_MAX = 1 << 16;

struct Treap {
    Treap* left;
    Treap* right;
    int key_tree;
    int key_heap;
    int value;
    // functions
    int minim;
    int min_pos;
    // methods
    int to_add;

    Treap(int key_tree_, int value_) {
        left = nullptr;
        right = nullptr;
        value = value_;
        key_tree = key_tree_;
        key_heap = (rand() % MY_RAND_MAX) * MY_RAND_MAX + rand() % MY_RAND_MAX;
        //cout << key_heap << endl;
        minim = value;
        min_pos = key_tree;
        to_add = 0;
    }
    void push() {
        value += to_add;
        if (left != nullptr) {
            left->to_add += to_add;
            left->minim += to_add;
        }
        if (right != nullptr) {
            right->to_add += to_add;
            right->minim += to_add;
        }
        to_add = 0;
    }
    void update() {
        int left_minim;
        int right_minim;
        int left_min_pos;
        int right_min_pos;
        if (left == nullptr) {
            left_minim = 1e+9;
            left_min_pos = 0;
        }
        else {
            left_minim = left->minim;
            left_min_pos = left->min_pos;
        }
        if (right == nullptr) {
            right_minim = 1e+9;
            right_min_pos = 0;
        }
        else {
            right_minim = right->minim;
            right_min_pos = right->min_pos;
        }
        minim = value;
        min_pos = key_tree;
        if (left_minim < minim) {
            minim = left_minim;
            min_pos = left_min_pos;
        }
        if (right_minim < minim) {
            minim = right_minim;
            min_pos = right_min_pos;
        }
    }
};

Treap* merge_treap(Treap* left_treap, Treap* right_treap) {
//Requires left_treap to be lefter then right_treap
    //cout << "merging" << endl;
    if (left_treap == nullptr) {
        return right_treap;
    }
    if (right_treap == nullptr) {
        return left_treap;
    }
    if (left_treap->key_heap < right_treap->key_heap) {
        right_treap->push();
        right_treap->left = merge_treap(left_treap, right_treap->left);
        right_treap->update();
        return right_treap;
    }
    else {
        left_treap->push();
        left_treap->right = merge_treap(left_treap->right, right_treap);
        left_treap->update();
        return left_treap;
    }
}

pair<Treap*, Treap*> split(Treap* treap, int key_tree_) {

    if (treap == nullptr) {
        return make_pair(treap, treap);
    }
    //cout << "splitting" << treap << endl << treap->left << " " << treap->right << endl;
    treap->push();
    Treap* left;
    Treap* right;
    Treap* temp;
    if (key_tree_ > treap->key_tree) {
        tie(temp, right) =  split(treap->right, key_tree_);
        left = treap;
        left->right = temp;
    }
    else {
        tie(left, temp) = split(treap->left, key_tree_);
        right = treap;
        right->left = temp;
    }
    treap->update();
    return make_pair(left, right);
}

Treap* add_elem (int key_tree, int value, Treap* treap) {
    cout << key_tree << ' ' << value << endl;
    Treap* new_left;
    Treap* new_right;
    tie(new_left, new_right) = split(treap, key_tree);
    Treap* new_treap = new Treap(key_tree, value);
    return merge_treap(merge_treap(new_left, new_treap), new_right);
}

Treap* add_on_range(int to_add, int left_cl, int right_op, Treap* treap){
    Treap *left_cut, *right_cut, *target;
    tie(left_cut, target) = split(treap, left_cl);
    tie(target, right_cut) = split(target, right_op);
    target->to_add += to_add;
    target->minim += to_add;
    return merge_treap(merge_treap(left_cut, target), right_cut);
}

int range_minimum(int left_cl, int right_op, Treap* treap) {
    Treap *left_cut, *right_cut, *target;
    tie(left_cut, target) = split(treap, left_cl);
    tie(target, right_cut) = split(target, right_op);
    int minim = target->minim;
    treap = merge_treap(merge_treap(left_cut, target), right_cut);
    return minim;
}

int main() {
    int n;
    cin >> n;
    Treap* treap = new Treap(0, 0);
    for (int tree_key = 0; tree_key < n; tree_key++) {
        int value;
        cin >> value;
        if (tree_key == 0) {
            treap->value = value;
        }
        else {
            treap = add_elem(tree_key, value, treap);
        }
    }
    //int req_nr;
    //cin >> req_nr;
    //for (int i = 0; i < req_nr; i++)
    while (1) {
        int req_type, left_cl, right_op;
        cin >> req_type;
        if (req_type == 0) {
            cin >> left_cl >> right_op;
            cout << range_minimum(left_cl, right_op, treap) << endl;
        }
        if (req_type == 1) {
            int to_add;
            cin >> left_cl >> right_op >> to_add;
            treap = add_on_range(to_add, left_cl, right_op, treap);
            cout << "DONE" << endl;
        }
    }
    // cout << treap1->minim << endl;
}
