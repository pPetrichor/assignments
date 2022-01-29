# <p align="center"><font face="楷体">OS lab3报告</p></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖
学号：181860092
邮箱：603102048@qq.com</font></p>
[TOC]

### <font face="楷体">一、实验进度</font>

<font size=4 face="楷体">	完成了实验的以下内容：完成了四个库函数；完成了`timeHandle`函数，实现了时钟中断的处理；实现了`syscallFork、syscallSleep、syscallExit、syscallExec`四个系统调用例程；通过了中断嵌套测试；设计了测试用例，验证了全局变量`displayCol`与`displayRow`的更新一致性问题。</font>



### <font face="楷体">二、实验结果</font>

<font size=4 face="楷体">	测试用例执行：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab3_1.png)

<font size=4 face="楷体">	中断嵌套测试结果(fork时间会变长，需要等待几秒)：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab3_4.png)

<font size=4 face="楷体">	`displayCol`与`displayRow`更新一致性问题的验证：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab3_2.png)

![](C:\Users\njuwhl2019hp\Desktop\os\lab3_3.png)



### <font face="楷体">三、实验修改的代码</font>

- <font size=4 face="楷体">库函数实现：在`lab3/lib/syscall.c`中补充完成fork、exec、sleep、exit四个函数；</font>
- <font size=4 face="楷体">时钟中断处理：在`lab3/kernel/kernel/irqHandle.c`中补充完成函数timerHandle；</font>
- <font size=4 face="楷体">系统调用历程实现：在`lab3/kernel/kernel/irqHandle.c`中补充完成函数`syscallFork、syscallSleep、syscallExit、syscallExec`；同时增添完成了`lab3/kernel/kernel/kvm.c`中、syscallExec中调用的`loadElf`函数；</font>
- <font size=4 face="楷体">中断嵌套测试：在`lab3/kernel/kernel/irqHandle.c`中的syscallFork函数拷贝父进程内存时增加开中断、`asm volatile("int $0x20")`、关中断的代码，手动模拟时钟中断；</font>
- <font size=4 face="楷体">全局变量更新一致性问题检验：需要更改`lab3/app/main.c`的内容；同时需要更改timeHandle与syscallPrint函数；考虑到只改main.c无法实现，故不独立出来作为新的用户程序；具体的改动在提交的代码中会注释或删除掉，验证需要参看报告第四部分最后的“全局变量更新一致性问题”；</font>

### <font face="楷体">四、实验思路以及实验遇到的问题与解决</font>

- <font size=5 face="楷体" color=00bfff>库函数实现：</font>

  <font size=4 face="楷体">	4个库函数的实现十分简单，就是执行系统调用函数`syscall`，将系统调用号等作为参数传入就可以了；需要注意的是这些库函数不应该总是执行`return 0`，而应该返回`syscall`的返回值，否则可能会导致返回值错误（最典型的例子就是`fork()`，如果保留`return 0`则父进程的返回值也会是0，以测试用例来说父进程也会跳转到子进程分支执行）；具体添加代码为：</font>
  
  ```C
pid_t fork() {
  	// TODO in lab3
	return syscall(SYS_FORK, 0, 0, 0, 0, 0);
  }
  
  int exec(const char *filename, char * const argv[]) {
  	// TODO in lab3
  	return syscall(SYS_EXEC, (uint32_t)filename, (uint32_t)argv, 0, 0, 0);
  }
  
  int sleep(uint32_t time) {
  	// TODO in lab3
  	return syscall(SYS_SLEEP, time, 0, 0, 0, 0);
  	
  }
  
  int exit() {
  	// TODO in lab3
  	return syscall(SYS_EXIT, 0, 0, 0, 0, 0);
  	
  }
  ```
  
