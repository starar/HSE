#include <bits/stdc++.h>

using namespace std;

template<class T>
class const_iterator;

template<class T>
struct Node {
    const T key;
    Node *parent;
    Node *left;
    Node *right;
    int height;

    int heightSize() {
        return height;
    }

    explicit Node(T const &value = T{}) : key(value), parent(nullptr), left(nullptr), right(nullptr), height(1) {};
};

template<class T>
class Set {
public:
    typedef Node<T> Node;

    Set() : tree_size(0), root() {}

    template<typename Iterator>
    Set(Iterator first, Iterator last): Set() {
        while (first != last) {
            insert(*first);
            first++;
        }
    }

    Set(std::initializer_list<T> elems) : Set(elems.begin(), elems.end()) {}

    Set(const Set &set) : Set() {
        if (this == &set) {
            return;
        }
        for (const T &key: set) {
            insert(key);
        }
    }

    size_t size() const {
        return tree_size;
    }

    bool empty() const {
        return tree_size == 0;
    }

    int height(Node *node) {
        if (node) {
            return node->heightSize();
        } else {
            return 0;
        }
    }

    int heightSubtree(Node *node) {
        if (node) {
            return max(height(node->left), height(node->right)) + 1;
        } else {
            return 0;
        }
    }

    Node *rightRotate(Node *b) {
        Node *a = b->left;
        Node *c = a->right;
        a->right = b;
        b->left = c;
        a->parent = b->parent;
        b->parent = a;
        if (c) {
            c->parent = b;
        }
        b->height = heightSubtree(b);
        a->height = heightSubtree(a);
        return a;
    }

    Node *leftRotate(Node *a) {
        Node *b = a->right;
        Node *c = b->left;
        b->left = a;
        a->right = c;
        b->parent = a->parent;
        a->parent = b;
        if (c) {
            c->parent = a;
        }
        b->height = heightSubtree(b);
        a->height = heightSubtree(a);
        return b;
    }

    int balance(Node *node) {
        if (node) {
            return height(node->left) - height(node->right);
        } else {
            return 0;
        }
    }

