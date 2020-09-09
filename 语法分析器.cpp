#include<stdio.h>
#include<stdlib.h>
#define MaxRuleNum 8
#define MaxVnNum 5
#define MaxVtNum 5
#define MaxStackDepth 20
#define MaxPLength 20
#define MaxStLength 50
using namespace std;

struct pRNode /*����ʽ�Ҳ��ṹ*/
{
    int rCursor;
    struct pRNode *next;
};

struct pNode
{
    int lCursor;
    int rLength; /*�Ҳ�����*/

    struct pRNode *rHead; /*�Ҳ����ͷָ��*/
};

char Vn[MaxVnNum + 1]; /*���ս����*/
int vnNum;
char Vt[MaxVtNum + 1]; /*�ս����*/
int vtNum;
struct pNode P[MaxRuleNum];
int PNum;
char buffer[MaxPLength + 1];
char ch;
char st[MaxStLength]; /*Ҫ�����ķ��Ŵ�*/

struct collectNode
{
    int nVt;
    struct collectNode *next;
};

struct collectNode* first[MaxVnNum + 1]; /*first��*/
struct collectNode* follow[MaxVnNum + 1]; /*follow��*/

int analyseTable[MaxVnNum + 1][MaxVtNum + 1 + 1];
int analyseStack[MaxStackDepth + 1]; /*����ջ*/
int topAnalyse; /*����ջ��*/
void Init();/*��ʼ��*/
int IndexCh(char ch);
void InputVt(); /*�����ս��*/
void InputVn();/*������ս��*/
void ShowChArray(char* collect, int num);/*���Vn��Vt������*/
void InputP();/*����ʽ����*/
bool CheckP(char * st);/*�жϲ���ʽ��ȷ��*/
void First(int U);
void AddFirst(int U, int nCh); /*����first��*/
bool HaveEmpty(int nVn);
void Follow(int V);/*����follow��*/
void AddFollow(int V, int nCh, int kind);
void ShowCollect(struct collectNode **collect);/*���first��follow��*/
void FirstFollow();/*����first��follow*/
void CreateAT();/*����Ԥ�������*/
void ShowAT();/*���������*/
void Identify(char *st);
void InitStack();
void ShowStack();
void Pop();
void Push(int r);

int main()
{
    char todo,ch;
    Init();
    InputVn();
    InputVt();
    InputP();
    getchar();
    FirstFollow();
    printf("����first��Ϊ��");
    ShowCollect(first);
    printf("����follow��Ϊ��");
    ShowCollect(follow);
    CreateAT();
    ShowAT();

    todo = 'y';
    while('y' == todo)
    {
        printf("\n�Ƿ�������о��ͷ���?(y/n):");
        todo = getchar();
        while('y' != todo && 'n' != todo)
        {
            printf("\n(y/n)? ");
            todo = getchar();
        }
        if('y' == todo)
        {
            int i;
            InitStack();
            printf("�������ַ���(�ԡ�#������): ");
            ch = getchar();
            i = 0;
            while('#' != ch && i < MaxStLength)
            {
                if(' ' != ch && '\n' != ch)
                {
                    st[i++] = ch;
                }
                ch = getchar();
            }
            if('#' == ch && i < MaxStLength)
            {
                st[i] = ch;
                Identify(st);
            }
            else
                printf("�������!\n");
        }
    }

    getchar();
}

void Init()
{
    int i,j;
    vnNum = 0;
    vtNum = 0;
    PNum = 0;
    for(i = 0; i <= MaxVnNum; i++)
        Vn[i] = '\0';
    for(i = 0; i <= MaxVtNum; i++)
        Vt[i] = '\0';
    for(i = 0; i < MaxRuleNum; i++)
    {
        P[i].lCursor = NULL;
        P[i].rHead = NULL;
        P[i].rLength = 0;
    }
    PNum = 0;
    for(i = 0; i <= MaxPLength; i++)
        buffer[i] = '\0';
    for(i = 0; i < MaxVnNum; i++)
    {
        first[i] = NULL;
        follow[i] = NULL;
    }
    for(i = 0; i <= MaxVnNum; i++)
    {
        for(j = 0; j <= MaxVnNum + 1; j++)
            analyseTable[i][j] = -1;
    }
}

