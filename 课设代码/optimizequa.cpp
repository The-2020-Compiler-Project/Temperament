#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <list>
#include <sstream>

using namespace std;

struct middle
{//�м����
    char x[10] = " ";
    char y[10];
};

extern struct middle mi[50],dd[50];

struct token
{
    string name;
    int type;        //2Ϊ��ʱ������3Ϊ���ͣ�1Ϊ����ʱ������4Ϊ�����ͣ���Ϊ0
    bool operator == (const token& t) const             //list remove�����ж��Ƿ����
    {
        return (name == t.name && type == t.type);
    }
};

struct Quatemion        //��Ԫʽ�ṹ��
{
    int block = 1;      //�������ʶ
    int type = -1;      //���� 0 1 2 3
    string op = " ";    //����
    token num1;         //������1
    token num2;         //������2
    token ans;          //���
};

struct Quatemion qua[200];  //��Ԫʽ����
int qua_num;                //��Ԫʽ����

struct Quatemion qua_out[200];  //�Ż�����Ԫʽ����
int out_num;                    //�Ż������Ԫʽ����

struct Quatemion qua_out2[200]; //ѭ���Ż��ĸ�������
int out_num2;

struct Node                  //DAG�ڵ�
{
    int left = -1;           //���ӽڵ��ʶ
    int right = -1;          //���ӽڵ��ʶ
    string op = " ";         //����
    token m_sign;            //�����
    list<token> sign;        //���ӱ��
};

struct DAG
{
    int num = 0;
    Node node[200];
};

struct DAG dag;

void clearDAG(DAG& dag)         //���DAG
{
    for(int i=0;i<dag.num;i++)
    {
        dag.node[i].left=-1;
        dag.node[i].right=-1;
        dag.node[i].op=" ";
        dag.node[i].sign.clear();
    }
    dag.num=0;
}
/*
bool isim_var(string s)     //�ж��Ƿ�Ϊ�м����
{
    string im_var[5] = { "t1","t2","t3","t4","t5" };
    for (int i = 0; i < 5; i++)
    {
        if (s == im_var[i])
            return true;
    }
    return false;
}*/

bool isquaeq(Quatemion* qua_out,Quatemion* qua_out2,int out_num,int out_num2) //�ж�������Ԫʽ���������Ƿ���ͬ
{
    if(out_num2!=out_num)
        return false;
    else
    {
        for(int i=1;i<out_num;i++)
        {
            if(qua_out[i].op!=qua_out2[i].op || qua_out[i].num1.name!=qua_out2[i].num1.name
               || qua_out[i].num2.name!=qua_out2[i].num2.name || qua_out2[i].ans.name!=qua_out[i].ans.name)
                return false;
        }
    }
    return true;
}

void qua_ass(Quatemion* qua_out,Quatemion* qua_out2,int& out_num,int out_num2)
{
    for(int i=1;i<out_num2;i++)
    {
        qua_out[i].op=qua_out2[i].op;
        qua_out[i].num1=qua_out2[i].num1;
        qua_out[i].num2=qua_out2[i].num2;
        qua_out[i].ans =qua_out2[i].ans;
        qua_out[i].type=qua_out2[i].type;
    }
    out_num=out_num2;
}

void indd(string s,middle* dd,middle* mi)         //�ж��м�����Ƿ��ڱ�������,�����������
{
    int i,j,flag=1;
    for(i=0;strcmp(dd[i].x," ")!=0;i++)
    {
        if(strcmp(s.c_str(),dd[i].x)==0)
        {
            flag=0;
            break;
        }
    }
    if(flag)
    {
        for(j=0;strcmp(mi[j].x," ")!=0;j++)
        {
            if(strcmp(s.c_str(),mi[j].x)==0)
                break;
        }
        strcpy(dd[i].x,s.c_str());
        strcpy(dd[i].y,mi[j].y);
    }
}

bool isim_var(string s,middle* mi)     //�ж��Ƿ�Ϊ�м����
{
    //string im_var[5] = { "t1","t2","t3","t4","t5" };
    for (int i = 0; i < 50; i++)
    {
        if (strcmp(mi[i].x, s.c_str()) == 0)
            return true;
    }
    return false;
}

bool isdight(string s)              //�ж��Ƿ�Ϊ����
{
    if (s[0] <= '9' && s[0] >= '0')
        return true;
    return false;
}

bool isfloat(string s)              //�ж��Ƿ�Ϊ������
{
    for (unsigned int i = 0; i < s.size(); i++)
    {
        if (s[i] == '.')
            return true;
    }
    return false;
}

