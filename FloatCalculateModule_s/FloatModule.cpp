#include "FloatModule.h"
#include <math.h>

using namespace std;

FloatType::FloatType()
{
}

FloatType::FloatType(string String) :Str2Binary(String)
{
	Binary2FloatType();
}

FloatType::~FloatType()
{

}

//输入函数，输入数值格式为字符串
void FloatType::FloatInput(string str)
{

	input(str);
	Binary2FloatType();
}

//将自定义的浮点类以二进制以及十进制的形式打印
void FloatType::printFloatObj(FloatType Data)
{
	cout << Data.sign<<" ";
	for (int i = 0; i < 8; i++)
	{
		cout << Data.exp[i];
	}
	cout << " ";
	for (int i = 0; i < 23; i++)
	{
		if (i % 4 == 0)
			cout << " ";
		cout << Data.mant[i];
	}
	cout << "   ";
//以下是为了输出自定义的浮点数转化成的实质浮点
	double temp;
	temp = FloatType2float(Data);
	printf("%lf", temp);
}

int FloatType::FloatTypeOBJis_0()
{
	int flag_0 = 0;
	for (int i = 0; i < 8; i++)
	{
		if (exp[i] != 0)
		{
			flag_0 = 1;
			break;
		}
	}
	if (flag_0 == 0)
	{
		for (int i = 0; i < 23; i++)
		{
			if (exp[i] != 0)
			{
				flag_0 = 1;
				break;
			}
		}
	}
	return flag_0;
}

//尾数右移函数，用于对阶
void FloatType::mantMovRight(int bits)
{
	for (int j = 0; j < bits &&j<23; j++)
		for (int i = 0; i < 23; i++)
		{
			if (i != 22)
				mant[22 - i] = mant[21 - i];
			else
				mant[0] = 0;
		}
}

//尾数左移函数，用于将运算结果标准化
void FloatType::mantMovLeft(int bits)
{
	for (int j = 0; j < bits&&j<23; j++)
		for (int i = 0; i < 23; i++)
		{
			if (i != 22)
				mant[i] = mant[i+1];
			else
				mant[22] = 0;
		}
}

//指数部分复制函数
void FloatType::copyexp(FloatType & a)
{
	for (int i = 0; i < 8; i++)
	{
		exp[i] = a.exp[i];
	}
}

//指数部分加上num，num可以为负数
void FloatType::expAdd(int num)
{
	int temp = abs(num);
	if (num > 0)
	{
		for (int i = 0; i < temp; i++)
		{
			int j = 0;
			for (j = 0; j < 8 && exp[7 - j] + 1>1; j++)
			{
				exp[7 - j] = 0;
			}
			if (j != 8)
				exp[7 - j] = 1;
		}
	}
	else if(num<0)
	{
		for (int i = 0; i < temp; i++)
		{
			int j = 0;
			for (j = 0; j < 8 && exp[7 - j] - 1<0; j++)
			{
				exp[7 - j] = 1;
			}
			if (j != 8)
				exp[7 - j] = 0;
		}
	}
}

