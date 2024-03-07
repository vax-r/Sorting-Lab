#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tree.h"

struct TreeNode *createNode(int data)
{
    struct TreeNode *newNode = malloc(sizeof(struct TreeNode));
    newNode->val = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct TreeNode *generateRandomBinaryTree(int height, int *num_of_nodes)
{
    if (height <= 0 || rand() % 2 == 0)
        return NULL;

    struct TreeNode *root = createNode(rand() % 100);
    (*num_of_nodes) += 1;

    root->left = generateRandomBinaryTree(height - 1, num_of_nodes);
    root->right = generateRandomBinaryTree(height - 1, num_of_nodes);
    return root;
}

void preorderTraversal(struct TreeNode *root, int *result, int *index)
{
    if (!root)
        return;

    result[(*index)++] = root->val;
    preorderTraversal(root->left, result, index);
    preorderTraversal(root->right, result, index);
}

void inorderTraversal(struct TreeNode *root, int *result, int *index)
{
    if (!root)
        return;

    inorderTraversal(root->left, result, index);
    result[(*index)++] = root->val;
    inorderTraversal(root->right, result, index);
}

bool compareTree(int *preorder,
                 int *clone_preorder,
                 int *inorder,
                 int *clone_inorder,
                 int size)
{
    for (int i = 0; i < size; i++)
        if (preorder[i] != clone_preorder[i] || inorder[i] != clone_inorder[i])
            return false;
    return true;
}

void freeTree(struct TreeNode *root)
{
    if (!root)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void)
{
    srand(time(NULL));

    int treeHeight = 100;
    int num_of_nodes = 0;

    struct TreeNode *root = generateRandomBinaryTree(treeHeight, &num_of_nodes);

    int *preorder_seq = malloc((num_of_nodes) * sizeof(int));
    int *inorder_seq = malloc((num_of_nodes) * sizeof(int));

    int preorderIndex = 0, inorderIndex = 0;

    preorderTraversal(root, preorder_seq, &preorderIndex);
    inorderTraversal(root, inorder_seq, &inorderIndex);

    clock_t time = clock();
    struct TreeNode *clone_root =
        buildTree(preorder_seq, num_of_nodes, inorder_seq, num_of_nodes);
    time = clock() - time;
    printf("CPU clock ellapsed : %ld\n", time);

    int *clone_preorder_seq = malloc((num_of_nodes) * sizeof(int));
    int *clone_inorder_seq = malloc((num_of_nodes) * sizeof(int));

    preorderIndex = 0;
    inorderIndex = 0;

    preorderTraversal(clone_root, clone_preorder_seq, &preorderIndex);
    inorderTraversal(clone_root, clone_inorder_seq, &inorderIndex);

    printf("Build Tree is %s\n",
           compareTree(preorder_seq, clone_preorder_seq, inorder_seq,
                       clone_inorder_seq, num_of_nodes)
               ? "the same"
               : "different");

    free(preorder_seq);
    free(inorder_seq);

    freeTree(root);
    freeTree(clone_root);

    return 0;
}