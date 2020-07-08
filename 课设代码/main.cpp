#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <list>
#include <sstream>
#include <vector>
#include <string>
#include <conio.h>

using namespace std;

extern void token_output();

extern void mjtyrhny_main();

extern void optimizequa_main();

extern void objectcode_main();

int main()
{
    token_output();
    cout << "按任意键进行下一阶段。。。" << endl;
    getch();
    cout << endl << "中间代码生成：" << endl;
    mjtyrhny_main();
	cout << "按任意键进行下一阶段。。。" << endl;
    getch();
    optimizequa_main();
	cout << "按任意键进行下一阶段。。。" << endl;
    getch();
	objectcode_main();
	return 0;
}
