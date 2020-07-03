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
    int type;        //2Ϊ��ʱ������3Ϊ���֣�1Ϊ����ʱ����
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

struct Quatemion qua[200];  //��Ԫʽ����

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

struct DAG dag;

bool isim_var(string s);     //�ж��Ƿ�Ϊ�м����

bool isdight(string s);          //�ж��Ƿ�Ϊ����

void inputqua(Quatemion* qua);             //�����ļ����ݶ�ȡ��Ԫʽ

void makequa(DAG dag);          //���ݻ�����������Ԫʽ

void optimizequa(int block_num,DAG& dag);  //�Ż���Ԫʽ����������DAG
