#include <iostream>
using namespace std;

template <typename T>
class BTreeNode {
public:
    T* keys;
    BTreeNode** children;
    int numKeys;
    int capacity;
    bool isLeaf;
    int t;

    BTreeNode(int _t, bool _isLeaf)
        : t(_t), isLeaf(_isLeaf), numKeys(0), capacity(2 * _t - 1) {
        keys = new T[capacity];
        children = new BTreeNode<T>*[capacity + 1];
    }

    ~BTreeNode() {
        delete[] keys;
        for (int i = 0; i <= numKeys; ++i) {
            delete children[i];
        }
        delete[] children;
    }

    void insertNonFull(T k) {
        int i = numKeys - 1;

        if (isLeaf) {
            while (i >= 0 && keys[i] > k) {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = k;
            numKeys++;
        }
        else {
            while (i >= 0 && keys[i] > k) {
                i--;
            }
            i++;

            if (children[i]->numKeys == capacity) {
                splitChild(i, children[i]);
                if (keys[i] < k) {
                    i++;
                }
            }
            children[i]->insertNonFull(k);
        }
    }

    void splitChild(int i, BTreeNode* y) {
        BTreeNode* z = new BTreeNode(y->t, y->isLeaf);
        z->numKeys = t - 1;

        for (int j = 0; j < t - 1; j++) {
            z->keys[j] = y->keys[j + t];
        }

        if (!y->isLeaf) {
            for (int j = 0; j < t; j++) {
                z->children[j] = y->children[j + t];
            }
        }

        y->numKeys = t - 1;

        for (int j = numKeys; j >= i + 1; j--) {
            children[j + 1] = children[j];
        }
        children[i + 1] = z;

        for (int j = numKeys - 1; j >= i; j--) {
            keys[j + 1] = keys[j];
        }
        keys[i] = y->keys[t - 1];
        numKeys++;
    }

    void traverse() {
        for (int i = 0; i < numKeys; i++) {
            if (!isLeaf) {
                children[i]->traverse();
            }
            cout << " " << keys[i];
        }
        if (!isLeaf) {
            children[numKeys]->traverse();
        }
    }

    bool search(T k) {
        int i = 0;
        while (i < numKeys && k > keys[i]) {
            i++;
        }

        if (i < numKeys && keys[i] == k) {
            return true;
        }

        if (isLeaf) {
            return false;
        }

        return children[i]->search(k);
    }

    void remove(T k) {
        int idx = findKey(k);

        if (idx < numKeys && keys[idx] == k) {
            if (isLeaf) {
                removeFromLeaf(idx);
            }
            else {
                removeFromNonLeaf(idx);
            }
        }
        else {
            if (isLeaf) {
                cout << "The key " << k << " does not exist in the tree\n";
                return;
            }

            bool flag = ((idx == numKeys) ? true : false);

            if (children[idx]->numKeys < t) {
                fill(idx);
            }

            if (flag && idx > numKeys) {
                children[idx - 1]->remove(k);
            }
            else {
                children[idx]->remove(k);
            }
        }
    }

private:
    int findKey(T k) {
        int idx = 0;
        while (idx < numKeys && keys[idx] < k) {
            ++idx;
        }
        return idx;
    }

    void removeFromLeaf(int idx) {
        for (int i = idx + 1; i < numKeys; ++i) {
            keys[i - 1] = keys[i];
        }
        numKeys--;
    }

    void removeFromNonLeaf(int idx) {
        T k = keys[idx];

        if (children[idx]->numKeys >= t) {
            T pred = getPred(idx);
            keys[idx] = pred;
            children[idx]->remove(pred);
        }
        else if (children[idx + 1]->numKeys >= t) {
            T succ = getSucc(idx);
            keys[idx] = succ;
            children[idx + 1]->remove(succ);
        }
        else {
            merge(idx);
            children[idx]->remove(k);
        }
    }

    T getPred(int idx) {
        BTreeNode* cur = children[idx];
        while (!cur->isLeaf) {
            cur = cur->children[cur->numKeys];
        }
        return cur->keys[cur->numKeys - 1];
    }

    T getSucc(int idx) {
        BTreeNode* cur = children[idx + 1];
        while (!cur->isLeaf) {
            cur = cur->children[0];
        }
        return cur->keys[0];
    }

    void fill(int idx) {
        if (idx != 0 && children[idx - 1]->numKeys >= t) {
            borrowFromPrev(idx);
        }
        else if (idx != numKeys && children[idx + 1]->numKeys >= t) {
            borrowFromNext(idx);
        }
        else {
            if (idx != numKeys) {
                merge(idx);
            }
            else {
                merge(idx - 1);
            }
        }
    }

    void borrowFromPrev(int idx) {
        BTreeNode* child = children[idx];
        BTreeNode* sibling = children[idx - 1];

        for (int i = child->numKeys - 1; i >= 0; --i) {
            child->keys[i + 1] = child->keys[i];
        }

        if (!child->isLeaf) {
            for (int i = child->numKeys; i >= 0; --i) {
                child->children[i + 1] = child->children[i];
            }
        }

        child->keys[0] = keys[idx - 1];

        if (!isLeaf) {
            child->children[0] = sibling->children[sibling->numKeys];
        }

        keys[idx - 1] = sibling->keys[sibling->numKeys - 1];

        child->numKeys += 1;
        sibling->numKeys -= 1;
    }

    void borrowFromNext(int idx) {
        BTreeNode* child = children[idx];
        BTreeNode* sibling = children[idx + 1];

        child->keys[child->numKeys] = keys[idx];

        if (!child->isLeaf) {
            child->children[child->numKeys + 1] = sibling->children[0];
        }

        keys[idx] = sibling->keys[0];

        for (int i = 1; i < sibling->numKeys; ++i) {
            sibling->keys[i - 1] = sibling->keys[i];
        }

        if (!sibling->isLeaf) {
            for (int i = 1; i <= sibling->numKeys; ++i) {
                sibling->children[i - 1] = sibling->children[i];
            }
        }

        child->numKeys += 1;
        sibling->numKeys -= 1;
    }

    void merge(int idx) {
        BTreeNode* child = children[idx];
        BTreeNode* sibling = children[idx + 1];

        child->keys[t - 1] = keys[idx];

        for (int i = 0; i < sibling->numKeys; ++i) {
            child->keys[i + t] = sibling->keys[i];
        }

        if (!child->isLeaf) {
            for (int i = 0; i <= sibling->numKeys; ++i) {
                child->children[i + t] = sibling->children[i];
            }
        }

        for (int i = idx + 1; i < numKeys; ++i) {
            keys[i - 1] = keys[i];
        }

        for (int i = idx + 2; i <= numKeys; ++i) {
            children[i - 1] = children[i];
        }

        child->numKeys += sibling->numKeys + 1;
        numKeys--;

        delete sibling;
    }

};

template <typename T>
class BTree {
    BTreeNode<T>* root;
    int t;

public:
    BTree(int _t) : t(_t), root(nullptr) {}

