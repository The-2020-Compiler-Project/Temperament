#include "optimizequa.h"

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

void inputqua(Quatemion *qua)        //根据文件内容读取四元式
{
    ifstream in;
    string next;
    in.open("optimizequa_test_in.txt");
    if (!in)
    {
        cout << "打开文件失败！" << endl;
        exit(1);
    }
    in >> next;
    for (int i = 0; !in.eof(); i++)
    {
        in >> qua[i].op;
        if (qua[i].op == ":=")
            qua[i].type = 0;
        else if (qua[i].op == "+" || qua[i].op == "-" || qua[i].op == "*" || qua[i].op == "/")
            qua[i].type = 1;
        in >> next >> qua[i].num1.name;
        if (isim_var(qua[i].num1.name))
            qua[i].num1.type = 2;
        else if (isdight(qua[i].num1.name))
            qua[i].num1.type = 3;
        else
            qua[i].num1.type = 1;
        in >> next >> qua[i].num2.name;
        if (isim_var(qua[i].num2.name))
            qua[i].num2.type = 2;
        else if (isdight(qua[i].num2.name))
            qua[i].num2.type = 3;
        else
            qua[i].num2.type = 1;
        in >> next >> qua[i].ans.name;
        if (isim_var(qua[i].ans.name))
            qua[i].ans.type = 2;
        else if (isdight(qua[i].num2.name))
            qua[i].ans.type = 3;
        else
            qua[i].ans.type = 1;
        in >> next >> next;
    }
    in.close();
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
        if (dag.node[i].sign.empty())
            continue;
        for (p = dag.node[i].sign.begin(); p != dag.node[i].sign.end(); ++p)
        {
            if (t.name == (*p).name)
                return i;
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

void makequa(DAG dag)
{
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
                }
            }
        }
        if (dag.node[i].left != -1 && dag.node[i].right != -1)                      //对于非叶子节点
        {
            cout << "( " << dag.node[i].op << " , " << dag.node[dag.node[i].left].m_sign.name;
            cout << " , " << dag.node[dag.node[i].right].m_sign.name << " , " << dag.node[i].m_sign.name;
            cout << " )" << endl;
        }
    }
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
                int ss = judge_sign(qua[j].num1);
                int m_s = judge_m_sign(qua[j].num1);
                if (s != -1)                           //判断A是否在附加标记中，是则删除         
                {
                    dag.node[s].sign.remove(qua[j].ans);
                }
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
                    dag.node[dag.num].m_sign = qua[j].num1;
                    dag.node[dag.num].sign.push_back(qua[j].ans);
                    swapm_s(dag.node[dag.num], qua[j].ans);
                    dag.num++;
                }
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
                if (dag.node[m_s1].m_sign.type == 3 && dag.node[m_s2].m_sign.type == 3) //操作数都是常数
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
                    if (m == -1)                                            //p不存在则新建
                    {
                        dag.node[dag.num].m_sign = p;
                        m = dag.num;
                    }
                    if (judge_sign(qua[j].ans) != -1)                   //判断A是否存在且为附加标记，是则删去
                        dag.node[judge_sign(qua[j].ans)].sign.remove(qua[j].ans);
                    dag.node[m].sign.push_back(qua[j].ans);             //将A添加至p附加标记
                    dag.num++;
                }
                else
                {
                    if (judge_sign(qua[j].ans) != -1)                   //判断A是否存在且为附加标记，是则删去
                        dag.node[judge_sign(qua[j].ans)].sign.remove(qua[j].ans);
                    int flag = 1;
                    for (int i = 0; i < dag.num; i++)
                    {
                        int l = dag.node[i].left, r = dag.node[i].right;
                        if (dag.node[i].op == qua[j].op && l != -1 && r != -1)
                        {
                            if ((dag.node[l].m_sign == qua[j].num1 || judge_sign(qua[j].num1)==l)
                                && (dag.node[r].m_sign == qua[j].num2)||judge_sign(qua[j].num2)==r)
                            {
                                dag.node[i].sign.push_back(qua[j].ans);
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
        //cout << dag.num << endl;
        makequa(dag);
    }
}

int main()
{
    inputqua(qua);
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