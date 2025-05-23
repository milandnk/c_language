//in this program is helping to find given tree is bst or not

#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

struct node *create_node(int data)
{
    struct node *n;
    n = (struct node *)malloc(sizeof(struct node));
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    return n;
}

void pre_order(struct node *root){
    if(root != NULL){
        printf("%d  ",root->data);
        pre_order(root->left);
        pre_order(root->right);
    }
    
}
void post_order(struct node *root){
    if(root != NULL){
        post_order(root->left);    
        post_order(root->right);   
        printf("%d  ", root->data);
    }
}
void in_order(struct node *root){
    if(root != NULL){
        in_order(root->left);  
        printf("%d  ", root->data);  
        in_order(root->right); 
    }
}

int is_bst(struct node *root){
    static struct node *prev;
    if(root != NULL){
        if (!is_bst(root->left))
        {
            return 0;
        }
        if (prev != NULL && root -> data <= prev->data)
        {
            return 0;
        }
        prev = root;
        return (is_bst(root->right));
    }
    else
    {
        return 1;
    }
    
}

int main()
{
    struct node *p = create_node(4);
    struct node *p1 = create_node(2);
    struct node *p2 = create_node(1);
    struct node *p3 = create_node(3);
    struct node *p4 = create_node(5);

    p->left = p1;
    p->right =p4;
    p1->left = p2;
    p1->right =p3;

    
    in_order(p);
    printf("\n");
    printf("%d",is_bst(p));
}