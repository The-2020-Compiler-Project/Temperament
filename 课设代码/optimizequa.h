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
    int type;        //2Ϊ��ʱ������3Ϊ���ͣ�1Ϊ����ʱ������4Ϊ�����ͣ���Ϊ0
    bool operator == (const token& t) const             //list remove�����ж��Ƿ����
    {
        return (name == t.name && type == t.type);
    }
};

struct Quatemion        //��Ԫʽ�ṹ��
{
    int block=1;          //�������ʶ
    int type=-1;           //���� 0 1 2 3
    string op=" ";      //����
    token num1;         //������1
    token num2;         //������2
    token ans;          //���
};

extern struct Quatemion qua[200];  //��Ԫʽ����
extern int qua_num;            //��Ԫʽ����

extern struct Quatemion qua_out[200];  //��Ԫʽ����
extern int out_num;

struct Node             //DAG�ڵ�
{
    int left=-1;           //���ӽڵ��ʶ
    int right=-1;          //���ӽڵ��ʶ
    string op=" ";          //����
    token m_sign;       //�����
    list<token> sign;     //���ӱ��
};

struct DAG
{
    int num = 0;
    Node node[200];
};

extern struct DAG dag;

bool isim_var(string s);     //�ж��Ƿ�Ϊ�м����

bool isdight(string s);          //�ж��Ƿ�Ϊ����

bool isfloat(string s);          //�ж��Ƿ�Ϊ������

void inputqua(Quatemion* qua,string fname,int& num);       //�����ļ����ݶ�ȡ��Ԫʽ

void qua_out_pre(Quatemion* qua, Quatemion* qua_out, int num);      //�����ԪʽԤ����

void makequa(DAG dag,int& out_num);          //���ݻ�����������Ԫʽ

void optimizequa(int block_num,DAG& dag);  //�Ż���Ԫʽ����������DAG
