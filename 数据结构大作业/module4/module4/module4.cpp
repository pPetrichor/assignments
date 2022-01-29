/*
本模块要求在O(1)的时间复杂度内实现身份证号的
查找；我的思路是利用布隆过滤器来实现查找；
首先，要根据存储数据的个数确定要开多长的比特串，
以及对应的应该选择多少个哈希函数使得错误率达到
极低；通过查表我选择了m/n=12，k=8这一情况，对应
的错误率为0.00314；
因此，根据输入的n值，利用m/n=12求得m，构造一个
长度为m的比特串，并设置8个不同的哈希函数，要保
证每一个输入的每一位都能对结果造成影响；由于8个
哈希函数不太容易构造，所以这里通过一个哈希函数
与8个哈希种子实现8个不同的哈希函数,选取哈希函数
为常用的效果较好的BKDRhash函数；
这样，每输入一个要存储的数据，将其与哈希种子计
算后再经过哈希函数映射到8个不同比特位，将其置一；
查询时将待查询数据经同样方式转换后查询对应比特位；
如果查到的均为1，则有充分的理由说明该数据存在。
*/
#include<iostream>
#include<string>
#include<time.h>
using namespace std;
int seeds[8];//hash种子
unsigned int BKDRHash(string str,int m)//BKDRhash函数
{
	unsigned int seed = 131;
	unsigned int hash = 0;

	for(int i=0;i<str.length();++i)
	{
		hash = hash * seed + str[i];
	}

	return (hash & 0x7FFFFFFF) % m;//保证返回的位置在0到m之间
}
string hashcaculate(int i,string s)//利用哈希种子生成不同的hash
{
	for (int j = 0; j < s.length(); ++j) s[j] = (s[j] ^ seeds[i]) % 61 + 30;//异或操作，代替新的hash
	return s;
}
int main()
{
	int m, n;
	cin >> n;//读入要存储的数据个数
	m = n * 12;//比特串长度
	bool *bits = new bool[m];
	for (int i = 0; i < m; ++i) bits[i] = 0;//初始化
	string current;//读取的字符串——身份证号
	for(int i=0;i<8;++i)
	{
		srand(time(NULL));
		seeds[i] = rand();//根据时间生成随机数种子
	}
	for(int i=0;i<n;++i)//读入要存储数据
	{
		cin >> current;
		for(int j=0;j<8;++j)
		{
			string now = hashcaculate(j, current);//新的hash
			int x = BKDRHash(now, m);//hash映射到的位置：0~(m-1)
			bits[x] = 1;//置1
		}
	}
	while(cin >> current)//查询
	{
		if (current == "end") break;//结束查询
		bool flag = 1;//表示是否查询到
		for (int j = 0; j < 8; ++j)
		{
			string now = hashcaculate(j, current);//新的hash
			int x = BKDRHash(now, m);//hash映射到的位置：0~(m-1)
			if (!bits[x]) { flag = 0; break; }//对应位置为0，没有存储
		}
		if (flag) cout << "true\n";
		else cout << "false\n";
	}
	
	system("pause");
	return 0;
}
