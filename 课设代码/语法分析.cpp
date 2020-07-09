#include <iostream>
#include <string>
#include <fstream>

using namespace std;

string TK[1000][2];     //token序列

string RW[2];           //当前读取的token

int pst;                //当前读取位置

void token_in(string (*TK)[2])      //从文件中读取token序列
{
    ifstream in;
    in.open("token.txt");
    if(!in)
    {
        cout << "打开文件失败" << endl;
        exit(1);
    }
    int i=0;
    while(!in.eof())
    {
        in >> TK[i][0] >> TK[i][1];
        i++;
    }
    in.close();
}

void NEXT_TK(string *RW,string (*TK)[2],int& pst)   //读取下一个token
{
    pst++;
    RW[0]=TK[pst][0];
    RW[1]=TK[pst][1];
    //cout << RW[0] << " " << RW[1] << endl;
}

void PROGRAM(string *RW,string (*TK)[2],int& pst);          //子程序PROGRAM
void SUB_PROGRAM(string *RW,string (*TK)[2],int& pst);      //子程序SUB_PROGRAM
void VARIABLE(string *RW,string (*TK)[2],int& pst);         //子程序VARIABLE
void ID_SEQUENCE(string *RW,string (*TK)[2],int& pst);      //子程序ID_SEQUENCE
void TYPE(string *RW,string (*TK)[2],int& pst);             //子程序TYPE
void COM_SENTENCE(string *RW,string (*TK)[2],int& pst);     //子程序COM_SENTENCE
void SEN_SEQUENCE(string *RW,string (*TK)[2],int& pst);     //子程序SEN_SEQUENCE
void EVA_SENTENCE(string *RW,string (*TK)[2],int& pst);     //子程序EVA_SENTENCE
void EXPRESSION(string *RW,string (*TK)[2],int& pst);       //子程序EXPRESSION
void TERM(string *RW,string (*TK)[2],int& pst);             //子程序TERM
void FACTOR(string *RW,string (*TK)[2],int& pst);           //子程序FACTOR
void main_MAIN(string *RW,string (*TK)[2],int& pst);        //主程序

void PROGRAM(string *RW,string (*TK)[2],int& pst)           //子程序PROGRAM
{
    if(RW[0][1]=='K'&&RW[1]=="program")
    {
        NEXT_TK(RW,TK,pst);
        if(RW[0][1]=='I')
        {
            NEXT_TK(RW,TK,pst);
            SUB_PROGRAM(RW,TK,pst);
        }
        else
        {
            cout << "error in PROGRAM2!" << endl;
            exit(1);
        }
    }
    else
    {
        cout << "error in PROGRAM1!" << endl;
        exit(1);
    }
}

void SUB_PROGRAM(string *RW,string (*TK)[2],int& pst)       //子程序SUB_PROGRAM
{
    VARIABLE(RW,TK,pst);
    COM_SENTENCE(RW,TK,pst);
}

void VARIABLE(string *RW,string (*TK)[2],int& pst)          //子程序VARIABLE
{
    if(RW[0][1]=='K'&&RW[1]=="var")
    {
        NEXT_TK(RW,TK,pst);
        while(1)
        {
            ID_SEQUENCE(RW,TK,pst);
            if(RW[0][1]=='P'&&RW[1]==":")
            {
                NEXT_TK(RW,TK,pst);
                TYPE(RW,TK,pst);
                if(RW[0][1]=='P'&&RW[1]==";")
                {
                    NEXT_TK(RW,TK,pst);
                    if(RW[0][1]=='K'&&RW[1]=="var")
                        NEXT_TK(RW,TK,pst);
                    else
                        return;
                }
                else
                {
                    cout << "error in VARIABLE3!" << endl;
                    exit(1);
                }
            }
            else
            {
                cout << "error in VARIABLE2!" << endl;
                exit(1);
            }

        }
    }
    else
    {
        cout << "error in VARIABLE1!" << endl;
        exit(1);
    }
}

