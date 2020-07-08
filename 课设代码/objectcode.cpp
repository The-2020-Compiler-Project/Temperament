#include <iostream>
#include <vector>
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
	int block = 1;          //�������ʶ
	int type = -1;           //���� 0 1 2 3
	string op = " ";      //����
	token num1;         //������1
	token num2;         //������2
	token ans;          //���
};

extern struct Quatemion qua_out[200];  //�Ż�����Ԫʽ����
extern int out_num;                //�Ż������Ԫʽ����

struct ACT_INF
{
	int num1 = -1;		//��Ծ��Ϣ��1Ϊy��0Ϊn
	int num2 = -1;
	int ans = -1;
};

struct ACT_INF actinf[200];

string RDV[2];				//�Ĵ���״̬������"0"��ʾ���У�"X"��ʾ��Xռ��

vector<vector<string> > Act_synbl;	//��Ծ��Ϣ��

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
		if (qua_out[i].ans.type == 1)		//Ϊ����ʱ����
		{
			if (judge == -1)
			{
				Act_synbl.push_back(vector<string>(1, qua_out[i].ans.name));		//�����ڸñ��������
				actinf[i].ans = 1;													//��ʼ��Ϊ y
				Act_synbl[Act_synbl.size()-1].push_back("n");						//��Ϊ n
			}
			else
			{
				actinf[i].ans = Act_synbl[judge][1] == "y" ? 1 : 0;					//������������ڻ�Ծ��Ϣ
				Act_synbl[judge][1] = "n";											//��Ϊ n
			}
		}
		if (qua_out[i].ans.type == 2)		//Ϊ��ʱ����
		{
			if (judge == -1)
			{
				Act_synbl.push_back(vector<string>(1, qua_out[i].ans.name));		//�����ڸñ��������
				actinf[i].ans = 0;													//��ʼ��Ϊ n
				Act_synbl[Act_synbl.size() - 1].push_back("n");						//��Ϊ n
			}
			else
			{
				actinf[i].ans = Act_synbl[judge][1] == "y" ? 1 : 0;					//������������ڻ�Ծ��Ϣ
				Act_synbl[judge][1] = "n";											//��Ϊ n
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
	string OPT;
	ofstream out;
	out.open("objectcode.txt");
	if (!out)
	{
		cout << "���ļ�ʧ��" << endl;
		exit(1);
	}
	int i;
	for (i = 1; i < num; i++)
	{
		if (qua_out[i].type == 1)
		{
			if (qua_out[i].op == "+")
				OPT = "ADD";
			else if (qua_out[i].op == "-")
				OPT = "SUB";
			else if (qua_out[i].op == "*")
				OPT = "MUL";
			else
				OPT = "DIV";
			if (RDV[0] == "0")
			{
				cout << "LD R," << qua_out[i].num1.name << endl;				//LD R,B
				out << "LD R," << qua_out[i].num1.name << endl;
				cout << OPT << " R," << qua_out[i].num2.name << endl; //op R,C
				out << OPT << " R," << qua_out[i].num2.name << endl;
			}
			else if (RDV[0] == qua_out[i].num1.name)
			{
				if (RDV[1] == "1")
				{
					cout << "ST R," << qua_out[i].num1.name << endl;
					out << "ST R," << qua_out[i].num1.name << endl;
				}
				cout << OPT << " R," << qua_out[i].num2.name << endl;
				out << OPT << " R," << qua_out[i].num2.name << endl;
			}
			else if (RDV[0] == qua_out[i].num2.name && (qua_out[i].op == "+" || qua_out[i].op == "*"))
			{
				if (RDV[1] == "1")
				{
					cout << "ST R," << qua_out[i].num2.name << endl;
					out << "ST R," << qua_out[i].num2.name << endl;
				}
				cout << OPT << " R," << qua_out[i].num1.name << endl;
				out << OPT << " R," << qua_out[i].num1.name << endl;
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
				cout << OPT << " R," << qua_out[i].num2.name << endl; //op R,C
				out << OPT << " R," << qua_out[i].num2.name << endl;
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
				cout << "ST R," << qua_out[i].ans.name << endl;
				out << "ST R," << qua_out[i].ans.name << endl;
				RDV[0] = qua_out[i].num1.name;
				RDV[1] = to_string(actinf[i].num1);
			}
			else if (RDV[0] == qua_out[i].num1.name)
			{
				cout << "ST R," << qua_out[i].ans.name << endl;
				out << "ST R," << qua_out[i].ans.name << endl;
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
				cout << "ST R," << qua_out[i].ans.name << endl;
				out << "ST R," << qua_out[i].ans.name << endl;
				RDV[0] = qua_out[i].num1.name;
				RDV[1] = to_string(actinf[i].num1);
			}
		}
	}
	if(qua_out[i-1].type==1)
    {
        cout << "ST R," << RDV[0] << endl;
        out << "ST R," << RDV[0] << endl;
    }
	out.close();
}

void objectcode_main()
{
	get_act_inf(Act_synbl, actinf, qua_out, out_num);
	cout << endl << "��Ծ��Ϣ��" << endl;
	for (int i = 1; i < out_num; i++)
	{
		cout << qua_out[i].num1.name << "," << actinf[i].num1 << "  ";
		cout << qua_out[i].num2.name << "," << actinf[i].num2 << "  ";
		cout << qua_out[i].ans.name << "," << actinf[i].ans << endl;
	}
	cout << endl << "Ŀ����룺" << endl;
	make_objectcode(qua_out, actinf, out_num, RDV);
}

