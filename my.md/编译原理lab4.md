# <p align="center"><font face="楷体">编译原理实验四报告</p></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖 学号：181860092 队伍编号：15 邮箱：603102048@qq.com</font></p>


### <font face="楷体">一、实验完成度</font>

<font size=3 face="楷体">	完成了实验的必做内容。</font>

### <font face="楷体">二、编译方法</font>

<font size=3 face="楷体">	采用默认的`makefile`执行`make`即可。</font>

### <font face="楷体">三、功能实现思路方法</font>

- <font size=4 face="楷体" color=00bfff>指令选择：</font>

  <font size=3 face="楷体">	指令选择过程就是逐条将中间代码对应到相应的目标代码，细节不再赘述。这里需要注意的一点是：在我的实现中，每处理到一条`Function`类型的中间代码，也即开始翻译一个新的函数时，会对当前函数体先进行一次遍历，统计所有的变量和临时变量，为它们在栈中预分配好空间，作为它们在内存中存储的位置。这样做是为了避免在循环语句中为变量分配栈空间后，后续程序执行到这里再次为变量分配空间，导致实际的帧大小与记录值不同，从而在`return`时错误。</font>

- <font size=4 face="楷体" color=#00bfff>寄存器分配：</font>

  <font size=3 face="楷体">	采用了局部寄存器分配算法。在我的实现中，5-25号寄存器都可以用于存储变量；实现局部寄存器分配算法主要包括：划分基本块，在每一块中统计活跃信息与后续使用信息，这样在后续`spill`时，就可以根据后续使用信息选出要替换的寄存器。这里主要介绍如何在每一块中统计活跃信息与后续使用信息。</font>

  <font size=3 face="楷体">	**使用到的结构体**：</font>

  ```c
  struct Vdescriber{
      int reg_no; //store in which reg
      int offset; //offset from sp, 1 means not in stack
      int next_use; //-1 means not active
  };
  
  struct active{
      Operand op;
      int next_use; 
  };
  
  struct active_in_block{
      struct active active_information[3];
  };
  ```
  

<font size=3 face="楷体">	`Vdescriber`是变量描述符，用于记录当前所有变量的信息，包括存储在哪个寄存器里，在栈的什么位置，下一次使用的行数；`VD_V`与`VD_T`是两个变量描述符类型数组，分别记录所有变量与所有临时变量的信息；</font>

<font size=3 face="楷体">	`Vdescriber`是变量描述符，用于记录当前所有变量的信息，包括存储在哪个寄存器里，在栈的什么位置，下一次使用的行数；`VD_V`与`VD_T`是两个变量描述符类型数组，分别记录所有变量与所有临时变量的信息；</font>

<font size=3 face="楷体">	`active`是变量的活跃信息记录，其中`op`记录相应的变量，`next_use`记录该变量在当前基本块内下一次使用的位置；`active_in_block.active active_information`则表示一条中间代码中所有变量的活跃信息，因为操作数至多3个，所以数组长度为3；</font>

<font size=3 face="楷体">	每次处理一个新的基本块时，首先统计基本块内的中间代码条数`length`，然后申请长度为`length`的`active_in_block`数组。接着从基本块末尾向前遍历，对于每一句中间代码，将其中操作数的活跃信息和后续使用信息记录到申请的数组中(下标与中间代码编号对应)，同时更改`VD_V`与`VD_T`中`next_use`的信息(赋值号左边变量不活跃，右边变量活跃且下一次使用为当前语句)。</font>

<font size=3 face="楷体">	这样处理后，翻译基本块内中间代码时，可以根据记录活跃信息的数组的内容不断更新`VD_V`与`VD_T`；当寄存器需要`spill`时，只需要遍历所有变量，找到`next_use`最大或者不活跃的那一个变量进行替换即可。</font>

- <font size=4 face="楷体" color=#00bfff>栈管理：</font>

<font size=3 face="楷体">	变量在栈中的位置通过`fp`寄存器加偏移量记录，而不是用`sp`加偏移量；寄存器5-25号用于存储变量；2号寄存器用于存储函数结果；4号寄存器用于为`write`函数传递参数；其他函数传参不使用寄存器，全部利用栈进行传参；调用函数时首先将所有参数压栈，然后压栈`fp`与`ra`；函数返回后要`pop fp、ra`，恢复`fp`与`ra`的值；</font>

<font size=3 face="楷体">	`call`语句的翻译过程为(函数参数之前已经在`arg`语句中压栈)：`fp、ra`压栈；`fp = sp`；跳转到相应函数；返回后恢复`fp、ra`的值；将`sp`加上`传参个数*4`，即回收函数调用传参的空间；最后如果函数返回值要赋值给其他变量，则将`v0`中的值赋值给相应的操作数。</font>