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

//���뺯����������ֵ��ʽΪ�ַ���
void FloatType::FloatInput(string str)
{

	input(str);
	Binary2FloatType();
}

//���Զ���ĸ������Զ������Լ�ʮ���Ƶ���ʽ��ӡ
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
//������Ϊ������Զ���ĸ�����ת���ɵ�ʵ�ʸ���
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

//β�����ƺ��������ڶԽ�
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

//β�����ƺ��������ڽ���������׼��
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

//ָ�����ָ��ƺ���
void FloatType::copyexp(FloatType & a)
{
	for (int i = 0; i < 8; i++)
	{
		exp[i] = a.exp[i];
	}
}

//ָ�����ּ���num��num����Ϊ����
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

//β��������Ӻ�������Ӧ�ð���ֵת��Ϊ�����Ժ��ٵ��øú�����ӣ�
int FloatType::mantAdd(FloatType & a, FloatType & b,int Flag)
{
//��23λmant��λ��ӣ�Ϊ��һ��ģ����������㣬������������жϵķ������У���û�в��á�/����%��������
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

//�������λ��λ����Լ�����λ
	if (a.sign == 0 && b.sign == 0)		//������������	
	{
		sign = 0;						//��ӽ����ȻΪ��
		if (isCarry == 1)				//β���������λ�н�λ
		{
			mantMovRight(1);			//Ϊ����Ϊ�����׼��ʽ��β������Ӧ������һλ
			if(Flag==0)					//����ͬ�״�
				mant[0] = 1;			//β���������λ�н�λ�����������Ժ�β�����λӦ����Ϊ1			
			else
				mant[0]=0;				//������߲�ͬ�״Σ������β�����λӦ����Ϊ0
			isCarry = 1;				//ָ����һ���Է���ֵ���أ�
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
	else if (a.sign == 0 || b.sign == 0)		//��һ���Ǹ���
	{
		if (isCarry == 1)					//β���������λ�н�λ
		{
			if (Flag == 0)					//����ͬ�״�
			{		
				sign = 0;
				int i = 0;
				for (i = 0; i < 23&&mant[0]==0; i++)
					mantMovLeft(1);
				mantMovLeft(1);
				isCarry = -(i+1);
				if (i + 1 >= 23)
					isCarry = -127;		//���Ƴ���23λ��˵�����Ӧ��Ϊ�㣬����Ӧ�ð�ָ��ֵΪ0������-127��Ϊ��־��
			}
			else
			{
				if (Flag < 0)			//��ʱ��b�Ľ״αȽϸߣ�˵���ƶ�����a��β��
				{
					if (b.sign == 1)		//b�Ǹ���
						sign = 1;
					else					//a�Ǹ���
						sign = 0;
				}
				else						//��ʱ��a�Ľ״αȽϸߣ�˵���ƶ�����b��β��
				{
					if (b.sign == 1)		//b�Ǹ���
						sign = 0;
					else					//a�Ǹ���
						sign = 1;
				}
				isCarry = 0;
			}

		}
		else								//β���������λû�н�λ
		{
			if (Flag == 0)
			{
				sign = 1;
				int i = 0;
				for (i = 0; i < 23 && mant[i] != 0; i++);
				mantMovLeft(i + 1);
				isCarry = -(i + 1);
				if (i + 1 >= 23)
					isCarry = -127;			//���Ƴ���23λ��˵�����Ӧ��Ϊ�㣬����Ӧ�ð�ָ��ֵΪ0������-127��Ϊ��־��
			}
			else
			{
				if (Flag < 0)								//��ʱ��b�Ľ״αȽϸߣ�˵���ƶ�����a��β��
				{
					if (b.sign == 1)						//b�Ǹ���
						sign = 1;							//����b�Ľ״αȽϸߣ�����������ӵĽ����ȻΪ����
					else									//a�Ǹ���
					{
						sign = 0;							//����b�Ľ״αȽϸߣ�����������ӵĽ����ȻΪ����
						int i = 0;
						//for (i = 0; i < 23 && mant[i] != 0; i++);		//�������ʾ�ɱ�׼��ʽ�ĸ�������ƴ���
						mantMovLeft(1);
						isCarry = -(1);
					}
				}
				else						//��ʱ��a�Ľ״αȽϸߣ�˵���ƶ�����b��β��
				{
					if (b.sign == 1)		//b�Ǹ���
					{
						sign = 0;			//����a�Ľ״αȽϸߣ�����������ӵĽ����ȻΪ����
						int i = 0;
						//for (i = 0; i < 23 && mant[i] != 0; i++);		//�������ʾ�ɱ�׼��ʽ�ĸ�������ƴ���
						mantMovLeft(1);
						isCarry = -1;
					}
					else					//a�Ǹ���
						sign = 1;			//����a�Ľ״αȽϸߣ�����������ӵĽ����ȻΪ����
				}
			}
		}
	}
	else									//�������Ǹ���
	{
		sign = 1;							//��ӱ�ȻΪ����
		if (isCarry == 1)					//����������λ�н�λ
		{
			if (Flag == 0)					//˵������ͬ�״�
			{
				mantMovRight(1);			//��ӱ�ȻҪ��ǰ��һλ����β������һλ
				mant[0] = 1;				//�����н�λ���������λӦ��Ϊ1�����룩
				isCarry = 1;				//ָ����һ
			}
			else
				isCarry = 0;				//�״β�ͬ�����ǲ�������н�λ��˵��ԭ�벻��Ҫ��λ
		}
		else								//����������λû�н�λ
		{
			if (Flag == 0)					//���߽״���ͬ
			{
				mantMovRight(1);			//��ӱ�ȻҪ��ǰ��һλ����β������һλ
				mant[0] = 0;				//����û�н�λ���������λӦ��Ϊ0�����룩
				isCarry = 1;				//ָ����һ
			}
			else							//�״β�ͬ���������û�н�λ��ԭ����Ҫ��λ
			{
				mantMovRight(1);			//β������һλ
				mant[0] = 1;				//����û�н�λ���������λӦ��Ϊ1�����룩
				isCarry = 1;				//ָ����һ
			}
		}
	}
	return isCarry;							//����ָ��Ӧ�ü��ϵ���ֵ������Ϊ������
}

//β�����ּ���num��num����Ϊ����
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

//���ڽ���ֵ�Ķ����ƴ���ת��Ϊ��׼�ĸ�������ƴ���,��Ϊ�Ӽ�����������ַ�����ʾ����ֵ������Str2Binaryת����Ϊ���ǷǸ���Ķ����ƴ���
void FloatType::Binary2FloatType()
{
	sign = sign_;
//����ָ��
	int L_Zero_bits = 0;
	for (int i = 0; i < IntBinaryLen; i++)
		if (Int_Binary[i] == 0)
			L_Zero_bits++;
		else 
			break;
	int temp_exp = IntBinaryLen - L_Zero_bits-1;	//��1����Ϊ���L_Zero_bits=IntBinary����ָ��Ӧ��Ϊ-1
	if (temp_exp == -1)
		for (int i = 0; i < DecBinaryLen; i++)
			if (Dec_Binary[i] == 0)
				temp_exp--;
			else
				break;
	int temp = temp_exp;
	if (temp == -24)		//����������⴦��
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
//�����������ֺ�С������,��β������
	if (temp >0)									//˵��Int_Binary����һ����Ҫת��Ϊβ��mant
	{
		int i = 0;
		for (i = L_Zero_bits + 1; i < IntBinaryLen && (i - L_Zero_bits - 1) < 23; i++)
		{
			mant[i - L_Zero_bits - 1] = Int_Binary[i];
		}
		if ((i - L_Zero_bits - 1) < 23)				//˵��Dec_Binary����һ����Ҫת��β��mant
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
		else if (i<IntBinaryLen)				//˵��Dec_Binary��û��Ҫת��Ϊmant�ģ�Ϊ��֤���ȣ��ж�mant�Ƿ���Ҫ��1
		{
			if(Int_Binary[i]==1)
				mantAdd(1);
		}
	}

	else			//˵��IntBinary����û��Ҫת��Ϊβ��mant��
	{
		temp = -temp;
		int i = 0;
		for ( i = temp; (i-temp) < 23 && i < DecBinaryLen; i++)
			mant[i - temp] = Dec_Binary[i];
		if ((i - temp) < 23)
			for (; (i - temp) < 23; i++)
				mant[i - temp] = 0;
		else if (i < DecBinaryLen)		//Ϊ��֤���ȣ��ж�mant�Ƿ���Ҫ��1
		{
			if (Dec_Binary[i] == 1)
				mantAdd(1);
		}
	}

	//�������
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

//���ڽ�FloatType�Ķ����ƴ���ת��Ϊfloat���ݣ��ú�����Ϊ�˷��������
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

//ȡ��������
FloatType complement(FloatType OBJ)
{
	FloatType temp;
	temp.sign = OBJ.sign;
	for (int j = 0; j < 8; j++)
		temp.exp[j] = OBJ.exp[j];
	if (OBJ.sign == 1)
	{
		for (int i = 0; i < 23; i++)			//ȡ��
		{
			if (OBJ.mant[i] == 1)
				temp.mant[i] = 0;
			else
				temp.mant[i] = 1;
		}

		int i = 0;
		for ( i = 0; temp.mant[22 - i] + 1 > 1 && i<23; i++)		//��һ
			temp.mant[22 - i] = 0;
		if(i!=23)
			temp.mant[22 - i] = 1;								
		//��ΪIEEE��׼�涨����β��ȫΪ�������£��ø�����Ϊ��ö���������ȡ�����ʱ����Բ��ÿ��Ǹ����
	}
	else
		for (int j = 0; j < 23; j++)
		{
			temp.mant[j] = OBJ.mant[j];
		}
	return FloatType(temp);
}

//���ز�������+��
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
		temp_a = complement(temp_a);										//ȡ����
		temp_b = complement(temp_b);
		isCarry = temp_c.mantAdd(temp_a, temp_b, Flag_mantMov);				//mantissa process
		temp_c.copyexp(temp_a);										//exp process
	}
	else															//�������ͬ�״εģ�Ҫ�����Ƴ�ͬ�״��ټ���
	{
		int temp;
		temp = exp_a - exp_b;
		Flag_mantMov = temp;
		if (temp > 0&&temp<23)
		{
			temp_b.mantMovRight(temp);
			temp_b.mant[temp-1] = 1;
			temp_a = complement(temp_a);									//ȡ����
			temp_b = complement(temp_b);
			isCarry = temp_c.mantAdd(temp_a, temp_b, Flag_mantMov);			//mantissa process
			temp_c.copyexp(temp_a);
		}
		else if(temp>-23&&temp<0)
		{
			temp = -temp;
			temp_a.mantMovRight(temp);
			temp_a.mant[temp - 1] = 1;
			temp_a = complement(temp_a);								//ȡ����
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
	if(isCarry==-127)		//˵���Ѿ�������
	{ 
		for (int i = 0; i < 8; i++)
			temp_c.exp[i] = 0;
	}
	else
		temp_c.expAdd(isCarry);
	temp_c = complement(temp_c);								//��ȡһ�β��룬���õ�ԭ��
	return FloatType(temp_c);
}

//���ز�������-��
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
