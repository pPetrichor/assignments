# <p align="center"><font face="楷体">OS lab2报告</p></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖
学号：181860092
邮箱：603102048@qq.com</font></p>

### <font face="楷体">一、实验进度</font>

<font size=4 face="楷体">	完成了实验的所有内容：完成cp函数、实现键盘按键的串口回显、实现printf的处理例程并完善了格式化输出。</font>



### <font face="楷体">二、实验结果</font>

<font size=4 face="楷体">	cp函数的实现：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\os\无标题.png" style="zoom:67%;" />

<font size=4 face="楷体">	printf测试代码的输出结果：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\os\无标题1.png" style="zoom:67%;" />

<font size=4 face="楷体">	键盘按键的串口回显：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\os\无标题2.png" style="zoom:67%;" />

### <font face="楷体">三、实验修改的代码及修改思路</font>

- <font size=5 face="楷体" color=00bfff>cp函数实现：</font>

  <font size=4 face="楷体">	根据手册提示，仿照mkdir对cp进行实现。阅读mkdir代码知，创建一个新的文件夹的过程为：首先将超级块中的内容加载到superBlock中，接着利用stringChRr函数提取出目的路径的上一级目录，然后调用readInode函数；readInode函数用来将传入的路径对应的Inode的信息读出到传入的参数中，因此mkdir调用此函数来将上一级目录的信息加载到`fatherInode`和`fatherInodeOffset`中；`allocInode`函数用于在指定的路径下申请一个新的Inode，同时更新上一级目录的Inode与Superblock信息；Inode类型可由输入参数指定；所以mkdir在得到了上一级目录的信息后，会调用`allocInode`函数来为新建文件夹申请一个目录型Inode，同时更新上一级目录的Inode与Superblock信息（包含对可用Inode、可用datablock、位图信息等内容的更新）；申请成功后，再调用initDir函数在申请到的空的Inode中加入`.`与`..`这两个内容来指向自己与上一级目录；上述过程中每一步都会检测目的路径的合法性、文件打开是否成功以及读取、申请Inode是否成功；如果都没有问题，就完成了一个新文件夹的建立；
  	对比建立一个文件夹与建立一个文件的过程，它们都需要申请新的Inode，并更新Superblock与上一级目录；只不过二者申请的Inode的类型不同，而且建立一个文件不需要在里面加入`.`与`..`的内容，具体到代码就是调用`allocInode`函数时最后一个参数应该为`REGULAR_TYPE`，而且不调用initDir函数；同时，因为cp函数要完成的任务是建立一个空文件，然后将`srcFilePath`指向的文件内容拷贝到建立的新文件中，所以除了不调用initDir函数外，我们需要利用框架代码给的copyData函数来实现这一复制；为了调用copyData函数我们要用一个`File *`打开`srcFilePath`指向的文件，所以要添加这一步，再在最后调用copyData函数；最后，考虑到cp函数实现的是对文件的拷贝，所以合法的`destFilePath`不会以'/'结尾，因此mkdir中对`destFilePath`结尾为'/'的判断以及处理（包括暂时置0以及最后恢复为'/'的处理）可以去掉，然后增加一个对`destFilePath`结尾是否为'/'的判断来检查路径的正确性；
  	分析了上述内容后，在mkdir函数上进行修改实现cp函数，具体的改动之处为：</font>

  <font size=4 face="楷体" color="red">	PS：cp函数可以实现对于文件夹的拷贝，对于这一点本cp函数没有进行实现，即本cp函数只实现了对于文件的拷贝；</font>

  ```c
  // 1、添加打开srcFilePath指向文件的操作：
  	fileSrc = fopen(srcFilePath, "r+");
  	if (fileSrc == NULL) {
          printf("Failed to open srcFile.\n");
          return -1;
      }
  
  // 2、删除mkdir中对于destFilePath以'/'结尾的判断与处理，增加以下判断：
   	if (destFilePath[length - 1] == '/') {
          printf("Incorrect destination file path.\n");
          fclose(file);
          fclose(fileSrc);
          return -1;
      }
  
  // 3、更改allocInode的最后一个参数，更改后为：
      ret = allocInode(file, &superBlock, &fatherInode, fatherInodeOffset,
          &destInode, &destInodeOffset, destFilePath + size + 1, REGULAR_TYPE);   
      
  // 4、调用allocInode成功后，添加对于copyData的调用进行文件复制
      ret = copyData (file, fileSrc, &superBlock, &destInode, destInodeOffset);
      if (ret == -1) {
          printf("Failed to copy data.\n");
          fclose(file);
          fclose(fileSrc);
          return -1;
      }
  
  // 5、更改printf的输出信息
  	printf("copy %s\n", destFilePath);
      printf("COPY success.\n%d inodes and %d data blocks available.\n", superBlock.availInodeNum, superBlock.availBlockNum);        
  ```

  <font size=4 face="楷体">	经检测上述实现可行；</font>

  

