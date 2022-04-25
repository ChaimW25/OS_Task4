// C program for linked list implementation of stack
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  MAXSIZE 1024
// A structure to represent a stack
struct StackNode {
    char data[MAXSIZE];
    struct StackNode* next;
};

struct StackNode* newNode(char data[])
{
    struct StackNode* stackNode =
            (struct StackNode*)
                    malloc(sizeof(struct StackNode));
    strcpy(stackNode->data, data);
    stackNode->next = NULL;
    return stackNode;
}

//int isEmpty(struct StackNode* root)
//{
//    return !root;
//}

void push(struct StackNode** root, char data[])
{
    struct StackNode* stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;
    printf("%s pushed to stack\n", data);
}

char* pop(struct StackNode** root)
{

    struct StackNode* temp = *root;
    *root = (*root)->next;
    char *popped = temp->data;
    printf("%s\n",popped);
//    free(temp);

    return popped;
}

char* top(struct StackNode* root)
{
    return root->data;
}

int main()
{
    struct StackNode* root = NULL;

    push(&root, "hello");
    push(&root, "what's");
    push(&root, "up");

    printf("%s popped from stack\n", pop(&root));

    printf("Top element is %s\n", top(root));

    return 0;
}




//// C program for linked list implementation of stack
//#include <limits.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//// A structure to represent a stack
//struct StackNode {
//    char data[100];
//    struct StackNode* next;
//};
//
//struct StackNode* newNode(char* data)
//{
//    struct StackNode* stackNode =
//            (struct StackNode*)
//                    malloc(sizeof(struct StackNode));
//    strcpy(stackNode->data,data);
////    printf("%s\n",stackNode->data);
////    stackNode->data = data;
//    stackNode->next = NULL;
//    return stackNode;
//}
//
//int isEmpty(struct StackNode* root)
//{
//    return !root;
//}
//
//void push(struct StackNode** root, char* data)
//{
//    struct StackNode* stackNode = newNode(data);
//    stackNode->next = *root;
//    *root = stackNode;
//    printf("%s pushed to stack\n", stackNode->data);
//}
//
//void pop(struct StackNode** root)
//{
//    if (isEmpty(*root)){
//        printf("stack is empty\n");
//        return ;}
//
//    struct StackNode* temp = *root;
//
//    *root = (*root)->next;
//    char popped[100];
//    strcpy(popped,temp->data);
//    free(temp);
//
//    printf("%s popped from stack\n", popped);
//}
//
//void top(struct StackNode* root)
//{
//    if (isEmpty(root))
//        return ;
//    printf("'%s' is in top of the stack\n", root->data);
//}
//
//int main()
//{
//    struct StackNode* root = NULL;
//
//    push(&root, "oria hello");
//    push(&root, "is");
//    push(&root, "hungry");
//    pop(&root);
//    top(root);
////    printf("%d popped from stack\n", );
//
////    printf("Top element is %d\n", top(root));
//
//
//
//    return 0;
//}