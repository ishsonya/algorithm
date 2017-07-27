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
        minim = value;
        min_pos = key_tree;
    }
    void push() {
        value += to_add;
        left->to_add += to_add;
        right->to_add += to_add;
        to_add = 0;
    }
    void update() {
        if (right->minim < left->minim) {
            minim = right->minim;
            min_pos = right->min_pos;
        }
        else {
            minim = left->minim;
            min_pos = left->min_pos;
        }
    }
    void set_left(Treap* left_) {
        push();
        left = left_;
        // cout << 'left set'  << endl;
        update();
    }
    void set_right(Treap* right_) {
        push();
        right = right_;
        // cout << 'right set' << endl;
        update();
    }
};

Treap* merge(Treap* treap1, Treap* treap2) {
    if (treap1 == nullptr) {
        return treap2;
    }
    if (treap2 == nullptr) {
        return treap1;
    }
    if (treap1->value < treap2->value) {
        treap2->set_left(merge(treap1, treap2->left));
        return treap2;
    }
    else {
        treap1->set_right(merge(treap1->right, treap2));
        return treap1;
    }
}
pair<Treap*, Treap*> split(Treap* treap, int value_) {
    Treap* left_;
    Treap* right_;
    Treap* temp;
    if (treap == nullptr) {
        return make_pair(treap, treap);
    }
    if (value_ > treap->value) {
        tie(temp, right_) =  split(treap->right, value_);
        left_ = treap;
        left_->set_right(temp);
    }
    else {
        tie(left_, temp) = split(treap->left, value_);
        right_ = treap;
        right_->set_left(temp);
    }
    return make_pair(left_, right_);
}

int main() {
    int n;
    cin >> n;
    Treap* treap1 = nullptr;
    for (int tree_key = 0; tree_key < n; tree_key++) {
        int value;
        cin >> value;
        Treap treap2(tree_key, value);
        Treap* treap = &treap2;
        treap1 = merge(treap1, treap);
    }
    // cout << treap1->minim << endl;
}
