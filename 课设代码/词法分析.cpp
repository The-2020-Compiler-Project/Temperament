#include <cstdio>
#include <cstdlib>
#include "string.h"
#include <iostream>

using namespace std;

//关键字表
static char KT[7][20]={"program","var","integer","real","char","begin","end"};

//界符运算符表
static char PT[11][10]={",",":",";",":=","*","/","+","-",".","(",")"};

//标识符表
static char IT[1000][50]={""};

//常数表
static char CT[1000][50]={""};

//字符常量表
static char cT[1000][100]={""};

//判断是否为关键字
int KWJudge(char KeyWord[][20], char s[])
{
    for(int i=0;i<7;i++)
    {
        if (!strcmp(KeyWord[i],s))
            return i+1;//若查找成功，则返回种别码
    }
    return -1;//若查找不成功，则为标识符
}

//判断是否为字母
bool letter(char ch)
{
    if ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch=='_'))
        return true;
    else
        return false;
}

//判断是否为数字
bool digit(char ch)
{
    if(ch>='0'&&ch<= '9')
        return true;
    else
        return false;
}

//编译预处理，去掉注释和无用的字符
void Pretreatment(char s[],int p,int& n)
{
    char Str[10000];
    n=0;
    for(int i=0;i<=p;i++)
    {
        if(s[i]=='/'&&s[i+1]=='/')//去掉//之后的注释
        {
            while(s[i]!='\n')
                i++;
        }
        if(s[i]=='/'&&s[i+1]=='*')//去掉/*...*/之间的注释
        {
            i=i+2;
            while(s[i]!='*'||s[i+1]!='/')
            {
                i++;
                if(s[i]=='$')
                {
                    printf("注释出错，无*/\n");
                    exit(0);
                }
            }
            i=i+2;
        }
        if(s[i]!='\n'&&s[i]!='\t'&&s[i]!='\v'&&s[i]!='\r')//去掉无用字符
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
    if(letter(r[p]))                                //判断是否为字母或下划线
    {
        token[n++]=r[p];
        p++;
        while(letter(r[p])||digit(r[p]))     //判断后面是否为字母或数字，是则继续读取
        {
            token[n++]=r[p];
            p++;
        }
        syn=KWJudge(KT,token);               //判断是否为关键字
        return;
    }
    else if(digit(r[p]))              //判断是否为数字
    {
        while(digit(r[p])||r[p]=='.')            //判断后面是否数字或小数点
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
        syn=-2;//常数
        return;
    }
        //判断是否为字符
    else if(ch=='\'')
    {
        p++;
        while(r[p]!='\'')
        {
            token[n++]=r[p];
            if(r[p]=='$')
            {
                printf("词法错误！\n");
                exit(0);
            }
            else
                p++;
        }
        p++;
        syn=-3;
        return;
    }

    //判断是否为界符
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
    else if(r[p]=='$')//结束符
        syn=0;
    else
    {//不能被以上词法分析识
        printf("%c不能被识别\n",ch);
        exit(0);
    }
}

void token_output()
{
    //打开一个文件，读取其中的源程序
    char r[10000];//fname[20];
    char token[20]={0};
    int syn=-3,i,p=0,n;
    FILE *fp, *fp1,*fp2,*fp3;
    //printf("请输入要分析的源文件名称：");
    //scanf("%s",fname);
    if(!(fp=fopen("source_program.txt","r")))
    {
        printf("打开文件出错！\n");
        exit(1);
    }
    if(!(fp3=fopen("sp_aft_pre.txt","w")))
    {
        printf("打开文件出错！\n");
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
    printf("源程序为：\n%s\n",r);
    cout << endl;
    //对源程序进行过滤
    Pretreatment(r,p,n);
    printf("过滤后程序为：\n%s\n",r);
    fprintf(fp3,"%s",r);
    fclose(fp3);
    r[n++]='$';
    r[n]='\0';
    p=0;
    if(!(fp1=fopen("token.txt", "w+")))
    {
        printf("打开文件出错！\n");
        exit(0);
    }
    printf("\ntoken序列为：\n");
    while(syn!=0)
    {
        Scanner(syn,r,token,p);
        if(syn==-1)                        //若为标识符
        {
            for(i=0;i<1000;i++)      //判断是否在标识符表中，若不在，则加入
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
            for(i=0;i<1000;i++)      //判断是否在常数表中，若不在，则加入
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
            for(i=0;i<1000;i++)      //判断是否在字符表中，若不在，则加入
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
        printf("打开文件出错！\n");
        exit(0);
    }
     if(!(fp3=fopen("SyTab.txt", "w+")))
    {
        printf("打开文件出错！\n");
        exit(0);
    }
    fprintf(fp3,"%s p\n",IT[0]);
    fclose(fp3);
    //输出关键字表
    printf("\t\t关键字表KT\n");
    fprintf(fp2,"\t\t关键字表KT\n");
    printf("**************************************\n");
    fprintf(fp2,"**************************************\n");
    printf("\t编号\t\t关键字\n");
    fprintf(fp2,"\t编号\t\t关键字\n");
    for (i=0;i<7;i++)
    {
        printf("\t%d\t\t%s\n",i+1,KT[i]);
        fprintf(fp2,"\t%d\t\t%s\n",i+1,KT[i]);
    }
    printf("**************************************\n\n\n");
    fprintf(fp2,"**************************************\n\n\n");
    //输出界符表
    printf("\t\t界符表PT\n");
    fprintf(fp2,"\t\t界符表PT\n");
    printf("**************************************\n");
    fprintf(fp2,"**************************************\n");
    printf("\t编号\t\t界符\n");
    fprintf(fp2,"\t编号\t\t界符\n");
    for (i=0;i<11;i++)
    {
        printf("\t%d\t\t%s\n",i+1,PT[i]);
        fprintf(fp2,"\t%d\t\t%s\n",i+1,PT[i]);
    }
    printf("**************************************\n\n\n");
    fprintf(fp2,"**************************************\n\n\n");
    //输出标识符表
    printf("\t\t标识符表IT\n");
    fprintf(fp2,"\t\t标识符表IT\n");
    printf("**************************************\n");
    fprintf(fp2,"**************************************\n");
    printf("\t编号\t\t标识符\n");
    fprintf(fp2,"\t编号\t\t标识符\n");
    for (i=0;strcmp(IT[i],"");i++)
    {
        printf("\t%d\t\t%s\n",i+1,IT[i]);
        fprintf(fp2,"\t%d\t\t%s\n",i+1,IT[i]);
    }
    printf("**************************************\n\n\n");
    fprintf(fp2,"**************************************\n\n\n");
     //输出常数表
    printf("\t\t常数表CT\n");
    fprintf(fp2,"\t\t常数表CT\n");
    printf("**************************************\n");
    fprintf(fp2,"**************************************\n");
    printf("\t编号\t\t常数\n");
    fprintf(fp2,"\t编号\t\t常数\n");
    for (i=0;strcmp(CT[i],"");i++)
    {
        printf("\t%d\t\t%s\n",i+1,CT[i]);
        fprintf(fp2,"\t%d\t\t%s\n",i+1,CT[i]);
    }
    printf("**************************************\n\n\n");
    fprintf(fp2,"**************************************\n\n\n");
     //输出字符表
    printf("\t\t字符表CT\n");
    fprintf(fp2,"\t\t字符表CT\n");
    printf("**************************************\n");
    fprintf(fp2,"**************************************\n");
    printf("\t编号\t\t字符\n");
    fprintf(fp2,"\t编号\t\t字符\n");
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