- <font size=5 face="楷体" color=#00bfff>键盘按键的串口回显实现：</font>

  <font size=4 face="楷体">	1、**设置门描述符**：在initIdt中增加键盘中断对应的门描述符；门描述符的设置通过调用setIntr函数实现，为此需要知道键盘中断的中断号、对应的代码段编号、键盘中断的处理函数与DPL等级；由手册可知键盘中断的中断号为0x21，键盘中断对应于内核代码段，irqKeyboard函数为键盘中断的处理函数，DPL应为内核级0，所以在idt表中添加键盘中断的门描述符需要在initIdt函数中添加：</font>

  ```c
  setIntr(idt + 0x21, SEG_KCODE, (uint32_t)irqKeyboard, DPL_KERN);
  ```

  <font size=4 face="楷体">	2、**完善中断服务例程**：在idt表中加入键盘中断的门描述符后，每次按键，内核就会调用注册好的irqKeyboard函数；分析框架代码中的`doIrq.S`可知，调用此函数后会依次**push** error code、中断号、各寄存器内容，然后跳转到irqHandle函数，由switch选择后跳转到keyboardHandle函数；在此函数中要实现：解析键盘传入的键码，得到要输出的字符信息，之后将要输出的字符输出到stdio上；这些功能在框架代码中已经实现了：解析键码由`kernel/keyboard.c`中的getKeyCode实现，根据code得到相应字符信息由`kernel/keyboard.c`中的getChar实现，最后的串口输出由`kernel/serial.c`中的putChar实现，利用这些接口，keyboardHandle的实现为：</font>

  ```c
  void keyboardHandle(struct TrapFrame *tf) {
  	// TODO in lab2
  	putChar(getChar(getKeyCode()));
  	return;
  }
  ```

  <font size=4 face="楷体">	完成了以上两步，键盘按键的串口回写就得到了实现。</font>

  