//尾数部分相加函数，（应该把数值转化为补码以后再调用该函数相加）
int FloatType::mantAdd(FloatType & a, FloatType & b,int Flag)
{
//将23位mant按位相加，为进一步模拟机器的运算，整理采用条件判断的方法进行，而没有采用‘/’或‘%’操作符
	int isCarry=0;
	for (int i = 0; i < 23; i++)
	{
		if (isCarry == 0)
		{
			if (a.mant[22 - i] + b.mant[22 - i] > 1)
			{
				isCarry = 1;
				mant[22 - i] = 0;
			}
			else mant[22 - i] = a.mant[22 - i] + b.mant[22 - i];
		}
		else
		{
			if (a.mant[22 - i] + b.mant[22 - i] > 1)
			{
				mant[22 - i] = 1;
				isCarry = 1;
			}
			else if(a.mant[22 - i] + b.mant[22 - i]==1)
			{
				mant[22 - i] = 0;
				isCarry = 1;
			}
			else
			{
				isCarry = 0;
				mant[22 - i] = 1;
			}
		}
	}

//处理最高位进位与否以及符号位
	if (a.sign == 0 && b.sign == 0)		//两个都是正数	
	{
		sign = 0;						//相加结果必然为正
		if (isCarry == 1)				//尾数补码最高位有进位
		{
			mantMovRight(1);			//为调整为浮点标准格式，尾数部分应该右移一位
			if(Flag==0)					//两者同阶次
				mant[0] = 1;			//尾数补码最高位有进位，所以右移以后尾数最高位应该置为1			
			else
				mant[0]=0;				//如果两者不同阶次，结果的尾数最高位应该置为0
			isCarry = 1;				//指数加一（以返回值返回）
		}
		else
		{
			if (Flag == 0)
			{
				mantMovRight(1);
				mant[0] = 0;
				isCarry = 1;
			}
			else
				isCarry = 0;
		}
	}
	else if (a.sign == 0 || b.sign == 0)		//有一个是负数
	{
		if (isCarry == 1)					//尾数补码最高位有进位
		{
			if (Flag == 0)					//两者同阶次
			{		
				sign = 0;
				int i = 0;
				for (i = 0; i < 23&&mant[0]==0; i++)
					mantMovLeft(1);
				mantMovLeft(1);
				isCarry = -(i+1);
				if (i + 1 >= 23)
					isCarry = -127;		//左移超过23位，说明结果应该为零，所以应该把指数值为0（返回-127作为标志）
			}
			else
			{
				if (Flag < 0)			//此时是b的阶次比较高，说明移动的是a的尾数
				{
					if (b.sign == 1)		//b是负数
						sign = 1;
					else					//a是负数
						sign = 0;
				}
				else						//此时是a的阶次比较高，说明移动的是b的尾数
				{
					if (b.sign == 1)		//b是负数
						sign = 0;
					else					//a是负数
						sign = 1;
				}
				isCarry = 0;
			}

		}
		else								//尾数补码最高位没有进位
		{
			if (Flag == 0)
			{
				sign = 1;
				int i = 0;
				for (i = 0; i < 23 && mant[i] != 0; i++);
				mantMovLeft(i + 1);
				isCarry = -(i + 1);
				if (i + 1 >= 23)
					isCarry = -127;			//左移超过23位，说明结果应该为零，所以应该把指数值为0（返回-127作为标志）
			}
			else
			{
				if (Flag < 0)								//此时是b的阶次比较高，说明移动的是a的尾数
				{
					if (b.sign == 1)						//b是负数
						sign = 1;							//由于b的阶次比较高，所以两者相加的结果必然为负数
					else									//a是负数
					{
						sign = 0;							//由于b的阶次比较高，所以两者相加的结果必然为正数
						int i = 0;
						//for (i = 0; i < 23 && mant[i] != 0; i++);		//将结果表示成标准格式的浮点二进制代码
						mantMovLeft(1);
						isCarry = -(1);
					}
				}
				else						//此时是a的阶次比较高，说明移动的是b的尾数
				{
					if (b.sign == 1)		//b是负数
					{
						sign = 0;			//由于a的阶次比较高，所以两者相加的结果必然为正数
						int i = 0;
						//for (i = 0; i < 23 && mant[i] != 0; i++);		//将结果表示成标准格式的浮点二进制代码
						mantMovLeft(1);
						isCarry = -1;
					}
					else					//a是负数
						sign = 1;			//由于a的阶次比较高，所以两者相加的结果必然为负数
				}
			}
		}
	}
	else									//两个都是负数
	{
		sign = 1;							//相加必然为负数
		if (isCarry == 1)					//补码相加最高位有进位
		{
			if (Flag == 0)					//说明两者同阶次
			{
				mantMovRight(1);			//相加必然要往前进一位，即尾数右移一位
				mant[0] = 1;				//由于有进位，所以最高位应该为1（补码）
				isCarry = 1;				//指数加一
			}
			else
				isCarry = 0;				//阶次不同，但是补码相加有进位，说明原码不需要进位
		}
		else								//补码相加最高位没有进位
		{
			if (Flag == 0)					//两者阶次相同
			{
				mantMovRight(1);			//相加必然要往前进一位，即尾数右移一位
				mant[0] = 0;				//由于没有进位，所以最高位应该为0（补码）
				isCarry = 1;				//指数加一
			}
			else							//阶次不同，补码相加没有进位，原码需要进位
			{
				mantMovRight(1);			//尾数右移一位
				mant[0] = 1;				//由于没有进位，所以最高位应该为1（补码）
				isCarry = 1;				//指数加一
			}
		}
	}
	return isCarry;							//返回指数应该加上的数值（可能为负数）
}

