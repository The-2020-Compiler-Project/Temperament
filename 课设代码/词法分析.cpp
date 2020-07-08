#include <cstdio>
#include <cstdlib>
#include "string.h"
#include <iostream>

using namespace std;

//�ؼ��ֱ�
static char KT[7][20]={"program","var","integer","real","char","begin","end"};

//����������
static char PT[11][10]={",",":",";",":=","*","/","+","-",".","(",")"};

//��ʶ����
static char IT[1000][50]={""};

//������
static char CT[1000][50]={""};

//�ַ�������
static char cT[1000][100]={""};

//�ж��Ƿ�Ϊ�ؼ���
int KWJudge(char KeyWord[][20], char s[])
{
    for(int i=0;i<7;i++)
    {
        if (!strcmp(KeyWord[i],s))
            return i+1;//�����ҳɹ����򷵻��ֱ���
    }
    return -1;//�����Ҳ��ɹ�����Ϊ��ʶ��
}

//�ж��Ƿ�Ϊ��ĸ
bool letter(char ch)
{
    if ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch=='_'))
        return true;
    else
        return false;
}

//�ж��Ƿ�Ϊ����
bool digit(char ch)
{
    if(ch>='0'&&ch<= '9')
        return true;
    else
        return false;
}

//����Ԥ����ȥ��ע�ͺ����õ��ַ�
void Pretreatment(char s[],int p,int& n)
{
    char Str[10000];
    n=0;
    for(int i=0;i<=p;i++)
    {
        if(s[i]=='/'&&s[i+1]=='/')//ȥ��//֮���ע��
        {
            while(s[i]!='\n')
                i++;
        }
        if(s[i]=='/'&&s[i+1]=='*')//ȥ��/*...*/֮���ע��
        {
            i=i+2;
            while(s[i]!='*'||s[i+1]!='/')
            {
                i++;
                if(s[i]=='$')
                {
                    printf("ע�ͳ�����*/\n");
                    exit(0);
                }
            }
            i=i+2;
        }
        if(s[i]!='\n'&&s[i]!='\t'&&s[i]!='\v'&&s[i]!='\r')//ȥ�������ַ�
        {
            Str[n++]=s[i];
        }
    }
    //cout << Str[n] << endl;
    n-=2;
    //cout << Str[n] << endl;
    Str[n]='\0';
    strcpy(s,Str);
}

void Scanner(int &syn, char r[],char token[],int &p)
{
    int i,n=0;
    char ch;
    ch=r[p];
    while(ch==' ')
    {
        p++;
        ch=r[p];
    }
    for(i=0;i<20;i++)
        token[i]='\0';
    if(letter(r[p]))                                //�ж��Ƿ�Ϊ��ĸ���»���
    {
        token[n++]=r[p];
        p++;
        while(letter(r[p])||digit(r[p]))     //�жϺ����Ƿ�Ϊ��ĸ�����֣����������ȡ
        {
            token[n++]=r[p];
            p++;
        }
        syn=KWJudge(KT,token);               //�ж��Ƿ�Ϊ�ؼ���
        return;
    }
    else if(digit(r[p]))              //�ж��Ƿ�Ϊ����
    {
        while(digit(r[p])||r[p]=='.')            //�жϺ����Ƿ����ֻ�С����
        {
            if(r[p]=='.')
                break;
            token[n++]=r[p];
            p++;
        }
        if(r[p]=='.')
        {
            if(digit(r[p+1]))
            {
                token[n++]=r[p];
                p++;
                while(digit(r[p]))
                {
                    token[n++]=r[p];
                    p++;
                }
            }
        }
        syn=-2;//����
        return;
    }
        //�ж��Ƿ�Ϊ�ַ�
    else if(ch=='\'')
    {
        p++;
        while(r[p]!='\'')
        {
            token[n++]=r[p];
            if(r[p]=='$')
            {
                printf("�ʷ�����\n");
                exit(0);
            }
            else
                p++;
        }
        p++;
        syn=-3;
        return;
    }

    //�ж��Ƿ�Ϊ���
    else if(ch==','||ch==';'||ch=='*'||ch=='/'||ch=='+'||ch=='-'
        ||ch=='.'||ch=='('||ch==')')
    {
        token[0]=r[p];
        for(i=0;i<11; i++)
        {
            if (!strcmp(token,PT[i]))
            {
                syn=8+i;
                break;
            }
        }
        p++;
        return;
    }
    else if(r[p]==':')
    {
        p++;
        if(r[p]=='=')
            syn=11;
        else
        {
            p--;
            syn=9;
        }
        p++;
        return;
    }
    else if(r[p]=='$')//������
        syn=0;
    else
    {//���ܱ����ϴʷ�����ʶ
        printf("%c���ܱ�ʶ��\n",ch);
        exit(0);
    }
}

