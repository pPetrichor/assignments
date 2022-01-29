# command.py

```
import lazy_object_proxy
from util import _tuple, _list

__all__ = ['Field', 'MultiField', 'KeyField', 'KWField',
           'Command', 'KeyCommand', 'Ref', 'Reference', 'cmdfunc','decode', 'encode', 'loads', 'dumps']
//定义公开接口

class uniquedict(dict):	//继承字典类,猜测是建立一个key不重复的字典
    def __setitem__(self, key, elem)://定义函数__setitem__
        if key in self:	//字典in操作，检测key是否在类的字典中存在
            raise KeyError('%r exists' % (key, ))	//如果key值已经在里面了，抛出异常
        super().__setitem__(key, elem)	//调用dict的函数，将（key，elem）放入

__command__ = uniquedict()
__context__ = uniquedict()
//实例化

def cmdfunc(func, name=None)://定义函数，如果没有传入name则默认为none；作用：将key为name的项值设为func
    if not name:
        name = func.__name__ //name被赋值为func运行所在的模块
    __command__[name.lower()] = func//name转化为小写
    //__command__实例中key值为name的对应设为func
    return func


@cmdfunc //将List函数的结果作为参数调用cmdfunc
def List(*args, **kwargs):
    return list(args)//将args转换为列表


class Field:	//Field类定义
    sequence = 0
    __slots__ = ('seqn', 'name', 'argn')	//限制属性
    //共享变量定义
    def __init__(self):
        Field.sequence += 1  //记录实例化的编号
        self.seqn = Field.sequence//编号
        self.name = ''
        self.argn = 0	//name、argn初始化

    def __call__(self, args, kwargs):
        return args[self.argn]//返回下标为argn的args元素

    def __repr__(self):
        return '<Field %s %s %s>' % (self.seqn, self.name, self.argn)
		//返回本实例信息

class MultiField(Field)://继承Field
    __slots__ = ('size', ) //增添共享变量
    def __init__(self, size=0): //size默认项为0
        super().__init__() //初始化调用
        self.size = size //设置size

    def __call__(self, args, kwargs):
        if self.size:如果size不为0
            return list(args[self.argn:self.argn + self.size])
        //将args的从argn到argn + self.size的元素打包为list返回
        else:
            return list(args[self.argn:])//否则将args的从argn到结尾的元素打包为list返回


class KWField(Field)://继承Field
    __slots__ = ('default', ) //增加default变量
    def __init__(self, default=None):
        super().__init__()
        self.default = default//default初始化

    def __call__(self, args, kwargs):
        if callable(self.default):
            default = self.default()
        else:
            default = self.default
        return kwargs.get(self.name, default)


class KeyField(Field): pass //pass占位，无实际作用


class MetaCommand(type):
    def __new__(mcs, name, bases, attrs):
        from itertools import count, chain
        # single inheritance between command
        _bases = [cls for cls in bases if isinstance(cls, MetaCommand)]
        assert len(_bases) <= 1

        base_command = next(iter(_bases), None)
        base_members = getattr(base_command, '__members__', [])
        members = [attr for attr in attrs.items() if isinstance(attr[1], Field)]
        members = sorted(members, key=lambda item: item[1].seqn)
        for (fnm, fld), argn in zip(members, count(len(base_members))):
            fld.name, fld.argn = fnm, argn
        members = base_members + [fld for fnm, fld in members]
        command = attrs.get('__command__', name)
        classkey = None
        if base_command and base_command.__name__ == 'KeyCommand':
            classkey = command.lower()
        else:
            classkey = getattr(base_command, '__classkey__', None)

        attrs['__members__'] = members
        attrs['__command__'] = command
        attrs['__classkey__'] = classkey

        cls = super().__new__(mcs, name, bases, attrs)
        __command__[command.lower()] = cls
        return cls


class Command(metaclass=MetaCommand):
    // command类定义
    def __init__(self, *args, **kwargs):
    //*args将传入参数打包为（），**kwargs打包为dict
        
```

