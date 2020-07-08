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
	int type;        //2为临时变量，3为整型，1为非临时变量，4为浮点型，空为0
	bool operator == (const token& t) const             //list remove函数判断是否相等
	{
		return (name == t.name && type == t.type);
	}
};

struct Quatemion        //四元式结构体
{
	int block = 1;          //基本块标识
	int type = -1;           //类型 0 1 2 3
	string op = " ";      //操作
	token num1;         //操作数1
	token num2;         //操作数2
	token ans;          //结果
};

extern struct Quatemion qua[200];  //四元式数组
extern int qua_num;                //四元式个数

extern struct Quatemion qua_out[200];  //优化后四元式数组
extern int out_num;                //优化后的四元式个数

struct Node             //DAG节点
{
	int left = -1;           //左子节点标识
	int right = -1;          //右子节点标识
	string op = " ";          //操作
	token m_sign;       //主标记
	list<token> sign;     //附加标记
};

struct DAG
{
	int num = 0;
	Node node[200];
};

extern struct DAG dag;

struct ACT_INF
{
	int num1 = -1;		//活跃信息，1为y，0为n
	int num2 = -1;
	int ans = -1;
};

struct ACT_INF actinf[200];

string RDV[2];				//寄存器状态变量，"0"表示空闲，"X"表示被X占用

vector<vector<string> > Act_synbl;	//活跃信息表

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
	string OPT;
	ofstream out;
	out.open("objectcode.txt");
	if (!out)
	{
		cout << "打开文件失败" << endl;
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

extern void inputqua(Quatemion* qua, string fname, int& num);

extern void qua_out_pre(Quatemion* qua, Quatemion* qua_out, int num);

extern void optimizequa(int block_num, DAG& dag);

class yuyi
{
public:
    void doit();
    int saomiao();
    int fanyi();
    int fuzhi();
    int jieshu();
    char *program();
    char *chengchu();
    char *xuan();
    char *jiajian();
    char *newt();
    void zhongj();
    void four(const char* op,const char* ag1,const char* ag2,const char* res);
private:
    char prog[10000],ht[10000],ch,tt;
    int cot,p,m,n,sum,ii,N,t,ppt;
    int kk=0,lg=0,k=0,i=0,dian=0,mid=0,midd=0;
    const char* key[7] = {"begin","program","var","real","integer","char","end"};
};
extern yuyi y;
/*
int main()
{
	//yuyi y;
	y.doit();
	cout << endl;
    for(int i=0;dd[i].x[0]!=' ';i++)
        printf("%s %s\n",dd[i].x,dd[i].y);
    for(int i=0;mi[i].x[0]!=' ';i++)
        printf("%s %s\n",mi[i].x,mi[i].y);
	string fname = "output.txt";
	//string fname = "optimizequa_test_in.txt";
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
	cout << "目标代码：" << endl;
	make_objectcode(qua_out, actinf, out_num, RDV);
	return 0;
}

*/
