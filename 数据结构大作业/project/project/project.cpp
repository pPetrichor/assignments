/*
综合所有功能为一个project
*/
#include<iostream>
#include<string>
#include<time.h>
using namespace std;

/*求平均值所要用到的结构及函数定义*/

struct node//线段树中结点结构
{
	int left;//区间左值
	int right;//区间右值
	long long val = 0;//该区间中数据和
};
void siftdown(int start, int end, int a[])//利用堆排序以nlog𝑛的复杂度对数据进行排序
//最大堆的下滑调整算法：在start的左右分支均已为最大堆的前提下，从start开始到end，自上向下比较子女与父节点的值，
//将关键码大的上浮，继续向下层比较，从而将一个数组局部调整为最大堆
{
	int i = start;
	int j = 2 * i + 1;//j为i的左子女
	int temp = a[i];//记录当前父节点位置关键码
	while (j <= end)//判断是否到达结束位置
	{
		if (j < end&&a[j] < a[j + 1]) j++;//j指示左右子女较大的一个
		if (temp >= a[j]) break;//父节点大，无需调整，直接结束
		else { a[i] = a[j]; i = j; j = 2 * j + 1; }//关键码大的上浮，i，j下降，继续执行
	}
	a[i] = temp;//找到最初start处关键码的正确位置i，赋值
}
void heapsort(int a[], int n)//n为待调整数组长度
{
	for (int i = (n - 2) / 2; i >= 0; i--) siftdown(i, n - 1, a);//将数组转化为堆，并调整成为最大堆
	for (int i = n - 1; i >= 0; i--)//对数组进行排序，得到从小到大排序数组
	{
		int temp = a[0];
		a[0] = a[i];
		a[i] = temp;//最大值位置i确定，交换元素位置
		siftdown(0, i - 1, a);//在前i个元素中重建最大堆
	}
}
void push_up(int root, node tree[])//根节点状态更新：利用已知左右子女的值求和求父结点的值
{
	tree[root].val = tree[root * 2].val + tree[root * 2 + 1].val;//父结点区间数据和 = 左子女区间数据和+右子女区间数据和
}
void build(int l, int r, int root, node tree[], int a[])//线段树建树
{
	tree[root].left = l; tree[root].right = r;
	if (l == r)//已经离散化，赋上相应区间值
	{
		tree[root].val = a[l];
		return;
	}
	int mid = (l + r) / 2;//否则，继续二分处理
	build(l, mid, root * 2, tree, a);//递归处理左分支，相应root为当前root左子女
	build(mid + 1, r, root * 2 + 1, tree, a);//递归处理左分支，相应root为当前root右子女
	push_up(root, tree);//处理完毕后求父结点对应区间数据和
}
//有了以上内容，我们就可以通过建立线段树来查找了，不过在线段树中的查找区间是[1,n]之间的，所以我们还需要
//将输入的区间转化为[1,n]中对应的一个区间；为实现log𝑛复杂度，这里使用二分查找
int findleft(int x, int l, int r, int a[])
//二分法转化区间:给定l找到其在数组a中的位置i，i满足a[i]<=x<a[i+1]
//考虑到可能有相同的数据，而左边界应该为相同数据的最左，右边界应该为相同数据的最右，故写为两个find
{
	if (x < a[l]) return l;//小于最小值，左边界为l
	if (x > a[r]) return r;//大于最大值，左边界为r
	if (l == r) return l;
	int mid = (l + r) / 2;
	if (x > a[mid]) return findleft(x, mid + 1, r, a);//在右区间
	else return findleft(x, l, mid, a);//在左区间，将等于情况归为左区间以得到相同数据的第一个位置
}
int findright(int x, int l, int r, int a[])
//二分法转化区间:给定r找到其在数组a中的位置i，i满足a[i]<=x<a[i+1]
{
	if (x < a[l]) return l - 1;//小于最小值，右边界为l-1；第一次查找不会出现此情况
	if (x > a[r]) return r;//大于最大值，右边界为r
	if (l == r) return l;
	int mid = (l + r) / 2;
	if (x >= a[mid]) return findright(x, mid + 1, r, a);//在右区间，将等于情况归为右区间以得到相同数据的最后一个位置
	else return findright(x, l, mid, a);//在左区间
}
long long mysum(int i, int l, int r, node tree[])//利用线段树,从结点i开始求区间[l,r]数据和
{
	if (l == tree[i].left&&r == tree[i].right)//查询到对应区间,直接返回数据和
		return tree[i].val;
	int mid = (tree[i].left + tree[i].right) / 2;//否则二分向下查找
	if (r <= mid) return mysum(2 * i, l, r, tree);//查找区间在左分支中
	else if (l > mid) return mysum(2 * i + 1, l, r, tree);//查找区间在右分支中
	else//查找区间分布在左右两个分支中
		return mysum(i * 2, l, mid, tree) + mysum(i * 2 + 1, mid + 1, r, tree);
}