- <font size=5 face="楷体" color=00bfff>`timeHandle`函数实现：</font>

  <font size=4 face="楷体">	根据手册提示，时钟中断有两个功能，一是更新状态为STATE_BLOCKED的进程，一是更新当前进程并判断是否要切换进程，如果要则切换；对于第一个功能，只需要遍历所有的pcb找出所有状态为STATE_BLOCKED的进程，将其`sleepTime`减1（由于会对sleep函数传入的time值进行检验，保证`sleepTime`的初始值为正数，所以可以先将`sleepTime`减一），减1后判断`sleepTime`是否为0，若为0说明sleep结束，则将该进程的状态改为STATE_RUNNABLE；否则进程仍保持STATE_BLOCKED状态。故在`timeHandle`中添加以下代码：</font>

  ```c
  	int i = 0;
  	for (i = 0; i < MAX_PCB_NUM; i++) {
  		if(pcb[i].state == STATE_BLOCKED)
  		{
              //找到阻塞进程，睡眠时间--
  			pcb[i].sleepTime--;
  			if (pcb[i].sleepTime == 0)
  			{
                  //睡眠结束
  				pcb[i].state = STATE_RUNNABLE;
  			}
  		}
  	}
  ```

  <font size=4 face="楷体">	对于第二个功能，需要对当前正在运行的进程进行更新；实现这里时遇到的问题就是没有注意一点：当前运行的进程的状态不一定为STATE_RUNNING；例如一个运行中进程执行了exit函数，那么执行后其状态为STATE_DEAD；如果简单地将`pcb[current]`的状态认定为STATE_RUNNING，那么进行切换时就要将该进程状态设置为STATE_RUNNABLE，从而导致错误；故需要区分`pcb[current]`的状态是否为STATE_RUNNING进行处理：
  	如果当前进程状态是STATE_RUNNING，则判断它的时间片是否用完（判断`pcb[current].timeCount`与`MAX_TIME_COUNT`的大小），如果没用完则将其`timeCount`加一，否则要切换进程：切换进程首先要找到一个状态为STATE_RUNNABLE的进程作为接下来要执行的进程：利用change标记找到的进程，初始化为-1表示无，然后遍历所有pcb寻找STATE_RUNNABLE的进程：</font>

  ```c
  			int change = -1;
  			int i = 0;
  		
  			for (i = current + 1; i < MAX_PCB_NUM; i++)
  			{
                  //遍历查找pcb[current+1]-pcb[MAX_PCB_NUM-1]的“后半段进程”是否有STATE_RUNNABLE的进程
  				if (pcb[i].state == STATE_RUNNABLE)
  				{
  					change = i;
                      //有则记录到change中
  					break;
  				}
  
  			}
  			if (change == -1)
  			{
                  //“后半段进程”中没有进程满足条件，到pcb[1]-pcb[current-1]的“前半段进程”中查找
  				for (i = 1; i < current; i++)
  				{
  					if (pcb[i].state == STATE_RUNNABLE)
  					{
  						change = i;
                          //若找到则标记
  						break;
  					}
  				}
  			}
  ```

  <font size=4 face="楷体">	这样遍历完之后，如果找到了其它状态为STATE_RUNNABLE的进程，则change就记录了进程号；如果没有找到，则change为-1；若change为-1，则说明除了IDIE进程和自己外没有状态为STATE_RUNNABLE的进程（自己为IDIE时也不冲突），根据课上说的，应该回到自己继续执行，故执行：</font>

  ```c
  if (change == -1) change = current;
  ```

  <font size=4 face="楷体">	这样，最终要切换的进程号就记录在change中了；接下来，就要将进程切换到pcb[change]：由于pcb[current]状态为STATE_RUNNING，所以要将它设置为STATE_RUNNABLE后将`timeCount`赋为0；然后将current赋值为change，将新的pcb[current]状态设置为STATE_RUNNING：</font>

  ```c
  			pcb[current].state = STATE_RUNNABLE;
  			pcb[current].timeCount = 0;
  			current = change;
  			pcb[current].state = STATE_RUNNING;
  			pcb[current].timeCount = 0;
  ```

  <font size=4 face="楷体">	最后，利用手册给出的进程切换代码，切换到新的pcb[current]执行就可以了；
  	如果当前运行进程状态不是STATE_RUNNING，则该进程一定是被阻塞或exit了，因此不用管时间片是否用完，一定要进行进程切换：同样的，在遍历了所有的pcb（除了自己和pcb[0]）之后，如果change是-1，则说明没有找到可以切换的进程，而自己又不是STATE_RUNNING，不能切换回自己，只能切换到IDIE，而且由于不是STATE_RUNNING，故切换后不会更新该进程状态为STATE_RUNNABLE。故执行：</font>

  ```c
  		if (change == -1) change = 0;
  		current = change;
  		pcb[current].state = STATE_RUNNING;
  		pcb[current].timeCount = 0;
  ```

  <font size=4 face="楷体">	然后，利用手册给出的进程切换代码，切换到新的pcb[current]执行就可以了；</font>

