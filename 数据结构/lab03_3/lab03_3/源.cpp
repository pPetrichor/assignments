#include<iostream>
using namespace std;
int main()
{
	int num;
	cin >> num;
	int max = 0;
	int *p = new int[num + 1];
	p[0] = 0;
	int x;
	for(int i=0;i<num;++i)
	{
		cin >> x;
		if (x > 8) p[i + 1] = p[i] + 1;
		else p[i + 1] = p[i] - 1;
	}
	int *a = new int[num+1];//存下标
	for (int i = 0; i < num + 1; ++i) a[i] = -1;
	int last = 0; a[last] = num;//记录最极端状况：头尾
	for(int i=num;i>=0;--i)
	{
		if (p[i] > p[a[last]]) { last++; a[last] = i; }
	}
	int current = 0;
	while(current<num+1)
	{
		while(a[0]>-1&&p[current]<p[a[last]])
		{
			int temp = a[last] - current;
			max = temp > max ? temp : max;
			a[last] = -1; last--;
		}
		current++;
	}
	cout << max;
	system("pause");
	return 0;
}