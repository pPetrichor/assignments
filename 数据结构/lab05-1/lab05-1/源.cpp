#include<iostream>
#include<string>
using namespace std;
int main()
{
	string st, sp;
	getline(cin, st);
	cin >> sp;
	//st.pop_back(); //读入待匹配串与目标串
	int *next = new int[sp.length()+1];
	int j = 0, k = -1;
	next[0] = -1;
	while(j<(int)sp.length()+1)
	{
		if(k==-1||sp[j]==sp[k])
		{
			++j; ++k;
			next[j] = k;
		}
		else k = next[k];
	}//计算next函数
	int posp = 0, post = 0;
	bool ifok = 0;
	while (post < (int)st.length())
	{
		if(posp==-1||(sp[posp]==st[post]))
		{
			posp++; post++;
			if(posp >= (int)sp.length()){ cout << post - sp.length() << " "; ifok = 1; posp = next[posp];
			}
		}
		else posp = next[posp];
	}
	if (!ifok) cout << "false";
	system("pause");
	return 0;
}