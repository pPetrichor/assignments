# align-score.py

```python
#!/usr/bin/python3
import argparse
from sexpdata import loads, Symbol

def read_align(filename):
#用于读入对齐文件信息的函数
    align = set()
    with open(filename) as f:
        for l in f:
        #打开文件迭代
            if l.startswith(' '): continue
            #以' '开头，continue
            o = l.split()
            #拆分
            if o[4] == '1':
                align.add('align-%d_%d-%d_%d' % tuple(map(int, o[:4])))
            #o[4]为'1'，将o[0-4]的内容转化为整形后增添至align集合中，可能是执行语句的顺序、次数等信息
    return align 

def read_z3(filename):
#读取z3求解后内容的信息，未用到
    align = set()
    with open(filename) as f:
        for l in f:
            l = l.strip()
            #去除首尾空格
            if l == 'true)':
                x = p.split()
                #按空格拆分
                var = x[1]
                #取第二个拆分得到的元素
                if var.startswith('align'):
                    align.add(var)
                    #以align开始，则加入
            p = l
            #l的值赋值给p
    return align


parser = argparse.ArgumentParser()
parser.add_argument('-align', required=True) # align.out
parser.add_argument('-soft', required=True) # soft
args = parser.parse_args()
#命令为align-score.py -align align.out -soft soft
#soft需要maxsat.out

align = read_align(args.align)	#读取处理align.out
# align = read_z3(args.align)
#???在makefile2中调用此文件计算apexscore与z3score，需要切换？？？

score = 0
xxx = set()
with open(args.soft) as f:
    for l in f:
        l = loads(l)
        #打开soft载入
        # print(l)
        assert l[0] == Symbol('assert-soft')
        sym = l[1]
        assert l[2] == Symbol(':weight')
        weight = l[3]
        #sym存储对齐信号信息，weight有1000、10、1的值，可能表示对齐程度？

        if isinstance(sym, Symbol):#sym是Symbol类型，则
            if sym.value() in align:#匹配到了
                # print('match')
                score += weight
                #加上weight，表示匹配得分
        else:
            assert sym[0] == Symbol('not')
            #否则就应该是not
            sym = sym[1]#sys取后面的align信息
            if sym.value() in align:
                # print('match not')
                score += weight
                #匹配到异常/错误，加分

print (score)
#输出得分






```