int IndexCh(char ch)
{
    int n;
    n = 0; /*is Vn?*/
    while(ch != Vn[n] && '\0' != Vn[n])
        n++;
    if('\0' != Vn[n])
        return 100 + n;
    n = 0; /*is Vt?*/
    while(ch != Vt[n] && '\0' != Vt[n])
        n++;
    if('\0' != Vt[n])
        return n;
    return -1;
}
/*���Vn��Vt������*/
void ShowChArray(char* collect)
{
    int k = 0;
    while('\0' != collect[k])
    {
        printf(" %c ", collect[k++]);
    }
    printf("\n");
}
/*������ս��*/
void InputVn()
{
    int inErr = 1;
    int n,k;
    char ch;
    while(inErr)
    {
        printf("\n���������еķ��ս��,ע�⣺");
        printf("������ʼ�����ڵ�һλ,�ԡ�#���Ž���:\n");
        ch = ' ';
        n = 0;
        /*��ʼ������*/
        while(n < MaxVnNum)
        {
            Vn[n++] = '\0';
        }
        n = 0;
        while(('#' != ch) && (n < MaxVnNum))
        {
            if(' ' != ch && '\n' != ch && -1 == IndexCh(ch))
            {
                Vn[n++] = ch;
                vnNum++;
            }
            ch = getchar();
        }
        Vn[n] = '#'; /*�ԡ�#����־���������жϳ����Ƿ�Ϸ�*/
        k = n;
        if('#' != ch)
        {
            if( '#' != (ch = getchar()))
            {
                while('#' != (ch = getchar()))
                    ;
                printf("\n������Ŀ��������!\n");
                inErr = 1;
                continue;
            }
        }
        /*��ȷ��ȷ�ϣ���ȷ��ִ�������棬������������*/
        Vn[k] = '\0';
        ShowChArray(Vn);
        ch = ' ';
        while('y' != ch && 'n' != ch)
        {
            if('\n' != ch)
            {
                printf("����ȷ��(y/n):");
            }
            scanf("%c", &ch);
        }
        if('n' == ch)
        {
            printf("�����������������!\n");
            inErr = 1;
        }
        else
        {
            inErr = 0;
        }
    }
}
/*�����ս��*/
void InputVt()
{
    int inErr = 1;
    int n,k;
    char ch;
    while(inErr)
    {
        printf("\n���������е��ս����ע�⣺");
        printf("�ԡ�#���Ž���:\n");
        ch = ' ';
        n = 0;
        /*��ʼ������*/
        while(n < MaxVtNum)
        {
            Vt[n++] = '\0';
        }
        n = 0;
        while(('#' != ch) && (n < MaxVtNum))
        {
            if(' ' != ch && '\n' != ch && -1 == IndexCh(ch))
            {
                Vt[n++] = ch;
                vtNum++;
            }
            ch = getchar();
        }
        Vt[n] = '#';
        k = n;
        if('#' != ch)
        {
            if( '#' != (ch = getchar()))
            {
                while('#' != (ch = getchar()))
                    ;
                printf("\n������Ŀ��������!\n");
                inErr = 1;
                continue;
            }
        }

        Vt[k] = '\0';
        ShowChArray(Vt);
        ch = ' ';
        while('y' != ch && 'n' != ch)
        {
            if('\n' != ch)
            {
                printf("����ȷ��(y/n):");
            }
            scanf("%c", &ch);
        }
        if('n' == ch)
        {
            printf("��������������룡\n");
            inErr = 1;
        }
        else
        {
            inErr = 0;
        }
    }
}
/*����ʽ����*/
void InputP()
{
    char ch;
    int i = 0, n,num;
    printf("�������ķ�����ʽ�ĸ�����");
    scanf("%d", &num);
    PNum = num;
    getchar(); /*�����س���*/
    printf("\n�������ķ���%d������ʽ,���Իس��ָ�ÿ������ʽ��", num);
    printf("\n");
    while(i < num)
    {
        printf("��%d����", i);
        /*��ʼ��*/
        for(n =0; n < MaxPLength; n++)
            buffer[n] = '\0';
        /*�������ʽ��*/
        ch = ' ';
        n = 0;
        while('\n' != (ch = getchar()) && n < MaxPLength)
        {
            if(' ' != ch)
                buffer[n++] = ch;
        }
        buffer[n] = '\0';

        if(CheckP(buffer))
        {

            pRNode *pt, *qt;
            P[i].lCursor = IndexCh(buffer[0]);
            pt = (pRNode*)malloc(sizeof(pRNode));
            pt->rCursor = IndexCh(buffer[3]);
            pt->next = NULL;
            P[i].rHead = pt;
            n = 4;
            while('\0' != buffer[n])
            {
                qt = (pRNode*)malloc(sizeof(pRNode));
                qt->rCursor = IndexCh(buffer[n]);
                qt->next = NULL;
                pt->next = qt;
                pt = qt;
                n++;
            }
            P[i].rLength = n - 3;
            i++;

        }
        else
            printf("������ź��Ƿ��ɷ֣�����������!\n");
    }
}
/*�жϲ���ʽ��ȷ��*/
bool CheckP(char * st)
{
    int n;
    if(100 > IndexCh(st[0]))
        return false;
    if('-' != st[1])
        return false;
    if('>' != st[2])
        return false;
    for(n = 3; '\0' != st[n]; n ++)
    {
        if(-1 == IndexCh(st[n]))
            return false;
    }
    return true;
}