```python
for field in type(self).__members__://在command.__members__中遍历
            setattr(self, field.name, field(args, kwargs))
        //将每一个__members__的name中添加Field类型的field(args, kwargs)属性
        fieldnames = set(f.name for f in type(self).__members__)
        //fieldnames利用set成为所有__members__的不重复name的集合
        for key, val in kwargs.items()://遍历kwargs字典
            if key not in fieldnames:
                setattr(self, key, val)
            //如果输入参数构成的字典中key值有不在fieldnames的，则self.key = val，即在self中创建新				的以key命名的属性
        
    def __repr__(self):
        from sexpdata import dumps
        return dumps(encode(self))

    def accept(self, visitor, *args):
        fn = getattr(visitor, 'visit' + self.__class__.__name__)
        return fn(self, *args)


class Ref(Command, lazy_object_proxy.Proxy):
    __key__ = Field()

    def __init__(self, cls, *key, context=__context__):
        cls = cls.lower()
        key = _tuple(cls, *key)
        # if not issubclass(__command__[cls], KeyCommand):
        #     raise KeyError("class %s is not KeyCommand" % cls)
        Command.__init__(self, key)
        lazy_object_proxy.Proxy.__init__(self, lambda: context[self.__key__])

Reference = Ref
        

class KeyCommand(Command):	//定义KeyCommand类
    def __init__(self, *args, context=__context__, **kwargs):
        super().__init__(*args, **kwargs)
        //调用command的初始化函数
        context[self.__key__] = self//以__key__返回值为索引项值设为self

    @property
    def __key__(self):	//函数返回值作为参数来调用property
        cls = self.__class__.__classkey__
        //将MetaCommand初始化的name小写化后返还给cls
        key = [getattr(self, fld.name) for fld in type(self).__members__
               if isinstance(fld, KeyField)]
        //key为MetaCommand的__members__列表中属于KeyField的元素对应的name组成的列表
        return _tuple(cls, *key)//返回元组

*********************************************************
//解码函数
def decode(sexpr, context=__context__):
    //默认传参为前面声明的__context__
    if isinstance(sexpr, list):
        //如果传入的sexpr为list
        if sexpr:
            //sexpr不为空
            cmd, *args = sexpr
            //list赋值法，应该是将sexpr中第一个值赋给cmd，后面的用args指针指向
            cmd = decode_cmd(cmd)
            
            args, kwargs = decode_arg(args, context)
            //处理cmd之后的sexpr内容
            return cmd(*args, **kwargs)
        	//应该是将cmd与args、kwargs这些解析后的内容重新整理成sexpr的形式返回
        else:
            return None
    else:
        return sexpr


def decode_cmd(cmd):
    //cmd解码
    from sexpdata import Symbol
    assert isinstance(cmd, Symbol)
    //cmd应该为Symbol类型
    cmd = cmd.value().lower()
    //转换为小写
    return __command__[cmd]
	//返回__command__字典中索引值为cmd的值

def decode_arg(lst, context):
    from sexpdata import Symbol
    args = []
    //申明一个空链表
    kwargs = {'context': context}
    //声明一个字典，先放入一对元素
    idx = 0
    while idx < len(lst):
        //遍历lst
        arg = lst[idx]
        //取出对应列表元素
        if isinstance(arg, Symbol) and arg.value()[0] == ':':
            //取出元素是个symbol且以“：”开头
            name = arg.value()[1:]
            //name取后面除了“：”以外的字符串内容
            arg = decode(lst[idx + 1], context)
            //解析下一个元素的内容
            kwargs[name] = arg
            //加入元素对{name，arg}
            idx += 2
            //跳过下一个
        else:
            arg = decode(arg, context)
            //否则直接解析
            args.append(arg)
            idx += 1
            //增加到args列表里
    return args, kwargs


def loads(s, context=__context__):
    import sexpdata
    return decode(sexpdata.loads(s), context)
//调用sexpdata中的loads读入后再解析

*****************************************
注：引入的util中的_list内容如下，在util.py中定义：
def _list(*args):
    return list(args)
就是将元素整理成列表

//一层一层对obj进行编码处理
def encode(obj, with_ref=False):
    from sexpdata import Symbol as Sym
    if obj is None:
        return []
    //如果obj为空，返回空列表
    elif with_ref and isinstance(obj, KeyCommand):
        return _list(Sym('ref'), *encode_arg(obj.__key__, with_ref))
    //如果with_ref为真且obj类型是KeyCommand，标记一个ref并将__key__中所有内容编码，最后整理成列表返回
    elif isinstance(obj, Command):
        return _list(Sym(obj.__class__.__command__), \
                     *encode_positional_args(obj)) + encode_keyword_args(obj)
    //如果是Command类型，指出此command并将其所有的__members__元素按照先非KWField元素编码后KWField的顺序返回一个list
    elif hasattr(obj, 'encode_'):
        return obj.encode_()
    //如果obj有encode_属性则返回这个值
    elif isinstance(obj, (int, str)):
        return obj
    //如果是int或者str类型则返回本身
    elif isinstance(obj, (list, set, tuple)):
        return _list(Sym('list'), *encode_arg(obj, with_ref))
    //如果是列表、集合、元组类型，则返回其中每个元素编码后的列表
    else:
        assert False, ("unknown type", type(obj))
	//报错，obj不是预期类型

def encode_arg(arg, with_ref):
    assert isinstance(arg, (list, set, tuple))
    //arg为列表或集合或元组
    return [encode(obj, with_ref=with_ref) for obj in arg]
//将arg中所有元素进行编码操作后整理成列表返回

def encode_positional_args(obj):
    positionals = [f for f in obj.__members__ if not isinstance(f, KWField)]
    //迭代obj中的成员，将不是KWField类型的整理成列表
    encoded = []
    //已编码好的列表，初始为空
    for field in positionals:
        encoded.append(encode(getattr(obj, field.name), with_ref=True))
       // 等价于encoded.append(encode(obj.“field.name”), with_ref=True))，就是对obj对应的“field.name”字符串指示的内容进行编码后加到已编码的列表
    return encoded
    

def encode_keyword_args(obj):
    from sexpdata import Symbol
    keywords = [f for f in obj.__members__ if isinstance(f, KWField)]
    //迭代obj中的成员，将KWField类型的整理成列表
    encoded = []
    for field in keywords:
        encoded.extend([Symbol(':' + field.name),
                        encode(getattr(obj, field.name), with_ref=True)])
        //一次添加以:field.name为内容的symbol项并将obj对应的“field.name”字符串指示的内容进行编码后加到已编码的列表
    return encoded


def dumps(obj):
    import sexpdata
    return sexpdata.dumps(encode(obj))
//编码后输出

```



