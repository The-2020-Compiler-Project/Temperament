#include "optimizequa.h"

struct Quatemion qua[200];  //四元式数组
int qua_num = 0;            //四元式个数

struct Quatemion qua_out[200];  //四元式数组
int out_num;

struct DAG dag;

bool isim_var(string s)     //判断是否为中间变量
{
    string im_var[5] = { "t1","t2","t3","t4","t5" };
    for (int i = 0; i < 5; i++)
    {
        if (s == im_var[i])
            return true;
    }
    return false;
}

bool isdight(string s)
{
    if (s[0] <= '9' && s[0] >= '0')
        return true;
    return false;
}

bool isfloat(string s)
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

void inputqua(Quatemion *qua,string fname,int& num)        //根据文件内容读取四元式
{
    ifstream in;
    string next;
    in.open(fname);
    if (!in)
    {
        cout << "打开文件失败！" << endl;
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
        else if (isim_var(qua[i].num1.name))
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
        else if (isim_var(qua[i].num2.name))
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
        else if (isim_var(qua[i].ans.name))
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

void qua_out_pre(Quatemion* qua, Quatemion* qua_out, int num)
{
    qua_out[0] = qua[0];
    qua_out[199] = qua[num - 1];
}

int judge_m_sign(token t)
{
    for (int i = 0; i < dag.num; i++)
    {
        if (dag.node[i].m_sign.name == t.name)
            return i;
    }
    return -1;
}

int judge_sign(token t)
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

void swapm_s(Node& n,token t)
{
    if (n.m_sign.type == 2 && t.type == 1)
    {
        n.sign.push_back(n.m_sign);
        n.m_sign = t;
        n.sign.remove(t);
    }
}

void makequa(DAG dag,int& out_num)
{
    ofstream out;
    out_num = 1;
    out.open("qua_out.txt");
    if (!out)
    {
        cout << "打开文件出错！" << endl;
        exit(1);
    }
    out << "( " << qua_out[0].op << " , " << qua_out[0].num1.name << " , ";
    out << qua_out[0].num2.name << " , " << qua_out[0].ans.name << " )" << endl;
    for (int i = 0; i < dag.num; i++)
    {
        list<token>::iterator p;
        if (!dag.node[i].sign.empty())
        {
            for (p = dag.node[i].sign.begin(); p != dag.node[i].sign.end(); ++p)
            {
                if ((*p).type == 1)
                {
                    cout << "( " << ":= , " << dag.node[i].m_sign.name << " , ";
                    cout << "_ , " << (*p).name << " )" << endl;
                    out << "( " << ":= , " << dag.node[i].m_sign.name << " , ";
                    out << "_ , " << (*p).name << " )" << endl;
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
        if (dag.node[i].left != -1 && dag.node[i].right != -1)                      //对于非叶子节点
        {
            cout << "( " << dag.node[i].op << " , " << dag.node[dag.node[i].left].m_sign.name;
            cout << " , " << dag.node[dag.node[i].right].m_sign.name << " , " << dag.node[i].m_sign.name;
            cout << " )" << endl;
            out << "( " << dag.node[i].op << " , " << dag.node[dag.node[i].left].m_sign.name;
            out << " , " << dag.node[dag.node[i].right].m_sign.name << " , " << dag.node[i].m_sign.name;
            out << " )" << endl;
            qua_out[out_num].op = dag.node[i].op;
            qua_out[out_num].type = 1;
            qua_out[out_num].num1 = dag.node[dag.node[i].left].m_sign;
            qua_out[out_num].num2 = dag.node[dag.node[i].right].m_sign;
            qua_out[out_num].ans = dag.node[i].m_sign;
            out_num++;
        }
    }
    out << "( " << qua_out[199].op << " , " << qua_out[199].num1.name << " , ";
    out << qua_out[199].num2.name << " , " << qua_out[199].ans.name << " )";
    out.close();
}

void optimizequa(int block_num,DAG& dag)
{
    for (int i = 1; i <= block_num; i++)
    {
        int j = 0;
        dag.num = 0;
        while (qua[j].op!=" "&&qua[j].block==i)
        {
            if(qua[j].type==0)                         //(=,B,_,A)
            {
                int s = judge_sign(qua[j].ans);
                //cout << "OK" << endl;
                int ss = judge_sign(qua[j].num1);
                //cout << "OK" << endl;
                int m_s = judge_m_sign(qua[j].num1);
                if (s != -1)                           //判断A是否在附加标记中，是则删除         
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
                int m_s1 = judge_m_sign(qua[j].num1);       //先判断B,C节点是否存在，不存在则建立
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
                if (qua[j] .num1.type>=3 && qua[j].num2.type >= 3) //操作数都是常数
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
                    if (m == -1)                                        //p不存在则新建
                    {
                        dag.node[dag.num].m_sign = p;
                        m = dag.num;
                    }
                    if (judge_sign(qua[j].ans) != -1)                   //判断A是否存在且为附加标记，是则删去
                        dag.node[judge_sign(qua[j].ans)].sign.remove(qua[j].ans);
                    dag.node[m].sign.push_back(qua[j].ans);             //将A添加至p附加标记
                    dag.num++;
                }
                else                                                    //不都为常数
                {
                    if (judge_sign(qua[j].ans) != -1)                   //判断A是否存在且为附加标记，是则删去
                        dag.node[judge_sign(qua[j].ans)].sign.remove(qua[j].ans);
                    int flag = 1;
                    for (int i = 0; i < dag.num; i++)
                    {
                        int l = dag.node[i].left, r = dag.node[i].right;
                        if (dag.node[i].op == qua[j].op && l != -1 && r != -1)  //是否已有B op C这样的节点
                        {
                            if ((dag.node[l].m_sign == qua[j].num1 || judge_sign(qua[j].num1)==l)
                                && (dag.node[r].m_sign == qua[j].num2)||judge_sign(qua[j].num2)==r)
                            {
                                dag.node[i].sign.push_back(qua[j].ans);         //存在则将A添加至节附加标记
                                swapm_s(dag.node[i], qua[j].ans);
                                flag = 0;
                            }
                        }
                    }
                    if(flag)
                    {
                        dag.node[dag.num].m_sign = qua[j].ans;
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
        makequa(dag,out_num);
    }
}
/*
int main()
{
    string fname = "optimizequa_test_in.txt";
    inputqua(qua,fname,qua_num);
    //cout << qua[2].ans.type << endl;
    qua_out_pre(qua, qua_out, qua_num);
    for (int i = 0; qua[i].op != " "; i++)
    {
        cout << "( " << qua[i].op << " , " << qua[i].num1.name;
        cout << " , " << qua[i].num2.name << " , " << qua[i].ans.name;
        cout << " )" << endl;
    }
    cout << "优化后：" << endl;
    optimizequa(1, dag);
    return 0;
}
*/