- <font size=5 face="楷体" color=#00bfff>系统调用例程实现：</font>

  <font size=4 face="楷体">	1、**syscallFork**：
  	`syscallFork`的实现，就是找一个空闲的，也就是状态为STATE_DEAD的进程，将自己的资源拷贝给它，从而产生自己的子进程；由于执行`fork`的进程状态一定不会是STATE_DEAD，IDIE不会dead，所以直接遍历寻找就可以了：</font>

  ```c
  	int empty = -1;
  	int i = 0;
  	for (i = 0; i < MAX_PCB_NUM; i++)
  	{
  		if (pcb[i].state == STATE_DEAD)
  		{
  			empty = i;
  			break;
  		}
  	}
  ```

  <font size=4 face="楷体">	如果遍历后empty值为-1，则说明没有空的进程可以用来产生子进程，故此时`fork()`失败，返回值设置为-1：</font>

  ```c
  	pcb[current].regs.eax = -1;
  ```

  <font size=4 face="楷体">	如果遍历后empty值不为-1，则找到了可以作为子进程的pcb，接下来就要利用父进程来完善子进程的信息了：首先，要利用手册给出的方法将父进程的内存拷贝到子进程中：</font>

  ```c
  		int j = 0;   
  		for (j = 0; j < 0x100000; j++) {      
  			 *(uint8_t *)(j + (empty + 1) * 0x100000) = *(uint8_t *)(j + (current + 1) * 0x100000);    
  			 } 
  ```

  <font size=4 face="楷体">	接下来考虑该进程对应的pcb的设置：</font>

  <font size=4 face="楷体">	对于1个pcb来说，其结构如下：</font>

  ```c
  	uint32_t stack[MAX_STACK_SIZE];
  	struct TrapFrame regs;
  	uint32_t stackTop;
  	uint32_t prevStackTop;
  	int state;
  	int timeCount;
  	int sleepTime;
  	uint32_t pid;
  	char name[32];
  ```

  <font size=4 face="楷体">	根据内核栈增长顺序分析：起始是name与pid，这两个是每个进程独有的，不能改变；sleepTime应该继承父进程的（一般情况下父、子进程的sleepTime都应该是0），对于timeCount我按照徐峰老师说过的父子进程时间片均分进行了实现；state设置为STATE_RUNNABLE；接着是stackTop与prevStackTop，这里我按照pcb[1]的初始化方式处理了：</font>

  ```c
  		pcb[empty].stackTop = (uint32_t)&(pcb[empty].regs);
  		pcb[empty].prevStackTop = (uint32_t)&(pcb[empty].stackTop);
  		pcb[empty].state = STATE_RUNNABLE;
  		int temp = (MAX_TIME_COUNT - pcb[current].timeCount)/2;
  		//父进程时间片均分
  		pcb[current].timeCount += temp;
  		pcb[empty].timeCount = pcb[current].timeCount;
  		//父、子进程各占之前时间片一半
  		pcb[empty].sleepTime = pcb[current].sleepTime;
  		pcb[empty].pid = empty;
  ```

  <font size=4 face="楷体">	接着是regs，这里不同的进程对应的段在初始化时是不同的，所以段寄存器的值应该是由子进程计算的；而其它寄存器（除eax）的值应该继承父进程的值，这样在之后切换到子进程执行，子进程执行一系列pop和iret后，就可以得到与父进程返回用户态时相同的状态（例如regs.esp记录了父进程陷入内核时下一条指令的地址，子进程返回用户态时也该从自己用户栈中相同的位置继续执行）；
  	由`initSeg`结合pcb[1]的初始化可以确定pcb[i]的代码段为USEL(i\*2 + 1)，数据段（以及其他段）为USEL(i\*2 + 2)；故增加以下初始化pcb的代码：</font>

  ```c
  		pcb[empty].regs.ss = USEL(empty*2 + 2);
  		pcb[empty].regs.esp = pcb[current].regs.esp;
  		……
  		pcb[empty].regs.cs = USEL(empty*2 + 1);
  		……
  		pcb[empty].regs.ds = USEL(empty*2 + 2);
  		……
  		pcb[empty].regs.ecx = pcb[current].regs.ecx;
  		……
  ```

  <font size=4 face="楷体">	对于stack变量，考虑到子进程初始化会将stackTop设为&regs，故子进程需要的只是regs的内容，stack的内容不需要，也就无需复制；
  	最后通过eax设置返回值即可：</font>

  ```c
  		pcb[empty].regs.eax = 0;
  		pcb[current].regs.eax = empty;
  ```

  <font size=4 face="楷体">	2、**syscallSleep与syscallExit**：syscallSleep需要从tf中合适位置取出time值，time值不应为负，然后将当前进程阻塞，设置sleepTime为time即可；syscallExit直接设置进程状态为STATE_DEAD即可；二者最后都要调用int 20，利用时间中断切换进程；syscallSleep判断time是否合法的过程为：</font>

  ```c
  void syscallSleep(struct TrapFrame *tf) {
  	// TODO in lab3
  	int t = tf->ecx;//time是第二个传的参数，故从tf->ecx取
  	if (t <= 0)//time不应为负
  	{
  		putString("Error SleepTime\n");
  		return;
  	}
  ……
  }
  ```

  <font size=4 face="楷体">	3、**syscallExec**：按照手册的思路分三步执行；
  	首先，参考`syscallPrint`读取传入的文件名：`syscallPrint`是从tf中取出了数据段ds、字符串首地址与字符串长度，然后利用循环，将字符串中字符一个个取到`character`变量中进行后续操作；模仿此可以实现对传入文件名的读取：从tf中取出数据段ds与文件名字符串首地址；用一个定长数组`character[30]`存放文件名字符串；利用while循环，以'\0'为结束标志依次将文件名内容一个个读到`character`数组中：</font>

  ```c
  	int sel = tf->ds;
  	char* tmp = (char*)tf->ecx;
  	//uint32_t argv = tf->edx;
  	int i = 0;
  	char character[30];
  	uint32_t entry = 0;
  	asm volatile("movw %0, %%es"::"m"(sel));
  	while(1)
  	{
  		asm volatile("movb %%es:(%1), %0":"=r"(character[i]):"r"(tmp + i));
          //将第i个字符读入character[i]中
  		if (character[i] == '\0') break;
  		i++;
  	}
  ```

  <font size=4 face="楷体">	接下来根据注释提示，要执行：</font>

  ```c
  	uint32_t ret = loadElf(character, (current + 1) * 0x100000, &entry);
  ```

  <font size=4 face="楷体">	通过调用loadElf将对应文件加载到内存，因此要完善`kvm.c`中loadElf的内容：根据手册2.1.1节提示，loadElf应该首先加载elf文件，然后根据elf文件的内容将需要加载的段加载到内存中；加载elf文件的操作我是参考了`kvm.c`中loadUMain函数进行实现的：阅读loadUMain函数可以看出它通过调用readInode函数与readBlock函数将elf文件拷贝到了以`elf`起始的地址；对于loadElf也是一样的，设置变量`elf`，赋值为传入的参数`physAddr`，然后仿照loadUMain调用readInode函数与readBlock函数，这样就将elf文件内容拷贝到了当前进程的内存起始处了；在这个过程中如果有函数返回-1，说明执行失败，loadElf直接返回-1;
  	接下来要阅读elf文件，将需要load的段加载到指定的内存区域；这一步的实现我参考了ics课程中的elf读取过程：由于elf文件被拷贝到了变量`elf`开始的地址处，所以可以将变量`elf`作为elf头使用来得到：执行用户进程的入口、程序头的偏移量和程序头的数目：</font>

  ```c
  		*entry = ((struct ELFHeader *)elf)->entry; // entry address of the program
  		//entry作为传入的指针参数会保存用户程序入口值，用于syscallExec返回值设置
  		phoff = ((struct ELFHeader *)elf)->phoff;
  		phnum = ((struct ELFHeader *)elf)->phnum;
  		//PS:变量均在函数起始处定义
  ```

  <font size=4 face="楷体">	根据phoff与phnum可以计算出所有程序头的范围，用[ph,phend]表示，则ph与phend取值为：</font>

  ```c
  		ph = (struct ProgramHeader*)(elf + phoff);
  		phend = (struct ProgramHeader*)(ph + phnum);
  ```

  <font size=4 face="楷体">	有了这些信息，就可以对所有的程序头进行遍历，找到type为LOAD即0x1的程序头，该头将包含：要加载的段在elf文件中的偏移量off、实际文件大小filesz、占用内存大小memsz与该段应该被加载到内存中的起始虚拟地址vaddr；要利用这些信息，将段的内容拷贝到[vaddr, vaddr + memsz)对应的物理地址，然后将[vaddr + lesz, vaddr + memsz)对应的物理地址取0；利用规定好的内存划分规则可知，vaddr就是地址对于其进程起始地址physAddr的偏移量，所以physAddr + vaddr就是vaddr对应的物理地址；故问题转化为：需要拷贝的段内容为[elf + off，elf + off + filesz)，要将这部分拷贝到[physAddr + vaddr, physAddr + vaddr + fliesz)，然后将[physAddr + vaddr + fliesz, physAddr + vaddr + memsz)清零；拷贝过程即一个字节一个字节拷贝：</font>

  ```c
  		for (;ph < phend; ++ph)//遍历所有程序头
  		{
          	if(ph->type==0x1) //该段需要被加载
          	{
  				for (i = 0; i < ph->filesz; i++)
  					*(uint8_t *)(physAddr + i + ph->vaddr) = *(uint8_t *)(elf + i + ph->off);//加载内容
  				for (; i < ph->memsz; i++)
  					*(uint8_t *)(physAddr + i + ph->vaddr) = (uint8_t)0;//清零
          	}
  		}
  ```

  <font size=4 face="楷体">	这样，就完成了loadElf函数；
  	最后一步，就是设置返回值；exec函数执行成功不会返回值，只有失败才返回-1；而ret变量就记录了loadElf是否成功，若ret为0执行成功，则将当前进程eip设置为用户程序入口entry，否则执行失败，设置当前进程eax为-1作为返回值；
  	完成以上实现后，在Ubuntu下运行，结果正确；但是同样的程序在debian下运行就失败了；通过比对、读取elf文件发现区别在于二者elf文件中可加载段数目不同：前者只有1个，但后者有4个；打印可加载段的各种信息发现，第一个可加载段打印出来的信息与elf文件中是一样的，但之后打印的信息全都与elf文件中的不同；进一步调试观察到第一个可加载段的vaddr为0x0，memsz为0x767，则加载了此段之后，当前进程虚拟地址[0,0x767)，对应物理地址[physAddr,physAddr + 0x767)就成为了第一个段的内容；但是由于变量elf设置为physAddr，elf文件也是存储在physAddr开始的地址，因此第一个段加载后就会将elf文件开始的一部分内容覆盖，而程序头正是被覆盖的一部分，所以之后ph访问到的地址存储的并不是程序头内容，自然就错误了。
  	明确了错误后，修改的思路也很简单了：只要保证elf文件中需要加载段的信息与加载段的内容不会因冲突丢失就好了；起初我的想法是找一个空闲的进程，将elf加载到空闲进程的内存中，这样就不会冲突了；之后该进程被使用到时，会拷贝内存重新覆盖，所以没有影响；具体的做法就是先遍历所有的pcb找到空闲的，然后将elf设置为该进程的内存起始处就好了：</font>

  ```c
  	int t = 1;
  	for (t = 0; t < MAX_PCB_NUM; ++t)
  	{
  		if (pcb[t].state == STATE_DEAD)
  			break;
  	}
  	elf = (t + 1) * 0x100000;
  ```

  <font size=4 face="楷体">	不过这样修改又会有另一个问题：如果没有空闲的进程，上述处理就行不通了；为此，在找不到空闲进程（即遍历后 t == MAX_PCB_NUM）的情况下，仍然将elf文件拷贝到自己进程的内存中，只是要在拷贝段到内存前先遍历一次程序头，将所有需要加载的段的信息记录下来，然后根据记录的信息加载段，依次避免覆盖导致的错误：</font>

  ```c
  	if(t != MAX_PCB_NUM)
  	{	
  		elf = (t + 1) * 0x100000;
  		……
  	}
  	else
  	{
          elf = physAddr;
  		……
  		uint32_t vaddrs[10];
          uint32_t offs[10];
  		uint32_t fileszs[10];
          uint32_t memszs[10];
          //用于记录可加载段信息
  		int n = 0;
  		for (;ph < phend; ++ph)
  		{
         		//先遍历一次，记录下要加载的段的信息
          	if(ph->type==0x1)
          	{
  				vaddrs[n] = ph->vaddr;
  				offs[n] = ph->off;
  				fileszs[n] = ph->filesz;
  				memszs[n] = ph->memsz;
  				n++;
          	}
  		}
  		for(i=0;i<n;++i)
          //根据记录的信息加载
  		{	
  			for (t = 0; t < fileszs[i]; t++)
  				*(uint8_t *)(physAddr + t + vaddrs[i]) = *(uint8_t *)(elf + t + offs[i]);
  			for (; t < memszs[i]; t++)
  				*(uint8_t *)(physAddr + t + vaddrs[i]) = (uint8_t)0;
  		}
  	}
  ```

  <font size=4 face="楷体">	至此，syscallExec函数实现完毕。</font>

