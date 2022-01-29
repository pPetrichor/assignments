#include<iostream>
using namespace std;
/*
本模块要求的任务是计算收入的最大断档。这一模块
感觉还是要排序，但注意到时间复杂度要求为O(n)，
所以直接排序肯定是不可以的。从O(n)复杂度出发，
寻找与排序相关的内容，可以找到在桶式排序中有
这样的描述：“将元素分配至各个桶中的操作的时间
复杂度为O(n)”。也就是说，在桶排序中使得时间复
杂度超出O(n)的操作就是对桶内元素的排序，排序就
意味着O(logn)的复杂度，所以要实现O(n)复杂度，关
键应该在避免桶内元素的排序；
对要求的“排序后相邻两数的差值”进行讨论：如果划
分桶个数为n，则同一个桶中数据相差不会超过这个桶
的长度：(max-min)/n；而考虑不在同一个桶中的两个
元素，如果这两个元素所在的两个桶之间还有空桶，那
这两个数的差值必定会超过一个桶的长度；所以，如果
可以保证分桶后存在至少一个空桶，那么最大断档一定
出自空桶两边不同的两个桶中数值之差；进一步考虑，
最大断档一定是后面桶中的min减去前面桶中的max，因为
这两个值是相邻的；所以这时我只需要依次遍历非空桶，
用后一个桶的min减去前一个桶的max得到断档，不断更新
至扫描完毕，这时得到的就是最大断档；
所以，我们应该保证有空桶，并且任意空桶的前后有非空桶
(如果没有那这个空桶没有任何意义)；简单的做法就是找出
数据中的max和min，各自放到最后一个桶、第一个桶；这样
就保证了任意空桶的前后有非空桶；对于有空桶，实际上只
要对n个数据分到n个桶里就可以了，因为此时不存在空桶的
情况只会是n个桶中每个桶都只有1个数(这时已完成排序)，
所以还是依次遍历非空桶，用后一个桶的min减去前一个桶
的max得到断档，不断更新至扫描完毕就可以了。但考虑到
桶号的计算为:(num - min) * n/(max - min)，其中num为
当前扫描到的数，n为数据总数，将max带入会得到max的桶
号为n，所以可以设置为(n+1)个桶，即映射了max，又保证
了一定有空桶，一举两得。
因此做法为：开3个长度为n+1的数组，分别指示每个桶：是
否为空桶、桶内最小数是多少、桶内最大数是多少；遍历数
据得到max与min，再遍历一次将数据映射到对应桶中，更新
每个桶内的最大数、最小数；最后依次遍历非空桶，用后一
个桶的min减去前一个桶的max得到断档，不断更新至扫描完
毕，这时得到的就是最大断档；
*/
int main()
{
	int n;
	cin >> n;
	int *num = new int[n];//存储要处理的数据
	long long max = -(long long)2147483649, min = (long long)2147483648;
	for(int i=0;i<n;++i)
	{
		cin >> num[i];//读入数据
		if (max < num[i]) max = num[i];//得到最大值
		if (min > num[i]) min = num[i];//得到最小值
	}
	if (max == min) { cout << 0; system("pause"); return 0; }//特殊情况：所有数据相等，断档为0
	bool *if_empty = new bool[n + 1];//记录是否为空桶
	long long *maxs = new long long[n + 1];//记录每个桶的最大值
	long long *mins = new long long[n + 1];//记录每个桶的最小值
	maxs[n] = max; mins[n] = max;//最大数单独一个桶
	for(int i=0;i<=n;++i)//初始化
	{
		if_empty[i] = 0;
		maxs[i] = -(long long)2147483649;
		mins[i] = (long long)2147483648;
	}
	for(int i=0;i<n;++i)
	{
		int x = (int)((long long)((num[i] - min) * n)/ (long long)(max - min));//计算桶号
		if_empty[x] = 1;//非空桶
		if (mins[x] > num[i]) mins[x] = num[i];
		if (maxs[x] < num[i])maxs[x] = num[i];//更新桶内最大最小值
	}
	int res = 0;//最大断档
	int lastmax = maxs[0];//lastmax记录上一个非空桶的max；第0号桶中至少有min，故是第一个非空桶
	for(int i=1;i<=n;++i)
	{
		if(if_empty[i])//遍历非空桶
		{
			if (res < mins[i] - lastmax)
				res = mins[i] - lastmax;//更新最大断档
			lastmax = maxs[i];
			
		}
	}
	cout << res;
	system("pause");
	return 0;
}