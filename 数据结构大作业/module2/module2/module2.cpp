#include<iostream>
using namespace std;
/*
本模块要求的任务是在一个动态的数据输入过程中输出
当前获取到的所有数据的中位数；要得到中位数，对数
据的排序必不可少；考虑到要求时间复杂度为O(nlog𝑛)，
易知应该从堆排序或归并排序(或锦标赛排序)中找一种
方法实现；而堆排序的附加存储较小，因此更加倾向于
堆排序；
利用堆排序的问题就是在一个堆中我们只能得到堆顶元
素，这对于中位数的取出显然不合适；因此，可以得到
将堆一分为二的想法；
本模块采取的方法是构造一个最大堆与最小堆，每个堆
都存储当前读入数据的一半；最大堆存储较小的一半数
据，最小堆存储较大的一半数据；这样，堆的堆顶元素
就是计算中位数需要的数据；
每次插入一个数据，先与最大堆堆顶元素比较，如果小于
最大堆堆顶的值，那么说明是较小的一半数据，应该插入
最大堆，反之应该插入最小堆；
由于我们要求两个堆各存一半数据，所以两个堆的数据个
数之差不能超过1，如果超过(为2)，则将数据多的那一个
堆的堆顶元素删除，并将此数据插入另一个堆；由于删的
堆顶元素为该堆最大或最小元素，因此delete后仍满足了
“最大堆存储较小的一半数据，最小堆存储较大的一半数据”
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
	int get_currentsize() { return currentsize; }//返回当前存储元素个数以调整两个堆的平衡
	int get_top() { return heap[0]; }//返回堆顶元素
};
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
			if (j<end&&heap[j]<heap[j + 1]) j++;//j指示左右子女较大的一个
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
int main()
{
	int n; cin >> n;//读入总数据个数
	minheap mymin(n / 2 + 1);//最小堆建立，容量为n/2 + 1
	maxheap mymax(n / 2 + 1);//最大堆建立，容量为n/2 + 1
	int current = 0;//存当前读入数据
	cin >> current;//读入第1个数据，由于此时两堆均空，故可以插入任意一个堆
	mymax.insert(current);//因为之后会与最大堆的堆顶元素比较故插入最大堆(当然也可以插入最小堆，每次与最小堆的堆顶元素比较)
	cout << current << endl;//只有一个数据，即为中位数
	for(int i=1;i<n;++i)//循环读入剩余数据
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
	system("pause");
	return 0;
}
