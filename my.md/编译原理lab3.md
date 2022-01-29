# <p align="center"><font face="楷体">编译原理实验三报告</p></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖 学号：181860092 队伍编号：15 邮箱：603102048@qq.com</font></p>


### <font face="楷体">一、实验完成度</font>

<font size=3 face="楷体">	完成了实验的必做内容与要求3.1​：完成了中间代码翻译的任务。</font>

### <font face="楷体">二、编译方法</font>

<font size=3 face="楷体">	采用默认的`makefile`执行`make`即可。</font>

### <font face="楷体">三、功能实现思路方法</font>

- <font size=4 face="楷体" color=00bfff>操作数与中间代码结构设计：</font>

  <font size=3 face="楷体">	操作数结构设计如下：</font>

  ```c
struct Operand_ {
      enum { 
          Variable, Temp, ConstantInt, ConstantFloat,
          TAddress, VAddress,
          Pointer,
          SAddress, AAddress,
          OLabel
      } kind;
      union {
          int number;
          int value_int;
          float value_float;
      } u;
      int length; // for array assign
  };
  ```
  
<font size=3 face="楷体">	组成中间代码的操作数可以是变量、常数、指针、$Label$、地址值，结构体中利用`kind`来区分不同类型的操作数。这里需要解释的是指针与地址值的表示：</font>
  
- <font size=3 face="楷体">`Pointer`表示操作数`a`是一个结构体或数组，需要用`&a`来得到首地址；在变量$v_n$是结构体或数组时使用；</font>
  - <font size=3 face="楷体">`TAddress`表示操作数`a`是一个临时变量，它的值是一个`basic`类型数据的地址，需要用`*a`来得到指向的数据；在`x.y`或者`x[i]`是`int`或`float`时使用；</font>
  - <font size=3 face="楷体">` VAddress`表示操作数`a`是一个变量，它的值是结构体或数组的首地址；在函数形参为结构体变量时使用；</font>
  - <font size=3 face="楷体">`SAddress`表示操作数`a`是一个临时变量，它的值是一个结构体变量的地址；在`x.y`或者`x[i]`是结构体变量时使用，这时操作数的值就是地址值，不必用`&`取地址；</font>
  - <font size=3 face="楷体">`AAddress`表示操作数`a`是一个临时变量，它的值是一个数组的首地址；在`x.y`是一维数组时使用，这时操作数的值就是地址值，不必用`&`取地址；</font>
  
  <font size=3 face="楷体">	操作数结构体中的`u`表示操作数的值。其中`number`记录变量、临时变量、`Label`的标号，`value_int`和`value_float`记录常数的值；`length`在操作数表示一个一维数组时记录其长度，用于完成数组赋值；</font>
  
  <font size=3 face="楷体">	在操作数结构体的基础上，定义中间代码的结构就很简单了；同样用`kind`表示中间代码的类型，`u`存储构成对应中间代码的成员；具体不再赘述。</font>
  
- <font size=4 face="楷体" color=#00bfff>翻译过程的实现与优化：</font>

  <font size=3 face="楷体">	翻译过程主要按照手册给出的方式进行了实现。主要思考的是数组、结构体变量的访问优化和一维数组赋值的实现。</font>

  <font size=3 face="楷体">	**一维数组赋值实现**：在`translate_Exp`函数中，如果发现是赋值语句，并且赋值的操作数类型为`Pointer`或`AAddress`或`VAddress`，则说明是数组赋值(因为结构体不允许赋值，选做一中没有高维数组，所以只可能是一维数组赋值)。对于这种情况，首先用`translate_Exp`函数得到两个赋值的操作数`t1,t2`；然后通过访问`Operand_`的`length`域得到两个数组的长度，取较小的长度作为赋值的长度`times`。然后手动生成下列语句：</font>

  ```c
  t4 := #0
  t5 := &t1
  t6 := &t2
  LABEL label1 :
  //取t1、t2首地址，如果t1、t2已经是首地址则不生成
  IF t4 >= times GOTO label2
  *t5 := *t6
  t5 := t5 + #4
  t6 := t6 + #4
  t4 := t4 + #1
  GOTO label1
  LABEL label2 :
  ```
  

<font size=3 face="楷体">	由此，实现了一维数组赋值。</font>

<font size=3 face="楷体">	**数组、结构体变量的访问及优化**：选做一修改了规则，使得数组、结构体访问可以出现以下情况：`e[i]; a.b[i]; a.b.c; a[i].b; a.b[i].c; a[b.c].d`等等。为了减少生成的中间代码量，针对`a.b.c.d`和`a.b[1].c.d[0].e`(`[]`内为常数)这种类型的变量访问，实现了优化。</font>

<font size=3 face="楷体">	对于结构体变量的访问，为了优化，使用全局变量`struct_type`记录当前访问到的元素类型(`BASIC,ARRAY,STRUCTURE`)，全局变量操作数`struct_v`记录最初始的结构体变量(例如：`a.b.c.d`中的`a`)；这样，首先在处理`a`时用`struct_v`进行记录，之后只要利用`place`传递一个类型为整型常数的偏移量，不产生中间代码，而是不断进行累加，在最后一次`dot`出现后添加中间代码`t := struct_v + offset`就可以找到对应的元素了；</font>

<font size=3 face="楷体">	上述思路可以处理`a.b.c.d`这样方式的访问，对于`a.b[1].c`也可以处理(只要在数组访问处理时进行判断，处理`b[1]`后返回一个累加后的偏移量就好了)；但是对于`a.b[i].c`这种访问，由于`i`未知，无法计算偏移量，因此只能根据目前的`struct_v`和偏移量，以及变量`i`先生成一条中间代码，然后从这里开始重新进行上述过程；</font>

<font size=3 face="楷体">	上述思路的实现需要知道何时表达式处理到了最后一个`dot`，因此考虑用一个全局变量`dot_times`(初始化为)进行记录，每次处理结构体头部进行`dot_times++`，处理结束后`dot_times--`，如果减后值为0，说明是最后一个`dot`；但这样简单的处理在处理`a.b[c.d].e`时会出错，因为它统计了`[]`中的`dot`；因此实现了一个全局的栈，每次遇到`[]`时，即处理数组时将当前`dot_times`进行push，数组处理结束后再pop出来就好了。</font>

<font size=3 face="楷体">	对于数组变量的访问，如果只是访问一个数组的变量，而不继续访问它的成员，则生成初始地址加上偏移量的中间代码即可；但如果是上述结构体访问`a.b[0].c`的处理，如果数组处理发现`EXP`翻译的结果类型是一个整形常数，说明它是一个结构体偏移量而且之后还有结构体域的访问，这时就累加偏移量，将`place`设为累加后的偏移量就好了；如果无法计算偏移量(例如`a.b[i].c`)，则生成中间代码计算`a.b[0]`的值，然后处理下一个`dot`时，`struct_v`保存`a.b[0]`的值，重新计算偏移量进行累加即可。</font>