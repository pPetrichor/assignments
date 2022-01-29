# <p align="center"><font face="楷体">OS lab1报告</p></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖
学号：181860092
邮箱：603102048@qq.com</font></p>

### <font face="楷体">一、实验进度</font>

<font size=4 face="楷体">	完成了实验的所有内容（实模式下打印、保护模式下打印、保护模式下加载磁盘中的程序进行打印），打印了字符画。</font>



### <font face="楷体">二、实验结果</font>

<font size=4 face="楷体">	实模式下打印：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\os1.png" style="zoom: 67%;" />

<font size=4 face="楷体">	保护模式下打印（不从磁盘读取，打印string长度为13，不计\n）：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\os3.png" style="zoom:67%;" />

<font size=4 face="楷体">	保护模式下打印（从磁盘读取，不更改app.s，打印string长度为14）：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\os2.png" style="zoom:67%;" />

<font size=4 face="楷体">	字符画打印：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\os4.png" style="zoom:67%;" />



### <font face="楷体">三、实验修改的代码及修改思路</font>

- <font size=5 face="楷体" color=00bfff>实模式下打印：</font>

  <font size=4 face="楷体">	无代码改动，make后make play即可；加深了对利用int指令陷入屏幕中断来调用BIOS打印字符串与利用通用寄存器传递参数的理解。</font>

- <font size=5 face="楷体" color=#00bfff>保护模式下打印：</font>

  <font size=4 face="楷体">	更改**start.s**：在lab1的3.2中给出的pdf的基础上增加代码内容；具体包括初始化各个段寄存器、初始化esp、增添通过写显存实现打印的代码以及调用此代码，下面将一一介绍；</font>

  <font size=4 face="楷体">	**段寄存器与esp初始化：**在**start32**模块中进行实现；考虑到在start模块中已经完成了实模式到保护模式的切换，再结合段寄存器的作用与结构可以知道，初始化段寄存器就是将合适的段选择符赋值给各个段寄存器，使得各个段寄存器与gdt中定义好的段描述符对应起来。段选择符是一个16位的数据，高13位为索引，对应gdt中段描述符的位置；低3位为TI与RPL，本实验中选择的是gdt，故TI为0；程序运行在内核态，故RPL为0；所以低三位是000；了解这些后，段寄存器的初始化思路就很清晰了；
  	在6个段寄存器CS、SS、DS、ES、FS、GS中，CS是代码段寄存器，已经在ljmp指令中初始化为0x8了，也就是二进制下的1000，对应的CS中段选择符索引值是1，对应到代码段描述符，正确；DS是数据段寄存器，所以应该指向数据段，索引为2，加上低3位000，就是二进制下的10000，即0x10；SS是栈段寄存器，ES是附加段寄存器；与DS作用相差不大，一般都是一起初始化，指向的也都是数据段，故全初始化为0x10；另外2个段寄存器可以指向任意的数据段，可不初始化（无所谓）；但观察app.s中的代码，会有这么一句：`movw %ax, %gs:(%edi)`;其中ax中存的是字符的ASCII码，edi指示的是偏离位置，这就不难推出显存对应的地址应该可以由gs存储的段选择符指向的段描述符指出；gdt中的第3项恰好就是指向显存地址0xb8000的视频段描述符，所以真相就是gs中的索引是3，应该初始化为0x18；所以段寄存器的初始化方式为：</font>

  ```python
  	movw    $0x10, %ax
  	movw    %ax, %ds
  	# 利用ax暂存
  	movw    %ax, %es
  	movw    %ax, %ss
      # 以上3个段寄存器与数据段描述符对应
  	movw    $0x18, %ax
  	movw    %ax, %gs	
      # GS与视频段描述符对应
  ```

  <font size=4 face="楷体">	至于esp的初始化，可以沿用实模式下的`movl $0x7d00, %esp`;</font>

  <font size=4 face="楷体">	**添加通过写显存实现打印的代码**：简单来说，就是利用pdf中提到的写显存打印字符的方式来实现message的打印；具体的做法我直接copy了app.s中的内容，利用循环来打印整个字符串；也就是在start.s中添加以下代码：</font>

  ```python
  loop:
  	jmp loop
  
  message:
  	.string "Hello, World!\n\0"
  
  displayStr:
  	movl 4(%esp), %ebx # 取字符串首地址
  	movl 8(%esp), %ecx # 取要打印的长度
  	movl $0, %edi # 起始偏移量为0
  	movb $0xf0, %ah # 白底黑字
  nextChar:
  	movb (%ebx), %al # 取要打印的字符
  	movw %ax, %gs:(%edi) # 写显存
  	addl $2, %edi # 取下一个打印位置
  	incl %ebx #指针移动到下一个字符
  	loopnz nextChar # loopnz decrease ecx by 1
      # 循环打印，长度减1（loopnz使ecx减1）
  	ret
  ```

  <font size=4 face="楷体">	**在start32中调用上面增添的代码**：在初始化段寄存器与esp之后，通过栈传递参数并调用displayStr函数；为此要先将message长度与首地址压栈，然后call：</font>

  ```python
  	pushl $13
      # "Hello, World!"长度为13，没有计入\n
  	pushl $message
      # 传入字符串首地址
  	calll displayStr
      # 函数调用
  ```

  <font size=4 face="楷体">	完成上述内容后，由于在本环节不需要从磁盘读取程序，所以将pdf中的`jmp bootMain`注释掉；这样make后make play就可以得到文档开头展示的结果。</font>