void token_output()
{
    //��һ���ļ�����ȡ���е�Դ����
    char r[10000];//fname[20];
    char token[20]={0};
    int syn=-3,i,p=0,n;
    FILE *fp, *fp1,*fp2,*fp3;
    //printf("������Ҫ������Դ�ļ����ƣ�");
    //scanf("%s",fname);
    if(!(fp=fopen("source_program.txt","r")))
    {
        printf("���ļ�����\n");
        exit(1);
    }
    if(!(fp3=fopen("sp_aft_pre.txt","w")))
    {
        printf("���ļ�����\n");
        exit(1);
    }
    while(!feof(fp))
    {
        if((r[p]=fgetc(fp))!=EOF)
            p++;
    }
    r[p++]='$';
    r[p]='\0';
    fclose(fp);
    printf("Դ����Ϊ��\n%s\n",r);
    cout << endl;
    //��Դ������й���
    Pretreatment(r,p,n);
    printf("���˺����Ϊ��\n%s\n",r);
    fprintf(fp3,"%s",r);
    fclose(fp3);
    r[n++]='$';
    r[n]='\0';
    p=0;
    if(!(fp1=fopen("token.txt", "w+")))
    {
        printf("���ļ�����\n");
        exit(0);
    }
    printf("\ntoken����Ϊ��\n");
    while(syn!=0)
    {
        Scanner(syn,r,token,p);
        if(syn==-1)                        //��Ϊ��ʶ��
        {
            for(i=0;i<1000;i++)      //�ж��Ƿ��ڱ�ʶ�����У������ڣ������
            {
                if(!strcmp(IT[i],token))
                    break;
                if(!strcmp(IT[i],""))
                {
                    strcpy(IT[i],token);
                    break;
                }
            }
            printf("<IT,%d> %s\n",i+1,token);
            fprintf(fp1,"<IT,%d> %s\n",i+1,token);
        }
        else if(syn>=1&&syn<=7)
        {
            printf("<KT,%d> %s\n",syn,token);
            fprintf(fp1,"<KT,%d> %s\n",syn,token);
        }
        else if(syn==-2)
        {
            for(i=0;i<1000;i++)      //�ж��Ƿ��ڳ������У������ڣ������
            {
                if(!strcmp(CT[i],token))
                    break;
                if(!strcmp(CT[i],""))
                {
                    strcpy(CT[i],token);
                    break;
                }
            }
            printf("<CT,%d> %s\n",i+1,token);
            fprintf(fp1,"<CT,%d> %s\n",i+1,token);
        }
        else if(syn==-3)
        {
            for(i=0;i<1000;i++)      //�ж��Ƿ����ַ����У������ڣ������
            {
                if(!strcmp(cT[i],token))
                    break;
                if(!strcmp(cT[i],""))
                {
                    strcpy(cT[i],token);
                    break;
                }
            }
            printf("<cT,%d> %s\n",i+1,token);
            fprintf(fp1,"<cT,%d> %s\n",i+1,token);
        }
        else if(syn>=8&&syn<=18)
        {
            printf("<PT,%d> %s\n",syn-8,PT[syn-8]);
            fprintf(fp1,"<PT,%d> %s\n",syn-8,PT[syn-8]);
        }
    }
    fclose(fp1);
     if(!(fp2=fopen("Table.txt", "w+")))
    {
        printf("���ļ�����\n");
        exit(0);
    }
     if(!(fp3=fopen("SyTab.txt", "w+")))
    {
        printf("���ļ�����\n");
        exit(0);
    }
    fprintf(fp3,"%s p\n",IT[0]);
    fclose(fp3);
    //����ؼ��ֱ�
    printf("\t\t�ؼ��ֱ�KT\n");
    fprintf(fp2,"\t\t�ؼ��ֱ�KT\n");
    printf("**************************************\n");
    fprintf(fp2,"**************************************\n");
    printf("\t���\t\t�ؼ���\n");
    fprintf(fp2,"\t���\t\t�ؼ���\n");
    for (i=0;i<7;i++)
    {
        printf("\t%d\t\t%s\n",i+1,KT[i]);
        fprintf(fp2,"\t%d\t\t%s\n",i+1,KT[i]);
    }
    printf("**************************************\n\n\n");
    fprintf(fp2,"**************************************\n\n\n");
    //��������
    printf("\t\t�����PT\n");
    fprintf(fp2,"\t\t�����PT\n");
    printf("**************************************\n");
    fprintf(fp2,"**************************************\n");
    printf("\t���\t\t���\n");
    fprintf(fp2,"\t���\t\t���\n");
    for (i=0;i<11;i++)
    {
        printf("\t%d\t\t%s\n",i+1,PT[i]);
        fprintf(fp2,"\t%d\t\t%s\n",i+1,PT[i]);
    }
    printf("**************************************\n\n\n");
    fprintf(fp2,"**************************************\n\n\n");
    //�����ʶ����
    printf("\t\t��ʶ����IT\n");
    fprintf(fp2,"\t\t��ʶ����IT\n");
    printf("**************************************\n");
    fprintf(fp2,"**************************************\n");
    printf("\t���\t\t��ʶ��\n");
    fprintf(fp2,"\t���\t\t��ʶ��\n");
    for (i=0;strcmp(IT[i],"");i++)
    {
        printf("\t%d\t\t%s\n",i+1,IT[i]);
        fprintf(fp2,"\t%d\t\t%s\n",i+1,IT[i]);
    }
    printf("**************************************\n\n\n");
    fprintf(fp2,"**************************************\n\n\n");
     //���������
    printf("\t\t������CT\n");
    fprintf(fp2,"\t\t������CT\n");
    printf("**************************************\n");
    fprintf(fp2,"**************************************\n");
    printf("\t���\t\t����\n");
    fprintf(fp2,"\t���\t\t����\n");
    for (i=0;strcmp(CT[i],"");i++)
    {
        printf("\t%d\t\t%s\n",i+1,CT[i]);
        fprintf(fp2,"\t%d\t\t%s\n",i+1,CT[i]);
    }
    printf("**************************************\n\n\n");
    fprintf(fp2,"**************************************\n\n\n");
     //����ַ���
    printf("\t\t�ַ���CT\n");
    fprintf(fp2,"\t\t�ַ���CT\n");
    printf("**************************************\n");
    fprintf(fp2,"**************************************\n");
    printf("\t���\t\t�ַ�\n");
    fprintf(fp2,"\t���\t\t�ַ�\n");
    for (i=0;strcmp(cT[i],"");i++)
    {
        printf("\t%d\t\t%s\n",i+1,cT[i]);
        fprintf(fp2,"\t%d\t\t%s\n",i+1,cT[i]);
    }
    printf("**************************************\n\n\n");
    fprintf(fp2,"**************************************\n\n\n");
    fclose(fp2);
}
/*
int main()
{
    token_output();
    return 0;
}
*/
