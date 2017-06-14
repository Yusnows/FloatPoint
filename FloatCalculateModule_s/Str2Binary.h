#pragma once
#include<iostream>
#include<string>

using namespace std;

class Str2Binary
{
public:
	Str2Binary();
	Str2Binary(string str);
	~Str2Binary();
	void input(string str);
protected:
	int *Int_Binary = NULL;
	int *Dec_Binary = NULL;
	int sign_ = 0;
	int DecBinaryLen;
	int IntBinaryLen;
private:
	string inputdata;
	int calculateSpace(int n);
	void Str2BinaryTran();
	int DecPartMulti_2(int *DecPart,int DecLen);
	int IntPartDivi_2(int *IntPart, int IntLen);
	int* LenCountAndCopy(string &Int_str,string& Dec_str);
};