- <font size=5 face="楷体" color=#00bfff>保护模式下加载磁盘程序进行打印：</font>

  <font size=4 face="楷体">	需要更改文件为**start.s**与**boot.c**，更改内容如下所示：
	**start.s**：从实验文档以及助教学长的解释中可知，这个阶段需要通过bootMain读取磁盘加载程序进行打印，而且打印的具体方式与内容在app.s中已经写好了，所以这里完全没有必要在start.s中添加循环打印的代码；因此，在对段寄存器和esp进行初始化后执行`jmp bootMain`这一指令就可以了；之后的工作将交由boot.c进行完成。
  	不过需要注意的是，这里段寄存器的初始化方式是不变的，但esp的初始化会因为磁盘程序的加载受到影响；根据后面的内容来看程序会从0x8c00开始加载磁盘的内容；这样来看的话esp再设置为0x7d00就不太合适了；根据Linux下内存的划分，栈应该设置在比代码段和数据段更高的地址处；如果按Linux下的划分的话，代码段和数据段应该分别从0x8048000和0x8049000开始，所以不能按这个标准下设为0xc00000000（太大了，直接赋值esp是0，试了一下会崩）；大概估计从磁盘读入数据的长度（从0x8c00开始），再大概分析给堆栈区预留的长度，选取了esp初始值为0xc00000，来满足加载磁盘程序的要求；所以最后start.s的内容就只是在pdf的基础上将段寄存器初始化、esp初始化为0xc00000后跳转到bootMain这些内容；
  	**boot.c**：boot.c中需要做的事情已经在C文件和文档中说明了，就是在通过readSect函数加载磁盘内容后跳转到加载好的程序处执行；实现这一点非常简单，从boot.c的这几行代码：</font>
  
  ```python
  elf = (void(*)(void))0x8c00;
  	
  for (i = 0; i < 200; i++) {
  	readSect((void*)(elf + i*512), 1+i);
  }
  ```
  
  <font size=4 face="楷体">	可以看出，位于磁盘中的可执行文件已经被加载到elf所指的地址0x8c00处了，所以只要在加载后跳转到0x8c00执行就可以了；这可以由内联汇编跳转实现，但elf作为一个void*型的函数指针，指向的就是加载好的程序的起始地址0x8c00，所以只要在循环结束后加一句`elf();`就**实现了到0x8c00的跳转**；
  	实现跳转本身并不难，下面说一点我对于这部分框架代码的理解；app.s在编译后进行链接的指令为：`ld -m elf_i386 -e start -Ttext 0x8c00 app.o -o app.elf`；这就说明在链接生成可执行目标文件时是将app.s中的start的地址设置为0x8c00的，因此，在加载程序（链接时地址被设置为0x7c00）的执行过程中要想在之后调用app.o中的内容实现字符串的打印，必须先将对应的可执行目标文件通过读磁盘读到0x8c00这个地方（否则就对应不上了），所以在bootMain中会设置elf为0x8c00，然后将位于磁盘中的可执行目标文件加载到这里。</font>

- <font size=5 face="楷体" color=#00bfff>保护模式下加载磁盘程序打印字符画：</font>

  <font size=4 face="楷体">	修改**app.s**：将指令</font>

  ```python
  	pushl $14
  	pushl $message
  ```

  <font size=4 face="楷体">	修改为</font>

  ```python
  	pushl $2000
  	pushl $ascpic
  ```

  <font size=4 face="楷体">	即可;即更改字符串起始地址与字符串长度。</font>

  

### <font face="楷体">四、思考与心得</font>

