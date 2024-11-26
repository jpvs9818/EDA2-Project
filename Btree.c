#include <stdio.h>
#include <stdlib.h>

// B-Tree Node Structure
typedef struct Node {
    int total;             // Number of keys in the node
    int *keys;             // Array of keys
    struct Node **children; // Array of child pointers
    struct Node *parent;   // Pointer to parent node
} Node;

// B-Tree Structure
typedef struct BTree {
    Node *root;            // Pointer to the root node
    int order;             // Order of the B-Tree
} BTree;

// Function to create a new B-Tree node
Node *createNode(int order) {
    Node *node = (Node *)malloc(sizeof(Node));
    int maxKeys = order * 2;

    node->keys = (int *)malloc((maxKeys + 1) * sizeof(int));
    node->children = (Node **)malloc((maxKeys + 2) * sizeof(Node *));
    node->total = 0;
    node->parent = NULL;

    for (int i = 0; i < maxKeys + 2; i++) {
        node->children[i] = NULL;
    }

    return node;
}

// Function to create a B-Tree
BTree *createBTree(int order) {
    BTree *tree = (BTree *)malloc(sizeof(BTree));
    tree->order = order;
    tree->root = createNode(order);
    return tree;
}

// Function to perform binary search within a node
int binarySearch(Node *node, int key) {
    int low = 0, high = node->total - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (node->keys[mid] == key) return mid;
        else if (node->keys[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return low;
}

// Check for underflow in a node
int isUnderflow(BTree *tree, Node *node) {
    return node->total < tree->order;
}

// Remove a key from a node
void removeKeyFromNode(Node *node, int index) {
    for (int i = index; i < node->total - 1; i++) {
        node->keys[i] = node->keys[i + 1];
        node->children[i + 1] = node->children[i + 2];
    }
    node->total--;
}

// Borrow a key from a sibling
void borrowFromSibling(BTree *tree, Node *node, int index, Node *parent, int borrowFromRight) {
    if (borrowFromRight) {
        Node *rightSibling = parent->children[index + 1];
        node->keys[node->total] = parent->keys[index];
        node->children[node->total + 1] = rightSibling->children[0];
        if (node->children[node->total + 1] != NULL) {
            node->children[node->total + 1]->parent = node;
        }
        node->total++;

        parent->keys[index] = rightSibling->keys[0];
        removeKeyFromNode(rightSibling, 0);
    } else {
        Node *leftSibling = parent->children[index - 1];
        for (int i = node->total; i > 0; i--) {
            node->keys[i] = node->keys[i - 1];
            node->children[i + 1] = node->children[i];
        }
        node->children[1] = node->children[0];
        node->keys[0] = parent->keys[index - 1];
        node->children[0] = leftSibling->children[leftSibling->total];
        if (node->children[0] != NULL) {
            node->children[0]->parent = node;
        }
        node->total++;

        parent->keys[index - 1] = leftSibling->keys[leftSibling->total - 1];
        leftSibling->total--;
    }
}

// Merge two sibling nodes
void mergeNodes(BTree *tree, Node *left, Node *right, int separatorIndex, Node *parent) {
    left->keys[left->total] = parent->keys[separatorIndex];
    left->total++;

    for (int i = 0; i < right->total; i++) {
        left->keys[left->total] = right->keys[i];
        left->children[left->total] = right->children[i];
        if (left->children[left->total] != NULL) {
            left->children[left->total]->parent = left;
        }
        left->total++;
    }
    left->children[left->total] = right->children[right->total];
    if (left->children[left->total] != NULL) {
        left->children[left->total]->parent = left;
    }

    removeKeyFromNode(parent, separatorIndex);
    free(right->keys);
    free(right->children);
    free(right);
}

// Fix underflow by borrowing or merging
void fixUnderflow(BTree *tree, Node *node) {
    if (node == tree->root) {
        if (node->total == 0 && node->children[0] != NULL) {
            tree->root = node->children[0];
            tree->root->parent = NULL;
            free(node->keys);
            free(node->children);
            free(node);
        }
        return;
    }

    Node *parent = node->parent;
    int index = 0;
    while (parent->children[index] != node) {
        index++;
    }

    if (index > 0 && parent->children[index - 1]->total > tree->order) {
        borrowFromSibling(tree, node, index, parent, 0);
    } else if (index < parent->total && parent->children[index + 1]->total > tree->order) {
        borrowFromSibling(tree, node, index, parent, 1);
    } else if (index > 0) {
        mergeNodes(tree, parent->children[index - 1], node, index - 1, parent);
    } else {
        mergeNodes(tree, node, parent->children[index + 1], index, parent);
    }

    if (isUnderflow(tree, parent)) {
        fixUnderflow(tree, parent);
    }
}

// Find the in-order predecessor
int findPredecessor(Node *node) {
    Node *current = node->children[node->total];
    while (current->children[0] != NULL) {
        current = current->children[current->total];
    }
    return current->keys[current->total - 1];
}

// Recursive removal of a key from the B-Tree
void removeKeyRecursive(BTree *tree, Node *node, int key) {
    if (node == NULL) return;

    int i = binarySearch(node, key);

    if (i < node->total && node->keys[i] == key) {
        if (node->children[0] == NULL) {
            removeKeyFromNode(node, i);
            if (isUnderflow(tree, node)) {
                fixUnderflow(tree, node);
            }
        } else {
            int predecessor = findPredecessor(node);
            node->keys[i] = predecessor;
            removeKeyRecursive(tree, node->children[i], predecessor);
        }
    } else if (node->children[0] != NULL) {
        removeKeyRecursive(tree, node->children[i], key);
    } else {
        printf("Key %d not found in the tree.\n", key);
    }
}

// Wrapper function to start recursive removal
void removeKey(BTree *tree, int key) {
    removeKeyRecursive(tree, tree->root, key);
}

// Check if a node is in overflow state
int isOverflow(BTree *tree, Node *node) {
    return node->total > tree->order * 2;
}

// Insert a key into a node
void insertKeyIntoNode(Node *node, int key, Node *rightChild) {
    int i = binarySearch(node, key);

    for (int j = node->total; j > i; j--) {
        node->keys[j] = node->keys[j - 1];
        node->children[j + 1] = node->children[j];
    }
    node->keys[i] = key;
    node->children[i + 1] = rightChild;
    node->total++;
}

// Split a node and return the new node
Node *splitNode(BTree *tree, Node *node) {
    int midIndex = node->total / 2;
    Node *newNode = createNode(tree->order);
    newNode->parent = node->parent;

    for (int i = midIndex + 1; i < node->total; i++) {
        newNode->keys[newNode->total] = node->keys[i];
        newNode->children[newNode->total] = node->children[i];
        if (newNode->children[newNode->total] != NULL) {
            newNode->children[newNode->total]->parent = newNode;
        }
        newNode->total++;
    }

    newNode->children[newNode->total] = node->children[node->total];
    if (newNode->children[newNode->total] != NULL) {
        newNode->children[newNode->total]->parent = newNode;
    }

    node->total = midIndex;
    return newNode;
}

// Recursive insertion handling with node splitting
void addKeyRecursive(BTree *tree, Node *node, int key, Node *rightChild) {
    insertKeyIntoNode(node, key, rightChild);

    if (isOverflow(tree, node)) {
        int promotedKey = node->keys[tree->order];
        Node *newNode = splitNode(tree, node);

        if (node->parent == NULL) {
            // Create a new root
            Node *newRoot = createNode(tree->order);
            newRoot->keys[0] = promotedKey;
            newRoot->children[0] = node;
            newRoot->children[1] = newNode;
            newRoot->total = 1;

            tree->root = newRoot;
            node->parent = newRoot;
            newNode->parent = newRoot;
        } else {
            // Promote the key to the parent
            addKeyRecursive(tree, node->parent, promotedKey, newNode);
        }
    }
}

// Wrapper function for adding a key to the B-Tree
void addKey(BTree *tree, int key) {
    Node *node = tree->root;

    // Traverse to the appropriate leaf node
    while (node->children[0] != NULL) {
        int i = binarySearch(node, key);
        node = node->children[i];
    }

    // Insert the key in the leaf node
    addKeyRecursive(tree, node, key, NULL);
}


// Traverse the B-Tree and print keys
void traverseBTree(Node *node) {
    if (node != NULL) {
        for (int i = 0; i < node->total; i++) {
            traverseBTree(node->children[i]);
            printf("%d ", node->keys[i]);
        }
        traverseBTree(node->children[node->total]);
    }
}

// Free memory for a node recursively
void freeNode(Node *node) {
    if (node != NULL) {
        for (int i = 0; i <= node->total; i++) {
            freeNode(node->children[i]);
        }
        free(node->keys);
        free(node->children);
        free(node);
    }
}

// Free the entire B-Tree
void freeBTree(BTree *tree) {
    freeNode(tree->root);
    free(tree);
}

int main() {
    BTree *tree = createBTree(2);

    addKey(tree, 10);
    addKey(tree, 20);
    addKey(tree, 5);
    addKey(tree, 6);
    addKey(tree, 12);
    addKey(tree, 30);
    addKey(tree, 7);
    addKey(tree, 17);

    printf("B-Tree keys before removal: ");
    traverseBTree(tree->root);
    printf("\n");

    removeKey(tree, 6);
    removeKey(tree, 12);

    printf("B-Tree keys after removal: ");
    traverseBTree(tree->root);
    printf("\n");

    freeBTree(tree);
    return 0;
}