void First(int U)
{
    int i,j;
    for(i = 0; i < PNum; i++)
    {
        if(P[i].lCursor == U)
        {
            struct pRNode* pt;
            pt = P[i].rHead;
            j = 0;
            while(j < P[i].rLength)
            {
                if(100 > pt->rCursor)
                {

                    AddFirst(U, pt->rCursor);
                    break;
                }
                else
                {
                    if(NULL == first[pt->rCursor - 100])
                    {
                        First(pt->rCursor);
                    }
                    AddFirst(U, pt->rCursor);
                    if(!HaveEmpty(pt->rCursor))
                    {
                        break;
                    }
                    else
                    {
                        pt = pt->next;
                    }
                }
                j++;
            }
            if(j >= P[i].rLength) /*������ʽ�Ҳ������Ƴ���ʱ*/
                AddFirst(U, -1);
        }
    }
}
/*����first��*/
void AddFirst(int U, int nCh)

{
    struct collectNode *pt, *qt;
    int ch; /*���ڴ���Vn*/
    pt = NULL;
    qt = NULL;
    if(nCh < 100)
    {
        pt = first[U - 100];
        while(NULL != pt)
        {
            if(pt->nVt == nCh)
                break;
            else
            {
                qt = pt;
                pt = pt->next;
            }
        }
        if(NULL == pt)
        {
            pt = (struct collectNode *)malloc(sizeof(struct collectNode));
            pt->nVt = nCh;
            pt->next = NULL;
            if(NULL == first[U - 100])
            {
                first[U - 100] = pt;
            }
            else
            {
                qt->next = pt; /*qtָ��first�������һ��Ԫ��*/
            }
            pt = pt->next;
        }
    }
    else
    {
        pt = first[nCh - 100];
        while(NULL != pt)
        {
            ch = pt->nVt;
            if(-1 != ch)
            {
                AddFirst(U, ch);
            }
            pt = pt->next;
        }
    }
}

bool HaveEmpty(int nVn)
{
    if(nVn < 100)
        return false;
    struct collectNode *pt;
    pt = first[nVn - 100];
    while(NULL != pt)
    {
        if(-1 == pt->nVt)
            return true;
        pt = pt->next;
    }
    return false;
}

void Follow(int V)
{
    int i;
    struct pRNode *pt ;
    if(100 == V) /*��Ϊ��ʼ��ʱ*/
        AddFollow(V, -1, 0 );
    for(i = 0; i < PNum; i++)
    {
        pt = P[i].rHead;
        while(NULL != pt && pt->rCursor != V)
            pt = pt->next;
        if(NULL != pt)
        {
            pt = pt->next;
            if(NULL == pt)
            {
                if(NULL == follow[P[i].lCursor - 100] && P[i].lCursor != V)
                {
                    Follow(P[i].lCursor);
                }
                AddFollow(V, P[i].lCursor, 0);
            }
            else
            {
                while(NULL != pt && HaveEmpty(pt->rCursor))
                {

                    AddFollow(V, pt->rCursor, 1);
                    pt = pt->next;
                }
                if(NULL == pt)
                {
                    if(NULL == follow[P[i].lCursor - 100] && P[i].lCursor != V)
                    {
                        Follow(P[i].lCursor);
                    }
                    AddFollow(V, P[i].lCursor, 0);
                }
                else
                {
                    AddFollow(V, pt->rCursor, 1);
                }
            }
        }
    }
}