- <font size=5 face="楷体" color=#00bfff>printf处理例程实现：</font>

  <font size=4 face="楷体">	本实验中printf的调用由系统调用引起，类似于键盘中断，系统调用会通过执行irqSyscall跳转到syscallHandle，进而调用syscallWrite，最终跳转到syscallPrint函数；这个函数要将指定的字符串通过写显存方式显示在标准输出上，其中字符串首地址由tf->edx给出，长度由tf->ebx给出；
  	在框架代码中，syscallPrint通过一个循环一个个字符地输出字符串，当前的待输出位置，即光标位置由全局变量`displayRow`和`displayCol`指明；根据手册的提示，`asm volatile("movb %%es:(%1), %0":"=r"(character):"r"(str+i))`执行后，要打印的字符已经赋给`character`了，所以接下来需要对`character`的内容进行分析来决定如何处理：如果是回车的话，光标需要移动到下一行的起始处，所以要将`displayRow`加一，`displayCol`置零；如果不是回车的话，要将该字符打印到光标位置，这一操作可由手册给出的方法实现；之后要更新光标位置，因为输出了一个字符，所以`displayCol`加1，考虑到换行的情况，而且显示屏大小为`80*25`，所以，如果`displayCol == 80`则说明要换行了；完成这些后，最后要考虑滚屏的实现；所谓滚屏就是在输出满屏后，屏幕所以内容上移一行的操作；利用`kernel/vga.c`中的scrollScreen可以实现“所有内容上移一行”的操作，所以只要判断一下，在满屏时，即`displayRow == 25时`调用此函数实现滚屏，然后将`displayRow`减一，更新光标位置就可以了；
  	具体的代码实现为：</font>

  ```c
  void syscallPrint(struct TrapFrame *tf) {
  	int sel = USEL(SEG_UDATA); //TODO segment selector for user data, need further modification
  	char *str = (char*)tf->edx;
  	int size = tf->ebx;
  	int i = 0;
  	int pos = 0;
  	char character = 0;
  	uint16_t data = 0;
  	asm volatile("movw %0, %%es"::"m"(sel));
  	for (i = 0; i < size; i++) {
  		asm volatile("movb %%es:(%1), %0":"=r"(character):"r"(str+i));
  		// TODO in lab2
  		if(character == '\n') //如果为回车
  			{
  				displayRow++;
  				displayCol = 0;
  				//光标更新到下一行起始处
  			}
  		else
  			{
  				data = character | (0x0c << 8); 
              	pos = (80*displayRow+displayCol)*2;
              	asm volatile("movw %0, (%1)"::"r"(data),"r"(pos+0xb8000));
              	//打印该字符
  				displayCol++;
              	//更新光标
  				if(displayCol == 80)
  					{
  						displayRow++;displayCol = 0;
  					}
              	//换行处理
  
  			}
  		if(displayRow == 25)
  			{
  				scrollScreen();displayRow--;
  			}
  			//屏满时滚屏
  	}
  	
  	updateCursor(displayRow, displayCol);
      //由displayRow, displayCol更新光标信息
  	//TODO take care of return value
  }
  ```

  <font size=4 face="楷体">	</font>

