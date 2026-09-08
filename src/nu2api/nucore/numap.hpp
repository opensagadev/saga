#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/numemory.h"

template <typename K, typename V> struct NuMapNode {
    bool red;
    u8 padding_0x1[3];
    K key;
    V value;
    NuMapNode *links[2];
};

template <typename K, typename V> class NuMap {
    typedef NuMapNode<K, V> Node;

    Node *root;
    u32 count;

    static bool IsRed(const Node *node) {
        return node != NULL && node->red;
    }

    static Node *RotateSingle(Node *root, i32 direction) {
        Node *save = root->links[direction ^ 1];
        root->links[direction ^ 1] = save->links[direction];
        save->links[direction] = root;
        root->red = true;
        save->red = false;
        return save;
    }

    static Node *RotateDouble(Node *root, i32 direction) {
        root->links[direction ^ 1] = RotateSingle(root->links[direction ^ 1], direction ^ 1);
        return RotateSingle(root, direction);
    }

    static Node *CreateNode(const K &key) {
        Node *node = static_cast<Node *>(
            NuMemoryGet()->GetThreadMem()->_BlockAlloc(sizeof(Node), 4, 1, "", NUMEMORY_CATEGORY_NONE));
        if (node != NULL) {
            node->red = true;
            node->key = key;
            node->links[0] = NULL;
            node->links[1] = NULL;
        }
        return node;
    }

  public:
    NuMap() : root(NULL), count(0) {
    }

    ~NuMap() {
        if (root != NULL) {
            if (root->links[0] != NULL) {
                Node *left = root->links[0];
                DeleteNodeLinks(left);
                NuMemoryGet()->GetThreadMem()->BlockFree(left, 0);
            }
            if (root->links[1] != NULL) {
                Node *right = root->links[1];
                DeleteNodeLinks(right);
                NuMemoryGet()->GetThreadMem()->BlockFree(right, 0);
            }
            NuMemoryGet()->GetThreadMem()->BlockFree(root, 0);
        }
        root = NULL;
        count = 0;
    }

    Node *InsertNode(const K &key) {
        Node *inserted;

        if (root == NULL) {
            root = CreateNode(key);
            count++;
            inserted = root;
        } else {
            Node head;
            Node *grandparent = NULL;
            Node *parent = NULL;
            Node *current = NULL;
            Node *great_grandparent = &head;
            i32 direction = 0;
            i32 last_direction = 0;

            inserted = NULL;
            head.red = false;
            head.links[1] = root;
            current = root;

            for (;;) {
                if (current == NULL) {
                    current = CreateNode(key);
                    parent->links[direction] = current;
                    count++;
                    inserted = current;
                } else if (IsRed(current->links[0]) && IsRed(current->links[1])) {
                    current->red = true;
                    current->links[0]->red = false;
                    current->links[1]->red = false;
                }

                if (IsRed(current) && IsRed(parent)) {
                    i32 great_direction = great_grandparent->links[1] == grandparent;
                    if (current == parent->links[last_direction]) {
                        great_grandparent->links[great_direction] = RotateSingle(grandparent, last_direction ^ 1);
                    } else {
                        great_grandparent->links[great_direction] = RotateDouble(grandparent, last_direction ^ 1);
                    }
                }

                if (current->key == key) {
                    break;
                }

                last_direction = direction;
                direction = current->key < key;
                if (grandparent != NULL) {
                    great_grandparent = grandparent;
                }
                grandparent = parent;
                parent = current;
                current = current->links[direction];
            }

            root = head.links[1];
        }

        root->red = false;
        return inserted;
    }

    bool Erase(const K &key) {
        if (root == NULL) {
            return false;
        }

        Node head;
        Node *current = &head;
        Node *parent = NULL;
        Node *grandparent = NULL;
        Node *found = NULL;
        i32 direction = 1;

        head.red = false;
        head.links[1] = root;

        while (current->links[direction] != NULL) {
            i32 last_direction = direction;
            grandparent = parent;
            parent = current;
            current = current->links[direction];
            direction = current->key < key;

            if (current->key == key) {
                found = current;
            }

            if (!IsRed(current) && !IsRed(current->links[direction])) {
                if (IsRed(current->links[direction ^ 1])) {
                    parent->links[last_direction] = RotateSingle(current, direction);
                    parent = parent->links[last_direction];
                } else {
                    Node *sibling = parent->links[last_direction ^ 1];
                    if (sibling != NULL) {
                        if (!IsRed(sibling->links[0]) && !IsRed(sibling->links[1])) {
                            parent->red = false;
                            current->red = true;
                            sibling->red = true;
                        } else {
                            i32 grand_direction = grandparent->links[1] == parent;
                            if (IsRed(sibling->links[last_direction])) {
                                grandparent->links[grand_direction] = RotateDouble(parent, last_direction);
                            } else if (IsRed(sibling->links[last_direction ^ 1])) {
                                grandparent->links[grand_direction] = RotateSingle(parent, last_direction);
                            }

                            current->red = true;
                            grandparent->links[grand_direction]->red = true;
                            grandparent->links[grand_direction]->links[0]->red = false;
                            grandparent->links[grand_direction]->links[1]->red = false;
                        }
                    }
                }
            }
        }

        bool erased = false;
        if (found != NULL) {
            found->key = current->key;
            found->value = current->value;
            parent->links[parent->links[1] == current] = current->links[current->links[0] == NULL];
            NuMemoryGet()->GetThreadMem()->BlockFree(current, 0);
            count--;
            erased = true;
        }

        root = head.links[1];
        if (root != NULL) {
            root->red = false;
        }
        return erased;
    }

    void DeleteNodeLinks(Node *node) {
        if (node->links[0] != NULL) {
            Node *left = node->links[0];
            DeleteNodeLinks(left);
            NuMemoryGet()->GetThreadMem()->BlockFree(left, 0);
        }
        if (node->links[1] != NULL) {
            Node *right = node->links[1];
            DeleteNodeLinks(right);
            NuMemoryGet()->GetThreadMem()->BlockFree(right, 0);
        }
    }

    const Node *FindNode(const K &key) const {
        const Node *node = root;
        while (node != NULL) {
            if (node->key == key) {
                return node;
            }
            node = node->links[node->key < key];
        }
        return NULL;
    }
};

DECOMP_ASSERT(sizeof(NuMap<void *, void *>) == 8, "NuMap size");