- <font size=5 face="楷体" color=#00bfff>中断嵌套测试</font>

  <font size=4 face="楷体">	中断嵌套即在执行一个中断处理程序时产生了一个优先级更高的中断，从而导致当前的中断处理程序被中止然后转向优先级更高的中断处理程序执行，执行之后再返回之前的中断处；对于本实验来说，就是优先级较低的系统调用被时钟中断中止，转向timeHandle执行，然后返回之前中断处继续执行的过程；
  	进一步分析此过程，假设进程A执行了某个系统调用，那么int指令会将cs、esp等内容先push到一个陷阱帧tf1中，然后将错误码、中断号、寄存器值等压入，接着在irqHandle函数中，将执行进程的stackTop赋值为tf1，用tmpStackTop1记录了之前的stackTop值；因此，这就等效于将cs、esp等内容压入了进程A的内核栈（虽然这个内核栈在物理地址上可能是不连续的）；然后才会执行系统调用的例程；在执行系统调用例程中，A突然被时钟中断中止，即执行到`int 20`，则int 20会再次将cs、esp等内容push到一个陷阱帧tf2中，然后在irqHandle函数中将stackTop赋值为tf2；注意到在给stackTop赋值前，irqHandle函数还执行了：</font>
  
  ```
  	uint32_t tmpStackTop = pcb[current].stackTop;
  	pcb[current].prevStackTop = pcb[current].stackTop;
  ```