void AddFollow(int V, int nCh, int kind)
{
    struct collectNode *pt, *qt;
    int ch;
    pt = NULL;
    qt = NULL;
    if(nCh < 100) /*Ϊ�ս��ʱ*/
    {
        pt = follow[V - 100];
        while(NULL != pt)
        {
            if(pt->nVt == nCh)
                break;
            else
            {
                qt = pt;
                pt = pt->next;
            }
        }
        if(NULL == pt)
        {
            pt = (struct collectNode *)malloc(sizeof(struct collectNode));
            pt->nVt = nCh;
            pt->next = NULL;
            if(NULL == follow[V - 100])
            {
                follow[V - 100] = pt;
            }
            else
            {
                qt->next = pt; /*qtָ��follow�������һ��Ԫ��*/
            }
            pt = pt->next;
        }
    }
    else
    {
        if(0 == kind)
        {
            pt = follow[nCh - 100];
            while(NULL != pt)
            {
                ch = pt->nVt;
                AddFollow(V, ch, 0);
                pt = pt->next;
            }
        }
        else
        {
            pt = first[nCh - 100];
            while(NULL != pt)
            {
                ch = pt->nVt;
                if(-1 != ch)
                {
                    AddFollow(V, ch, 1);
                }
                pt = pt->next;
            }
        }
    }
}
/*���first��follow��*/
void ShowCollect(struct collectNode **collect)
{
    int i;
    struct collectNode *pt;
    i = 0;
    while(NULL != collect[i])
    {
        pt = collect[i];
        printf("\n%c:\t", Vn[i]);
        while(NULL != pt)
        {
            if(-1 != pt->nVt)
            {
                printf(" %c", Vt[pt->nVt]);
            }
            else
                printf(" #");
            pt = pt->next;
        }
        i++;
    }
    printf("\n");
}
/*����first��follow*/
void FirstFollow()
{
    int i;
    i = 0;
    while('\0' != Vn[i])
    {
        if(NULL == first[i])
            First(100 + i);
        i++;
    }
    i = 0;
    while('\0' != Vn[i])
    {
        if(NULL == follow[i])
            Follow(100 + i);
        i++;
    }
}
/*����Ԥ�������*/
void CreateAT()
{
    int i;
    struct pRNode *pt;
    struct collectNode *ct;
    for(i = 0; i < PNum; i++)
    {
        pt = P[i].rHead;
        while(NULL != pt && HaveEmpty(pt->rCursor))
        {

            ct = first[pt->rCursor - 100];
            while(NULL != ct)
            {
                if(-1 != ct->nVt)
                    analyseTable[P[i].lCursor - 100][ct->nVt] = i;
                ct = ct->next;
            }
            pt = pt->next;
        }
        if(NULL == pt)
        {

            ct = follow[P[i].lCursor - 100];
            while(NULL != ct)
            {
                if(-1 != ct->nVt)
                    analyseTable[P[i].lCursor - 100][ct->nVt] = i;
                else
                    analyseTable[P[i].lCursor - 100][vtNum] = i;
                ct = ct->next;
            }
        }
        else
        {
            if(100 <= pt->rCursor) /*�����յķ��ս��*/
            {
                ct = first[pt->rCursor - 100];
                while(NULL != ct)
                {
                    analyseTable[P[i].lCursor - 100][ct->nVt] = i;
                    ct = ct->next;
                }
            }
            else /*�ս�����߿�*/
            {
                if(-1 == pt->rCursor)
                {
                    ct = follow[P[i].lCursor - 100];
                    while(NULL != ct)
                    {
                        if(-1 != ct->nVt)
                            analyseTable[P[i].lCursor - 100][ct->nVt] = i;
                        else /*������#��ʱ*/
                            analyseTable[P[i].lCursor - 100][vtNum] = i;
                        ct = ct->next;
                    }
                }
                else /*Ϊ�ս��*/
                {
                    analyseTable[P[i].lCursor - 100][pt->rCursor] = i;
                }
            }
        }
    }
}
/*���������*/
void ShowAT()
{
    int i,j;

    printf("����Ԥ����������£�\n");
    printf("\t|\t");
    for(i = 0; i < vtNum; i++)
    {
        printf("%c\t", Vt[i]);
    }
    printf("#\t\n");

    printf("- - -\t|- - -\t");
    for(i = 0; i <= vtNum; i++)
        printf("- - -\t");
    printf("\n");

    for(i = 0; i < vnNum; i++)
    {
        printf("%c\t|\t", Vn[i]);
        for(j = 0; j <= vtNum; j++)
        {
            if(-1 != analyseTable[i][j])
                printf("R(%d)\t", analyseTable[i][j]);
            else
                printf("error\t");
        }
        printf("\n");
    }
}