void qua_initialize(Quatemion* qua,int& num)
{
    for (int i = 0; i < num; i++)
    {
        qua[i].op = " ";
        qua[i].type = -1;
    }
    num = 0;
}

void inputqua(Quatemion *qua,string fname,int& num)        //�����ļ����ݶ�ȡ��Ԫʽ
{
    ifstream in;
    string next;
    in.open(fname);
    if (!in)
    {
        cout << "���ļ�ʧ�ܣ�" << endl;
        exit(1);
    }
    in >> next;
    int i;
    for (i = 0; !in.eof(); i++)
    {
        in >> qua[i].op;
        if (qua[i].op == ":=")
            qua[i].type = 0;
        else if (qua[i].op == "+" || qua[i].op == "-" || qua[i].op == "*" || qua[i].op == "/")
            qua[i].type = 1;
        in >> next >> qua[i].num1.name;
        if (qua[i].num1.name == "_")
            qua[i].num1.type = 0;
        else if (isim_var(qua[i].num1.name,mi))
            qua[i].num1.type = 2;
        else if (isdight(qua[i].num1.name))
        {
            if (!isfloat(qua[i].num1.name))
                qua[i].num1.type = 3;
            else
                qua[i].num1.type = 4;
        }
        else
            qua[i].num1.type = 1;
        in >> next >> qua[i].num2.name;
        if (qua[i].num2.name == "_")
            qua[i].num2.type = 0;
        else if (isim_var(qua[i].num2.name,mi))
            qua[i].num2.type = 2;
        else if (isdight(qua[i].num2.name))
        {
            if (!isfloat(qua[i].num2.name))
                qua[i].num2.type = 3;
            else
                qua[i].num2.type = 4;
        }
        else
            qua[i].num2.type = 1;
        in >> next >> qua[i].ans.name;
        if (qua[i].ans.name == "_")
            qua[i].ans.type = 0;
        else if (isim_var(qua[i].ans.name,mi))
            qua[i].ans.type = 2;
        else if (isdight(qua[i].ans.name))
        {
            if (!isfloat(qua[i].ans.name))
                qua[i].ans.type = 3;
            else
                qua[i].ans.type = 4;
        }
        else
            qua[i].ans.type = 1;
        in >> next >> next;
    }
    num = i;
    //qua_out[0] = qua[0];
    //qua_out[199] = qua[i - 1];
    //cout << i << endl;
    in.close();
}

void qua_out_pre(Quatemion* qua, Quatemion* qua_out, int num)   //�����ԪʽԤ����
{
    qua_out[0] = qua[0];
    qua_out[199] = qua[num - 1];
}

int judge_m_sign(token t)                       //�ж�t�Ƿ��������
{
    for (int i = 0; i < dag.num; i++)
    {
        if (dag.node[i].m_sign.name == t.name)
            return i;
    }
    return -1;
}

int judge_sign(token t)                 //�ж�t�Ƿ��ڸ��ӱ��
{
    list<token>::iterator p;
    for (int i = 0; i < dag.num; i++)
    {
        if (!dag.node[i].sign.empty())
        {
            for (p = dag.node[i].sign.begin(); p != dag.node[i].sign.end(); ++p)
            {
                if (t.name == (*p).name)
                    return i;
            }
        }
    }
    return -1;
}

void swapm_s(Node& n,token t)                   //�������ȼ��ж������Ƿ���Ҫ����
{
    if (n.m_sign.type == 2 && t.type == 1)
    {
        n.sign.push_back(n.m_sign);
        n.m_sign = t;
        n.sign.remove(t);
    }
}

void makequa(DAG dag,int& out_num,Quatemion* qua_out)              //���ݻ�����������Ԫʽ
{
    out_num = 1;
    for (int i = 0; i < dag.num; i++)
    {
        list<token>::iterator p;
        if (dag.node[i].left != -1 && dag.node[i].right != -1)                      //���ڷ�Ҷ�ӽڵ�
        {
            qua_out[out_num].op = dag.node[i].op;
            qua_out[out_num].type = 1;
            qua_out[out_num].num1 = dag.node[dag.node[i].left].m_sign;
            qua_out[out_num].num2 = dag.node[dag.node[i].right].m_sign;
            qua_out[out_num].ans = dag.node[i].m_sign;
            out_num++;
        }
        if (!dag.node[i].sign.empty())
        {
            for (p = dag.node[i].sign.begin(); p != dag.node[i].sign.end(); ++p)
            {
                if ((*p).type == 1)
                {
                    qua_out[out_num].ans = *p;
                    qua_out[out_num].op = ":=";
                    qua_out[out_num].num1 = dag.node[i].m_sign;
                    qua_out[out_num].num2.name = "_";
                    qua_out[out_num].num2.type = 0;
                    qua_out[out_num].type = 0;
                    out_num++;
                }
            }
        }
    }
}

