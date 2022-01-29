# <p align="center"><font face="楷体">编译原理实验一报告</p></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖 学号：181860092 队伍编号：15 邮箱：603102048@qq.com</font></p>


### <font face="楷体">一、实验完成度</font>

<font size=3 face="楷体">	完成了实验的必做内容与要求$1.2$：完成了词法分析(包括指数形式浮点数的识别)、语法分析(正确时打印语法树、错误时报语法错)的任务。</font>

### <font face="楷体">二、编译方法</font>

<font size=3 face="楷体">	进入`/Code`目录后，执行`make`进行编译即可在`/Code`目录下生成可执行文件`parser`；之后可以使用命令`./parser <input_file_path>`进行测试。</font>

### <font face="楷体">三、功能实现思路方法</font>

- <font size=4 face="楷体" color=00bfff>词法分析设计：</font>

  <font size=3 face="楷体">	词法分析在`lexical.l`中实现：编写了识别所有合法**token**的正则表达式.其中对于浮点型**token**，要求小数点前后必须都有数字，且除$0.xxx$外，整数部分不能以$0$开头(eg:$001.100和.100$是非法的，$0.100$合法);</font>

  <font size=3 face="楷体">	除了合法的**token**，在这一部分还实现了`ERROR_INT、ERROR_FlOAT、ERROR_ID`三个**token**用于匹配非法的整型、浮点型与$ID$，将它们作为词法错误进行处理：</font>

  - <font size=3 face="楷体">ERROR_INT：除0外，以0开始的整数非法；</font>
  - <font size=3 face="楷体">ERROR_FlOAT：小数点前或后没有数字(eg: 1.)、有多个小数点(eg: 1.2.3)、不是0.xxx但整数部分以0开始(eg:001.2)的小数非法；对于指数形式，缺少指数部分(eg: 1.e)、基数部分错误(eg: 12.3.4e+1)或二者皆有(eg: 12.3.4e)的指数形式浮点数非法.这里没有考虑缺少基数部分的错误是因为`[eE]`可能是变量，所以缺少基数的表达式也可以理解为计算式(eg:a.e+1可以看作结构体变量`a`的成员`e`加一)，不能看作词法错误；</font>
  - <font size=3 face="楷体">ERROR_ID：以数字开头的，由字母、数字、下划线组成的是非法ID；</font>

  <font size=3 face="楷体">	如果检测到三个错误**token**，相应的`action`会打印词法错误信息，但仍然将`INT、FLOAT、ID`作为返回值，这样既进行了词法报错，又不会影响语法检测。</font>

- <font size=4 face="楷体" color=#00bfff>语法树设计：</font>

  <font size=3 face="楷体">	在`ptree.h`与`ptree.c`中实现了语法树的结构定义与相关函数操作：</font>

  <font size=3 face="楷体">	**语法树结构的定义**：对于不同类型的节点，打印语法树时输出的内容也不同，故首先定义`NodeType`区分节点类型；其次每个节点都有值(整数、浮点数或字符串)，所以用联合体`nodevalue`进行表示；由此，构造节点结构`Pn`：其中包含节点类型、行号、值；此外建树过程为自底向上，会有将多个子节点归约到一个父节点的操作，所以还会有`Pn*`类型的指针`sons`(用于找到第一个子节点)和`list`(用于连接兄弟节点)。</font>
  
  <font size=3 face="楷体">	**语法树相关函数**：`newleafnode`用于生成叶子节点，根据词法分析得到的信息，对叶子节点的各个属性值进行赋值；`newfathernode`用于生成父节点，传入参数为父节点的`nodevalue`(因为父节点是非终止符，所以`nodevalue`只可能是字符串)与第一个子节点的位置，利用这两个参数对父节点进行赋值；父节点的行号应该与第一个子节点的行号相等；`showTree`函数用于打印语法树，采用递归方式实现：每个节点首先打印自己，然后打印自己的子树，最后打印自己的下一个兄弟节点；如果遇到类型为`nodenull`的空节点，则没有子树，跳过这一步；打印的空格数也通过递归实现：树下降一层，多打印两个空格。</font>

  <font size=3 face="楷体">	**语法树建立**：所有的`token`类型均定义为`Pn*`；在词法分析中会得到所有叶子节点，每检测到一个**token**就调用一次`newleafnode`，根据`yylineno`与`yytext`对叶子节点赋值；如果有词法错误，将变量`error1`置为$1$；在语法分析中会得到父节点，每匹配到一个产生式，首先会将产生式右侧的所有节点利用`list`连接起来，然后调用`newfathernode`给\$\$赋值；连接右侧节点在`action`中执行，因为`empty`也会调用`creatnull`生成一个类型为`nodenull`的节点，所以连接时无需担心访问`null`指针的问题；如果有词法错误，将变量`error2`置为$1$；最后，在`main.c`中执行`yyparse`后，如果`error1`和`error2`均为$0$，则打印语法树。</font>

