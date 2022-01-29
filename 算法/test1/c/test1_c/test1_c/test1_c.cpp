#include<iostream>
#include<unordered_map>
using namespace std;
int main()
{
	unordered_map <int, int> hash_;
	int n, target, temp;
	cin >> n;
	int *vals = new int[n];
	for (int i = 0; i < n; ++i)
	{
		cin >> temp;
		vals[i] = temp;
		hash_[temp] = i;
	}
	int not_exist = temp + 1;//�����Ĵ󣬱ز�����
	cin >> target;
	for (int i = 0; i < n; ++i)
	{
		int need = target - vals[i];
		if (vals[i] != not_exist && hash_.find(need) != hash_.end() && hash_[need] != i)//���ڴ�Ԫ����a��=b
		{
			cout << vals[i] << " " << need << endl;
			vals[hash_[need]] = not_exist;//�����Ӧ��ֵ����ʹ��
		}
	}
	return 0;
}