<font size=4 face="楷体">	因此，tmpStackTop2与pcb.prevStackTop就记录了A在被中断之前的内核栈的stackTop值（即tf1），同时，A执行到int 20后会将下一条指令的地址压入tf2中，故形成以下结构：</font>

```
  +---------------+                 
  |      PID      |                 
  +---------------+                
  |   SLEEPTIME   |                 
  +---------------+               
  |   TIMECOUNT   |              
  +---------------+              
  |     STATE     |              
  +-------+-------+<----+			 +-------+-------+
  |#######|  SS   |     |            |#######|  SS   |
  +---------------+	tmpStackTop1	 +---------------+  
  |      ESP      |                  |      ESP      | <----+  
  +---------------+                  +---------------+ 	    |
  |     EFLAGS    |                  |     EFLAGS    |	记录值为地址x	
  +-------+-------+                  +-------+-------+
  |#######|  CS   |                  |#######|  CS   |
  +-------+-------+                  +-------+-------+ 
  |      EIP      |                  |      EIP      |
  +---------------+                  +---------------+
  |     ERROR     |                  |     ERROR     | 
  +---------------+                  +---------------+         
  |      IRQ      |                  |      IRQ      |         
  +---------------+                  +---------------+         
  |      EAX      |                  |      EAX      |         
  +---------------+                  +---------------+         
  |      ECX      |                  |      ECX      |         
  +---------------+                  +---------------+         
  |      EDX      |                  |      EDX      |        
  +---------------+                  +---------------+         
  |      EBX      |                  |      EBX      |         
  +---------------+                  +---------------+         
  |      XXX      |                  |      XXX      |         
  +---------------+                  +---------------+         
  |      EBP      |                  |      EBP      |         
  +---------------+                  +---------------+         
  |      ESI      |                  |      ESI      |         
  +---------------+                  +---------------+         
  |      EDI      |                  |      EDI      |         
  +-------+-------+                  +-------+-------+         
  |#######|  DS   |                  |#######|  DS   |         
  +-------+-------+                  +-------+-------+         
  |#######|  ES   |                  |#######|  ES   |         
  +-------+-------+                  +-------+-------+         
  |#######|  FS   |                  |#######|  FS   |        
  +-------+-------+   tf1		     +-------+-------+   tf2（当前的stackTop） 
  |#######|  GS   |     |            |#######|  GS   |    |          
  +-------+-------+<----+            +-------+-------+<----+
  |###############|                  |               |        
  +-------+-------+<----+            +-------+-------+<----+
  |###############|     |
  					int 20下一条指令地址x
此时pcb.prevStackTop与tmpStackTop2记录的是tf1
```