//尾数部分加上num，num可以为负数
void FloatType::mantAdd(int num)
{
	int temp = abs(num);
	if (num > 0)
	{
		for (int i = 0; i < temp; i++)
		{
			int j = 0;
			for (j = 0; j < 23 && mant[22 - j] + 1>1; j++)
			{
				mant[22 - j] = 0;
			}
			if (j != 23)
				mant[22 - j] = 1;
		}
	}
	else if (num<0)
	{
		for (int i = 0; i < temp; i++)
		{
			int j = 0;
			for (j = 0; j < 23 && mant[22 - j] - 1<0; j++)
			{
				mant[22 - j] = 1;
			}
			if (j != 23)
				mant[22 - j] = 0;
		}
	}
}

//用于将数值的二进制代码转化为标准的浮点二进制代码,因为从键盘输入的是字符串表示的数值，经过Str2Binary转化成为的是非浮点的二进制代码
void FloatType::Binary2FloatType()
{
	sign = sign_;
//处理指数
	int L_Zero_bits = 0;
	for (int i = 0; i < IntBinaryLen; i++)
		if (Int_Binary[i] == 0)
			L_Zero_bits++;
		else 
			break;
	int temp_exp = IntBinaryLen - L_Zero_bits-1;	//减1是因为如果L_Zero_bits=IntBinary，则指数应该为-1
	if (temp_exp == -1)
		for (int i = 0; i < DecBinaryLen; i++)
			if (Dec_Binary[i] == 0)
				temp_exp--;
			else
				break;
	int temp = temp_exp;
	if (temp == -24)		//对于零的特殊处理
		temp_exp = -127;
	temp_exp += 127;
	for (int j = 0; j < 8; j++)
	{
		int temp1 = 1;
		for (int Count = 0; Count < 7 - j; Count++)
		{
			temp1 = temp1 * 2;
		}
		exp[j] = temp_exp / temp1;					//exp[0]  is MSB;
		if (temp_exp - temp1 >= 0)
			temp_exp = temp_exp - temp1;
		temp1 = 1;
	}
//处理整数部分和小数部分,即尾数部分
	if (temp >0)									//说明Int_Binary中有一部分要转化为尾数mant
	{
		int i = 0;
		for (i = L_Zero_bits + 1; i < IntBinaryLen && (i - L_Zero_bits - 1) < 23; i++)
		{
			mant[i - L_Zero_bits - 1] = Int_Binary[i];
		}
		if ((i - L_Zero_bits - 1) < 23)				//说明Dec_Binary中有一部分要转化尾数mant
		{
			int j = 0;
			for (j = 0; (i - L_Zero_bits - 1) < 23 && j < DecBinaryLen; i++, j++)
				mant[i - L_Zero_bits - 1] = Dec_Binary[j];
			if ((i - L_Zero_bits - 1) < 23)
				for (; (i - L_Zero_bits - 1) < 23; i++)
					mant[(i - L_Zero_bits - 1)] = 0;
			else if (j<DecBinaryLen)
			{
				if(Dec_Binary[j]==1)
					mantAdd(1);
			}
		}
		else if (i<IntBinaryLen)				//说明Dec_Binary中没有要转化为mant的，为保证经度，判断mant是否需要加1
		{
			if(Int_Binary[i]==1)
				mantAdd(1);
		}
	}

	else			//说明IntBinary里面没有要转化为尾数mant的
	{
		temp = -temp;
		int i = 0;
		for ( i = temp; (i-temp) < 23 && i < DecBinaryLen; i++)
			mant[i - temp] = Dec_Binary[i];
		if ((i - temp) < 23)
			for (; (i - temp) < 23; i++)
				mant[i - temp] = 0;
		else if (i < DecBinaryLen)		//为保证经度，判断mant是否需要加1
		{
			if (Dec_Binary[i] == 1)
				mantAdd(1);
		}
	}

	//测试输出
	//cout<<endl << "Binary2FloatType:";
	//cout << endl << sign << " ";
	//for (int i = 0; i < 8; i++)
	//	cout << exp[i];
	//cout << " ";
	//for (int i = 0; i < 23; i++)
	//{
	//	if (i % 4 == 0)
	//		cout << " ";
	//	cout << mant[i];
	//}
}

