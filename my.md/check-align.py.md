# check-align.py

```python
#!/usr/bin/python3
import argparse
#引入argparse模块

def read_align(filename):
    align = set()
    #创建空集合
    with open(filename) as f:
        for l in f:
        #遍历，每次读取一个字符串
            if l.startswith(' '): continue
            #第一个char为空则continue
            o = l.split()
            #以空格、\t或\n分割l
            if o[4] == '1':
                align.add(tuple(map(int, o[:4])))
                #若分割后o4是‘1’，则从o[0]到o[4]，每个字符都被转换成整型后以列表/迭代器形式返回后转化为元组（之后不可更改），然后加入到align集合中（重复则不添加）
    return align 

parser = argparse.ArgumentParser()
#从命令行直接读取数据
parser.add_argument('-a1', required=True) # align.out
parser.add_argument('-a2', required=True) # align-z3.out
parser.add_argument('-o1', required=True) # a.xxx
parser.add_argument('-o2', required=True) # a.yyy
args = parser.parse_args()
#required均为true，结合注释可以得出命令行应该是这样的形式：
#check-align.py -a1 align.out -a2 align-z3.out -o1 xxx -o2 yyy……

align1 = read_align(args.a1)
align2 = read_align(args.a2)
#分别调用read_align处理align.out与align-z3.out
align0 = align1 & align2
#返回二者的交集，猜测是对齐的部分

len0 = len(align0)
len1 = len(align1)
len2 = len(align2)
#获取规模大小

# r1 = len0 / len1 if len1 != 0 else 0
# r2 = len0 / len2

# print(len0, r1 * r2 / (r1 + r2) )
# print(len1, r1)
# print(len2, r2)

align01 = set(o[0:2] for o in align0)
align02 = set(o[2:4] for o in align0)
#在二者的交集中遍历，align01存储每个数据的前3个数，align02存储2-4的数

def show_p(common, o):
#传入一个字符串common，一个集合元素o（猜测是多个数字组成）
    if o[4] != 1:
        return False
    #不是1返回错
    if o[:4] in common:
        return False
    #common包含o的0-4，则返回false
    o1 = o[0:2]
    if o1 in align01:
        return False
    o2 = o[2:4]
    if o2 in align02:
        return False
    #o的0-2不能包含在align01中，2-4不能包含在align02中
    return True

def write_remain(common, _f1, _f2):
   #猜测是输出未对齐的部分
    p = False
    a = set()
    x = set()
    y = set()
    aligns = []

    with open(_f1) as f1, open(_f2, 'w') as f2:
        #打开_f1，向_f2中写信息
        for l in f1:
            if not l.startswith(' '):
                o = tuple(map(int, l.split()))
                #条件反转，其它同read_align的操作，来处理不以' '开头的字符串
                p = show_p(common, o)
                #根据align0中的信息更新p
                o = o[:4]
                f = True
                if o[0:2] in x:
                    f = False
                if o[2:4] in y:
                    f = False
				

                if p:
                    x.add(o[0:2])
                    y.add(o[2:4])
				#如果p，说明o不在align0中，x、y加入相应部分信息
                    if f:
                        f2.write("%03d %03d %03d %03d\n" % o)
                        a.add(o)
                 #x、y中都没有存储o的相关信息,则加入到a中，作为未对齐的部分，并输出提示信息
                    else:
                        f2.write("-- %03d %03d %03d %03d\n" % o)
				#输出另外的对齐信息
            else: 
                if p: f2.write(l) #否则直接输出对应的信息
    return a


an1 = write_remain(align0, args.a1, args.o1)
an2 = write_remain(align0, args.a2, args.o2)
#调用输出为xxx、yyy，an1存储align.out的未对齐，an2存储align-z3.out的未对齐信息


ap0 = align0
ap1 = an1 | align0
ap2 = an2 | align0
#取交集

len0 = len(ap0)
len1 = len(ap1)
len2 = len(ap2)
#取长度

r1 = len0 / len1 if len1 > 0 else 1
#len1不空则r1是len0/len1，否则取1
r2 = len0 / len2 if len2 > 0 else 1
#同上

print(len0, 2* r1 * r2 / (r1 + r2) if r1 + r2 != 0 else 0)
print(len1, r1)
print(len2, r2)
#输出计算的信息
```

