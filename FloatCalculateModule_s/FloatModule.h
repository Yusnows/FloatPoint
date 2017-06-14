#pragma once
#include"Str2Binary.h"
#include <iostream>

//���帡����
class FloatType : public Str2Binary
{
public:
	FloatType();
	FloatType(string String);									//�����ַ�����ʽ������ֵ��ʱ�򣬵��øù��캯��
	~FloatType();
	void FloatInput(string str);								//���뺯����������ֵ��ʽΪ�ַ���
	void printFloatObj(FloatType Data);							//���Զ���ĸ������Զ������Լ�ʮ���Ƶ���ʽ��ӡ
	friend FloatType operator+(const FloatType& a, const FloatType& b);		//���ز�������+��
	friend FloatType complement(FloatType OBJ);									//ȡ��������
	friend FloatType operator-(const FloatType& a, const FloatType& b);		//���ز�������-��

private:
	unsigned int sign;										//use for sign bit, just can be 0 or 1;
	int exp[8];												//use for exponent,
	unsigned int mant[23];									//use for mantissa,
//�����Զ��帡��������ĸ�������
	int FloatTypeOBJis_0();
	void mantMovRight(int bits);							//β�����ƺ��������ڶԽ�
	void mantMovLeft(int bits);								//β�����ƺ��������ڽ���������׼��
	void copyexp( FloatType &a);							//ָ�����ָ��ƺ���
	void expAdd(int num);									//ָ�����ּ���num
	int mantAdd(FloatType &a, FloatType &b,int Flag);		//β��������Ӻ���
	void mantAdd(int num);									//β�����ּ���num
	void Binary2FloatType();								//���ڽ���ֵ�Ķ����ƴ���ת��Ϊ��׼�ĸ�������ƴ���
	float FloatType2float(FloatType Data);					//���ڽ�FloatType�Ķ����ƴ���ת��Ϊfloat���ݣ��ú�����Ϊ�˷��������
};
