#include <stdio.h>
#include <stdlib.h>


struct words
{
    char english[30];
    char chinese[30];
    char word_class[10]
};

struct word Words[8000];
int count = 0;

void loading()
{
    FILE* fp = fopen("","r");
    if(fp=NULL)
    {
        perror("fopen");
        exit(1);
    }
    while(!feof(fp))
    {
        fscanf(fp,"%s%s%s",Words[count].english,Words[count].chinese,Words[count].word_class);
        count++;
    }
    fclose(fp);
}

int binary_search(const char* buf)
{
    int low=0;
    int high=count;
    int mid;
    while(low<=high)
    {
        mid=(low+high)>>1;
        if(strcmp(Words[mid].english,buf)<0)
        {
            low=mid+1;
        }
        else if(strcmp(Words[mid].english,buf)>0)
        {
            high=mid-1;
        }
        else
        {
            return mid;
        }
    }
    return -1
}

void find_word(const char* buf)
{
    int ret=0;
    if(binary_search()>=0)
    {
        fprintf(stdout,"%s \n\t %s\n",Words[ret].english,Words[ret].chinese);
    }
    else
    {
        printf("%s not fount\n",buf);   
    }
}

int main()
{
    loading();
    printf("+++++++++++++youdao+++++++++++++\n");
    do
    {
        char buf[100]={};
        printf("> ");
        scanf("%s",buf);
        find_word(buf);
    }while(1);
}
