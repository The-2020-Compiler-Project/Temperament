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
    cout << "�������������һ�׶Ρ�����" << endl;
    getch();
    cout << endl << "�м�������ɣ�" << endl;
    mjtyrhny_main();
	cout << "�������������һ�׶Ρ�����" << endl;
    getch();
    optimizequa_main();
	cout << "�������������һ�׶Ρ�����" << endl;
    getch();
	objectcode_main();
	return 0;
}
