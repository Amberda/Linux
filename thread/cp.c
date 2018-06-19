#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

//初始化互斥量
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//初始化条件变量
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//链表节点
typedef struct Node
{
    int data;
    struct Node* next;
}Node,*pNode,**ppNode;

//申请节点
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
//初始化链表
void InitList(ppNode head)
{
    *head=BuyNode(0);
}
//链表判空
int IsEmpty(pNode head)
{
    return head->next==NULL?1:0;
}
//往链表中插入元素---头插
void pushList(pNode head,int x)
{
    pNode n=BuyNode(x);
    n->next=head->next;
    head->next=n;
}
//从链表中删除元素---头删
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
void* runC(void* arg)
{
    pNode lhead = *((ppNode)arg);
    int d=0;
    while(1)                                     
    {                                                                      
        pthread_mutex_lock(&mutex);//加锁
        while(lhead->next==NULL)//如果链表为空
        {
            printf("consumer need to wait...\n");
            pthread_cond_wait(&cond,&mutex);//消费者挂起等待
        }
        popList(lhead,&d);//不为空消费数据
        printf("consumer get data:%d\n",d);
        sleep(2);
    }
}
//生产数据
void* runP(void* arg)
{
    pNode lhead = *((ppNode)arg);
    int d=0;
    while(1)
    {
        sleep(1);//为了演示消费者等待让生产者先运行
        d = rand()%100+1;                
        pushList(lhead,d);//生产数据
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