# check_sat.py

```python
#!/usr/bin/python3
from llvm import *
from inst import *
from expr import *
from util import *
import json
import sys
import argparse
from itertools import product, chain, count
import subprocess
import os
import z3
import time
from collections import defaultdict

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-symbA', required=True)
    parser.add_argument('-symbB', required=True)
    parser.add_argument('-inf', default='z3.out')
    parser.add_argument('-out', default='align-z3.out')
    args = parser.parse_args()
	#规定命令行输入形式：symbA、symbB后必须加相应的文件，inf、out后可以不加，默认项如上
    
    import pickle
    execA = pickle.load(open(args.symbA, 'rb'))
    execB = pickle.load(open(args.symbB, 'rb'))
    #将symbA与symbB反序列化，就是把symba.out和symbB.out的信息处理成python对象（处理成啥对象未知）


    for I, seq in zip(execA, count(1)):
     #count（1）表示从1开始迭代，zip用于将execA与这些1、2、3……对应起来
        I.seq = seq
    for I, seq in zip(execB, count(1)):
        I.seq = seq
	#同上
	#应该就是给execA与execB里面的内容编号，id记录在.seq属性里

    f = open(args.inf)
    #打开z3.out进行读取
    with open(args.out, "w") as out: 
        #向align-z3.out中输出信息
        p = ''
        aligns = []
        #初始化为空
        for l in f:
        #读取一句
            l = l.strip()
            #以空格拆分
            if l == 'true)':
            #如果拆分后是true)
                x = p.split()	#将p拆分
                var = x[1]	#取第二个
                if var.startswith('align'):
                #var以align开始
                    _, Ia, Ib = var.split('-')  
                    #将var按照字符'-'拆分，分别赋值给_、Ia、Ib
                    Ia = tuple(map(int, Ia.split('_')))
                    Ib = tuple(map(int, Ib.split('_')))
                    #按照'_'拆分Ia与Ib后，将各部分转化为整形后化为元组赋值给Ia、Ib（结合上文来看应该是只有数字了，所以拆不拆可能影响不大？）
                    Ia = execA[Ia]
                    Ib = execB[Ib]
					#取Ia、Ib对应的元素
                    
                    # print(var, type(Ia), type(Ib))

                    flag =3
                    if isinstance(Ia, STIns) and isinstance(Ib, STIns):
                        flag = 1
                    if isinstance(Ia, BRIns) and isinstance(Ib, BRIns):
                        flag = 2

                    aligns.append((Ia, Ib, flag))
                    #加入列表
                    #flag用于标记Ia、Ib的类型；默认为3；1表示指令装载指令，2表示分支指令
            p = l
            #无论if是否成立，p的值赋给l
            
        aligns.sort(key=lambda x: (x[0].seq, x[1].seq))
        #调用列表的sort函数，按照x[0].seq、x[1].seq为比较元素，进行升序排序
        for Ia, Ib, flag in aligns:
            #遍历列表元素，向align-z3.out中输出信息
            out.write("%03d %03d %03d %03d %s\n" % (Ia.vid, Ia.vii, Ib.vid, Ib.vii, flag))
            #依次输出Ia.vid, Ia.vii, Ib.vid, Ib.vii, flag；前面数据的含义还要结合z3.out分析；补齐为3位数据
            out.write('  %-80s ! %s\n' % (Ia.src, Ia.as_code() if hasattr(Ia, 'as_code') else ''))
            out.write('  %-80s ! %s\n' % (Ib.src, Ib.as_code() if hasattr(Ib, 'as_code') else ''))
            #如果Ia、Ib有'as_code'这一属性则输出相应内容，否则输出空
            
            # print('  %-80s' % Ia.src, '!', Ia.as_code() if hasattr(Ia, 'as_code') else '')
            # print('  %-80s' % Ib.src, '!', Ib.as_code() if hasattr(Ib, 'as_code') else '')





if __name__ == '__main__':
    main()


```