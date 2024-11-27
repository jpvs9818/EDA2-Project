#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Order 1
#define VSize 10000
long long unsigned int cont = 0;

typedef struct Node {
    int total;
    int *keys;
    struct Node **children;
    struct Node *parent;
} Node;

typedef struct BTree {
    Node *root;
    int order;
} BTree;

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
    cont++;
    return node;
}

BTree *createBTree(int order) {
    BTree *tree = (BTree *)malloc(sizeof(BTree));
    tree->order = order;
    tree->root = createNode(order);
    return tree;
}

int binarySearch(Node *node, int key) {
    int low = 0, high = node->total - 1, mid;
    while (low <= high) {
        cont++;
        mid = (low + high) / 2;
        if (node->keys[mid] == key) return mid;
        else if (node->keys[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return low;
}

int isUnderflow(BTree *tree, Node *node) {
    return node->total < tree->order;
}

void removeKeyFromNode(Node *node, int index) {
    for (int i = index; i < node->total - 1; i++) {
        node->keys[i] = node->keys[i + 1];
        node->children[i + 1] = node->children[i + 2];
        cont++;
    }
    node->total--;
}

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
        cont += 3;
    }
}

// Merge two sibling nodes
void mergeNodes(BTree *tree, Node *left, Node *right, int separatorIndex, Node *parent) {
    printf("Merging nodes...\n");

    left->keys[left->total] = parent->keys[separatorIndex];
    left->total++;

    for (int i = 0; i < right->total; i++) {
        left->keys[left->total] = right->keys[i];
        left->children[left->total] = right->children[i];
        if (left->children[left->total] != NULL) {
            left->children[left->total]->parent = left;
        }
        left->total++;
        cont++;
    }

    left->children[left->total] = right->children[right->total];
    if (left->children[left->total] != NULL) {
        left->children[left->total]->parent = left;
    }

    removeKeyFromNode(parent, separatorIndex);

    free(right->keys);
    free(right->children);
    free(right);
    right = NULL; // Nullify pointer
    cont += 2;
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
        cont++;
    }
    if (index > 0 && parent->children[index - 1]->total > tree->order) {
        borrowFromSibling(tree, node, index, parent, 0);
        cont++;
    } else if (index < parent->total && parent->children[index + 1]->total > tree->order) {
        borrowFromSibling(tree, node, index, parent, 1);
        cont++;
    } else if (index > 0) {
        mergeNodes(tree, parent->children[index - 1], node, index - 1, parent);
        cont++;
    } else {
        mergeNodes(tree, node, parent->children[index + 1], index, parent);
        cont++;
    }
    if (isUnderflow(tree, parent)) {
        fixUnderflow(tree, parent);
    }
    cont++;
}

// Find the in-order predecessor
int findPredecessor(Node *node) {
    Node *current = node->children[node->total];
    while (current->children[0] != NULL) {
        current = current->children[current->total];
        cont++;
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
        cont += 3;
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
        cont += 2;
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
        cont += 2; //Key movement, pointer adjustment
        newNode->total++;
    }

    newNode->children[newNode->total] = node->children[node->total];
    if (newNode->children[newNode->total] != NULL) {
        newNode->children[newNode->total]->parent = newNode;
    }
    cont++; //Pointer adjustment
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
    cont += 2;
}

// Wrapper function for adding a key to the B-Tree
void addKey(BTree *tree, int key) {
    Node *node = tree->root;

    // Traverse to the appropriate leaf node
    while (node->children[0] != NULL) {
        int i = binarySearch(node, key);
        node = node->children[i];
        cont++;
    }

    // Insert the key in the leaf node
    addKeyRecursive(tree, node, key, NULL);
}


// Traverse the B-Tree and print keys
void traverseBTree(Node *node, int depth) {
    if (node != NULL) {
        // Print indentation based on depth
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }

        // Print all keys in the current node
        printf("[");
        for (int i = 0; i < node->total; i++) {
            printf("%d", node->keys[i]);
            if (i < node->total - 1) {
                printf(", ");
            }
        }
        printf("]\n");

        // Recursively print children
        for (int i = 0; i <= node->total; i++) {
            traverseBTree(node->children[i], depth + 1);
        }
    }
}

// Free memory for a node recursively
void freeNode(Node *node) {
    if (node != NULL) {
        for (int i = 0; i <= node->total; i++) {
            freeNode(node->children[i]);
            node->children[i] = NULL; // Nullify after freeing
        }
        free(node->keys);
        free(node->children);
        free(node);
        node = NULL; // Nullify node pointer
    }
}



// Free the entire B-Tree
void freeBTree(BTree *tree) {
    freeNode(tree->root);
    free(tree);
}

void shuffle(int *array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Random index between 0 and i
        // Swap elements at i and j
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int* createArray(int size){
    int *v = (int*)malloc(sizeof(int)*size);
    for(int i = 0; i < size; i++){
        v[i] = i+1;
    }
    return v;
}
int main() {
    int *v = createArray(VSize);
    FILE *ptr = fopen("BTreeInput1.txt", "w");
    FILE *ptr2 = fopen("BTreeOutput1.txt", "w");
    for(int i = 0; i < 10; i++){
        cont = 0;
        srand(time(NULL) + i);
        shuffle(v, VSize);
        BTree *tree = createBTree(Order);
        for (int j = 0; j < VSize; j++) {
            addKey(tree, v[j]);
        }
        fprintf(ptr, "B,Input,%llu\n", cont);
        cont = 0;
        fprintf(ptr2, "B,Output,%llu\n", cont);
        freeBTree(tree);
    }
    fclose(ptr);
    fclose(ptr2);
    free(v);
    return 0;
}