void qua_output(Quatemion* qua_out,int out_num)
{
    ofstream out;
    out.open("qua_out.txt");
    if (!out)
    {
        cout << "���ļ�����" << endl;
        exit(1);
    }
    for(int i=0;i<out_num;i++)
    {
        out << "( " << qua_out[i].op << " , " << qua_out[i].num1.name << " , ";
        out << qua_out[i].num2.name << " , " << qua_out[i].ans.name << " )" << endl;
    }
    out << "( " << qua_out[199].op << " , " << qua_out[199].num1.name << " , ";
    out << qua_out[199].num2.name << " , " << qua_out[199].ans.name << " )";
    out.close();
}

void qua_output_s(Quatemion* qua_out,int out_num)
{
    for(int i=0;i<out_num;i++)
    {
        cout << "( " << qua_out[i].op << " , " << qua_out[i].num1.name << " , ";
        cout << qua_out[i].num2.name << " , " << qua_out[i].ans.name << " )" << endl;
    }
    cout << "( " << qua_out[199].op << " , " << qua_out[199].num1.name << " , ";
    cout << qua_out[199].num2.name << " , " << qua_out[199].ans.name << " )" << endl;
}

void optimizequa(Quatemion *qua,DAG& dag,int num)                //�Ż���Ԫʽ����������DAG
{
    int j = 0;
    clearDAG(dag);
    //dag.num = 0;
    while (j<num)
    {
        if(qua[j].type==0)                         //(=,B,_,A)
        {
            int s = judge_sign(qua[j].ans);
            //cout << "OK" << endl;
            int ss = judge_sign(qua[j].num1);
            //cout << "OK" << endl;
            int m_s = judge_m_sign(qua[j].num1);
            if (s != -1)                           //�ж�A�Ƿ��ڸ��ӱ���У�����ɾ��
            {
                dag.node[s].sign.remove(qua[j].ans);
            }
            if (m_s == -1 && ss == -1)
            {
                dag.node[dag.num].m_sign = qua[j].num1;
                dag.node[dag.num].sign.push_back(qua[j].ans);
                swapm_s(dag.node[dag.num], qua[j].ans);
                dag.num++;
            }
            else
            {
                if (m_s != -1)
                {
                    dag.node[m_s].sign.push_back(qua[j].ans);
                    swapm_s(dag.node[m_s], qua[j].ans);
                }
                else
                {
                    dag.node[ss].sign.push_back(qua[j].ans);
                    swapm_s(dag.node[ss], qua[j].ans);
                }
            }
            /*
            if (m_s != -1)
            {
                dag.node[m_s].sign.push_back(qua[j].ans);
                swapm_s(dag.node[m_s], qua[j].ans);
            }
            else if (ss != 1)
            {
                dag.node[ss].sign.push_back(qua[j].ans);
                swapm_s(dag.node[ss], qua[j].ans);
            }
            else
            {
                cout << "OK" << endl;
                dag.node[dag.num].m_sign = qua[j].num1;
                dag.node[dag.num].sign.push_back(qua[j].ans);
                swapm_s(dag.node[dag.num], qua[j].ans);
                dag.num++;
            }
            */
        }
        if(qua[j].type==1)                         //(op,B,C,A)
        {
            int m_s1 = judge_m_sign(qua[j].num1);       //���ж�B,C�ڵ��Ƿ���ڣ�����������
            int m_s2 = judge_m_sign(qua[j].num2);
            int s1 = judge_sign(qua[j].num1);
            int s2 = judge_sign(qua[j].num2);
            if (m_s1 == -1 && s1 == -1)
            {
                dag.node[dag.num].m_sign = qua[j].num1;
                m_s1 = dag.num;
                dag.num++;
            }
            if (m_s2 == -1 && s2 == -1)
            {
                dag.node[dag.num].m_sign = qua[j].num2;
                m_s2 = dag.num;
                dag.num++;
            }
            if (qua[j] .num1.type>=3 && qua[j].num2.type >= 3) //���������ǳ���
            {
                token p;
                p.type = 3;
                string s1 = qua[j].num1.name;
                string s2 = qua[j].num2.name;
                //int n1, n2;
                if (qua[j].op == "+")
                    p.name = to_string(stoi(s1) + stoi(s2));
                else if (qua[j].op == "-")
                    p.name = to_string(stoi(s1) - stoi(s2));
                else if (qua[j].op == "*")
                    p.name = to_string(stoi(s1) * stoi(s2));
                else
                    p.name = to_string(stoi(s1) / stoi(s2));
                int m = judge_m_sign(p);
                if (m == -1)                                        //p���������½�
                {
                    dag.node[dag.num].m_sign = p;
                    m = dag.num;
                }
                if (judge_sign(qua[j].ans) != -1)                   //�ж�A�Ƿ������Ϊ���ӱ�ǣ�����ɾȥ
                    dag.node[judge_sign(qua[j].ans)].sign.remove(qua[j].ans);
                dag.node[m].sign.push_back(qua[j].ans);             //��A�����p���ӱ��
                dag.num++;
            }
            else                                                    //����Ϊ����
            {
                if (judge_sign(qua[j].ans) != -1)                   //�ж�A�Ƿ������Ϊ���ӱ�ǣ�����ɾȥ
                    dag.node[judge_sign(qua[j].ans)].sign.remove(qua[j].ans);
                int flag = 1;
                for (int i = 0; i < dag.num; i++)
                {
                    int l = dag.node[i].left, r = dag.node[i].right;
                    if (dag.node[i].op == qua[j].op && l != -1 && r != -1)  //�Ƿ�����B op C�����Ľڵ�
                    {
                        if ((dag.node[l].m_sign == qua[j].num1 || judge_sign(qua[j].num1)==l)
                            && (dag.node[r].m_sign == qua[j].num2||judge_sign(qua[j].num2)==r))
                        {
                            dag.node[i].sign.push_back(qua[j].ans);         //������A������ڸ��ӱ��
                            swapm_s(dag.node[i], qua[j].ans);
                            flag = 0;
                        }
                    }
                }
                if(flag)
                {
                    dag.node[dag.num].m_sign = qua[j].ans;                  //���������½�
                    dag.node[dag.num].op = qua[j].op;
                    if (judge_sign(qua[j].num1) != -1)
                        dag.node[dag.num].left = judge_sign(qua[j].num1);
                    else
                        dag.node[dag.num].left = judge_m_sign(qua[j].num1);
                    if (judge_sign(qua[j].num2) != -1)
                        dag.node[dag.num].right = judge_sign(qua[j].num2);
                    else
                        dag.node[dag.num].right = judge_m_sign(qua[j].num2);
                    dag.num++;
                }
            }
        }
        //cout << dag.num << endl;
        //if (j == 1) break;
        j++;
    }
    /*if (dag.num > 2)
    {
        cout << dag.node[2].m_sign.name << endl;
        if (!dag.node[2].sign.empty())
        {
            list<token>::iterator p;
            for (p = dag.node[2].sign.begin(); p != dag.node[2].sign.end(); ++p)
            {
                cout << (*p).name << " " << (*p).type << endl;
            }
        }
    }*/
    //cout << dag.num << endl;
}

