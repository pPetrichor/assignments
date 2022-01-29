#include<iostream>
using namespace std;
class minheap//��С��ʵ��
{
	int size;//����
	int *heap;//�洢���ݵ�������ʼָ��
	int currentsize;//��ǰ�洢���ݸ���
public:
	minheap()
	{

	}
	minheap(int n)//��������Ϊn����С��
	{
		size = n;
		heap = new int[size];
		currentsize = 0;
	}
	void siftup(int start)
	//��С�ѵ��ϻ������㷨
	{
		int j = start;
		int i = (j - 1) / 2, temp = heap[j];//iָʾj�ĸ����
		while (j > 0)//�ж��Ƿ��������
		{
			if (heap[i] <= temp) break;//�����С���������
			else//��Ů���С�������
			{
				heap[j] = heap[i];//����
				j = i;
				i = (i - 1) / 2;//����ִ��
			}
			heap[j] = temp;//����
		}
	}
	void insert(int x)//����С���в���Ԫ��
	{
		heap[currentsize] = x;//�ڶ�ĩ��ֵx
		siftup(currentsize);//���ϵ�������x�ŵ���ȷλ��
		currentsize++;//�洢���ݸ�����1
	}
	void siftdown(int start, int end)
	//��С�ѵ��»������㷨
	{
		int i = start;
		int j = 2 * i + 1;//jΪi������Ů
		int temp = heap[i];//��¼��ǰ���ڵ�λ�ùؼ���
		while (j <= end)//�ж��Ƿ񵽴����λ��
		{
			if (j<end&&heap[j]>heap[j + 1]) j++;//jָʾ������Ů��С��һ��
			if (temp <= heap[j]) break;//���ڵ�С�����������ֱ�ӽ���
			else { heap[i] = heap[j]; i = j; j = 2 * j + 1; }//�ؼ���С���ϸ���i��j�½�������ִ��
		}
		heap[i] = temp;//�ҵ����start���ؼ������ȷλ��i����ֵ
	}
	void remove(int &x)//ɾ����С�ѵĶѶ�Ԫ��
	{
		x = heap[0];//��¼�Ѷ�Ԫ�ص�x��
		heap[0] = heap[currentsize - 1];//���ѵ����һ��Ԫ������Ѷ�
		currentsize--;//���ݸ�����1���Ѷ�Ԫ�ر�ɾ������ʱ�������ҷ�֧��Ϊ��С��
		siftdown(0, currentsize - 1);//ִ��һ���»����������µõ���С��
	}
};
int main()
{
	int k,n; cin >> k >> n;
	minheap mh(n);
	int temp;
	for(int i=0;i<n;++i)
	{
		cin >> temp;
		mh.insert(temp);
	}
	int out;
	if (n % 2 == 0)
	{
		for (int i = 0; i < n / 2 + k - 1; ++i)
		{
			mh.remove(out);
			if (i >= n / 2 - 1 - k)
				cout << out << " ";
		}
		mh.remove(out);
		cout << out;
	}
	else
	{
		for (int i = 0; i < n / 2 + k; ++i)
		{
			mh.remove(out);
			if (i >= n / 2 - k)
				cout << out << " ";
		}
		mh.remove(out);
		cout << out;
	}
	return 0;
}