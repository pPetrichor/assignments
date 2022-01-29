#include<iostream>
using namespace std;
struct AVLnode
{
	int val;
	int bf;
	AVLnode *leftchild;
	AVLnode *rightchild;
};
class stack
{
	AVLnode *s[20];
	int top;
public:
	stack() 
	{
		for (int i = 0; i < 20; ++i) s[i] = NULL;
		top = 0;
	}
	void push(AVLnode *p)
	{
		s[top] = p;
		top++;
	}
	AVLnode *pop()
	{
		AVLnode* re;
		top--;
		re = s[top];
		return re;
	}
	bool isempty() { return top == 0; }
	AVLnode *gettop()
	{
		AVLnode* re;
		top--;
		re = s[top];
		top++;
		return re;
	}
};

void RotateL(AVLnode *&ptr)
{
	AVLnode *temp = ptr;
	ptr = temp->rightchild;
	temp->rightchild = ptr->leftchild;
	ptr->leftchild = temp;
	ptr->bf = 0; temp->bf = 0;
}
void RotateR(AVLnode *&ptr)
{
	AVLnode *temp = ptr;
	ptr = temp->leftchild;
	temp->leftchild = ptr->rightchild;
	ptr->rightchild = temp;
	ptr->bf = 0; temp->bf = 0;
}
void RotateLR(AVLnode *&ptr)
{
	AVLnode *temp_r = ptr;
	AVLnode *temp_l = temp_r->leftchild;
	ptr = temp_l->rightchild;
	temp_l->rightchild = ptr->leftchild;
	ptr->leftchild = temp_l;
	if(ptr->bf<=0)  temp_l->bf = 0;
	else temp_l->bf = -1;
	temp_r->leftchild = ptr->rightchild;
	ptr->rightchild = temp_r;
	if (ptr->bf == -1) temp_r->bf = 1;
	else temp_r->bf = 0;
	ptr->bf = 0;
}
void RotateRL(AVLnode *&ptr)
{
	AVLnode *temp_l = ptr;
	AVLnode *temp_r = temp_l->rightchild;
	ptr = temp_r->leftchild;
	temp_r->leftchild = ptr->rightchild;
	ptr->rightchild = temp_r;
	if (ptr->bf >= 0)  temp_r->bf = 0;
	else temp_r->bf = 1;
	temp_l->rightchild = ptr->leftchild;
	ptr->leftchild = temp_l;
	if (ptr->bf == 1) temp_l->bf = -1;
	else temp_l->bf = 0;
	ptr->bf = 0;
}
void Insert(AVLnode *&ptr,int el)
{
	AVLnode* pr = NULL;
	AVLnode *p = ptr;
	AVLnode *q;
	int d;
	stack st;
	while(p)
	{
		pr = p; st.push(pr);
		if (el < p->val) p = p->leftchild;
		else p = p->rightchild;
	}
	p = new AVLnode;
	p->val = el; p->leftchild = NULL; p->rightchild = NULL; p->bf = 0;
	if (pr == NULL) { ptr = p; return; }
	if (el < pr->val) pr->leftchild = p;
	else pr->rightchild = p;
	while(!st.isempty())
	{
		pr = st.pop();
		if (p == pr->leftchild) pr->bf--;
		else pr->bf++;
		if (pr->bf == 0) break;
		if(pr->bf==1||pr->bf==-1)
		{
			p = pr;
		}
		else
		{
			d = (pr->bf < 0) ? -1 : 1;
			if(p->bf==d)//同号单旋转
			{
				if (d == -1) RotateR(pr);
				else RotateL(pr);
			}
			else//双旋转
			{
				if (d == -1) RotateLR(pr);
				else RotateRL(pr);
			}
			break;
		}
	}
	if (st.isempty()) ptr = pr;
	else//中间重新连接
	{
		q = st.gettop();
		if (q->val > pr->val) q->leftchild = pr;
		else q->rightchild = pr;
	}
}
void preorder(AVLnode*r)
{
	if (r == NULL) return;
	cout << r->val << " ";
	preorder(r->leftchild);
	preorder(r->rightchild);
}
int main()
{
	int n; cin >> n;
	AVLnode* root = NULL;
	for(int i=0;i<n;++i)
	{
		int t; cin >> t;
		Insert(root, t);
	}
	preorder(root);
	system("pause");
	return 0;
}