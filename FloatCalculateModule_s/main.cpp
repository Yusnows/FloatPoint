#include<iostream>
#include<string>
#include"Str2Binary.h"
#include"FloatModule.h"

using namespace std;

void calculate();

int main()
{
	calculate();
	return 0;
}

void calculate()
{
	string isEnd = "w";
	string str_a;
	FloatType a;
	string opera;
	string str_b;
	FloatType b;
	FloatType c;
	while (isEnd.compare("#"))
	{
		cout << "please input the first num:" << endl;
		cin >> str_a;
		a.FloatInput(str_a);
		a.printFloatObj(a);
		cout << endl;
		cout << "please input the operator" << endl;
		cin >> opera;
		cout << "please input the second num:" << endl;
		cin >> str_b;
		b.FloatInput(str_b);
		b.printFloatObj(b);
		cout << endl;
		if (opera.compare("+") == 0)
		{
			c = a + b;
			cout << "the result is:" << endl;
			c.printFloatObj(c);
		}
		else if (opera.compare("-") == 0)
		{
			c = a - b;
			cout << "the result is:" << endl;
			c.printFloatObj(c);
		}
		else
		{
			cout << "error operartor input" << endl;
		}
		cout << endl;
		cout << "do you want to continue?" << endl << "if not, press #; if Yes, press any key" << endl;
		cin >> isEnd;
		cout << endl;
	}
}