<font size=4 face="楷体">	所以，在timeHandle执行，如果A的时间片没有用完，则继续进程A的执行：timeHandle返回后，irqHandle会将pcb.stackTop设置为tmpStackTop2（即tf1），然后执行`doIrq.S`中的一系列pop与iret，而iret取出的esp值就是x，所以这样就会跳转到地址x继续执行；即处理完时钟中断后返回了A被中断的地址继续执行；如果A的时间片用完，则会切换到其它进程执行；之后切换回A执行时，在timeHandle切换进程时会执行：</font>

```c
			tmpStackTop = pcb[current].stackTop;
			pcb[current].stackTop = pcb[current].prevStackTop;
			tss.esp0 = (uint32_t)&(pcb[current].stackTop);
			asm volatile("movl %0, %%esp"::"m"(tmpStackTop)); // switch kernel stack
			asm volatile("popl %gs");    
			……
```

<font size=4 face="楷体">	即：用tmpStackTop记录当前pcb.stackTop，具体到上述场景就是tf2，然后pcb.stackTop更新为pcb.prevStackTop，即tf1，然后设置TSS，设置esp为tmpStackTop（即tf2）后执行pop与iret，也返回到了地址x继续执行，所以与不切换进程最终的效果是一样的；
	因此，按照手册中的进程切换方式结合irqHandle增加的保存恢复部分，就已经实现时钟中断打断系统调用的中断嵌套了；按照手册的测试方式进行测试，可以通过。</font>

