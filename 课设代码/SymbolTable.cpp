#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct PFINFL
{
    int level;
    int off;
    int fn;
    struct SYNBL *param;
    int entry=0;
};

vector<PFINFL> pfin;

//struct PFINFL pfin[10];

struct ADDR_VAR
{
    int lv;
    int off;
};

struct SYNBL
{
    string name;
    string typ;
    string cat;
    struct ADDR *addr;
};

vector<SYNBL> sy;

/*struct SYNBL sy[200];
int sy_num;*/

struct ADDR
{
    struct ADDR_VAR *addr_var;
    struct PFINFL *pf;
};

void sy_in(vector<SYNBL>& sy,vector<PFINFL>& pfin)
{
    //sy_num=0;
    ifstream in;
    string s1,s2;
    in.open("SyTab.txt");
    if(!in)
    {
        cout << "打开文件失败！" << endl;
        exit(1);
    }
    /*
    SYNBL *s=(SYNBL*)malloc(sizeof(SYNBL));
    ADDR *a=(ADDR*)malloc(sizeof(ADDR));
    ADDR_VAR *a_v=(ADDR_VAR*)malloc(sizeof(ADDR_VAR));
    PFINFL *p=(PFINFL*)malloc(sizeof(PFINFL));*/
    SYNBL s;
    ADDR a;
    ADDR_VAR a_v;
    PFINFL p;
    in >> s1 >> s2;
    s.name=s1;
    s.cat=s2;
    s.typ=" ";
    p.fn=0;
    p.level=0;
    p.off=0;
    p.param=NULL;
    pfin.push_back(p);
    //pfin[0]=p;
    a.pf=&pfin[0];
    a.addr_var=NULL;
    s.addr=&a;
    sy.push_back(s);
    //sy[sy_num++]=s;
    int ofsd=0;
    while(in >> s1 >> s2)
    {
        /*s=(SYNBL*)malloc(sizeof(SYNBL));
        a=(ADDR*)malloc(sizeof(ADDR));
        a_v=(ADDR_VAR*)malloc(sizeof(ADDR_VAR));*/
        s.name=s1;
        s.typ=s2;
        s.cat="v";
        a_v.lv=0;
        a_v.off=ofsd;
        if(s.typ=="integer")
            ofsd+=4;
        else if(s.typ=="real")
            ofsd+=8;
        else
            ofsd+=1;
        a.addr_var=&a_v;
        a.pf=NULL;
        s.addr=&a;
        sy.push_back(s);
        //sy[sy_num++]=s;
    }
    in.close();
}

void sy_out(vector<SYNBL> sy,vector<PFINFL> pfin)
{
    ofstream out;
    int ofsd=0;
    out.open("SyTab_out.txt");
    if(!out)
    {
        cout << "打开文件失败！" << endl;
        exit(1);
    }
    cout << "NAME\t" << "TYPE\t" << "CAT\t" << "ADDR" << endl;
    out << "NAME\t" << "TYPE\t" << "CAT\t" << "ADDR" << endl;
    for(unsigned int i=0;i<sy.size();i++)
    {
        cout << sy[i].name << "\t" << sy[i].typ << "\t";
        cout << sy[i].cat << "\t";
        out << sy[i].name << "\t" << sy[i].typ << "\t";
        out << sy[i].cat << "\t";
        if(sy[i].cat=="p")
        {
            cout << "PFINF,0" << endl;
            out << "PFINF,0" << endl;
        }
        else
        {
            cout << "(0," << ofsd << ")" << endl;
            out << "(0," << ofsd << ")" << endl;
            if(sy[i].typ=="integer")
                ofsd+=4;
            else if(sy[i].typ=="real")
                ofsd+=8;
            else
                ofsd+=1;
        }
    }
    cout << endl << "PFINFL" << endl << "LEVEL\t" << "OFF\t";
    cout << "FN\t" << "ENTRY\t" << "PARAM" << endl;
    out << endl << "PFINFL" << endl << "LEVEL\t" << "OFF\t";
    out << "FN\t" << "ENTRY\t" << "PARAM" << endl;
    cout << pfin[0].level << "\t" << pfin[0].off << "\t";
    cout << pfin[0].fn << "\t" << pfin[0].entry << "\t0" << endl;
    out << pfin[0].level << "\t" << pfin[0].off << "\t";
    out << pfin[0].fn << "\t" << pfin[0].entry << "\t0";
    out.close();
}

void sytab_main()
{
    sy_in(sy,pfin);
    cout << endl << "符号表：" << endl;
    sy_out(sy,pfin);
}
/*
int main()
{
    sytab_main();
    return 0;
}*/