void Identify(char *st)
{
    int current,step,r; /*r��ʹ�õĲ���ʽ�����*/
    printf("\n%s�ķ������̣�\n", st);
    printf("����\t��������ջ\t��ǰָʾ�ַ�\tʹ�ò���ʽ���\n");

    step = 0;
    current = 0;
    printf("%d\t",step);
    ShowStack();
    printf("\t\t%c\t\t- -\n", st[current]);

    while('#' != st[current])
    {
        if(100 > analyseStack[topAnalyse])
        {
            if(analyseStack[topAnalyse] == IndexCh(st[current]))
            {

                Pop();
                current++;
                step++;
                printf("%d\t", step);
                ShowStack();
                printf("\t\t%c\t\t��ջ������\n", st[current]);
            }
            else
            {
                printf("%c-%c��ƥ�䣡", analyseStack[topAnalyse], st[current]);
                printf("�˴����Ǵ��ķ��ľ��ӣ�\n");
                return;
            }
        }
        else /*��Ϊ���ս��ʱ*/
        {
            r = analyseTable[analyseStack[topAnalyse] - 100][IndexCh(st[current])];
            if(-1 != r)
            {
                Push(r);
                step++;
                printf("%d\t", step);
                ShowStack();
                printf("\t\t%c\t\t%d\n", st[current], r);
            }
            else
            {
                printf("�˴����Ǵ��ķ��ľ��ӣ�\n");
                return;
            }
        }
    }
    if('#' == st[current])
    {

        if(0 == topAnalyse && '#' == st[current])
        {
            step++;
            printf("%d\t", step);
            ShowStack();
            printf("\t\t%c\t\t�����ɹ�!\n", st[current]);
            printf("%s�Ǹ����ķ��ľ���!\n", st);
        }
        else
        {
            while(topAnalyse > 0)
            {
                if(100 > analyseStack[topAnalyse])
                {
                    printf("�˴����Ǵ��ķ��ľ���!\n");
                    return;
                }
                else
                {
                    r = analyseTable[analyseStack[topAnalyse] - 100][vtNum];
                    if(-1 != r)
                    {
                        Push(r); /*����ʽ�Ҳ������󲿣�ָʾ�����ƶ�*/
                        step++;
                        printf("%d\t", step);
                        ShowStack();
                        if(0 == topAnalyse && '#' == st[current])
                        {
                            printf("\t\t%c\t\t�����ɹ�,������£�\n", st[current]);
                            printf("%s�Ǹ����ķ��ľ���!\n", st);
                        }
                        else
                            printf("\t\t%c\t\t%d\n", st[current], r);
                    }
                    else
                    {
                        printf("�ַ������Ǵ��ķ��ľ��ӣ�\n");
                        return;
                    }
                }
            }
        }
    }
}
/*��ʼ��ջ�����Ŵ�*/
void InitStack()
{
    int i;
    /*����ջ�ĳ�ʼ��*/
    for(i = 0; i < MaxStLength; i++)
        st[i] = '\0';
    analyseStack[0] = -1; /*#(-1)��ջ*/
    analyseStack[1] = 100; /*��ʼ����ջ*/
    topAnalyse = 1;
}
/*��ʾ����ջ������*/
void ShowStack()
{
    int i;
    for(i = 0; i <= topAnalyse; i++)
    {
        if(100 <= analyseStack[i])
            printf("%c", Vn[analyseStack[i] - 100]);
        else
        {
            if(-1 != analyseStack[i])
                printf("%c", Vt[analyseStack[i]]);
            else
                printf("#");
        }
    }
}
/*ջ����ջ*/
void Pop()
{
    topAnalyse--;
}
void Push(int r)
{
    int i;
    struct pRNode *pt;
    Pop();
    pt = P[r].rHead;
    if(-1 == pt->rCursor)
        return;
    topAnalyse += P[r].rLength;
    for(i = 0; i < P[r].rLength; i++)
    {
        analyseStack[topAnalyse - i] = pt->rCursor;/*������ջ*/
        pt = pt->next;
    }
}
