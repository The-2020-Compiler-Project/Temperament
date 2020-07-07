#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "optimizequa.h"

using namespace std;

struct ACT_INF
{
	int num1 = -1;		//活跃信息，1为y，0为n
	int num2 = -1;
	int ans = -1;
};

struct ACT_INF actinf[200];

string RDV[2];				//寄存器状态变量，"0"表示空闲，"X"表示被X占用

vector<vector<string> > Act_synbl;	//活跃信息表

int judge_actsynbl(vector<vector<string> > Act_synbl, string s);		//判断变量是否在活跃信息表中

void get_act_inf(vector<vector<string> >& Act_synbl, ACT_INF* actinf, Quatemion* qua_out, int num);	//构建变量活跃信息

void make_objectcode(Quatemion* qua_out, ACT_INF* actinf, int num,string* RDV);		//根据四元式和活跃信息生成目标代码