- <font size=5 face="楷体" color=#00bfff>printf格式化处理实现：</font>

  <font size=4 face="楷体">	本阶段中要实现对printf格式化输出信息的处理；框架代码给出了printf最基本的内容，观察代码可知printf实现的思路是将format这一格式化字符串的内容存到buffer中，然后调用syscall，syscallWrite，利用syscallPrint实现打印；我们的任务就是解析format，将其内容填到buffer中；对于format这一格式化字符串的解析，可分为非法情况处理、'%'处理与合法字符处理三种情况：
		**对于非法情况的处理**：在printf要处理的格式化字符串format中，非法情况出现必然与特殊字符`\`与`%`有关，因为在format中，`\`表示转义符，而`%`表示格式转换说明符，这两个符号如果单独出现而后面没有合适的字符与其匹配，就会出现非法现象；在实验中，`\`无需我们考虑，因为在编译时`\`会作为转义符与下一个字符共同被解释，由`\`引起的非法情况会在编译时报错，例如在测试用例：`printf("#######################################################\n");`里加入一个`\`，进行编译就会报错：</font>
  
  <img src="C:\Users\njuwhl2019hp\Desktop\os\无标题3.png" style="zoom:67%;" />
  
  <font size=4 face="楷体">	所以在实验中，非法情况主要就是`%`的处理；在lab2中只实现`%d %x %c %s`四种格式转换说明符，因此认为合法情况就是`%d %x %c %s %%`5种，除了这5种情况外，其余情况都看作`%`单独出现；对于这种非法情况，printf会丢弃`%`这一字符；
		**对于`%`的处理**：即对合法的`%d %x %c %s %%`5种情况的处理；首先`%%`的处理是最简单的，它表示输出一个'%'字符，所以只要向buffer中加入一个'%'就可以了；对于前4种情况，则需要从栈中相应的位置取出对应的参数转换为字符（串）加入到buffer中。根据ics课程对函数调用的介绍可知，printf语句在编译时，在跳转到函数体内容执行前，会将format以及后面的参数从后往前依次push到栈中；例如语句`printf("format %d %x %c %s",1,0xa,'a',"aa")`在编译时，“aa”、'a'、0xa、1和"format %d %x %c %s"是依次压栈的；框架代码中的paraList指向了format在栈中的地址，index表示参数的编号，所以不难推出`paraList + index * 4`指向的就是第index个元素的地址（栈向低地址增长，每次push压入4个字节）；故对于第index个参数，如果是`%d`对应的参数，`*(int *)(paraList + index * 4)`可以取出该整型元素；如果是`%x`对应的参数，`*(unsigned int*)(paraList + index * 4)`可以取出该元素；如果是`%c`对应的参数，`*(char *)(paraList + index * 4)`可以取出该字符型元素；如果是`%s`对应的参数，`*(unsigned int*)(paraList + index * 4)`可以取出该字符串的首地址值，将该值强制转化为char*型就可以得到字符串的起始；然后，通过调用提供的dec2Str、hex2Str、str2Str可以将得到的值/字符串转化为一个个字符并依次放入buffer中(`%c`可以直接加入到buffer中)，就完成了对`%`的处理；
  	**合法字符处理**：将该字符放入buffer中即可；
		基于以上分析，设计printf的格式化处理实现如下：
  	首先，判断当前循环一次处理的format[i]是否为'%'，从而决定此次处理属于哪种情况；state初始为0，表示处理的是合法字符，如果format[i]是'%'，而且format[i+1]是'c'、'd'、'x'、's'之一，则需要涉及后面参数的处理，这里就设state = 1表示要处理参数；如果format[i]是'%'，而且format[i+1]是'%'，简单处理为i++，state保持为0；这样，之后就会按普通字符处理后一个'%'，将其加入buffer，使得整体的效果就是"%%"是向buffer中加入了一个'%'；如果format[i]是'%'，但format[i+1]不是以上情况，则设state = 2表示非法情况（'%'单独出现）；</font>
  
  ```c
  if(format[i] == '%')
	{						                                                           	   if(format[i+1]=='c'||format[i+1]=='d'||format[i+1]=='x'||format[i+1]=='s')
  				state = 1;
     			//涉及后面的参数处理
  		else if(format[i+1]!='%')
  				state = 2;
       			//非法情况
  		else if(format[i+1] == '%')
  				i++;
       			//“%%”，只向buffer加入一个'%'
  	}
  ```
  
  <font size=4 face="楷体">	设置好state的值后，就要进行相应的处理：如果state = 1，则要根据'%'后的字符进行相应的处理，将对应的参数转化为字符串后加入到buffer中；如果state = 0，则将当前format的字符加入到buffer中；如果state = 2，则跳过此非法字符；代码实现为：</font>
  
  ```c
  		if(state == 1)
  		{
  			i++;
              //取'%'的后一个字符
  			index++;
              //参数编号增加
  			switch(format[i]) {
  				case 'c':
                      //参数为字符
  					character = *(char *)(paraList + index * 4);
                      //从栈中取出该元素，取出数据类型为char
  					buffer[count] = character;
  					count++;
                      //将取出字符加入到buffer中
  					break;
  				case 'd':
                      //参数为十进制整数
  					decimal = *(int *)(paraList + index * 4);
                      //从栈中取出该元素，取出数据类型为int
  					count = dec2Str(decimal,buffer,MAX_BUFFER_SIZE,count);
                      //调用dec2Str函数，将decimal转为字符串后加入到bufer中
  					break;
  				case 'x':
                      //参数为十六进制整数
  					hexadecimal = *(uint_32*)(paraList + index * 4);
                      //从栈中取出该元素，取出数据类型为uint32_t
  					count = hex2Str(hexadecimal,buffer,MAX_BUFFER_SIZE,count);
                      //调用hex2Str函数，将hexadecimal转为字符串后加入到bufer中
  					break;
  				case 's':
                      //参数为字符串
  					temp = *(uint_32*)(paraList + index * 4);
                      //从栈中取出该参数的首地址，数据类型为uint32_t
  					string = (char *)temp;//字符串首地址转换为char*类型，赋值给string
  					count = str2Str(string,buffer,MAX_BUFFER_SIZE,count);
                      //调用str2Str函数，将string的内容加入到bufer中
  					break;
  			}
  			i++;
  			state = 0;
              //'%'处理完毕，进入下一次循环，state恢复为0
  		}			
  		else if(state == 0)//合法字符处理
  		{
  			buffer[count]=format[i];
  			count++;
              //字符加入到buffer中
  			i++;
              //进入下一次循环
  		}
  		else//处理非法字符
  			{
  				i++;
              	//跳过当前非法字符
  				state = 0;
              	//进入下一次循环，state恢复为0
  			}
  ```
  
  <font size=4 face="楷体">	补充完成后，向`app/main.c`中添加给好的测试代码，经验证，输出符合预期。</font>

### <font face="楷体">四、思考与心得</font>

<font size=4 face="楷体">	本次实验中关于完成实验内容的思考可见本文档第三部分的“修改思路”。
	**本次实验讲义中提到的思考内容**：</font>

-  <font size=4 face="楷体">**与存储管理机制进行比较**</font>

   <font size=4 face="楷体">	内存分段、分页机制与ext文件系统有相似之处，都有通过块划分、基址加偏移量、多级索引等方式来进行管理；相较于分页机制对物理页的管理，ext文件系统中一个文件的大小并非固定的；有的文件大小很大，有的很小；如果都用二级索引来存储的话，对于小文件而言反而是浪费了存储块，同时也增加了索引时间的开销，对于大文件则可能出现无法将整个文件内容表示下来的情况，因此不应该采用固定的二级索引。</font>
   
-  <font size=4 face="楷体">**表上参数的计算**</font>

   <font size=4 face="楷体">	块大小为1KB时，一个文件最大大小包含了数据块的直接索引、一、二、三级索引；按照一个指针4字节来看，一个块中会包含256个指针，因此可以计算一个文件的最大大小为：</font>
   $$
   12*1+256*1+{256}^{2}*1+{256}^{3}*1 =16843020(KB)\approx16GB
   $$
   <font size=4 face="楷体">	在32位块地址索引下，文件系统最多可以有2^32个块，所以文件系统最大大小为：</font>
   $$
   1KB*{2}^{32}=4TB
   $$
   <font size=4 face="楷体">	块大小为2KB时，按照一个指针4字节来看，一个块中会包含512 = 2^9个指针，因此可以计算一个文件的最大大小为：</font>
   $$
   12*2+512*2+{512}^{2}*2+{512}^{3}*2=268960792(KB)\approx256GB
   $$
   <font size=4 face="楷体">	在32位块地址索引下，文件系统最多可以有2^32个块，所以文件系统最大大小为：</font>
   $$
   2KB*{2}^{32}=8TB
   $$
   <font size=4 face="楷体">	块大小为4KB时，按照一个指针4字节来看，一个块中会包含1024个指针，因此可以计算一个文件的最大大小为：</font>
   $$
   12*4+1024*4+{1024}^{2}*4+{1024}^{3}*4=4299165744(KB)\approx4TB
   $$
   <font size=4 face="楷体">	在32位块地址索引下，文件系统最多可以有2^32个块，所以文件系统最大大小为：</font>
   $$
   4KB*{2}^{32}=16TB
   $$
   
-  <font face="楷体" size=4>**幽灵空间问题**</font>

   <font size=4 face="楷体">	因为磁盘是按照一个个分配单元来存放每个磁盘上的文件的，而且一个分配单元只能存放一个文件的内容，所以存放这个大小为1字节的文本文件，磁盘也会给其分配一个分配单元；此分配单元的大小远远大于一个字节，又只能存放这一个文件的信息，所以size on disk会远大于一个字节。</font>
   
-  <font face="楷体" size=4>**Linux不允许对目录创建硬链接，你知道为什么吗？如果允许对目录创建硬链接，你能否举出一个影响系统工作的例子？**</font>

   <font size=4 face="楷体">	因为Linux下对目录创建硬链接会带来目录结构混乱、在目录中引入循环，使得遍历目录无限循环，无法正确定位目录这样的问题；如果允许对目录创建硬链接，则可以：有A、B两个目录，在A下建立B的硬链接C，在B下建立A的硬链接D；则ls A得到C，ls C得到D，ls D又得到C，从而陷入到一个无限循环之中。</font>
   
-  <font face="楷体" size=4>**IA-32提供了四个特权级，但TSS中只有3个堆栈位置信息，分别用于ring0、ring1、ring2的堆栈切换，为什么TSS中没有ring3的堆栈信息？**</font>

   <font size=4 face="楷体">	TSS用于程序在外层到内层的切换过程中寻找内层堆栈信息的情况下；从外层切换到内层时，会从TSS中加载内层的ss和esp，然后将外层的ss和esp压入内层栈中；之后返回时外层的ss和esp可以从栈中恢复，所以TSS没有必要包含最外层ring3的堆栈信息。</font>
   
-  <font face="楷体" size=4>**我们在使用eax、ecx、edx、ebx、esi、edi前将寄存器的值保存到了栈中，如果去掉保存和恢复的步骤，从内核返回以后会不会产生不可恢复的错误？**</font>

   <font size=4 face="楷体">	如果没有保存和恢复的步骤，那么在内核代码执行过程中就会不可避免地修改这些寄存器里的值，之后执行完返回时，寄存器的值已经不是原来的值了，即现场信息无法恢复；这样继续之后的执行自然会与预期不同，进而导致错误。</font>



<font size=4 face="楷体">	**本次实验的问题与心得**：</font>

<font size=4 face="楷体">	本次实验中遇到的问题在printf的格式化输出实现阶段；问题就是如果参数是一个计算表达式而非一个数值的话应该如何处理（即测试用例中的1 + 1与123 * 456）；一开始我觉得需要写一个表达式计算器来计算出这个表达式的值；基于这个思路，我首先要得到计算表达式这个字符串，但这就矛盾了：参数是计算表达式时，其内容只是一个计算式，而非一个字符串，所以编译时编译器不会将这个计算式的首地址指针压栈；想到这里时，另一个问题就出现了：编译器如何将这个计算式参数压栈来完成传参呢？传参利用的是push，那么计算式必然不可能被压栈，如果计算式是一个一个被压栈的话则太过繁琐，那编译器如何处理？基于这个问题我搜索了一下，找到了“常量表达式”这个内容，从搜索到的资料来看，像1 + 1与123 * 456这样的值不会变化的计算式，编译器在编译时就会知道它的值；这样来看，实际上这些计算式是由编译器在编译时就计算好的，之后传参就直接将计算结果压栈；所以这个问题并不存在，直接去相应栈的位置取值就可以了。
	完成本次实验，使我初步了解了层次结构文件系统的内容，对于文件与块划分、寻找等内容有了简单的认识；同时，也复习了在ics课程学习的写磁盘、串口输出、中断机制实现、系统调用等内容；TR和TSS的介绍解决了我对于内核栈与用户栈切换的疑惑；此外，框架代码中使用的可变参数机制（…）也让我学到了新的C语言知识；对于遇到问题的解决也让我认识到从错误的思路出发，发现矛盾，进而找到正确的思路也不失为一种好的方法。</font>