- <font size=4 face="楷体" color=#00bfff>错误恢复设计：</font>

  <font size=3 face="楷体">	错误恢复设计中，如果匹配到产生式中的`error`会自动调用`yyerror`，但参数默认`Syntax error`,无法传参；如果`yyerror`中不输出`msg`，而是在后面接着`printf`，如果有没有考虑到的错误，则无法输出提示信息；故实现了函数`myyyerror`与`myyyerror_`；前者用于匹配到`error`时的报错：由于匹配到`error`会自动调用`yyerror`，所以首先利用`printf("\033[K")`清除掉`yyerror`的输出信息，然后输出详细的报错信息；后者则会利用传入节点的信息生成相应的报错信息。</font>

  <font size=3 face="楷体">	实现错误恢复中利用的方法有两类：一是编写含有`error`的产生式，并利用`myyyerror`与`myyyerror_`输出详细的错误信息；另一个是编写错误的但不含`error`的产生式，然后在语义动作中调用`yyerror`或`myyyerror_`进行报错。</font>
  
  <font size=3 face="楷体">	**错误行号的确定**：匹配到`error`时的行号与匹配完这条产生式后执行语义动作的行号大概率不一样，所以输出错误行号不可以直接用`yylineno`；为此实现了一个栈`stack`，每次执行`yyerror`都将当前`error`的行号入栈，之后匹配完整个产生式后调用`myyyerror`时则执行`pop`得到`error`所在的行号，从而打印正确的错误所在行号；对于自己调用`yyerror`的情况，则需要将语义动作放置到产生式中合适的位置来打印正确的错误所在行号，同时为了消除手动调用`yyerror`对栈的影响，要手动执行一次`pop`。</font>
  
  <font size=3 face="楷体">	**实现的错误恢复**：</font>
  
  - <font size=3 face="楷体">针对各种`List`中连接符的错误进行修复：`List`中应该用`','`进行连接，针对`','`位置不对、有多余`','`或是使用非法连接符连接不同量的错误进行了恢复(eg:`int a,,b;`)；</font>
  
  - <font size=3 face="楷体">针对变量定义时问题进行了修复；</font>
  
  - <font size=3 face="楷体">针对定义或使用数组变量时`'[]'`内出现错误，或是`'['`、`']'`缺失的错误进行了恢复；</font>
  
  - <font size=3 face="楷体">针对函数定义时的形参，即`()`内出现问题的错误进行了恢复，包括`','`使用问题、使用非法连接操作符问题、形参内参数没有`Specifier`等各种非法问题；(eg:`int fun(int a,b){...}`)</font>
  
  - <font size=3 face="楷体">针对函数调用时的参数出错，即`()`内出现问题的错误进行了恢复，包括`','`使用问题、使用非法连接操作符问题、非法实参表达式等各种非法问题；(eg:`fun(.)`)</font>
  
  - <font size=3 face="楷体">针对函数前部出现错误进行修复：包括函数之前有非法语句(例如在前面给全局变量赋值、头部写了`#include`)、函数名错误、函数`Specifier`出错等问题(eg:`int if(){...}`)；</font>
  
  - <font size=3 face="楷体">针对各种`;`丢失问题进行了修复；</font>
  
  - <font size=3 face="楷体">针对函数与函数体之间存在非法内容的问题进行了修复；(eg:`int f(int a)int b{...}`)</font>
  
  - <font size=3 face="楷体">针对非法`Exp`错误进行了修复；</font>
  
  - <font size=3 face="楷体">针对`IF`语句以及`IF ELSE`语句中条件表达式非法、左括号丢失错误进行了修复；</font>
  
  - <font size=3 face="楷体">针对没有将所有变量的定义写在函数头部的问题进行了修复，举例如下：</font>
  
    ```c
    int main()
    {
        int i = 1;
        i = 2;
        int j = 2;
    }
    ```