    ~BTree() {
        deleteTree(root);
    }

    void deleteTree(BTreeNode<T>* node) {
        if (node == nullptr) return;
        if (!node->isLeaf) {
            for (int i = 0; i <= node->numKeys; i++) {
                deleteTree(node->children[i]);
            }
        }
    }
    void printTree() {
        if (root != nullptr) {
            printLevelOrder(root);
        }
    }
    void printLevelOrder(BTreeNode<T>* root) {
        int h = height(root);
        for (int i = 1; i <= h; i++) {
            printGivenLevel(root, i);
            cout << endl;
        }
    }

    int height(BTreeNode<T>* node) {
        if (node == nullptr) {
            return 0;
        }
        else {
            int lheight = height(node->isLeaf ? nullptr : node->children[0]);
            int rheight = height(node->isLeaf ? nullptr : node->children[node->numKeys]);
            return (lheight > rheight ? lheight : rheight) + 1;
        }
    }

    void printGivenLevel(BTreeNode<T>* root, int level) {
        if (root == nullptr)
            return;
        if (level == 1) {
            printNode(root);
        }
        else if (level > 1) {
            for (int i = 0; i <= root->numKeys; i++) {
                printGivenLevel(root->children[i], level - 1);
            }
        }
    }

    void printNode(BTreeNode<T>* node) {
        for (int i = 0; i < node->numKeys; i++) {
            cout << node->keys[i] << " ";
        }
        cout << "| ";
    }

    void traverse() {
        if (root != nullptr) {
            root->traverse();
        }
    }

    void insert(T k) {
        if (root == nullptr) {
            root = new BTreeNode<T>(t, true);
            root->keys[0] = k;
            root->numKeys = 1;
        }
        else {
            if (root->numKeys == 2 * t - 1) {
                BTreeNode<T>* s = new BTreeNode<T>(t, false);

                s->children[0] = root;
                s->splitChild(0, root);

                int i = 0;
                if (s->keys[0] < k) {
                    i++;
                }
                s->children[i]->insertNonFull(k);

                root = s;
            }
            else {
                root->insertNonFull(k);
            }
        }
    }

    bool search(T k) {
        return (root != nullptr) ? root->search(k) : false;
    }

    void remove(T k) {
        if (root == nullptr) {
            cout << "The tree is empty\n";
            return;
        }

        root->remove(k);

        if (root->numKeys == 0) {
            BTreeNode<T>* tmp = root;
            if (root->isLeaf) {
                root = nullptr;
            }
            else {
                root = root->children[0];
            }

            delete tmp;
        }
    }

};