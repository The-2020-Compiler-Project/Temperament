#include "objectcode.h"

using namespace std;

int judge_actsynbl(vector<vector<string> > Act_synbl, string s)
{
	for (unsigned int i = 0; i < Act_synbl.size(); i++)
	{
		if (!Act_synbl[i].empty())
		{
			if (Act_synbl[i][0] == s)
				return i;
		}
	}
	return -1;
}

void get_act_inf(vector<vector<string> >& Act_synbl, ACT_INF* actinf, Quatemion* qua_out, int num)
{
	int judge;
	for (int i = num - 1; i > 0; i--)
	{
		judge = judge_actsynbl(Act_synbl, qua_out[i].ans.name);
		if (qua_out[i].ans.type == 1)		//为非临时变量
		{
			if (judge == -1)
			{
				Act_synbl.push_back(vector<string>(1, qua_out[i].ans.name));		//不存在该变量则加入
				actinf[i].ans = 1;													//初始化为 y
				Act_synbl[Act_synbl.size()-1].push_back("n");						//变为 n
			}
			else
			{
				actinf[i].ans = Act_synbl[judge][1] == "y" ? 1 : 0;					//存在则填入对于活跃信息
				Act_synbl[judge][1] = "n";											//变为 n
			}
		}
		if (qua_out[i].ans.type == 2)		//为临时变量
		{
			if (judge == -1)
			{
				Act_synbl.push_back(vector<string>(1, qua_out[i].ans.name));		//不存在该变量则加入
				actinf[i].ans = 0;													//初始化为 n
				Act_synbl[Act_synbl.size() - 1].push_back("n");						//变为 n
			}
			else
			{
				actinf[i].ans = Act_synbl[judge][1] == "y" ? 1 : 0;					//存在则填入对于活跃信息
				Act_synbl[judge][1] = "n";											//变为 n
			}
		}
		judge = judge_actsynbl(Act_synbl, qua_out[i].num1.name);
		if (qua_out[i].num1.type == 1)
		{
			if (judge == -1)
			{
				Act_synbl.push_back(vector<string>(1, qua_out[i].num1.name));
				actinf[i].num1 = 1;
				Act_synbl[Act_synbl.size() - 1].push_back("y");
			}
			else
			{
				actinf[i].num1 = Act_synbl[judge][1] == "y" ? 1 : 0;
				Act_synbl[judge][1] = "y";
			}
		}
		if (qua_out[i].num1.type == 2)
		{
			if (judge == -1)
			{
				Act_synbl.push_back(vector<string>(1, qua_out[i].num1.name));
				actinf[i].num1 = 0;
				Act_synbl[Act_synbl.size() - 1].push_back("y");
			}
			else
			{
				actinf[i].num1 = Act_synbl[judge][1] == "y" ? 1 : 0;
				Act_synbl[judge][1] = "y";
			}
		}
		judge = judge_actsynbl(Act_synbl, qua_out[i].num2.name);
		if (qua_out[i].num2.type == 1)
		{
			if (judge == -1)
			{
				Act_synbl.push_back(vector<string>(1, qua_out[i].num2.name));
				actinf[i].num2 = 1;
				Act_synbl[Act_synbl.size() - 1].push_back("y");
			}
			else
			{
				actinf[i].num2 = Act_synbl[judge][1] == "y" ? 1 : 0;
				Act_synbl[judge][1] = "y";
			}
		}
		if (qua_out[i].num2.type == 2)
		{
			if (judge == -1)
			{
				Act_synbl.push_back(vector<string>(1, qua_out[i].num2.name));
				actinf[i].num2 = 0;
				Act_synbl[Act_synbl.size() - 1].push_back("y");
			}
			else
			{
				actinf[i].num2 = Act_synbl[judge][1] == "y" ? 1 : 0;
				Act_synbl[judge][1] = "y";
			}
		}
	}
}

