#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "optimizequa.h"

using namespace std;

struct ACT_INF
{
	int num1 = -1;		//��Ծ��Ϣ��1Ϊy��0Ϊn
	int num2 = -1;
	int ans = -1;
};

struct ACT_INF actinf[200];

string RDV[2];				//�Ĵ���״̬������"0"��ʾ���У�"X"��ʾ��Xռ��

vector<vector<string> > Act_synbl;	//��Ծ��Ϣ��

int judge_actsynbl(vector<vector<string> > Act_synbl, string s);		//�жϱ����Ƿ��ڻ�Ծ��Ϣ����

void get_act_inf(vector<vector<string> >& Act_synbl, ACT_INF* actinf, Quatemion* qua_out, int num);	//����������Ծ��Ϣ

void make_objectcode(Quatemion* qua_out, ACT_INF* actinf, int num,string* RDV);		//������Ԫʽ�ͻ�Ծ��Ϣ����Ŀ�����