- <font size=5 face="楷体" color=#00bfff>全局变量更新一致性问题</font>

  <font size=4 face="楷体">	全局变量更新一致性问题的验证我主要是参考手册2.5部分考虑的；手册2.5中给出了一个产生更新一致性问题的场景，核心思想就是在cpu将全局变量的值取到相应进程的寄存器之后，通过时钟中断切换进程，其中一个进程输出'\n'后更新了全局变量`displayRow`的值后切换到另一个进程，因为这时进程继续执行，认为全局变量的值就是其通用寄存器中保存的值，所以打印一个字符时不会从打印完'\n'后开始，而会在上一行打印，同时由于`displayCol`也没有更新，会导致字符覆盖；
  	有了这个思路，首先按照手册说的，在syscallPrint中for循环最后增加`asm volatile("int $0x20");`，但是通过反汇编发现我的反汇编结果只加这一处是不够的，因为在执行`pos = (80 * displayRow + displayCol) * 2;`时都要重新到全局变量的地址处取一次值，因此我在此句执行后加了`asm volatile("int $0x20");`（即增加了两个`asm volatile("int $0x20");`），这样如果打印的不是'\n'，则在pos取值结束后将会时钟中断，切换进程；
  	为了方便进程切换的实现，我修改了`timerHandle`函数中时间片的大小：</font>
  
  ```c
  	if (pcb[current].state == STATE_RUNNING)
  	{
  		
  		if (pcb[current].timeCount >= 0)
  		{
              ……//切换进程
          }
          ……
      }
  ```
  
  <font size=4 face="楷体">	这样，就使得一旦一个进程执行到int 20就会执行进程切换；
  	然后，设计如下测试：
  	修改`lab3/app/main.c`中uEntry为：</font>
  
  ```c
  	int ret = fork();
  	if (ret == 0) {
  		printf("2");
  		exit();
  	}
  	else if (ret != -1) {
  		printf("1");
  		exit();
  	}
  	while(1);
  	return 0;
  ```
  
  <font size=4 face="楷体">	正常来说，首先执行父进程，输出1后输出2；但手动模拟了时钟中断后，就有可能出现这样的场景：父进程输出1，在计算完pos的值之后被中断，此时全局变量`displayRow`与`displayCol`都是0，已经被取到父进程的寄存器了；切换到子进程后，子进程也在计算完pos后被中断，`displayRow`与`displayCol`也取到了子进程的寄存器中，值都是0；接着父进程继续执行，打印了1，并更新了全局变量`displayCol`的值；在输出1完成后又执行了int 20，回到子进程执行；子进程从被中断处执行，会根据寄存器中的值输出2；由于全局变量被父进程更新了，但是子进程中存储`displayCol`的寄存器值未被更新，所以子进程会在（0，0）处打印2；最后执行`updateCursor`，重新读取全局变量值更新光标，导致了出现空位；所以由于全局变量更新不一致，导致应该输出“12_”输出了“2 _”（'\_'是光标）：</font>
  
  ![](C:\Users\njuwhl2019hp\Desktop\os\lab3_2.png)
  
  <font size=4 face="楷体">	类似的，设计如下测试验证`displayRow`的更新一致性问题：</font>
  
  ```c
  	int ret = fork();
  	if (ret == 0) {
  		printf("2\n");
  		exit();
  	}
  	else if (ret != -1) {
  		printf("13");
  		exit();
  	}
  	while(1);
  	return 0;
  ```
  
  <font size=4 face="楷体">	进行此测试时，将`timerHandle`函数中时间片的大小设置为3：</font>
  
  ```c
  if (pcb[current].timeCount >= 3)
  ```
  
  <font size=4 face="楷体">	根据在syscallPrint中添加的`asm volatile("int $0x20");`位置可知，打印'\n'会使时间片减1，打印其它字符会使时间片减2；故该测试中，父进程打印1后时间片还剩下1，然后打印3时，在计算完pos值后执行int 20，时间片用完，切换到子进程；子进程打印2，时间片减2，打印'\n'，时间片减一，刚好打印完；这时输出应该为：</font>
  
  ​							**12**
  
  ​							**_**
  
  <font size=4 face="楷体">	回到父进程继续执行，同样的，父进程中的寄存器值没有随全局变量更新，所以会在(0,1)打印3，即覆盖2；然后`updateCursor`重新读取全局变量值，更新光标，故正常应输出：</font>
  
  ​							**12**
  
  ​							**3_**
  
  <font size=4 face="楷体">	的程序输出结果变为：</font>
  
  ![](C:\Users\njuwhl2019hp\Desktop\os\lab3_3.png)

<font size=4 face="楷体" color=red>	PS：由于自动产生的时钟中断对于测试也有影响但又无法避免，故有时输出与图示有差别，不过多数情况还是如图所示的；测试时最好注释掉uEntry原有内容后添加用例。</font>

### <font face="楷体">五、实验心得</font>

<font size=4 face="楷体">	lab3的内容帮助我进一步加深了对于整个进程切换过程的理解，同时框架代码对中断嵌套发生时进程内核栈变化的模拟实现也让我接触了中断嵌套实现的一种思路；对于系统调用的实现则助我复习了fork等系统调用的执行内容与特点；loadElf的实现、解决遇到的内存覆盖问题让我对于多进程模式下加载elf文件有了更多的思考；而全局变量的更新一致性问题则让我对老师说的“进程切换导致的临界区变量更新不一致”问题有了更直观的理解；
	完成本次实验，最大的感受就是手册和框架代码其实对很多问题都有提示，例如选做的内容，实际上做法和思考方式手册里或框架代码都或多或少有提示；遇到理解不了的过程，仔细阅读手册或者按照框架代码执行流程走一遍，认识就会清楚很多。
	最后，感谢18级的李屹同学，和他在中断嵌套理解问题上的讨论给予了我很多的帮助。</font>



