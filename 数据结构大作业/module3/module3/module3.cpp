#include<iostream>
#include<string>
using namespace std;
/*
本模块要求的任务是在一个动态的数据输入过程中输出
当前获取到的所有数据中前k大的数；要得到前k大的数，
对数据的排序必不可少；考虑到要求时间复杂度为O(nlog𝑛)，
与module2一样，本模块中实现对数据的排序更加倾向于
堆排序；
利用堆排序的思路就很清晰了：建立大小为n的最大堆，
随着数据流的输入不断向堆中插入数据；每次查询时依
次将堆顶元素删除、调整并输出，重复k次即得到前k大
数据的有序排列；check过后再将这k个数据插回堆中以
维持最大堆状态；但是这种方法直观感觉上也会有一点
繁琐；在最大堆中，前k大数据以外的数据都是用不到的，
但我们还是将它们存储并排序；如果记查询次数为m次，
则建立最大堆复杂度为O(nlog𝑛)，每次查询时，删除k次
时间代价为klog𝑛，输出后再插入，时间代价为klog𝑛，故
查询时间代价共为2mklog𝑛，总时间代价为O(2mklog𝑛+nlog𝑛)，
并不太理想；
由于前k大数据以外的数据都是用不到的，所以考虑通过构造
一个大小为k的堆来存储动态数据流的前k大个数据；由于我们
要的是前k大个数据，所以其中最关键的元素应该为最小的元素；
如果某个新数据大于这一元素，就说明新数据是更新后前k大的
数据；因此，构建的大小为k的堆应该为最小堆；当堆中数据个
数不足k时，直接将数据插入；否则，数据需要与堆顶元素比较，
如果大于堆顶元素，则用新数据替换堆顶元素并进行调整；这样，
最小堆中保留的就总是前k大的数据了；这样，比较次数为n，插
入时间代价为nlogk，总体复杂度为O(nlogk)；查询时，要有序输
出最小堆中的所有元素，可以用空间换时间，用一个大小为k的堆
拷贝当前最小堆，将其进行堆排序即可；时间代价为O(k+klogk)
，查询m次，总时间代价为O((mk+n)logk)；(k<=n)
*/
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
		for(int i=0;i<currentsize;++i)
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
	for(int i=0;i<n.length();++i)
	{
		x *= 10;
		x += n[i] - '0';
	}
	return x;
}
int main()
{
	int n, k;
	cin >> n >> k;
	minheap myheap(k);//存储前k大数的最小堆
	minheap copy(k);//最小堆副本，用于Check
	string current;//读入每次输入
	while (cin >> current)
	{
		if (current == "end") break;//结束求top k
		else if(current!="Check")//不是查询是数据
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
	system("pause");
	return 0;
}