    Node *rebalance(Node *node) {
        if (balance(node) < -1) {
            if (balance(node->right) <= 0) {
                return leftRotate(node);
            } else {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }
        if (balance(node) > 1) {
            if (balance(node->left) <= 0) {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            } else {
                return rightRotate(node);
            }
        }
        return node;
    }

    Node *addNode(Node *node, const T &key) {
        if (node == nullptr) {
            tree_size++;
            return (new Node(key));
        }
        if (key < node->key) {
            node->left = addNode(node->left, key);
            node->left->parent = node;
        } else if (node->key < key) {
            node->right = addNode(node->right, key);
            node->right->parent = node;
        } else {
            return node;
        }
        node->height = heightSubtree(node);
        return rebalance(node);
    }

    void insert(const T &elem) {
        root = addNode(root, elem);
    }

    Node *nearestLeft(Node *node) const {
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    Node *nearestRight(Node *node) const {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    void swapNodes(Node *&a, Node *&b) {
        if (a->left == b) {
            b->parent = a->parent;
            a->parent = b;
            a->left = b->left;
            if (a->left) {
                a->left->parent = a;
            }
            b->left = a;
            swap(a->right, b->right);
            if (a->right) {
                a->right->parent = a;
            }
            if (b->right) {
                b->right->parent = b;
            }
            swap(a, b);
            return;
        }
        if (a->right == b) {
            b->parent = a->parent;
            a->parent = b;
            a->right = b->right;
            if (a->right) {
                a->right->parent = a;
            }
            b->right = a;
            swap(a->left, b->left);
            if (a->left) {
                a->left->parent = a;
            }
            if (b->left) {
                b->left->parent = b;
            }
            swap(a, b);
            return;
        }
        if (a->parent) {
            if (a->parent->left == a) {
                a->parent->left = b;
            } else {
                a->parent->right = b;
            }
        }
        if (b->parent) {
            if (b->parent->left == b) {
                b->parent->left = a;
            } else {
                b->parent->right = a;
            }
        }
        if (a->left) {
            a->left->parent = b;
        }
        if (a->right) {
            a->right->parent = b;
        }
        if (b->left) {
            b->left->parent = a;
        }
        if (b->right) {
            b->right->parent = a;
        }
        swap(a, b);
        swap(a->parent, b->parent);
        swap(a->left, b->left);
        swap(a->right, b->right);
        swap(a->height, b->height);
    }

    Node *removeNode(Node *node, const T &key) {
        if (node == nullptr) {
            return node;
        }
        if (key < node->key) {
            node->left = removeNode(node->left, key);
        } else if (node->key < key) {
            node->right = removeNode(node->right, key);
        } else {
            if (node->left) {
                Node *temp = nearestLeft(node->left);
                swapNodes(node, temp);
                node->left = removeNode(node->left, temp->key);
            } else if (node->right) {
                Node *temp = nearestRight(node->right);
                swapNodes(node, temp);
                node->right = removeNode(node->right, temp->key);
            } else {
                delete (node);
                node = nullptr;
            }
        }
        if (node == nullptr) {
            tree_size--;
            return node;
        }
        node->height = heightSubtree(node);
        return rebalance(node);
    }

    void erase(const T &elem) {
        root = removeNode(root, elem);
    }

    Set &operator=(const Set &rvalue) {
        if (this == &rvalue) {
            return *this;
        }
        destroy(root);
        tree_size = 0;
        root = nullptr;
        for (const T &key: rvalue) {
            insert(key);
        }
        return *this;
    }

    typedef const_iterator<T> iterator;

    iterator begin() const {
        if (root == nullptr)
            return end();
        Node *temp = root;
        return iterator(nearestRight(temp), root);
    }

    iterator end() const {
        return iterator(nullptr, root);
    }

    iterator find(const T &elem) const {
        iterator temp = lower_bound(elem);
        if (temp == end() || (*temp < elem || elem < *temp)) {
            return end();
        } else {
            return temp;
        }
    }

    iterator lower_bound(const T &elem) const {
        if (root == nullptr) {
            return end();
        }
        Node *node = root;
        Node *ans = nullptr;
        while (node) {
            if (node->key < elem) {
                node = node->right;
            } else if (elem < node->key) {
                ans = node;
                if (node->left) {
                    node = node->left;
                } else {
                    return iterator(node, root);
                }
            } else {
                return iterator(node, root);
            }
        }
        return iterator(ans, root);
    }

    void destroy(Node *node) {
        if (node == nullptr) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete (node);
    }

    ~Set() {
        destroy(root);
        tree_size = 0;
    }

private:
    size_t tree_size;
    Node *root;
};

template<class T>
class const_iterator {
public:
    typedef Node<T> Node;

    const_iterator() = default;

    explicit const_iterator(Node const *node, Node const *root) : node(node), root(root) {}

    Node const *nearestLeft(Node const *node) {
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    Node const *nearestRight(Node const *node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    const const_iterator &operator++() {
        if (node == nullptr) {
            return *this;
        }
        if (node->right) {
            node = nearestRight(node->right);
        } else {
            Node const *temp = node;
            do {
                temp = temp->parent;
            } while (temp && temp->key < node->key);
            node = temp;
        }
        return *this;
    }

    const const_iterator operator++(int junk) {
        const_iterator temp = *this;
        ++(*this);
        return temp;
    }

    const const_iterator &operator--() {
        if (node == nullptr) {
            node = root;
            node = nearestLeft(node);
            return *this;
        }
        if (node->left) {
            node = nearestLeft(node->left);
        } else {
            Node const *temp = node;
            do {
                temp = temp->parent;
            } while (temp && node->key < temp->key);
            node = temp;
        }
        return *this;
    }

    const const_iterator operator--(int junk) {
        const_iterator temp = *this;
        --(*this);
        return temp;
    }

    const T &operator*() const {
        return node->key;
    }

    const T *operator->() const {
        return &(node->key);
    }

    bool operator==(const const_iterator &rvalue) const {
        return node == rvalue.node;
    }

    bool operator!=(const const_iterator &rvalue) const {
        return !((*this) == rvalue);
    }

private:
    Node const *node;
    Node const *root;
};
