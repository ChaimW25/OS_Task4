#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#define PORT "3490"  // the port users will be connecting to
char data[1000];
struct StackNode* root = NULL;
pthread_mutex_t mut;


#define BACKLOG 10   // how many pending connections queue will hold
int i =0;



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

// A structure to represent a stack
//struct StackNode {
//    char data[1000];
//    struct StackNode* next;
//};

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
//    char popped[1000];
//    strcpy(popped,temp->data);
//    free(temp);
//
//    printf("%s popped from stack\n", popped);
//}
//
//char* top(struct StackNode* root)
//{
//    if (isEmpty(root))
//        return "stack is empty";
//    return root->data;
////    printf("'%s' is in top of the stack\n", root->data);
//}

void *dummy(void *new_fd)
{
    int sock=*(int* )new_fd;
    printf("in1\n");
    printf("zibi %s\n", root->data);
    printf("in2\n");
    pthread_mutex_lock(&mut);
    if(!strncmp("PUSH",data,4)){
        push(&root, data+5);
    }
    else if(!strcmp("TOP",data)){


        char temp[1000];
        printf("top the stack = %s\n", top(root));
        strcat(temp, top(&root));
        printf("cosomo %s\n", temp);
        strcpy(data,"OUTPUT: ");
        strcat(data,temp);
        printf("deede\n");

        printf("we gytftft %s\n",data);
        if(send(sock,data,1000,0)==-1){
            perror("send herer");
        }
    }
    else if(!strncmp("POP",data,3)){
        pop(&root);
    }

    pthread_mutex_unlock(&mut);

//    close(sock);
    return NULL;
}


void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    pthread_mutex_init(&mut,NULL);
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");

            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        if(recv(new_fd,data,1000,0)==-1){
            perror("recv");
        }

        pthread_t thread;
        if (!pthread_create(&thread, NULL, dummy, &new_fd)) { // this is the child process

        }
    }
    pthread_mutex_destroy(&mut);
    close(sockfd);
    return 0;
}