<font size=4 face="楷体">	本次实验中关于完成实验内容的思考可见本文档第三部分的“修改思路”。
	**本次实验讲义中提到的思考内容**：</font>

-  <font face="楷体" size=4>**CPU、内存、BIOS、磁盘、主引导扇区、加载程序、操作系统的含义与联系**</font>

   <font size=4 face="楷体">	CPU是计算机的中央处理器；内存是计算机的存储空间，用于存储代码、数据、执行代码等功能；BIOS是一组固定到计算机内主板上一个ROM的程序，用于开机自检，加载主引导扇区；磁盘也是计算机的存储介质，容量大但加载慢，断电后内容不丢失；主引导扇区是硬盘0号柱面0号磁头0号扇区处大小为512字节的数据，最后两个字节为0x55、0xaa作为结束标志；基本加载程序（primary bootloader）是MBR的一部分，它会遍历分区表寻找可用的分区，找到后进一步定位分区中的Stage 2 bootloader，它主要用于加载操作系统并跳转到操作系统执行（从中可以看出现代操作系统中加载程序并不是MBR的全部，MBR还要对磁盘进行管理，故要包含更多的内容）；操作系统是计算机中管理计算机硬件与软件资源的程序；
   	联系为：CPU加电后，会跳转到内存ROM中的BIOS执行；BIOS会在开机自检后从磁盘加载MBR到内存（0x7c00），然后检测魔数，之后跳转到0x7c00执行加载程序，来载入os的kernel，并将控制权交由OS来继续接下来的操作。</font>

-  <font face="楷体" size=4>**gdt中各段描述符的含义**</font>

   <font size=4 face="楷体">	第一个表项为空，不予解释：</font>

   <font size=4 face="楷体">	第二个表项：代码段描述符</font>

   ```python
   .word 0xffff,0
   .byte 0,0x9a,0xcf,0 
   ```

   <font size=4 face="楷体">	第一个word与第三个byte中的低4位指明了代码段的限界为0xfffff；第二个word、第一个byte和最后一个byte共同指明了代码段的基地址为0；第二个byte 0x9a是二进制下的10011010，即P=1（该段已在内存中），DPL=0（内核态下访问），S=1（指明是普通的代码段描述符），TYPE=101（指示段的访问权限），A=0（未被访问过）；第三个byte的高4位为1100，即G=1（代码段以页为基本单位），D=1（地址和数据为32位宽），最后一个0是AVL，在Linux中被忽略；</font>

   <font size=4 face="楷体">	第三个表项：数据段描述符</font>

   ```python
   .word 0xffff,0 
   .byte 0,0x92,0xcf,0
   ```

   <font size=4 face="楷体">	相较于代码段描述符，只有第二个byte值的低四位发生了改变，这意味着数据段的限界、基地址、G、D、P、DPL、S均与代码段一样；分析第二个byte 0x92的低四位是二进制下的0010，即TYPE=001（指示段的访问权限），A=0（未被访问过）；</font>

   <font size=4 face="楷体">	第四个表项：视频段描述符</font>

   ```python
   .word 0xffff,0x8000
   .byte 0x0b,0x92,0xcf,0 
   ```

   <font size=4 face="楷体">	第一个word与第三个byte中的低4位指明了代码段的限界为0xfffff；第二个word、第一个byte和最后一个byte共同指明了代码段的基地址为0xb << 16 + 0x8000 = 0xb8000；第二个byte 0x92是二进制下的10010010，即P=1（该段已在内存中），DPL=0（内核态下访问），S=1（指明是普通的代码段描述符），TYPE=001（指示段的访问权限），A=0（未被访问过）；第三个byte的高4位为1100，即G=1（代码段以页为基本单位），D=1（地址和数据为32位宽），最后一个0是AVL，在Linux中被忽略；</font>



<font size=4 face="楷体">	**本次实验的心得**：</font>

- <font size=4 face="楷体">先要对框架代码进行理解，不应盲目操作：先结合讲义与框架代码充分理解了要做的事情后再开始好于一头扎到实验要完成的内容瞎试；说不定从框架代码中会得到帮助；例如本实验中GS的初始化；再例如实现“保护模式下直接输出”，可以copy app.s中循环打印的内容（早点发现就不用自己写bug了）；</font>

- <font size=4 face="楷体">善用报错提示信息与gdb：gdb可以帮助发现一些不易觉察的bug（eg：esp的初始化）；某些情况下报错信息则直接指出了问题所在，例如直接在start.s中试图加入字符画的信息，就会得到`ERROR: boot block too large: 1000 bytes (max 510)`的报错，从而可以认识到正确的做法应该是修改app.s。</font>