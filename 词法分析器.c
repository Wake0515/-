#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
char *Key[10]= {"main","void","int","char","printf","scanf","else","if","return"}; //定义关键字
char Word[20],ch;                   // 存储识别出的单词流

int IsKey(char *Word)           //识别关键字
{
    int m,i;
    for(i=0; i<9; i++)
    {
        if((m=strcmp(Word,Key[i]))==0)
        {
            if(i==0)
                return 2;
            return 1;
        }
    }
    return 0;
}

int IsAlpha(char c)                  //判断是否为字母
{
    if(((c<='z')&&(c>='a'))||((c<='Z')&&(c>='A'))) return 1;
    else return 0;
}

int IsNum(char c)                 //判断是否为数字
{
    if(c>='0'&&c<='9') return 1;
    else return 0;
}
void scanner(FILE *fp)         //扫描函数
{
    char Word[20]= {'\0'};
    char ch;
    int i,c;
    ch=fgetc(fp);        //获取字符，指针fp并自动指向下一个字符
    if(IsAlpha(ch))             //判断该字符是否是字母
    {
        Word[0]=ch;
        ch=fgetc(fp);
        i=1;
        while(IsNum(ch)||IsAlpha(ch))    //判断该字符是否是字母或数字
        {
            Word[i]=ch;
            i++;
            ch=fgetc(fp);
        }
        Word[i]='\0';       //'\0' 代表字符结束(空格)
        fseek(fp,-1,1);                              //回退一个字符
        c=IsKey(Word);                              //判断是否是关键字
        if(c==0) printf("%s\t!普通标识符\n\n",Word);//不是关键字
        else if(c==2) printf("%s\t~主函数\n\n",Word);
        else     printf("%s\t*关键字\n\n",Word);      //输出关键字
    }

    else                          //开始判断的字符不是字母
        if(IsNum(ch))                 //判断是否是数字
        {
            Word[0]=ch;
            ch=fgetc(fp);
            i=1;
            while(IsNum(ch))
            {
                Word[i]=ch;
                i++;
                ch=fgetc(fp);
            }
            Word[i]='\0';
            fseek(fp,-1,1);                    //回退
            printf("%s\t@无符号实数\n\n",Word);
        }
        else         //开始判断的字符不是字母也不是数字
        {
            Word[0]=ch;
            switch(ch)
            {
            case'[':
            case']':
            case'(':
            case')':
            case'{':
            case'}':
            case',':
            case'"':
            case';':
                printf("%s\t$界符\n\n",Word);
                break;
            case'+':
                ch=fgetc(fp);
                Word[1]=ch;
                if(ch=='=')
                {
                    printf("%s\t-运算符\n\n",Word);//运算符“+=”
                }
                else if(ch=='+')
                {
                    printf("%s\t-运算符\n\n",Word); //判断结果为“++”
                }
                else
                {
                    fseek(fp,-1,1);
                    printf("%s\t-运算符\n\n",Word); //判断结果为“+”
                }
                break;
            case'-':
                ch=fgetc(fp);
                Word[1]=ch;
                if(ch=='=')
                {
                    printf("%s\t-运算符\n\n",Word);
                }
                else if(ch=='-')
                {
                    printf("%s\t-运算符\n\n",Word); //判断结果为“--”
                }
                else
                {
                    fseek(fp,-1,1);
                    printf("%s\t-运算符\n\n",Word); //判断结果为“-”
                }
                break;
            case'*':
            case'/':
            case'!':
            case'=':
                ch=fgetc(fp);
                if(ch=='=')
                {
                    printf("%s\t-运算符\n\n",Word);
                }
                else
                {
                    fseek(fp,-1,1);
                    printf("%s\t-运算符\n\n",Word);
                }
                break;
            case'<':
                ch=fgetc(fp);
                Word[1]=ch;
                if(ch=='=')
                {
                    printf("%s\t-运算符\n\n",Word); //判断结果为运算符“<=”
                }
                else if(ch=='<')
                {
                    printf("%s\t-运算符\n\n",Word); //判断结果为“<<”
                }
                else
                {
                    fseek(fp,-1,1);
                    printf("%s\t-运算符\n\n",Word); //判断结果为“<”
                }
                break;
            case'>':
                ch=fgetc(fp);
                Word[1]=ch;
                if(ch=='=') printf("%s\t-运算符\n\n",Word);
                else
                {
                    fseek(fp,-1,1);
                    printf("%s\t-运算符\n\n",Word);
                }
                break;
            case'%':
                ch=fgetc(fp);
                Word[1]=ch;
                if(ch=='=')
                {
                    printf("%s\t-运算符\n\n",Word);
                }
                if(IsAlpha(ch)) printf("%s\t^类型标识符\n\n",Word);
                else
                {
                    fseek(fp,-1,1);
                    printf("%s\t&取余运算符\n\n",Word);
                }
                break;
            default:
                printf("无法识别字符!\n\n");
                break;
            }
        }
}

main()
{
    char in_fn[30];                          //文件路径
    FILE *fp;
    printf("\n请输入源文件名（包括路径和后缀名）:");
    while(1)
    {
        gets(in_fn);
        if((fp=fopen(in_fn,"r"))!=NULL)  break;  //读取文件内容，并返回文件指针，该指针指向文件的第一个字符
        else printf("文件路径错误!请重新输入:");
    }
    printf("\n******************* 词法分析结果如下 *******************\n");
    do
    {
        ch=fgetc(fp);
        if(ch=='#')   break;         //文件以#结尾，作为扫描结束条件
        else if(ch==' '||ch=='\t'||ch=='\n') {} //忽略空格，空白，和换行
        else
        {
            fseek(fp,-1,1);           //回退一个字节开始识别单词流
            scanner(fp);
        }
    }
    while(ch!='#');
    return(0);
}
