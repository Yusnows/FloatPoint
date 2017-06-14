#include "Str2Binary.h"
#include<iostream>
#include<new>
#include<new.h>

using namespace std;

Str2Binary::Str2Binary()
{

}

Str2Binary::Str2Binary(string str)
{
	inputdata = str;
	Str2BinaryTran();
}

Str2Binary::~Str2Binary()
{

}

void Str2Binary::input(string str)
{
	inputdata = str;
	Str2BinaryTran();
}

int Str2Binary::calculateSpace(int n)
{
	int space = 0;
	space = n * 3 + n * 3 / 10 + 2;
	return space;
}

void Str2Binary::Str2BinaryTran()
{
	int *temp;
	string Int_str;
	string Dec_str;
	temp = LenCountAndCopy(Int_str,Dec_str);
	int IntLen = temp[0];
	int DecLen = temp[1];
	if (IntLen != -1&&DecLen!=-1)
	{
		int *IntPart = new int[IntLen];
		int *DecPart = new int[DecLen];
		for (int i = 0; i < IntLen; i++)
			IntPart[i] = Int_str[i] - 48;
		for (int i = 0; i < DecLen; i++)
			DecPart[i] = Dec_str[i] - 48;
		//测试输出:
		//for (int i = 0; i < IntLen; i++)
		//	cout << (IntPart[i]);
		//for (int i = 0; i < DecLen; i++)
		//	cout << (DecPart[i]);

		//处理小数部分
		DecBinaryLen = calculateSpace(DecLen);
		if (DecBinaryLen < 23)
			DecBinaryLen = 23;
		Dec_Binary = new int[DecBinaryLen];
		for (int i = 0; i < DecBinaryLen; i++)
		{
			Dec_Binary[i] = DecPartMulti_2(DecPart, DecLen);		//Dec_Binary[0] is MSB
		}
		//处理整数部分
		IntBinaryLen = calculateSpace(IntLen);
		Int_Binary = new int[IntBinaryLen];
		for (int i = IntBinaryLen - 1; i >= 0; i--)
		{
			Int_Binary[i] = IntPartDivi_2(IntPart, IntLen);
		}
		//测试输出:
		//cout << endl;
		//for (int i = 0; i < IntBinaryLen; i++)
		//	cout << Int_Binary[i];
		//cout << endl;
		//for (int i = 0; i < DecBinaryLen; i++)
		//	cout << Dec_Binary[i];
		//cout << endl;
		//float temp_F = 0;
		//for (int i = 0; i < DecBinaryLen; i++)
		//	temp_F = temp_F / 2 + Dec_Binary[DecBinaryLen - 1 - i];
		//temp_F /= 2;
		//printf("%0.9f", temp_F);
		delete[] IntPart;
		delete[] DecPart;
	}
}

int Str2Binary::DecPartMulti_2(int * DecPart,int DecLen)
{
	int isCarry = 0;
	for (int i = DecLen-1; i >=0; i--)
	{
		int temp = DecPart[i];
		DecPart[i] = (temp + temp + isCarry) % 10;
		isCarry = (temp + temp + isCarry) / 10;
	}
	return isCarry;
}

int Str2Binary::IntPartDivi_2(int * IntPart, int IntLen)
{
	int Remainder = 0;
	for (int i = 0; i <IntLen; i++)
	{
		int temp = IntPart[i];
		IntPart[i] = (temp + Remainder * 10) / 2;
		Remainder = (temp + Remainder * 10) % 2;
	}
	return Remainder;
}

int* Str2Binary::LenCountAndCopy(string &Int_str, string &Dec_str)
{
	int IntLen = 0;
	int DecLen = 0;
	int flag=0;
	string temp_str;
	//处理符号位
	if (inputdata[0] == '-')
	{
		sign_ = 1;
		temp_str.append(inputdata, 1, DecLen - 1);
	}
	else if (inputdata[0] == '+')
	{
		sign_ = 0;
		temp_str.append(inputdata, 1, DecLen - 1);
	}
	else if (inputdata[0] > 47 && inputdata[0] < 58)
	{
		sign_ = 0;
		temp_str.append(inputdata);
	}
	else
	{
		cout << "error input format" << endl;
		flag = 1;
	}
	//处理数字部分
	if (flag == 0)
	{
		for (int i = 0; i < temp_str.length() && temp_str[i] != '.'; i++)
			IntLen++;														//统计整数部分的长度
		Int_str.append(temp_str, 0, IntLen);
		if (IntLen < temp_str.length())
		{
			DecLen = temp_str.length() - IntLen - 1;
			Dec_str.append(temp_str, IntLen + 1, DecLen);
		}
		else
		{
			DecLen = 0;
		}
		//检查Int_str和Dec_str中是否存在非法字符
		for (int i = 0; i < IntLen; i++)
		{
			if (Int_str[i] < 48 || Int_str[i]>57)
			{
				flag = 1;
				Int_str.clear();
				cout << "error input format" << endl;
				break;
			}
		}
		for (int i = 0; i < DecLen; i++)
		{
			if (Dec_str[i] < 48 || Dec_str[i]>57)
			{
				flag = 1;
				Dec_str.clear();
				cout << "error input format" << endl;
				break;
			}
		}
	}
	//处理函数返回值，为调用者判断是否有错误输入
	int temp_int[2];
	if (flag == 1)
	{
		temp_int[0] = -1;
		temp_int[1] = -1;
	}
	else
	{
		temp_int[0] = IntLen;
		temp_int[1] = DecLen;
	}
	return temp_int;
}