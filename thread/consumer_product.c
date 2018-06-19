#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

typedef struct Node
{
    int data;
    struct Node* next;
}Node,*pNode,**ppNode;

pNode BuyNode(int x)
{
    pNode newNode=(pNode)malloc(sizeof(Node));
    if(newNode==NULL)
    {
        perror("malloc");
        exit(1);
    }
    newNode->data=x;
    newNode->next=NULL;
    return newNode;
}

void InitList(ppNode head)
{
    *head=BuyNode(0);
}

int IsEmpty(pNode head)
{
    return head->next==NULL?1:0;
}

void pushList(pNode head,int x)
{
    pNode n=BuyNode(x);
    n->next=head->next;
    head->next=n;
}

int popList(pNode head,int* x)
{
    if(IsEmpty(head))
        return -1;

    pNode n=head->next;
    head->next=n->next;
    *x=n->data;
    free(n);
    return 0;
}

//void Print(pNode head)
//{
//    if(!IsEmpty(head))
//    {
//        head=head->next;
//        while(head)
//        {
//            printf("%d ",head->data);
//           head=head->next;
//        }   
//       printf("\n");
//    }
//}
//消费者
void* runC(void* arg)
{
    pNode lhead = *((ppNode)arg);
    int d=0;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(lhead->next==NULL)
        {
            printf("consumer need to wait...\n");
            pthread_cond_wait(&cond,&mutex);
        }
        popList(lhead,&d);
        printf("consumer get data:%d\n",d);
        sleep(2);
    }

}

//生产者
void* runP(void* arg)
{
    pNode lhead = *((ppNode)arg);
    int d=0;
    while(1)
    {
        sleep(1);
        d = rand()%100+1;
        pushList(lhead,d);
        printf("product data:%d\n",d);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
}

int main()
{
    pthread_t c,p;

    pNode head=NULL;
    InitList(&head);
    
    srand((unsigned long)time(NULL));

    pthread_create(&c,NULL,runC,(void*)&head);
    pthread_create(&p,NULL,runP,(void*)&head);

    pthread_join(c,NULL);
    pthread_join(p,NULL);

    return 0;
}