<font size=4 color=orange face="楷体">下面是根据代码描述结合一些已有的例子做出的一些猜测分析：
解码： 以symb.ot.xxx第一句为例：(ITIns 11 1 :src () :loc 0 :cdep (ref "instr" 0 0) :pdf (list) :expr (Val :e0 (expr 48) :e1 (expr 48) :e2 (expr 48) :e3 (expr 48) :e4 (expr 48) :v 48) :addr (Val :e0 (expr 4204660) :e1 (expr 4204660) :e2 (expr 4204660) :e3 (expr 4204660) :e4 (expr 4204660) :v 4204660))
那按照decode的说法，就是先让cmd为itins（小写化）在字典__command__中对应的内容（未知），然后args指向后面的内容，后面的继续解码，在decode_arg中实现：后面的11、1是int直接加入args里面，像:src ()这一组就会被整理成{“src”，（）}被加入到kwargs中；:loc 0 同理；再后面的内容就稍微复杂一点，最典型的可以拿:expr (Val :e0 (expr 48) :e1 (expr 48) :e2 (expr 48) :e3 (expr 48) :e4 (expr 48) :v 48)来说明：
首先键对的name为“expr”，后面对应的值为一个list解码后的内容：在这里面cmd为__command__[val]，参数为键对{e0,"cmd为__command__[expr]，args为48，kwargs只有{{'context': context}}的list"}……{v 48}；其他的与此类似；
处理完所有args后，会将整理出的cmd、args、kwargs组合为某种形式返回；
编码加密：开始是想找一组初始数据带入一下，但是不调用此加密函数的代码形式看不太懂（就是田晓滨发到群里的），而且加密也不仅是这一个函数完成的（这可能是最后一步），所以没有类似解码的分析；不过其功能还应该是将信息组织成一个list样式的字符串，具体含义还要结合其他代码共同分析。</font>