/*求中位数以及前k大数据要用到的最大堆、最小堆类定义；求前k大数据使用转换函数定义*/

class minheap//最小堆实现
{
	int size;//容量
	int *heap;//存储数据的数组起始指针
	int currentsize;//当前存储数据个数
public:
	minheap()
	{

	}
	minheap(int n)//构造容量为n的最小堆
	{
		size = n;
		heap = new int[size];//开辟最小堆存储空间
		currentsize = 0;//初始存储0个数据
	}
	minheap& operator = (const minheap &x)//重载赋值操作符以实现拷贝
	{
		currentsize = x.currentsize;
		for (int i = 0; i < currentsize; ++i)
		{
			heap[i] = x.heap[i];
		}
		return *this;
	}
	void siftup(int start)
		//最小堆的上滑调整算法：从start开始到结点0，自下向上比较子女与父节点的值，
		//如果子女关键码小则交换子女与父结点，依次操作找到start处关键码的位置，从而重新生成最小堆
	{
		int j = start;
		int i = (j - 1) / 2, temp = heap[j];//i指示j的父结点
		while (j > 0)//判断是否调整到根
		{
			if (heap[i] <= temp) break;//父结点小，无需调整
			else//子女结点小，需调整
			{
				heap[j] = heap[i];//交换
				j = i;
				i = (i - 1) / 2;//继续执行
			}
			heap[j] = temp;//回填
		}
	}
	void insert(int x)//向最小堆中插入元素
	{
		heap[currentsize] = x;//在堆末赋值x
		siftup(currentsize);//向上调整，将x放到正确位置
		currentsize++;//存储数据个数加1
	}
	void siftdown(int start, int end)
		//最小堆的下滑调整算法：在start的左右分支均已为最小堆的前提下，从start开始到end，自上向下比较子女与父节点的值，
		//将关键码小的上浮，继续向下层比较，从而将一个数组局部调整为最小堆
	{
		int i = start;
		int j = 2 * i + 1;//j为i的左子女
		int temp = heap[i];//记录当前父节点位置关键码
		while (j <= end)//判断是否到达结束位置
		{
			if (j<end&&heap[j]>heap[j + 1]) j++;//j指示左右子女较小的一个
			if (temp <= heap[j]) break;//父节点小，无需调整，直接结束
			else { heap[i] = heap[j]; i = j; j = 2 * j + 1; }//关键码小的上浮，i，j下降，继续执行
		}
		heap[i] = temp;//找到最初start处关键码的正确位置i，赋值
	}
	void remove(int &x)//删除最小堆的堆顶元素
	{
		x = heap[0];//记录堆顶元素到x中
		heap[0] = heap[currentsize - 1];//将堆的最后一个元素填补到堆顶
		currentsize--;//数据个数减1，堆顶元素被删除，此时根的左右分支仍为最小堆
		siftdown(0, currentsize - 1);//执行一次下滑调整，重新得到最小堆
	}
	void change(int x)//替换最小堆的堆顶元素并调整
	{
		heap[0] = x;//用新的数据替代堆顶元素
		siftdown(0, currentsize - 1);//执行一次下滑调整，重新得到最小堆
	}
	void sort()//堆排序，用以在查询时从大到小输出
	{
		for (int i = currentsize - 1; i >= 0; i--)
		{
			int temp = heap[i];
			heap[i] = heap[0];
			heap[0] = temp;//交换heap[0]与heap[i]，使得最小元素被放在后面
			siftdown(0, i - 1);//调整前i个元素，重新建立局部最小堆
		}
	}
	void print()//输出数据
	{
		for (int i = 0; i < currentsize; ++i) cout << heap[i] << " ";
		cout << endl;
	}
	int get_currentsize() { return currentsize; }//返回当前存储元素个数以判断直接插入还是与堆顶比较
	int get_top() { return heap[0]; }//返回堆顶元素
};
int s_to_n(string n)//将string转化为对应数字?时间复杂度
{
	int x = 0;
	for (int i = 0; i < n.length(); ++i)
	{
		x *= 10;
		x += n[i] - '0';
	}
	return x;
}
class maxheap//最大堆实现
{
	int size;//容量
	int *heap;//存储数据的数组起始指针
	int currentsize;//当前存储数据个数
public:
	maxheap()
	{

	}
	maxheap(int n)//构造容量为n的最大堆
	{
		size = n;
		heap = new int[size];//开辟最小堆存储空间
		currentsize = 0;//初始存储0个数据
	}
	void siftup(int start)
		//最大堆的上滑调整算法：从start开始到结点0，自下向上比较子女与父节点的值，
		//如果子女关键码大则交换子女与父结点，依次操作找到start处关键码的位置，从而重新生成最大堆
	{
		int j = start;
		int i = (j - 1) / 2, temp = heap[j];//i指示j的父结点
		while (j > 0)//判断是否调整到根
		{
			if (heap[i] >= temp) break;//父结点大，无需调整
			else//子女结点大，需调整
			{
				heap[j] = heap[i];//交换
				j = i;
				i = (i - 1) / 2;//继续执行
			}
			heap[j] = temp;//回填
		}
	}
	void insert(int x)//向最大堆中插入元素
	{
		heap[currentsize] = x;//在堆末赋值x
		siftup(currentsize);//向上调整，将x放到正确位置
		currentsize++;//存储数据个数加1
	}
	void siftdown(int start, int end)
		//最大堆的下滑调整算法：在start的左右分支均已为最大堆的前提下，从start开始到end，自上向下比较子女与父节点的值，
		//将关键码大的上浮，继续向下层比较，从而将一个数组局部调整为最大堆
	{
		int i = start;
		int j = 2 * i + 1;//j为i的左子女
		int temp = heap[i];//记录当前父节点位置关键码
		while (j <= end)//判断是否到达结束位置
		{
			if (j < end&&heap[j] < heap[j + 1]) j++;//j指示左右子女较大的一个
			if (temp >= heap[j]) break;//父节点大，无需调整，直接结束
			else { heap[i] = heap[j]; i = j; j = 2 * j + 1; }//关键码大的上浮，i，j下降，继续执行
		}
		heap[i] = temp;//找到最初start处关键码的正确位置i，赋值
	}
	void remove(int &x)//删除最大堆的堆顶元素
	{
		x = heap[0];//记录堆顶元素到x中
		heap[0] = heap[currentsize - 1];//将堆的最后一个元素填补到堆顶
		currentsize--;//数据个数减1，堆顶元素被删除，此时根的左右分支仍为最大堆
		siftdown(0, currentsize - 1);//执行一次下滑调整，重新得到最大堆
	}
	int get_currentsize() { return currentsize; }//返回当前存储元素个数以调整两个堆的平衡
	int get_top() { return heap[0]; }//返回堆顶元素
};

