//this code is represent the delition of first node of singly linked list

#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *next;
};

struct node *delete_first_node(struct node *head)
{
    struct node *ptr = head;
    ptr = (struct node *)malloc(sizeof(struct node));
    head = head->next;
    free(ptr);
    ptr = NULL;
    return head;
}

void print_ll(struct node *n)
{
    while (n != NULL)
    {
        printf("%d ->", n->data);
        n = n->next;
    }
    printf("NULL\n");
}
int count(struct node *n)
{
    struct node *ptr = n;
    int cnt = 0;
    while (ptr != NULL)
    {
        ptr = ptr->next;
        cnt++;
    }
    return cnt;
}

int main()
{
    struct node *head = NULL;
    struct node *second = NULL;
    struct node *third = NULL;

    head = (struct node *)malloc(sizeof(struct node));
    second = (struct node *)malloc(sizeof(struct node));
    third = (struct node *)malloc(sizeof(struct node));

    if (head == NULL || second == NULL || third == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    head->data = 1;
    head->next = second;

    second->data = 2;
    second->next = third;

    third->data = 3;
    third->next = NULL;

    printf("Count of nodes before adding: %d\n", count(head));
    print_ll(head);

    head = delete_first_node(head);

    printf("Count of nodes after adding: %d\n", count(head));
    print_ll(head);

    // Free the allocated memory
    // free(third->next);
    free(third);
    free(second);
    free(head);

    return 0;
}