//用于将FloatType的二进制代码转化为float数据，该函数是为了方面便验算
float FloatType::FloatType2float(FloatType Data)
{
	int exp_ = 0;
	for (int i = 0; i < 8; i++)
	{
		exp_ = exp_ * 2 + Data.exp[i];
	}
	exp_ = exp_ - 127;
	if (exp_ != -127)
	{
		float mant = 0;
		for (int i = 0; i < 23; i++)
		{
			mant = mant / 2 + Data.mant[22 - i];
		}
		mant = mant / 2 + 1;
		float temp = mant;
		if (exp_ > 0)
			for (int i = 0; i < exp_; i++)
			{
				temp = temp * 2;
			}
		else
			for (int i = 0; i < abs(exp_); i++)
			{
				temp = temp / 2;
			}
		if (Data.sign == 1)
		{
			temp = -temp;
		}
		return temp;
	}
	else
	{
		return 0;
	}
}

//取补码运算
FloatType complement(FloatType OBJ)
{
	FloatType temp;
	temp.sign = OBJ.sign;
	for (int j = 0; j < 8; j++)
		temp.exp[j] = OBJ.exp[j];
	if (OBJ.sign == 1)
	{
		for (int i = 0; i < 23; i++)			//取反
		{
			if (OBJ.mant[i] == 1)
				temp.mant[i] = 0;
			else
				temp.mant[i] = 1;
		}

		int i = 0;
		for ( i = 0; temp.mant[22 - i] + 1 > 1 && i<23; i++)		//加一
			temp.mant[22 - i] = 0;
		if(i!=23)
			temp.mant[22 - i] = 1;								
		//因为IEEE标准规定，在尾数全为零的情况下，该浮点数为可枚举情况，故取补码的时候可以不用考虑该情况
	}
	else
		for (int j = 0; j < 23; j++)
		{
			temp.mant[j] = OBJ.mant[j];
		}
	return FloatType(temp);
}

//重载操作符‘+’
FloatType operator+(const FloatType & a, const FloatType & b)
{
	FloatType temp_a, temp_b, temp_c;
	temp_a = a;								
	temp_b = b;
	int exp_a=0, exp_b=0;
	int i = 0;
	int Flag_mantMov = 0;
	for ( i = 0; i < 8; i++)
	{
		exp_a = exp_a * 2 + temp_a.exp[i];
		exp_b = exp_b * 2 + temp_b.exp[i];						//exp[0] is MSB
	}
	int isCarry = 0;
	if (exp_a==exp_b)
	{
		temp_a = complement(temp_a);										//取补码
		temp_b = complement(temp_b);
		isCarry = temp_c.mantAdd(temp_a, temp_b, Flag_mantMov);				//mantissa process
		temp_c.copyexp(temp_a);										//exp process
	}
	else															//如果不是同阶次的，要先右移成同阶次再计算
	{
		int temp;
		temp = exp_a - exp_b;
		Flag_mantMov = temp;
		if (temp > 0&&temp<23)
		{
			temp_b.mantMovRight(temp);
			temp_b.mant[temp-1] = 1;
			temp_a = complement(temp_a);									//取补码
			temp_b = complement(temp_b);
			isCarry = temp_c.mantAdd(temp_a, temp_b, Flag_mantMov);			//mantissa process
			temp_c.copyexp(temp_a);
		}
		else if(temp>-23&&temp<0)
		{
			temp = -temp;
			temp_a.mantMovRight(temp);
			temp_a.mant[temp - 1] = 1;
			temp_a = complement(temp_a);								//取补码
			temp_b = complement(temp_b);
			isCarry = temp_c.mantAdd(temp_a, temp_b, Flag_mantMov);			//mantissa process
			temp_c.copyexp(temp_b);
		}
		else
		{
			if (temp < -23)
				temp_c = complement(temp_b);
			if (temp > 23)
				temp_c =complement(temp_a);
		}
	}
	if(isCarry==-127)		//说明已经是零了
	{ 
		for (int i = 0; i < 8; i++)
			temp_c.exp[i] = 0;
	}
	else
		temp_c.expAdd(isCarry);
	temp_c = complement(temp_c);								//再取一次补码，即得到原码
	return FloatType(temp_c);
}

//重载操作符‘-’
FloatType operator-(const FloatType & a, const FloatType & b)
{
	FloatType temp_a, temp_b, temp_c;
	temp_a = a;
	temp_b = b;
	if (temp_b.sign == 1)
		temp_b.sign = 0;
	else
	{
		int flag_0 = temp_b.FloatTypeOBJis_0();
		if(flag_0==1)
			temp_b.sign = 1;
	}
	temp_c = temp_a + temp_b;
	return FloatType(temp_c);
}
