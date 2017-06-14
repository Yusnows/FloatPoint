#pragma once
#include"Str2Binary.h"
#include <iostream>

//定义浮点类
class FloatType : public Str2Binary
{
public:
	FloatType();
	FloatType(string String);									//当以字符串格式输入数值的时候，调用该构造函数
	~FloatType();
	void FloatInput(string str);								//输入函数，输入数值格式为字符串
	void printFloatObj(FloatType Data);							//将自定义的浮点类以二进制以及十进制的形式打印
	friend FloatType operator+(const FloatType& a, const FloatType& b);		//重载操作符‘+’
	friend FloatType complement(FloatType OBJ);									//取补码运算
	friend FloatType operator-(const FloatType& a, const FloatType& b);		//重载操作符‘-’

private:
	unsigned int sign;										//use for sign bit, just can be 0 or 1;
	int exp[8];												//use for exponent,
	unsigned int mant[23];									//use for mantissa,
//用于自定义浮点类运算的辅助函数
	int FloatTypeOBJis_0();
	void mantMovRight(int bits);							//尾数右移函数，用于对阶
	void mantMovLeft(int bits);								//尾数左移函数，用于将运算结果标准化
	void copyexp( FloatType &a);							//指数部分复制函数
	void expAdd(int num);									//指数部分加上num
	int mantAdd(FloatType &a, FloatType &b,int Flag);		//尾数部分相加函数
	void mantAdd(int num);									//尾数部分加上num
	void Binary2FloatType();								//用于将数值的二进制代码转化为标准的浮点二进制代码
	float FloatType2float(FloatType Data);					//用于将FloatType的二进制代码转化为float数据，该函数是为了方面便验算
};