void make_Sytab(Quatemion *qua_out,int out_num,middle* dd,middle* mi)
{
    FILE *fp;
    for(int i=1;i<out_num;i++)
    {
        if(qua_out[i].type==0||qua_out[i].type==1)
        {
            if(qua_out[i].num1.type==2)
                indd(qua_out[i].num1.name,dd,mi);
            if(qua_out[i].num2.type==2)
                indd(qua_out[i].num2.name,dd,mi);
            if(qua_out[i].ans.type==2)
                indd(qua_out[i].ans.name,dd,mi);
        }
    }
    if(!(fp=fopen("SyTab.txt","ab")))
    {
        cout << "���ļ�ʧ��" << endl;
        exit(1);
    }
    for(int i=0;strcmp(dd[i].x," ")!=0;i++)
    {
        fprintf(fp,"%s %s\n",dd[i].x,dd[i].y);
    }
    fclose(fp);
}

void optimizequa_main()
{
    string fname = "output.txt";
	//string fname = "optimizequa_test_in.txt";
	inputqua(qua, fname, qua_num);
	//cout << qua[2].ans.type << endl;
	qua_out_pre(qua, qua_out, qua_num);
	qua_out_pre(qua, qua_out2,qua_num);
	cout << endl << "�м�����Ż���" << endl;
	optimizequa(qua,dag,qua_num);
	makequa(dag,out_num,qua_out);
	while(1)
    {
        optimizequa(qua_out,dag,out_num);
        makequa(dag,out_num2,qua_out2);
        //qua_output_s(qua_out2,out_num2);
        if(!isquaeq(qua_out,qua_out2,out_num,out_num2))
            qua_ass(qua_out,qua_out2,out_num,out_num2);
        else
            break;
    }
    qua_output_s(qua_out,out_num);
    qua_output(qua_out,out_num);
	make_Sytab(qua_out,out_num,dd,mi);
}
