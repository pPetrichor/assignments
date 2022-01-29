# model.py

```python
from inst import *
from util import _list
#就是def _list(*args):	return list(args)
from itertools import takewhile
from functools import reduce
import operator
import itertools

__all__ = ['run']
#建立列表，放入元素run
__functions__ = {}
#建立空字典

input_seq = 0
#记录输入的序号，初始为0

class SymbExecException(Exception):
    pass
#继承python常规异常，以支持try，expect语句


_count = itertools.count
#_count成为一个从0开始的整数序列，无穷长
def count(x):
    return _count(int(x))
#将x转化为10进制整数后返回以其为下标的数字，就是int(x)?

def register(func):
    __functions__[func.__name__] = func
    return func
#注册函数，将键对(func.__name__,func)放入__functions__中

def initialize(_execute, _makeval, _mem):
    global execute, makeval, mem
    # execute = symbexec.execute
    # makeval = symbexec.makeval
    execute = _execute
    makeval = _makeval
    mem = _mem
#execute、makeval、mem这几个全局变量/函数被初始化到地址_execute, _makeval, _mem处


def set_argument(args):
    from itertools import count
    global input_variables
    input_variables = [(addr, Input(seq, value)) \
                       for (addr, value), seq in zip(args, count(1))]
#更改全局变量input_variables为新的列表，其值为args中的addr、value，并编号（seq）

def run(function, instr, value, args):
    if function in __functions__:
        __functions__[function](instr, args, value)
    else:
        print ('default', function)
        execute(OPIns, instr, expr=makeval(value))
#如果function在已有的集合内则执行（参数为指令、参数、值），否则调用execute函数（在symexec中定义）


def as_list(f):
    from functools import wraps
    @wraps(f)
    def wrapped(*args, **kwargs):
        return list(f(*args, **kwargs))
    return wrapped
#在不改变f函数原有的功能代码的基础上，增添一项将函数返回值以list形式返回的功能

def as_str(f):
    from functools import wraps
    @wraps(f)
    def wrapped(*args, **kwargs):
        return ''.join(f(*args, **kwargs))
    return wrapped
#在不改变f函数原有的功能代码的基础上，增添一项将函数返回值以字符串形式返回的功能

def readmems(ptr, size=None):
#从ptr处读取size个数据
    global mem
    # mem = symbexec.mem
    if size is None:
        r = count(int(ptr))
        #size为空，r为ptr转化为10进制整型的值
    else:
        r = range(int(ptr), int(ptr) + int(size))
        #否则r为一个整数列表，内容为ptr-ptr+size

    for p, i in zip(r, count(0)):
    #迭代，将ptr-ptr+size与0-size对应起来，用p、i指示
        m = mem[p]
        #取p处的mem内容
        if m is None:
        #m为空
            try:
            #尝试执行
                e3 = ptr.expr.e3
                if e3.fn == 'ptr' and e3.op[1] == 0 and e3.op[2] == 0:
                    ptr = e3.op[0].term
                #取ptr中的相应信息，具体含义要结合调用者看
            except AttributeError:
                pass
            #若try过程中发生了AttributeError异常，即ptr对象没有某个属性，跳转到此pass
            
            raise SymbExecException(
                "Use uninitialized value",
                "%s[%s] is not initialized" % (ptr.as_code(), i),
                "This may happen because you are missing terminating null character '\\0'")
            #由于m为空，所以必然触发此异常，输出相应报错
        yield m
        #打印出m，但调用readmems不会直接输出，而是会将返回结果作为一个可迭代对象（，使用.next函数可获得一次输出，即在上述的for中得到多个m是调一次next执行一次循环）
        

@as_list
#cstr返回值再经过as_list函数处理后返回，增加一个转化为list的功能
def cstr(ptr, size=None, ending=False):
    for m in readmems(ptr, size):
        #遍历readmems中得到的所有元素
        if int(m) == 0:
            if ending:
                yield(m)
                #如果m为0且是ending，只yield一次；若m为0但ending为false，field两次；此处0指的可能是空字符null
            return
        yield(m)
        #m整型不为0，输出为可迭代对象
		#yield加括号???

@as_str
#返回值经as_str处理后再返回，增加一个转化为str的功能
def _str(s):
    for c in s:
    #遍历s中的所有字符
        c = int(c)#转化为整型
        if c == 10:
            yield('\\n')
            #c为10，可迭代对象中加入'\\n'
        else:
            yield(chr(c))
            #否则将此字符转回char型加入


class PArg:
    def __init__(self, type, idx):
        self.type = type
        self.idx = idx
#定义PArg类，该类有type与idx属性

def _printf(fmt):
    i = 0
    f = False
    for c in fmt:
    #遍历fmt中的元素
        if f:
            #f=true
            if int(c) == ord('%'):
                yield c
             #如果c的整型是'%'的ASCII码，则加入可迭代对象中
            else:
                yield PArg(chr(int(c)), i)
                #否则加入以该字符的int值为type，i为idx的类对象
                i += 1
                f = False
                #id加一，置false
        elif int(c) == ord('%'):
            f = True
            #否则等到c的整型是'%'的ASCII码时再执行


@register
#生成后注册到__functions__中
def printf(instr, args, value):
    fmt = cstr(args[0])
    #fmt为args的起始元素经cstr返回的可迭代对象
    arge, argv = [fmt], [_str(fmt)]
    #arge为该可迭代对象形成的list，argv则是经_str处理fmt后的list
    
    for c in _printf(fmt):
        #调用_printf处理后迭代
        if isinstance(c, PArg):
            #是PArg类型，对应加入时f为true且int(c) ！= ord('%')的情况
            a = args[c.idx + 1]
            #a取args中的idx+1项
            if c.type == 'd':
                arge.append(a)
                argv.append(int(a))
             #加入时%后跟的是d，则加入的是整型
            elif c.type == 's':
                s = cstr(a)
                arge.append(s)
                argv.append(_str(s))
              #%后是s，加入的是cstr处理后的值
   
    execute(PRINT, instr, args=args, arge=arge, argv=argv)
    #调用execute，应该是调用标准输出，args仍然为指示输出内容、格式、值的参数，arge是args中类型为PArg（也就是加入时f为true且int(c) ！= ord('%')的情况下）时的元素（也就是%d或%s后一个位置的内容，应该是数据值？），argv则是这些元素经cstr处理后的内容，具体差别就是不是\0结束符的话，空字符会多加入一次（？？分开得更明显一点？），且argv是一个可迭代对象


@register
#生成后注册到__functions__中，内容是空语句，不做事
def INTERNAL_ARRAY(instr, args, value):
    pass


@register
#生成后注册到__functions__中，不做事
def INTERNAL_VARIABLE(instr, args, value):
    pass


def _pow(x, y):
    _x, _y = signed(x), signed(y)#转化为带符号数
    if _y < 0:
        desc = ["The second argument of pow() is negative"]
        if isinstance(y, Instr):
            desc.append(y.var_values())
        #如果y还是Instr类型的，在报错后加上一个值信息
        raise SymbExecException(*desc)
        #报错
    elif _y == 0:
        return 1
    elif _y == 1:
        return x
    else:
        return ['*'] + ([x] * _y)#没看懂为啥要这样，返回一个['*',x,x,x,……,x]
    #返回x^y


def _atoi(ptr, base):
#看名字是把一个字符串转化为整数
    def digit(d):
    #将一个合适的字符转化为整数
        v = int(d)
        if 48 <= v <= 57:
            return ['-', d, 48], v - 48
        #'0'-'9'转化为0-9
        if 65 <= v <= 90:
            return ['-', d, 55], v - 55
        #'A'-'Z'转化为10-35
        if 97 <= v <= 122:
            return ['-', d, 87], v - 87
        #'a'-'z'转化为10-35
        return None
    	#否则返回空

    s = cstr(ptr)
    #取出mem从ptr处开始的内容，由于未传size，取默认值none，故只取出来一个元素
    b = int(base)
    #base转化为整型
    negative = False
    if int(s[0]) == 45: # -，即取出的s起始元素为'-'
        negative = True
        s = s[1:]
        #转化nedative，s取除去'-'的内容
    s = list(takewhile(operator.truth, map(digit, s)))
    #调用digit[s[i]]，返回如果不为空则加入到要返回的list，否则不加入
    s.reverse()
    #将list逆序排列

    vexpr = [['*', d, _pow(base, r)] for (d, _), r in zip(s, count(0))]
    #猜测r是从0开始的编号，d是s的起始元素表示d或s，vexpr是元素为['*',d/s,_pow(base,r)]的list？？逆序了
    value = [d * (b ** r) for (_, d), r in zip(s, count(0))]
	#r是编号，d是值，
    !!!!!重新结合实例看
    
    
    if not vexpr:
        vexpr = 0
    elif len(vexpr) > 1:
        vexpr = ['+'] + vexpr
    else:
        vexpr = vexpr[0]
        #褪去一层list

    if value:
        value = reduce(operator.add, value)
        #将value中的所有值加起来赋值给value
    else:
        value = 0

    if negative:
        vexpr = ['*', -1, vexpr]
        value *= -1

    return vexpr, value, len(s)


@register
#生成后注册到__functions__中
def atoi(instr, args, value):
    vexpr, _, _ = _atoi(args[0], 10)
    execute(FNIns, instr, expr=makeval(vexpr, value),
            fname='atoi', fargs=args)
#利用_atoi完成实际的转换功能

def _strlen(ptr):
    return len(cstr(ptr))
#求从ptr处的mem元素的长度

def _strcpy(instr, dst, src, size=None):
    global mem
    # mem = symbexec.mem
    if size is None:
    #只取了mem中的一个元素
        l = _strlen(src)
    else:
        l = min(_strlen(src), size)
    #取较小值
    if int(src) + l >= int(dst) and int(dst) + l >= int(src):
        #？？？可能走到这里吗
        raise SymbExecException("source overlaps destination")
    
    for d, s in zip(count(dst), cstr(src, size, True)):
        execute(STIns, instr, expr=makeval(s), addr=makeval(d))
        # mem[d] = s
        #否则将src处拷贝到d处？？？


@register
#生成后注册到__functions__中
def strlen(instr, args, value):
    execute(FNIns, instr, expr=makeval(value),
            fname="strlen", fargs=args)
#计算args的长度，指定函数名为strlen，执行并存储指令
        
@register
#生成后注册到__functions__中
def strdup(instr, args, value):
    _strcpy(instr, value, args[0])
    execute(OPIns, instr, expr=makeval(value))
#新建一个地址（与value有关），将args拷贝到里面

@register
#生成后注册到__functions__中
def strcat(instr, args, value):
    dst, src = args
    off = len(cstr(dst))
    _strcpy(instr, int(dst) + off, src)
    execute(OPIns, instr, expr=makeval(args[0], value))
#将args前后部分拼接在一起，调用makeval返回args[0]，即字符串开头

@register
#生成后注册到__functions__中
def strcpy(instr, args, value):
    dst, src = args
    _strcpy(instr, dst, src)
    execute(OPIns, instr, expr=makeval(args[0], value))
#执行strcpy并记录，返回copy后的字符串起始位置

@register
#生成后注册到__functions__中
def strchr(instr, args, value):
    string, char = args
    char = int(char)
    for c, p in zip(cstr(string), count(0)):
        if int(c) == char:
            execute(FNIns, instr, expr=makeval(['+', string, p], value),
                    fname='strchr', fargs=args)
            #匹配到，调用execute，记录并返回匹配位置
            return
    else:
        execute(FNIns, instr, expr=makeval(0, value),
                fname='strchr', fargs=args)
        	#未匹配到，记录，返回空
#执行并记录函数，用于检测args中char第一次出现在string中的位置
        

@register
#生成后注册到__functions__中
def strrchr(instr, args, value):
    string, char = args
    char = int(char)
    for c, p in reversed(list(zip(cstr(string), count(0)))):
        if int(c) == char:
            execute(OPIns, instr, expr=makeval(['+', string, p], value))
            return
    else:
        execute(OPIns, instr, expr=makeval(0, value))
#函数内容与strchr一样，只是调用了一次reversed，所以得到的是从string尾开始查找遇到的第一个char的位置


@register
#生成后注册到__functions__中
def strcmp(instr, args, value):
    execute(FNIns, instr, expr=makeval(value),
            fname="strcmp", fargs=args)
#调用strcmp函数并记录

@register
#生成后注册到__functions__中
def strtol(instr, args, value):
    vexpr, _, endp = _atoi(args[0], args[2])

    if int(args[1]) != 0:
        execute(STIns, instr,
                expr=makeval(['+', args[0], endp], int(args[0]) + endp),
                addr=makeval(args[1]))

    execute(OPIns, instr, expr=makeval(vexpr, value))
#将args[1]的内容转化为长整数类型返回？？？

@register
#生成后注册到__functions__中
def strncat(instr, args, value):
    dst, src, size = args
    off = len(cstr(dst, size))
    _strcpy(instr, int(dst) + off, src, int(size) - off)
    execute(OPIns, instr, expr=makeval(args[0], value))
#用于在args里指示的dst后边追加至多size个字符，加的内容由src指定


def _memcpy(dst, src, size):
    global mem
    # mem = symbexec.mem
    for d, s in zip(count(int(dst)), readmems(src, int(size))):
        mem[d] = s
#将mem中从dst开始的地方拷贝为从src开始指示的内容，长度为size
        
def _memset(dst, c, size):
    global mem
    # mem = symbexec.mem
    dst, size = int(dst), int(size)
    for d in range(dst, dst + size):
        mem[d] = c
# 将mem从d-d+size的内容换为c

@register
#生成后注册到__functions__中
def memcpy(instr, args, value):
    dst, src, size = args
    _memcpy(dst, src, size)
    execute(OPIns, instr, expr=makeval(args[0], value))


@register
#生成后注册到__functions__中
def memset(instr, args, value):
    dst, c, size = args
    _memset(dst, c, size)
    execute(OPIns, instr, expr=makeval(args[0], value))
#调用_memcpy与_memset，参数从args获取，执行并记录

@register
#生成后注册到__functions__中
def llvm_memset(instr, args, value):
    dst, c, size, _, _ = args
    _memset(dst, c, size)


@register
#生成后注册到__functions__中
def llvm_memcpy(instr, args, value):
    dst, src, size, _, _ = args
    _memcpy(dst, src, size)
#功能一致，只是args的内容有所改变和增加，但不影响执行，没有记录

@register
#生成后注册到__functions__中
def sscanf(instr, args, value):
    #格式化解析字符串后读入信息
    fmtstr = _str(cstr(args[1]))
    #取args[1]指示的内容
    if fmtstr == '%d':#表示读入整数
        vexpr, value, _ = _atoi(args[0], 10)
        #转化args[0]为十进制整数
        execute(STIns, instr, expr=makeval(vexpr, value),
                addr=makeval(args[2]))
        #执行，返回值为vexpr指示内容（提示信息？），读取后要存储到的地址由args[2]指出
    else:
        assert False, fmtstr
		#只读整数

@register
#生成后注册到__functions__中
def DESCRIPTION(instr, args, value):
    descstr = _str(cstr(args[0]))
    symbexec.desc = descstr


@register
#生成后注册到__functions__中
def END_DESCRIPTION(instr, args, value):
    symbexec.desc = None
#更新symbexec.desc为args[0]指示的字符串化后的内容（描述信息？）

def _int2str(n, b, symbols='0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'):
        return (_int2str(n//b（n/b的整数部分）, b, symbols) if n >= b else "") + symbols[n%b] 
#递归函数，将整型数n转化为b进制的数，并用字符串解释

def _itoa(instr, ptr, val, n, radix, lower=False):
#整数值转化为字符串，n是要转化的数，radix是进制，lower表示要不要小写化；结果中会添加一些%符号，执行并记录，最后返回字符串的长度
    i = 0
    if n < 0:
        execute(STIns, instr,
                expr=makeval(ord('-')),
                addr=makeval(['ptr', ptr, i], int(ptr) + i))
        #负数前面加个-
        n = -n
        #取负
        i += 1

    s = _int2str(n, radix)
    #n转化为radix进制下的字符串
    for l, c in reversed(list(zip(count(0), reversed(s)))):
        #字符串字符从后向前编号
        if lower:
            c = c.lower()
        #小写化
        x = (n // (radix ** l)) % radix
        e = ['%', ['/', val, radix ** l], radix]
        if x < 10:
            x += ord('0')
            e = ['+', e, ord('0')]
        elif lower:
            x += ord('a') - 10
            e = ['+', e, ord('a') - 10]
        else:
            x += ord('A') - 10
            e = ['+', e, ord('A') - 10]

        assert x == ord(c), (x, c)
        execute(STIns, instr,
                expr=makeval(e, ord(c)),
                addr=makeval(['ptr', ptr, i], int(ptr) + i))
        i += 1
    return i


@register
#生成后注册到__functions__中
def sprintf(instr, args, value):
#将格式化数据输入一个字符串中？？不是很理解“格式化数据”
    global mem
    # mem = symbexec.mem
    dst = args[0]#取args[0]，应该是要写到的字符串的起始地址
    fmt = cstr(args[1])
    #取出字符串（所谓的格式化数据？）
    idx = 0
    #标记id
    for c in _printf(fmt):
        #迭代遍历
        if isinstance(c, PArg):
            #是PArg类型
            a = args[c.idx + 2]
            #取后面2位的数据，应该是因为args[0]多了个dest元素
            #有d、x、X、s、c几种数据类型
            if c.type == 'd':
                off = _itoa(instr, dst, a, signed(a), 10)
                idx += off
                
            elif c.type == 'x':
                off = _itoa(instr, dst, a, signed(a), 16, lower=True)
                idx += off

            elif c.type == 'X':
                off = _itoa(instr, dst, a, signed(a), 16)
                idx += off
			#？？？对于数值型数据，转化为字符串加到idx后面？？？
            elif c.type == 's':
                for src in cstr(a):
                    execute(STIns, instr,
                            expr=makeval(src),
                            addr=makeval(['ptr', dst, idx], int(dst) + idx))
                    idx += 1

            elif c.type == 'c':
                execute(STIns, instr,
                        expr=makeval(a),
                        addr=makeval(['ptr', dst, idx], int(dst) + idx))
                idx += 1
             #字符类型数据则在ptr所指的地址返回相应字符（串）值
        else:
            execute(STIns, instr,
                    expr=makeval(c),
                    addr=makeval(['ptr', dst, idx], int(dst) + idx))
            idx += 1
		#否则直接记录
    execute(STIns, instr,
            expr=makeval(0),
            addr=makeval(['ptr', dst, idx], int(dst) + idx))
    execute(OPIns, instr, expr=makeval(value))
	#加结束符？？？

@register
#生成后注册到__functions__中
def isalpha(instr, args, value):
    execute(FNIns, instr, expr=makeval(value),
            fname='isalpha', fargs=args)
#注册一个判断是否是字母的函数但没有实现

@register
#生成后注册到__functions__中
def isdigit(instr, args, value):
    execute(FNIns, instr, expr=makeval(value),
            fname='isdigit', fargs=args)
#注册一个判断是否是十进制数的函数但没有实现

@register
#生成后注册到__functions__中
def isspace(instr, args, value):
    execute(FNIns, instr, expr=makeval(value),
            fname='isspace', fargs=args)
#注册一个判断是否是空格的函数但没有实现

@register
#生成后注册到__functions__中
def toupper(instr, args, value):
    c = args[0]
    v = int(c)
    if 97 <= v <= 122:
        vexpr = ['-', c, 32]
    else:
        vexpr = c
    execute(OPIns, instr, expr=makeval(vexpr, value))
#将args[0]的字符作大写处理（为小写字符则处理，否则不处理），没有改变值，增加了描述['-', c, 32]        

@register
#生成后注册到__functions__中
def tolower(instr, args, value):
    c = args[0]
    v = int(c)
    if 65 <= v <= 90:
        vexpr = ['+', c, 32]
    else:
        vexpr = c
    execute(OPIns, instr, expr=makeval(vexpr, value))
#将args[0]的字符作小写处理（为大写字符则处理，否则不处理），没有改变值，增加了描述['+', c, 32]  

@register
#生成后注册到__functions__中
def abs(instr, args, value):
    v = args[0]
    if signed(v) >= 0:
        execute(FNIns, instr, expr=makeval(v, value),
                fname='abs', fargs=args)
    else:
        execute(FNIns, instr, expr=makeval(['-', 0, v], value),
                fname='abs', fargs=args)
#求绝对值，改变就是增加一个最开始的描述为['-', 0, v]
###猜测args[0]就是一个计算式，利用int等可计算出结果，execute用于记录

@register
#生成后注册到__functions__中
def pow(instr, args, value):
    vexpr = _pow(args[0], args[1])
    execute(FNIns, instr, expr=makeval(vexpr, value),
            fname='pow', fargs=args)
#印证了上述猜测

def signed(v):
    v = int(v)
    if v <= 0x7fffffff:
        return v
    else:
        return -((~v & 0xffffffff) + 1)
#从v这一表达式中提取为带符号数

@register
#生成后注册到__functions__中
def INPUT_VARIABLE(instr, args, value):
    global input_seq, input_variables
    input_seq += 1
    
    V = input_variables[input_seq - 1]
    assert int(args[0]) == V[0]

    execute(STIns, instr, expr=makeval(V[1]), addr=makeval(args[0]))


@register
#生成后注册到__functions__中
def INPUT_MATRIX(instr, args, value):
    global input_seq, input_variables 

    base = int(args[0])
    row = int(args[1])
    col = int(args[2])
    m = int(args[3])
    n = int(args[4])

    for i in range(m):
        for j in range(n):
            input_seq += 1
            V = input_variables[input_seq - 1]
            execute(STIns, instr, expr=makeval(V[1]),
                    addr=makeval(V[0]))



@register
#生成后注册到__functions__中
def INPUT_STRING(instr, args, value):
    global input_seq, input_variables

    for i in count(0):
        input_seq += 1
        V = input_variables[input_seq - 1]
        assert int(args[0]) + i == V[0]
        execute(STIns, instr, expr=makeval(V[1]), addr=makeval(['ptr', args[0], i], int(args[0]) + i))

        if int(V[1]) == 0:
            break


@register
#生成后注册到__functions__中
def INPUT_CHAR(instr, args, value):
    global input_seq, input_variables
    input_seq += 1

    V = input_variables[input_seq - 1]
    assert int(args[0]) == V[0]
    
    execute(STIns, instr, expr=makeval(V[1]), addr=makeval(args[0]))
    execute(OPIns, instr, expr=makeval(value))
#input_seq记录了输入的id？assert不太明白
    

```