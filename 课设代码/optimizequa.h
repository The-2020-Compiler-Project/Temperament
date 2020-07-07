#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <sstream>
using namespace std;

struct token
{
    string name;
    int type;        //2为临时变量，3为整型，1为非临时变量，4为浮点型，空为0
    bool operator == (const token& t) const             //list remove函数判断是否相等
    {
        return (name == t.name && type == t.type);
    }
};

struct Quatemion        //四元式结构体
{
    int block=1;          //基本块标识
    int type=-1;           //类型 0 1 2 3
    string op=" ";      //操作
    token num1;         //操作数1
    token num2;         //操作数2
    token ans;          //结果
};

extern struct Quatemion qua[200];  //四元式数组
extern int qua_num;            //四元式个数

extern struct Quatemion qua_out[200];  //四元式数组
extern int out_num;

struct Node             //DAG节点
{
    int left=-1;           //左子节点标识
    int right=-1;          //右子节点标识
    string op=" ";          //操作
    token m_sign;       //主标记
    list<token> sign;     //附加标记
};

struct DAG
{
    int num = 0;
    Node node[200];
};

extern struct DAG dag;

bool isim_var(string s);     //判断是否为中间变量

bool isdight(string s);          //判断是否为数字

bool isfloat(string s);          //判断是否为浮点型

void inputqua(Quatemion* qua,string fname,int& num);       //根据文件内容读取四元式

void qua_out_pre(Quatemion* qua, Quatemion* qua_out, int num);      //输出四元式预处理

void makequa(DAG dag,int& out_num);          //根据基本块生成四元式

void optimizequa(int block_num,DAG& dag);  //优化四元式函数，构建DAG