void make_objectcode(Quatemion* qua_out, ACT_INF* actinf, int num,string* RDV)
{
	RDV[0] = "0";
	ofstream out;
	out.open("objectcode.txt");
	if (!out)
	{
		cout << "打开文件失败" << endl;
		exit(1);
	}
	for (int i = 1; i < num; i++)
	{
		if (qua_out[i].type == 1)
		{
			if (RDV[0] == "0")
			{
				cout << "LD R," << qua_out[i].num1.name << endl;				//LD R,B
				out << "LD R," << qua_out[i].num1.name << endl;
				cout << qua_out[i].op << " R," << qua_out[i].num2.name << endl; //op R,C
				out << qua_out[i].op << " R," << qua_out[i].num2.name << endl;
			}
			else if (RDV[0] == qua_out[i].num1.name)
			{
				if (RDV[1] == "1")
				{
					cout << "ST R," << qua_out[i].num1.name << endl;
					out << "ST R," << qua_out[i].num1.name << endl;
				}
				cout << qua_out[i].op << " R," << qua_out[i].num2.name << endl;
				out << qua_out[i].op << " R," << qua_out[i].num2.name << endl;
			}
			else if (RDV[0] == qua_out[i].num2.name && (qua_out[i].op == "+" || qua_out[i].op == "*"))
			{
				if (RDV[1] == "1")
				{
					cout << "ST R," << qua_out[i].num2.name << endl;
					out << "ST R," << qua_out[i].num2.name << endl;
				}
				cout << qua_out[i].op << " R," << qua_out[i].num1.name << endl;
				out << qua_out[i].op << " R," << qua_out[i].num1.name << endl;
			}
			else
			{
				if (RDV[1] == "1")
				{
					cout << "ST R," << RDV[0] << endl;
					out << "ST R," << RDV[0] << endl;
				}
				cout << "LD R," << qua_out[i].num1.name << endl;				//LD R,B
				out << "LD R," << qua_out[i].num1.name << endl;
				cout << qua_out[i].op << " R," << qua_out[i].num2.name << endl; //op R,C
				out << qua_out[i].op << " R," << qua_out[i].num2.name << endl;
			}
			RDV[0] = qua_out[i].ans.name;
			RDV[1] = to_string(actinf[i].ans);
		}
		if (qua_out[i].type == 0)
		{
			if (RDV[0] == "0")
			{
				cout << "LD R," << qua_out[i].num1.name << endl;
				out << "LD R," << qua_out[i].num1.name << endl;
				cout << "ST ,R" << qua_out[i].ans.name << endl;
				out << "ST ,R" << qua_out[i].ans.name << endl;
				RDV[0] = qua_out[i].num1.name;
				RDV[1] = to_string(actinf[i].num1);
			}
			else if (RDV[0] == qua_out[i].num1.name)
			{
				cout << "ST ,R" << qua_out[i].ans.name << endl;
				out << "ST ,R" << qua_out[i].ans.name << endl;
			}
			else
			{
				if (RDV[1] == "1")
				{
					cout << "ST R," << RDV[0] << endl;
					out << "ST R," << RDV[0] << endl;
				}
				cout << "LD R," << qua_out[i].num1.name << endl;
				out << "LD R," << qua_out[i].num1.name << endl;
				cout << "ST ,R" << qua_out[i].ans.name << endl;
				out << "ST ,R" << qua_out[i].ans.name << endl;
				RDV[0] = qua_out[i].num1.name;
				RDV[1] = to_string(actinf[i].num1);
			}
		}
	}
	out.close();
}

int main()
{
	string fname = "optimizequa_test_in.txt";
	inputqua(qua, fname, qua_num);
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
	get_act_inf(Act_synbl, actinf, qua_out, out_num);
	cout << "活跃信息：" << endl;
	for (int i = 1; i < out_num; i++)
	{
		cout << qua_out[i].num1.name << "," << actinf[i].num1 << "  ";
		cout << qua_out[i].num2.name << "," << actinf[i].num2 << "  ";
		cout << qua_out[i].ans.name << "," << actinf[i].ans << endl;
	}
	make_objectcode(qua_out, actinf, out_num, RDV);
	return 0;
}