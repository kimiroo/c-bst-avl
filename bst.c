#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Node {
    int key;
    struct Node* left;
    struct Node* right;
};

struct Node* search_node(struct Node* node, const int key, bool return_parent);
struct Node* get_smallest_node(struct Node* node);
struct Node* create_node(const int key);
int insert_node(struct Node* node, const int key);
int delete_node(struct Node* node, const int key);

int main() {

    int tree_size = 11;
    //int tree_raw[11] = {4, 1, 2, 3, 8, 6, 5, 7, 10, 9, 11};
    int tree_raw[11] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    struct Node* node = create_node(tree_raw[0]);

    for (int i = 1; i < tree_size; i++) {
        printf("Inserting %d...\n", tree_raw[i]);
        insert_node(node, tree_raw[i]);
    }

    printf("finished inserting array\n\n");
    //getchar(); //debug

    printf("=========\n");
    printf("node: %p\nnkey: %d\nleft: %p\nrigh: %p\n", node, node->key, node->left, node->right);
    delete_node(node, 1);
    printf("=========\n");
    printf("node: %p\nnkey: %d\nleft: %p\nrigh: %p\n", node, node->key, node->left, node->right);

    for (int i = 0; i < tree_size; i++) {
        struct Node* temp = search_node(node, tree_raw[i], false);
        if (temp == NULL) continue;
        printf("%d, %p, %p, %p\n", temp->key, temp, temp->left, temp->right);
    }

    //search_node_err(node, 5);

    return 0;
}

struct Node* search_node(struct Node* node, const int key, bool return_parent) {

    struct Node* n = node;
    struct Node* pn = NULL; // parent node
    bool result = false;

    while (n != NULL) {
        if (key < n->key) {
            printf("1");
            pn = n;
            n = n->left;
        } else if (key > n->key) {
            printf("2");
            pn = n;
            n = n->right;
        } else break;
    }
    printf("3\n");
    
    return return_parent? pn : n;
};

struct Node* create_node(const int key) {
    struct Node* n;
    n = malloc(sizeof(struct Node));
    n->key = key;
    n->left = NULL;
    n->right = NULL;

    return n;
}

int insert_node(struct Node* node, const int key) {
    struct Node* t; // target address
    struct Node* n; // child node to be inserted

    // create a child node to be inserted
    n = create_node(key);

    t = search_node(node, key, true);

    if (key < t->key) t->left = n;
    else t->right = n;

    return 0;
}

int delete_node(struct Node* node, const int key) {
    //struct Node* n;
    struct Node* pn; // parent node
    struct Node* tn; // target node
    int counter = 0;

    pn = search_node(node, key, true);
    tn = search_node(node, key, false);

    if (pn == NULL) printf("pn is NULL!\n"); //debug
    printf("node: %p\nnkey: %d\nleft: %p\nrigh: %p\n", node, node->key, node->left, node->right); //debug

    if (tn->left != NULL) counter ++;
    if (tn->right != NULL) counter ++;

    switch (counter) {
        case 0:
            if (tn->key < pn->key) pn->left = NULL;
            else pn->right = NULL;

            free(tn);

            break;
        
        case 1:
            if (pn != NULL) {
                printf("good!\n"); //debug
                if (tn->left == NULL) pn->right = tn->right;
                else pn->left = tn->left;
            } else {
                /*
                !!! BROKEN !!!
                */

                printf("NULL!\n"); //debug
                if (tn->left == NULL) node = tn->right;
                else node = tn->left;

                tn->right == NULL; //debug
                tn->left == NULL; //debug
            }
            free(tn);

            break;
        
        case 2:
            struct Node* sn = get_smallest_node(tn->right); // get smallest node from right sub nodes
            struct Node* p_sn = search_node(tn, sn->key, true); // parent node of sn

            sn->right = tn->right; // transfer child node
            sn->left = tn->left; // transfer child node
            p_sn->left = NULL; // always left since it's smallest

            /*
            TODO: WRAP WITH "ROOT NODE" HANDLING LIKE CASE 1
            */

            if (tn->key < pn->key) pn->left = sn;
            else pn->right = sn;

            sn->key == sn->right->key? sn->right = NULL : NULL; // NULL the child if self-reference

            free(tn);

            break;
        
        default:
            // ? you somehow broke the code lol
            break;
    }

    printf("node: %p\nnkey: %d\nleft: %p\nrigh: %p\n", node, node->key, node->left, node->right); //debug

    return 0;
}

struct Node* get_smallest_node(struct Node* node) {

    struct Node* cn = node; // current node
    struct Node* pn; // parent node

    while (cn != NULL) {
        pn = cn;
        cn = cn->left;
    }

    //debug
    printf("sm key: %d\n", pn->key);
    
    return pn;
}