/*查找身份证号要使用到的结构及函数定义*/

int seeds[8];//hash种子
unsigned int BKDRHash(string str, int m)//BKDRhash函数
{
	unsigned int seed = 131;
	unsigned int hash = 0;

	for (int i = 0; i < str.length(); ++i)
	{
		hash = hash * seed + str[i];
	}

	return (hash & 0x7FFFFFFF) % m;//保证返回的位置在0到m之间
}
string hashcaculate(int i, string s)//利用哈希种子生成不同的hash
{
	for (int j = 0; j < s.length(); ++j) s[j] = (s[j] ^ seeds[i]) % 61 + 30;//异或操作，代替新的hash
	return s;
}

/*主函数：实现流程控制*/
int main()
{
	int choose;
	printf("input -1 to end ;input a number to choose what to be caculated (1 - 5):");
	scanf("%d",&choose);//选择要使用的功能
	while(choose != -1)
	{
		switch (choose)
		{
		case 1://计算平均数
		{
			printf("current work:caculate the average of a certain region\n");
			int n, m;
			scanf("%d", &n);
			while (n != -1)
			{
				scanf("%d", &m);
				node *tree = new node[4 * n + 1];//创建线段树，最大空间为4n
				int *a = new int[n + 1];//存储数据
				for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
				heapsort(a + 1, n);//排序
				build(1, n, 1, tree, a);
				int l, r;//定义要查询区间
				for (int i = 0; i < m; ++i)
				{
					scanf("%d", &l);
					scanf("%d", &r);
					if (l > r) { int temp = r; r = l; l = temp; }//取相应区间
					if (r < a[1]) printf("0\n");
					else if (l > a[n]) printf("0\n");//不在查询区间中的两种情况，均返回0
					else
					{
						int fl = findleft(l, 1, n, a);
						int fr = findright(r, 1, n, a);//转化查找区间
						if (fr < fl)  printf("0\n");//该区间内无数据
						else
						{
							long long sum = mysum(1, fl, fr, tree);//得到相应数据和
							printf("%d\n", sum / (fr - fl + 1));//输出平均值
						}
					}
				}
				printf("current work:caculate the average of a certain region\n");
				scanf("%d", &n);
			}
			break;
		}
		case 2:
		{
			printf("current work:caculate median\n");
			int n; cin >> n;//读入总数据个数
			while (n != -1)
			{
				minheap mymin(n / 2 + 1);//最小堆建立，容量为n/2 + 1
				maxheap mymax(n / 2 + 1);//最大堆建立，容量为n/2 + 1
				int current = 0;//存当前读入数据
				cin >> current;//读入第1个数据，由于此时两堆均空，故可以插入任意一个堆
				mymax.insert(current);//因为之后会与最大堆的堆顶元素比较故插入最大堆(当然也可以插入最小堆，每次与最小堆的堆顶元素比较)
				cout << current << endl;//只有一个数据，即为中位数
				for (int i = 1; i < n; ++i)//循环读入剩余数据
				{
					cin >> current;
					if (current < mymax.get_top()) mymax.insert(current);//小于最大堆堆顶的值，是较小的一半数据，应该插入最大堆
					else mymin.insert(current);//否则插入最小堆
					int gap = mymax.get_currentsize() - mymin.get_currentsize();//记录两堆元素个数差
					if (gap > 1)//两堆元素个数不平衡，最大堆数据较多
					{
						int x = 0;//存储最大堆堆顶元素
						mymax.remove(x);
						mymin.insert(x);//调整两堆元素个数，调整后两堆元素个数相同，中位数为两堆顶元素平均值
						cout << (mymax.get_top() + mymin.get_top()) / 2 << endl;
					}
					else if (gap == 1)//两堆元素个数平衡，最大堆数据较多，中位数存储在最大堆的堆顶元素
						cout << mymax.get_top() << endl;
					else if (gap == 0)//两堆元素个数相同，中位数为两堆顶元素平均值
						cout << (mymax.get_top() + mymin.get_top()) / 2 << endl;
					else if (gap == -1)//两堆元素个数平衡，最小堆数据较多，中位数存储在最小堆的堆顶元素
						cout << mymin.get_top() << endl;
					else//两堆元素个数不平衡，最小堆数据较多
					{
						int x = 0;//存储最小堆堆顶元素
						mymin.remove(x);
						mymax.insert(x);//调整两堆元素个数，调整后两堆元素个数相同，中位数为两堆顶元素平均值
						cout << (mymax.get_top() + mymin.get_top()) / 2 << endl;
					}
				}
				printf("current work:caculate median\n");
				cin >> n;
			}
			break;
		}
		case 3:
		{
			printf("current work:caculate top k\n");
			int n, k; cin >> n;
			while (n != -1)
			{
				cin >> k;
				minheap myheap(k);//存储前k大数的最小堆
				minheap copy(k);//最小堆副本，用于Check
				string current;//读入每次输入
				while (cin >> current)
				{
					if (current == "end") break;//结束求top k
					else if (current != "Check")//不是查询是数据
					{
						int now = s_to_n(current);
						if (myheap.get_currentsize() < k) myheap.insert(now);//堆未插满，直接插入
						else//否则与堆顶元素比较
						{
							if (now > myheap.get_top()) myheap.change(now);//大于堆顶元素则替换当前堆顶元素，否则什么也不做
						}
					}
					else//查询
					{
						copy = myheap;//拷贝
						copy.sort();//堆排序，使copy内部按从大到小排列
						copy.print();//输出查询结果
					}
				}
				printf("current work:caculate top k\n");
				cin >> n;
			}
			break;
		}
		case 4://只需要输入要存多少数据，之后输入数据全部按查询处理
		{
			printf("current work:find\n");
			int m, n;
			cin >> n;//读入要存储的数据个数
			while (n != -1)
			{
				m = n * 12;//比特串长度
				bool *bits = new bool[m];
				for (int i = 0; i < m; ++i) bits[i] = 0;//初始化
				string current;//读取的字符串——身份证号
				for (int i = 0; i < 8; ++i)
				{
					srand(time(NULL));
					seeds[i] = rand();//根据时间生成随机数种子
				}
				for (int i = 0; i < n; ++i)//读入要存储数据
				{
					cin >> current;
					for (int j = 0; j < 8; ++j)
					{
						string now = hashcaculate(j, current);//新的hash
						int x = BKDRHash(now, m);//hash映射到的位置：0~(m-1)
						bits[x] = 1;//置1
					}
				}
				while (cin >> current)//查询
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
				printf("current work:find\n");
				cin >> n;
			}
			break;
		}
		case 5:
		{
			printf("current work:caculate max gap\n");
			int n;
			cin >> n;
			while (n != -1)
			{
				int *num = new int[n];//存储要处理的数据
				long long max = -(long long)2147483649, min = (long long)2147483648;
				for (int i = 0; i < n; ++i)
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
				for (int i = 0; i <= n; ++i)//初始化
				{
					if_empty[i] = 0;
					maxs[i] = -(long long)2147483649;
					mins[i] = (long long)2147483648;
				}
				for (int i = 0; i < n; ++i)
				{
					int x = (int)((long long)((num[i] - min) * n) / (long long)(max - min));//计算桶号
					if_empty[x] = 1;//非空桶
					if (mins[x] > num[i]) mins[x] = num[i];
					if (maxs[x] < num[i])maxs[x] = num[i];//更新桶内最大最小值
				}
				int res = 0;//最大断档
				int lastmax = maxs[0];//lastmax记录上一个非空桶的max；第0号桶中至少有min，故是第一个非空桶
				for (int i = 1; i <= n; ++i)
				{
					if (if_empty[i])//遍历非空桶
					{
						if (res < mins[i] - lastmax)
							res = mins[i] - lastmax;//更新最大断档
						lastmax = maxs[i];

					}
				}
				cout << res << endl;
				printf("current work:caculate max gap\n");
				cin >> n;
			}
			break;
		}
		}
		printf("input -1 to end ;input a number to choose what to be caculated (1 - 5):");
		cin >> choose;
	}
	system("pause");
	return 0;
}
