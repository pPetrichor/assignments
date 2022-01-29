/*
��ģ��Ҫ����O(1)��ʱ�临�Ӷ���ʵ�����֤�ŵ�
���ң��ҵ�˼·�����ò�¡��������ʵ�ֲ��ң�
���ȣ�Ҫ���ݴ洢���ݵĸ���ȷ��Ҫ���೤�ı��ش���
�Լ���Ӧ��Ӧ��ѡ����ٸ���ϣ����ʹ�ô����ʴﵽ
���ͣ�ͨ�������ѡ����m/n=12��k=8��һ�������Ӧ
�Ĵ�����Ϊ0.00314��
��ˣ����������nֵ������m/n=12���m������һ��
����Ϊm�ı��ش���������8����ͬ�Ĺ�ϣ������Ҫ��
֤ÿһ�������ÿһλ���ܶԽ�����Ӱ�죻����8��
��ϣ������̫���׹��죬��������ͨ��һ����ϣ����
��8����ϣ����ʵ��8����ͬ�Ĺ�ϣ����,ѡȡ��ϣ����
Ϊ���õ�Ч���Ϻõ�BKDRhash������
������ÿ����һ��Ҫ�洢�����ݣ��������ϣ���Ӽ�
����پ�����ϣ����ӳ�䵽8����ͬ����λ��������һ��
��ѯʱ������ѯ���ݾ�ͬ����ʽת�����ѯ��Ӧ����λ��
����鵽�ľ�Ϊ1�����г�ֵ�����˵�������ݴ��ڡ�
*/
#include<iostream>
#include<string>
#include<time.h>
using namespace std;
int seeds[8];//hash����
unsigned int BKDRHash(string str,int m)//BKDRhash����
{
	unsigned int seed = 131;
	unsigned int hash = 0;

	for(int i=0;i<str.length();++i)
	{
		hash = hash * seed + str[i];
	}

	return (hash & 0x7FFFFFFF) % m;//��֤���ص�λ����0��m֮��
}
string hashcaculate(int i,string s)//���ù�ϣ�������ɲ�ͬ��hash
{
	for (int j = 0; j < s.length(); ++j) s[j] = (s[j] ^ seeds[i]) % 61 + 30;//�������������µ�hash
	return s;
}
int main()
{
	int m, n;
	cin >> n;//����Ҫ�洢�����ݸ���
	m = n * 12;//���ش�����
	bool *bits = new bool[m];
	for (int i = 0; i < m; ++i) bits[i] = 0;//��ʼ��
	string current;//��ȡ���ַ����������֤��
	for(int i=0;i<8;++i)
	{
		srand(time(NULL));
		seeds[i] = rand();//����ʱ���������������
	}
	for(int i=0;i<n;++i)//����Ҫ�洢����
	{
		cin >> current;
		for(int j=0;j<8;++j)
		{
			string now = hashcaculate(j, current);//�µ�hash
			int x = BKDRHash(now, m);//hashӳ�䵽��λ�ã�0~(m-1)
			bits[x] = 1;//��1
		}
	}
	while(cin >> current)//��ѯ
	{
		if (current == "end") break;//������ѯ
		bool flag = 1;//��ʾ�Ƿ��ѯ��
		for (int j = 0; j < 8; ++j)
		{
			string now = hashcaculate(j, current);//�µ�hash
			int x = BKDRHash(now, m);//hashӳ�䵽��λ�ã�0~(m-1)
			if (!bits[x]) { flag = 0; break; }//��Ӧλ��Ϊ0��û�д洢
		}
		if (flag) cout << "true\n";
		else cout << "false\n";
	}
	
	system("pause");
	return 0;
}
