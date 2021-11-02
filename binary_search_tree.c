#include <stdlib.h>
#include <stdio.h>

struct binary_search_tree;

typedef struct binary_search_tree *binary_search_tree;
typedef struct binary_search_tree *node_position;

#define true 1
#define false 0
#define bool int

struct binary_search_tree
{
    binary_search_tree left;
    binary_search_tree right;
    int element;
};

binary_search_tree clear_tree(binary_search_tree tree)
{
    if (tree != NULL)
    {
        clear_tree(tree->left);
        clear_tree(tree->right);
        free(tree);
    }
    return NULL;
}

node_position find_max(binary_search_tree tree)
{
    if (tree->right != NULL)
    {
        return find_max(tree->right);
    }
    else
    {
        return tree;
    }
}

node_position find_min(binary_search_tree tree)
{
    if (tree->left != NULL)
    {
        return find_min(tree->left);
    }
    else
    {
        return tree;
    }
}

binary_search_tree new_tree()
{
    return NULL;
}

binary_search_tree insert(binary_search_tree tree, int element)
{
    if (tree == NULL)
    {
        binary_search_tree tree_instance = malloc(sizeof(struct binary_search_tree));
        if (tree_instance == NULL)
        {
            printf("failed to allocate new node for binary search tree: out of memory.\n");
            exit(127);
        }
        else
        {
            tree_instance->element = element;
            tree_instance->left = NULL;
            tree_instance->right = NULL;
        }
        return tree_instance;
    }
    else
    {
        if (tree->element > element)
        {
            tree->left = insert(tree->left, element);
        }
        else if (tree->element < element)
        {
            tree->right = insert(tree->right, element);
        }
        else
        {
            // do nothing
        }
        return tree;
    }
}

node_position find(binary_search_tree tree, int element)
{
    if (tree == NULL)
    {
        return NULL;
    }
    if (tree->element == element)
    {
        return tree;
    }
    else if (tree->element > element)
    {
        return find(tree->left, element);
    }
    else if (tree->element < element)
    {
        return find(tree->right, element);
    }
}

binary_search_tree delete (binary_search_tree tree, int element)
{
    if (tree == NULL)
    { /*element not found*/
        printf("warning: no element %d in the binary search tree, nothing to do.\n", element);
    }
    else if (element < tree->element)
    {
        tree->left = delete (tree->left, element);
    }
    else if (element > tree->element)
    {
        tree->right = delete (tree->right, element);
    }
    else /*element found*/
    {
        node_position tmp_node;
        if (tree->left && tree->right)
        { /*two children*/
            tmp_node = find_min(tree->right);
            tree->element = tmp_node->element;
            tree->right = delete (tree->right, tmp_node->element);
        }
        else
        { /*one child or no child*/
            if (tree->left != NULL)
            { /*has left and no right or no child*/
                tmp_node = tree;
                tree = tree->left;
                free(tmp_node);
            }
            else
            {
                tmp_node = tree;
                tree = tree->right;
                free(tmp_node);
            }
        }
    }
    return tree;
}

void print_binary_search_tree(binary_search_tree tree, int depth)
{
    if (tree->right != NULL)
        print_binary_search_tree(tree->right, depth + 1);
    else
        printf("\n");

    for (int i = 0; i < depth; i++)
    {
        printf("    ");
    }
    printf("%d\n", tree->element);

    if (tree->left != NULL)
        print_binary_search_tree(tree->left, depth + 1);
    else
        printf("\n");
}

int main(int argc, char const *argv[])
{
    binary_search_tree tree = clear_tree(NULL);
    tree = insert(tree, 6);
    tree = insert(tree, 5);
    tree = insert(tree, 3);
    tree = insert(tree, 1);
    tree = insert(tree, 7);
    tree = insert(tree, 8);
    tree = insert(tree, 2);
    tree = insert(tree, 9);
    tree = insert(tree, 4);
    tree = insert(tree, 0);

    print_binary_search_tree(tree, 0);

    tree = delete (tree, 8);
    tree = delete (tree, 3);

    printf("--- deleted 8 and 3 ---\n");

    print_binary_search_tree(tree, 0);
    return 0;
}
