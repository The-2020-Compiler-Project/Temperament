#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

using namespace std;
#define MAXs 50
struct Quad
{//��Ԫʽ
    char op[20];
	char ag1[20];
	char ag2[20];
	char res[20];
}quad[100];

struct middle
{//�м����
    char x[10]=" ";
	char y[10];
};

struct middle mi[50],dd[50];

class yuyi
{
public:
    void doit();
    int saomiao();
    int fanyi();
    int fuzhi();
    int jieshu();
    int  gg(char *ag);
    char *program();
    char *chengchu();
    char *xuan();
    char *jiajian();
    char *newt();
    void zhongj();
    void four(const char* op,const char* ag1,const char* ag2,const char* res);
private:
    char prog[10000],ht[10000],ch,tt;
    int cot,p,m,n,sum,ii,N,t,ppt;
    int kk=0,lg=0,k=0,i=0,dian=0,mid=0,midd=0;
    const char* key[7] = {"begin","program","var","real","integer","char","end"};
};

void yuyi::doit()
{//������
    p=0;
    //char a[100];           //��ȡ���ļ�·��
    char fk;           //��ȡ���ļ�·��
    FILE *fp;
    /*cout<<"������Դ�ļ�·������׺��:";
    while(1)
    {
        gets(a);
        if((fp=fopen(a,"r"))!=NULL)
            break;      //��ȡ�ļ����ݣ��������ļ�ָ��
        else
            cout<<"�ļ�·������!����������:";
	}*/
    if (!(fp = fopen("sp_aft_pre.txt", "r")))
    {
        cout << "���ļ�����" << endl;
        exit(1);
    }
     while(ch!=EOF){
        ch=fgetc(fp);
        if(ch==EOF)
            break;            //ɨ�����
        else{
            fseek(fp,-1,1);           //����һ���ֽڿ�ʼʶ��
            fk=fgetc(fp);
            prog[p++]=fk;
        }
    }
	p=0;
	saomiao();
	fanyi();

}
char *yuyi::newt()
{//�ú�������һ���µ���ʱ������
	char *aa;
	char bb[MAXs];
	aa=(char *)malloc(MAXs);
	k++;
	sprintf(bb,"%d",k);
	//itoa(k,bb,10);//������ֵת��Ϊ�ַ���,��һ��������Ҫת�������֣��ڶ���������Ҫд��ת�������Ŀ���ַ�����������������ת������ʱ���õĻ�����
	strcpy(aa+1,bb);
	aa[0] = 't';
	return(aa);
}
int yuyi::fanyi()
{//����fanyi��ԭ���﷨�����Ļ����ϲ�����Ӧ�����嶯���������봮�������Ԫʽ���С���ʵ��������ֻ�Ա��ʽ����ֵ�����з���
    char *jin;
	jin=(char *)malloc(MAXs);
	int pt=0;
	kk =0;
	jin=program();
	saomiao();
	while(cot != 1)
    {

        if(cot==3)
        {
            zhongj();
        }

    }
	if(cot == 1)
	{
		saomiao();
		pt=jieshu();
		if(cot == 7)
		{
			saomiao();
			four("end",jin,"_","_");
			//if(cot==0 && kk==0)
				//cout<<"success";
		}
		/*else
		{
			if(kk!=1)
				cout<<"ȱ��end";
			kk=1;
		}*/
	}
	else
	{
		cout<<"begin����";
		kk=1;
	}
    return pt;
}
void yuyi::zhongj()
{
    int l,ik;
    l=mid;
    while(cot!=18)
    {
        saomiao();
        if(cot==8)
        {
            strcpy(dd[mid].x,ht);
            mid++;
        }
        else if(cot==4||cot==5||cot==6)
        {
            for(ik=l;ik<mid;ik++)
            {
                strcpy(dd[ik].y,key[cot-1]);
            }
        }
    }
    saomiao();
}
char *yuyi::program()
{//programʶ��
    char *jin;
	jin=(char *)malloc(MAXs);
    if(cot == 2)
	{
        saomiao();
        if(cot == 8)//�ַ���
        {
            strcpy(jin,ht);
            four("program",jin,"_","_");
        }
	}
	return jin;
}
int yuyi::jieshu()
{//�ֺ�ʶ��
	int pt = 0;
	pt = fuzhi();
	while (cot == 18)
	{
		saomiao();
		pt = fuzhi();
	}
	return pt;
}
int yuyi::fuzhi()
{//��ֵ����ʶ��
	char tt[MAXs],ag[MAXs];
	int pt = 0;
	switch(cot)
	{
	case 8:
		strcpy(tt,ht);
		saomiao();
		if(cot == 15)
		{
			saomiao();
			strcpy(ag,jiajian());
			four(":=",ag,"_",tt);
			pt = 0;
		}
		else
		{
			cout<<"ȱ�ٸ�ֵ��";
			kk=1;
		}
		return pt;
		break;
	}
}
int yuyi::gg(char *ag)
{
    char ff[100],chr;
    int ll=0;
    strcpy(ff,ag);
    chr=ff[0];
    if(chr>='0'&&chr<='9')
    {
            while((chr>='0'&&chr<='9')||chr=='.')
            {
                chr=ff[i++];
                if(chr=='.')
                {
                    ll++;
                }
            }
            if(ll==0)
                return 9;
            else
                return 10;
    }
    else if(((chr>='a')&&(chr<='z'))||((chr>='A')&&(chr<='Z')))
    {
        if(chr=='t')//t1 t2 t3
        {
            int ss=0;
            while(strcmp(ag,mi[ss].x)!=0)
            {
                ss++;
            }
            return 20+ss;
        }
        else//
        {
            int ss=0;
            while(strcmp(ag,dd[ss].x)!=0)
            {
                ss++;
            }
            return 40+ss;
        }
    }
}
char * yuyi::jiajian(void)
{//�Ӽ�����ʶ��
    int ppk;
	char *res,*ag2,*ag1,*op;
	res = (char *)malloc(MAXs);
	ag2 = (char *)malloc(MAXs);
	ag1 = (char *)malloc(MAXs);
	op = (char *)malloc(MAXs);
	strcpy(ag1,chengchu());
	while(cot == 11 || cot == 12)
	{
		strcpy(op,ht);
		saomiao();
		strcpy(ag2,chengchu());
		strcpy(res,newt());

        ppk=gg(ag1);
        if(ppk==9)
        {
            strcpy(mi[midd].y,key[4]);
        }
        else if(ppk==10)
        {
            strcpy(mi[midd].y,key[3]);
        }
        else if(ppk>=20&& ppk<40)
        {
            ppk=ppk-20;
            strcpy(mi[midd].y,mi[ppk].y);
        }
        else
        {
            ppk=ppk-40;
            strcpy(mi[midd].y,dd[ppk].y);
        }
        strcpy(mi[midd].x,res);
        midd++;


		four(op,ag1,ag2,res);
		strcpy(ag1,res);
	}
	return ag1;
}
char * yuyi::chengchu(void)
{//�˳�����ʶ��
    int ppk;
	char *res,*ag2,*ag1,*op;
	res=(char *)malloc(MAXs);
	ag2=(char *)malloc(MAXs);
	ag1=(char *)malloc(MAXs);
	op=(char *)malloc(MAXs);
	strcpy(ag1,xuan());

	while(cot == 13|| cot == 14)
	{
		strcpy(op,ht);
		saomiao();
		strcpy(ag2,xuan());
		strcpy(res,newt());


        ppk=gg(ag1);
        if(ppk==9)
        {
            strcpy(mi[midd].y,key[4]);
        }
        else if(ppk==10)
        {
            strcpy(mi[midd].y,key[3]);
        }
        else if(ppk>=20&& ppk<40)
        {
            ppk=ppk-20;
            strcpy(mi[midd].y,mi[ppk].y);
        }
        else
        {
            ppk=ppk-40;
            strcpy(mi[midd].y,dd[ppk].y);
        }
         strcpy(mi[midd].x,res);
         midd++;
		/*if(ag1== )
        {
           strcpy(mi[mid].x,res);
           mi[mid].y= ;
           mid++;
        }*/
		four(op,ag1,ag2,res);
		strcpy(ag1,res);
	}
	return ag1;
}
char * yuyi::xuan(void)
{
	char * jin;
	jin=(char *)malloc(MAXs);
	strcpy(jin," ");
	if(cot == 8)//�ַ���
	{
		strcpy(jin,ht);
		saomiao();
	}
	else if(cot == 9)//����
	{
		strcpy(jin,ht);
		saomiao();
		ppt=cot;
	}
	else if(cot == 10)//������
	{
		strcpy(jin,ht);
		saomiao();
		ppt=cot;
	}
	else if(cot == 19)//������kk
	{
		saomiao();
		strcpy(jin,jiajian());
		if(cot == 20)
			saomiao();
		else
		{
			cout<<"')'����";
			kk=1;
		}
	}
	else
	{
		cout<<"'('����";
		kk=1;
	}
	return jin;
}
void yuyi::four(const char* op,const char* ag1,const char* ag2,const char* res)
{//������Ԫʽ��ﲢд���ļ�
	strcpy(quad[lg].res,res);
	strcpy(quad[lg].ag1,ag1);
	strcpy(quad[lg].op,op);
	strcpy(quad[lg].ag2,ag2);
	printf("( %s , %s , %s , %s )\n",quad[lg].op,quad[lg].ag1,quad[lg].ag2,quad[lg].res);
	FILE *fbu;
    fbu=fopen("output.txt","ab");
    if(!fbu)
    {
        cout<<"���ļ�ʧ��\n";
        exit(0);
    }
    fprintf(fbu,"( %s , %s , %s , %s )\n",quad[lg].op,quad[lg].ag1,quad[lg].ag2,quad[lg].res);
    fclose(fbu);
	lg++;

}
int yuyi::saomiao()
{//ɨ��
    for(n=0;n<100;n++)
       ht[n]=0;
    ch=prog[p++];
    m=0;
    while(ch==' '||ch=='\n')
    {
       ch=prog[p++];
    }
    if(ch>='0'&&ch<='9')
    {
            while((ch>='0'&&ch<='9')||ch=='.')
            {
                ht[m++]=ch;
                ch=prog[p++];
                if(ch=='.')
                {
                    dian++;
                }
            }
            ht[m++]='\0';
            p--;
            if(dian==0)
                cot=9;
            else
                cot=10;
    }
    else if(((ch>='a')&&(ch<='z'))||((ch>='A')&&(ch<='Z')))
    {
        while((ch>='a'&&ch<='z')||(ch>='0'&&ch<='9')||(ch>='A'&&ch<='Z'))
        {
            ht[m++]=ch;
            ch=prog[p++];
        }
        ht[m++]='\0';
        p--;
        cot=8;
        for(n=0;n<7;n++)
        {
            if(strcmp(ht,key[n])==0)
            {
                cot=n+1;
                break;
            }
        }
    }
    else
        switch(ch)
        {
    case':': m=0;
        ht[m++]=ch;
        ch=prog[p++];
        if(ch=='=')
        {
            cot=15;
            ht[m++]=ch;
        }
        else
        {
            cot=16;
            p--;
        }
        break;
   case'+':
       cot=11;
       ht[0]=ch;
       break;
   case'-':
       cot=12;
       ht[0]=ch;
       break;
   case'*':
       cot=13;
       ht[0]=ch;
       break;
   case'/':
       cot=14;
       ht[0]=ch;
       break;
   case'=':
       cot=17;
       ht[0]=ch;
       break;
   case';':
       cot=18;
       ht[0]=ch;
       break;
   case'(':
       cot=19;
       ht[0]=ch;
       break;
   case')':
       cot=20;
       ht[0]=ch;
       break;
   case',':
       cot=21;
       ht[0]=ch;
       break;
       //",",":",";",":=","*","/","+","-",".","(",")"
   default:
       cot=-1;
    }
    return cot;
}

void mjtyrhny_main()
{
    yuyi y;
    y.doit();
    /*
    cout << endl;
    for(int i=0;dd[i].x[0]!=' ';i++)
    {
        printf("%s %s\n",dd[i].x,dd[i].y);
    }
    for(int i=0;mi[i].x[0]!=' ';i++)
    {
        printf("%s %s\n",mi[i].x,mi[i].y);
    }*/
}