void ID_SEQUENCE(string *RW,string (*TK)[2],int& pst)       //子程序ID_SEQUENCE
{
    while(1)
    {
        if(RW[0][1]=='I')
        {
            NEXT_TK(RW,TK,pst);
            if(RW[0][1]=='P'&&RW[1]==",")
                NEXT_TK(RW,TK,pst);
            else
                return;
        }
        else
        {
            cout << "error in ID_SEQUENCE!" << endl;
            exit(1);
        }
    }
}

void TYPE(string *RW,string (*TK)[2],int& pst)              //子程序TYPE
{
    if(RW[0][1]=='K'&&(RW[1]=="integer"||RW[1]=="real"||RW[1]=="char"))
        NEXT_TK(RW,TK,pst);
    else
    {
        cout << "error in TYPE!" << endl;
        exit(1);
    }
}

void COM_SENTENCE(string *RW,string (*TK)[2],int& pst)      //子程序COM_SENTENCE
{
    if(RW[0][1]=='K'&&RW[1]=="begin")
    {
        NEXT_TK(RW,TK,pst);
        SEN_SEQUENCE(RW,TK,pst);
        if(RW[0][1]=='K'&&RW[1]=="end")
            NEXT_TK(RW,TK,pst);
        else
        {
            cout << "error in COM_SENTENCE2!" << endl;
            exit(1);
        }
    }
    else
    {
        cout << "error in COM_SENTENCE1!" << endl;
        exit(1);
    }
}

void SEN_SEQUENCE(string *RW,string (*TK)[2],int& pst)      //子程序SEN_SEQUENCE
{
    while(1)
    {
        EVA_SENTENCE(RW,TK,pst);
        if(RW[0][1]=='P'&&RW[1]==";")
            NEXT_TK(RW,TK,pst);
        else
            return;
    }
}

void EVA_SENTENCE(string *RW,string (*TK)[2],int& pst)      //子程序EVA_SENTENCE
{
    if(RW[0][1]=='I')
    {
        NEXT_TK(RW,TK,pst);
        if(RW[0][1]=='P'&&RW[1]==":=")
        {
            NEXT_TK(RW,TK,pst);
            EXPRESSION(RW,TK,pst);
        }
        else
        {
            cout << "error in EVA_SENTENCE2!" << endl;
            exit(1);
        }
    }
    else
    {
        cout << "error in EVA_SENTENCE1!" << endl;
        exit(1);
    }
}

void EXPRESSION(string *RW,string (*TK)[2],int& pst)        //子程序EXPRESSION
{
    while(1)
    {
        TERM(RW,TK,pst);
        if(RW[0][1]=='P'&&(RW[1]=="+"||RW[1]=="-"))
            NEXT_TK(RW,TK,pst);
        else
            return;
    }
}

void TERM(string *RW,string (*TK)[2],int& pst)              //子程序TERM
{
    while(1)
    {
        FACTOR(RW,TK,pst);
        if(RW[0][1]=='P'&&(RW[1]=="*"||RW[1]=="/"))
            NEXT_TK(RW,TK,pst);
        else
            return;
    }
}

void FACTOR(string *RW,string (*TK)[2],int& pst)
{
    if(RW[0][1]=='I'||RW[0][1]=='C')
    {
        NEXT_TK(RW,TK,pst);
    }
    else if(RW[0][1]=='P'&&RW[1]=="(")
    {
        NEXT_TK(RW,TK,pst);
        EXPRESSION(RW,TK,pst);
        if(RW[0][1]=='P'&&RW[1]==")")
            NEXT_TK(RW,TK,pst);
        else
        {
            cout << "error in FACTOR2!" << endl;
            exit(1);
        }
    }
    else
    {
        cout << "error in FACTOR1!" << endl;
        exit(1);
    }
}

void main_MAIN(string *RW,string (*TK)[2],int& pst)     //主程序
{
    token_in(TK);
    pst=-1;
    NEXT_TK(RW,TK,pst);
    PROGRAM(RW,TK,pst);
    if(RW[0][1]=='_'&&RW[1]=="#")
        return;
    else
    {
        cout << "error in main_MAIN!" << endl;
        exit(1);
    }
}

void yufa_main()
{
    main_MAIN(RW,TK,pst);
    cout << "Accepted!" << endl;
}
