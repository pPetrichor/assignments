# <p align="center"><font face="楷体">OS lab4报告</p></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖
学号：181860092
邮箱：603102048@qq.com</font></p>
[TOC]

### <font face="楷体">一、实验进度</font>

<font size=4 face="楷体">	完成了实验的所有内容：实现了格式化输入函数；完成了进程通信处理例程，实现了进程通信；实现了信号量相关的系统调用，并利用信号量机制解决了生产者-消费者、哲学家就餐与读者写者3个进程同步问题，也实现了一个随机函数来生成随机的`sleep`时间。</font>



### <font face="楷体">二、实验结果</font>

<font size=4 face="楷体">	`scanf`测试结果（在测试代码`return 0;`前增加了`exit();`）：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab4_1.png)

<font size=4 face="楷体">	进程通信测试结果：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab4_2.png)

<font size=4 face="楷体">	信号量相关系统调用测试结果：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab4_3.png)

<font size=4 face="楷体">	生产者-消费者问题测试结果（`sleep`随机的一段时间；运行时间较长，学长可以直接看截图;所有截图均是Ubuntu下运行的结果）：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab4_4.png)



<font size=4 face="楷体">	哲学家就餐问题测试结果（`sleep`随机的一段时间）：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab4_6.png)

![](C:\Users\njuwhl2019hp\Desktop\os\lab4_7.png)

![](C:\Users\njuwhl2019hp\Desktop\os\lab4_8.png)

<font size=4 face="楷体">	读者-写者问题测试结果（`sleep`随机的一段时间）：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab4_9.png)

![](C:\Users\njuwhl2019hp\Desktop\os\lab4_11.png)

![](C:\Users\njuwhl2019hp\Desktop\os\lab4_10.png)



### <font face="楷体">三、实验修改的代码</font>

- <font size=4 face="楷体">系统调用例程实现：在`lab4/kernel/kernel/irqHandle.c`中补充完成了`keyboardHandle、syscallReadStdIn、syscallWriteShMem、syscallReadShMem、syscallSemInit、syscallSemWait、syscallSemPost与syscallSemDestroy`这些内容；</font>
- <font size=4 face="楷体">进程同步问题实现：在`lab4/bounded_buffer/main.c`、`lab4/philosopher/main.c`与`lab4/reader_writer/main.c`中补充完成了对生产者-消费者、哲学家就餐与读者写者3个进程同步问题的实现；</font>
- <font size=4 face="楷体">随机数生成实现了`random`函数；在`lab4/lib/syscall.c`中实现了`random()`系统调用；在`lab4/kernel/kernel/irqHandle.c`中实现了`syscallGetRandom`例程，利用`irqHandle.c`中的全局变量`seed`实现随机数的生成；</font>

### <font face="楷体">四、实验思路以及实验遇到的问题与解决</font>

- <font size=5 face="楷体" color=00bfff>格式化输入函数实现：</font>

  <font size=4 face="楷体">	即实现`keyboardHandle、syscallReadStdIn`两个例程；实现的思路guide中说明的十分清楚，参照着实现即可；
  	`keyboardHandle`：
  	首先，将读取到的`keyCode`放入到`keyBuffer`中：`keyCode`可以利用函数`getKeyCode()`读取到，对于非法的`keyCode`要丢弃；`keyBuffer`是由`bufferHead`与`bufferTail`决定的，所以只要将`keyCode`赋值给`keyBuffer[bufferTail]`，再移动`bufferTail`即可；唯一需要注意的就是`keyBuffer`满的情况；这里采用了keyBuffer满了就不再添加地处理方式，具体实现就是用一个变量`if_full`标记keyBuffer是否满：</font>

  ```C
		uint32_t keycode = getKeyCode();
  	if(keycode == 0) return;	//丢弃非法keyCode
  	if(if_full == 1) //buffer已满
  		return;
  	keyBuffer[bufferTail] = keycode;
  	bufferTail++;
  	bufferTail = bufferTail%MAX_KEYBUFFER_SIZE;
  	//将读取到的keyCode放入keyBuffer中
  	if(bufferHead == bufferTail) if_full = 1;
  	//keyCode放入keyBuffer后，如果head与tail重合，则说明buffer已满
  ```
  
  <font size=4 face="楷体">	接着，如果`dev[STD_IN]`上还有阻塞的进程，就将阻塞的进程唤醒；即由`dev[STD_IN].value`的值判断是否有阻塞的进程；由于`dev[STD_IN]`上最多只有一个进程被阻塞，所以`dev[STD_IN].value`只会等于0与-1，如果等于-1，说明有进程阻塞在上面，则利用guide给出的方法从`dev[STD_IN]`上阻塞的进程列表取下唯一的进程，将其设置为`STATE_RUNNABLE`（sleepTime无需处理，因为STATE_RUNNABLE状态下该值不起作用，之后进程再次阻塞/睡眠时sleepTime又会重新赋值），更新`dev[STD_IN].value`为0（`dev[STD_IN]`上没有阻塞的进程了）即可；
  	`syscallReadStdIn`：
  	该例程要实现的内容为：如果`dev[STD_IN].value == 0`，则将current进程阻塞在`dev[STD_IN]`上；等之后该进程被唤醒之后，读取buffer中所有的数据；若不为0，则只可能为-1，说明此时已经有进程阻塞在上面了，所以直接设置返回值为-1，结束；因此，首先判断`dev[STD_IN].value`的值，若为0则利用guide给出的方法将`pcb[current]`阻塞在`dev[STD_IN]`上，并将`pcb[current]`设置为BLOCK状态，再利用`asm volatile("int $0x20");`手动切换进程；需要注意的就是这里阻塞进程只能被唤醒操作唤醒，不可以阻塞一段时间后自动苏醒，所以将`sleepTime`设为-1，满足无法自动苏醒的要求；被阻塞的唯一进程会在下一次调用`keyboardHandle`时被唤醒，之后到它执行时，会从中断处继续执行，所以读取keyBuffer所有内容可以直接写在阻塞进程的后面；
  	读取keyBuffer中所有内容的实现：首先从tf中获得读取的目的地址与读取长度等参数，利用循环读取keyBuffer的数据直到读够长度或读完keyBuffer；需要注意的有以下几点：1、判断读取长度的合法性，读取长度应该非负；2、每次读取一个字符时，要调用`getChar`得到相应的字符，对得到字符的合法性也要判断；3、读取的字符串应该以'\0'结尾，从而保证`scanf`中`if(buffer[count]==0)`在处理完一次读取后成立，进而进行之后的读取（如果format未结束的话）；对于前两点无需多说，第三点我采取了如下方法实现：本实验中，在`syscallReadStdIn`的循环中要做的就是把keyBuffer内容读到scanf的缓存中去，由`scanf`代码可知其缓存大小为256，所以只要读取长度小于256，就可以通过循环读取之后再在尾部加入一个'\0'满足；如果读取长度大于等于256，则在读取255个数据后跳出循环，再加一个'\0'，从而保证了scanf中的缓存字符串以'\0'结尾；而`scanf`框架中的`buffer[0]=0`保证了一开始向`buffer`写东西时`buffer[count]==0`，不会因为未初始化无法读入数据，所以以上可以实现数据的读取；以下是读取keyBuffer内容的实现：</font>
  
  ```c
  while(i < read_size && bufferHead != bufferTail)
  //未读取到read_size长度的字节且keyBuffer内容仍未读完
  {
  	if(i == 255) break;//读取255字节后，要为'\0'留位置，故停止读取
  	character = getChar(keyBuffer[bufferHead]);
  	if(character != 0)//不为非法字符
  	{
  		asm volatile("movb %0, %%es:(%1)"::"r"(character),
                       "r"(str + i));
  		i++;
  	}
  	bufferHead++;
  	bufferHead = bufferHead % MAX_KEYBUFFER_SIZE;
  }
  if_full = 0;//keyBuffer被读取，不再满
  character = '\0';
  asm volatile("movb %0, %%es:(%1)"::"r"(character),"r"(str + i));//加'\0'
  ```
  
  <font size=4 face="楷体">	PS:对于格式化输入的实现，一开始我是以单个字符的读入作为V操作，将单个字符从`keyBuffer`中取出作为P操作；这样，在`keyboardHandle`中，每次向`keyBuffer`中加入一个键值就将`dev[STD_IN].value`自增，这样自增之后如果`dev[STD_IN].value <= 0`（实际上就是判断是否为0），则说明之前有进程阻塞在上面，因此取下来唤醒；如果大于0则不做处理；相应的，在`syscallReadStdIn`中，如果`dev[STD_IN].value == 0`,则进行阻塞（只阻塞，不读取数据）；这之后判断`if(dev[STD_IN].value >= 0)`，如果是则读取keyBuffer中的数据；否则返回-1；这样做可以适应两种情况：如果一个读取进程被阻塞后唤醒，则其会从`if(dev[STD_IN].value >= 0)`继续执行；而此时刚唤醒一个阻塞进程的`dev[STD_IN].value`一定大于等于0，所以会读取数据；同时由于唤醒阻塞进程时，`dev[STD_IN].value`首先会从-1增为0；但此时`keyBuffer`中键值的个数为1，所以对于被阻塞进程`dev[STD_IN].value`会比`keyBuffer`中键值的个数少一；故利用一个变量`flag`标记进程是否被阻塞过，如果是则首先从`keyBuffer`中读一个数据出来，但不对`dev[STD_IN].value`执行减一操作，以实现`dev[STD_IN].value`与`keyBuffer`中键值的个数保持一致；如果不是被阻塞后唤醒的进程，则dev[STD_IN].value就是`keyBuffer`中键值的个数，全部读取出来即可；读取的循环条件是`while(i < read_size && bufferHead != bufferTail)`，每次循环执行`dev[STD_IN].value--`；这样在`bufferHead == bufferTail`时`dev[STD_IN].value`恰为0，即将`keyBuffer`中所有数据读出；
  	经验证上述方法是可行的，但是这样操作误解了`dev[STD_IN].value`的涵义；`dev[STD_IN].value`是用于标记是否有进程阻塞在`dev[STD_IN]`上的，而不是表示`keyBuffer`中键值的个数，因此最后还是改为了正文中的实现。本思路具体的实现可以参看注释。</font>
  
- <font size=5 face="楷体" color=00bfff>进程通信实现：</font>

  <font size=4 face="楷体">	即补充完成`syscallWriteShMem`和`syscallReadShMem`两个例程内容；根据guide与框架代码提示，这两个例程的功能就是对`shMem`和用户进程传入地址处值的相互赋值；二者的实现方法也基本一样：首先通过tf取出需要的一系列参数（以`syscallReadShMem`为例）：</font>

  ```c
  	int sel = tf->ds;
  	uint8_t* buffer = (uint8_t*)tf->edx; //传入要写的地址
  	int read_size = tf->ebx;//要从共享内存读取的长度
  	int index = tf->esi; //共享内存读取起始位置
  	
  	asm volatile("movw %0, %%es"::"m"(sel));
  ```
  
  <font size=4 face="楷体">	然后判断读取长度与`index`的合法性，如果不合法则返回错误值0；否则就进行循环赋值操作，赋值结束后设置返回值为实际读写的字节数；
  	对于共享内存与用户传入地址相互赋值操作的实现，有两种方法：一种是利用已知的内存划分方式，利用current值直接计算出用户传入地址的实际物理地址然后赋值：</font>
  
  ```c
  syscallWriteShMem：
      int i = 0;
		for(;(i < write_size) && (i < MAX_SHMEM_SIZE - index);++i)
  	{			
  		shMem[index + i] = *(uint8_t *)((current + 1) * 0x100000
	                                        + buffer + i);	
  	}
  
  syscallReadShMem:
      int i = 0;
  	for(;(i < read_size) && (i < MAX_SHMEM_SIZE - index);++i)
  	{			
  		*(uint8_t *)((current + 1) * 0x100000 + buffer + i)
              							= shMem[index + i];
  	}
  ```
  
  <font size=4 face="楷体">	这样的方法相当于是利用先验知识实现的，但如果物理内存划分变化就不可行了；另一种方法利用数据段值来定位更加普适一些：</font>
  
  ```c
  syscallWriteShMem：
      int i = 0;
  	for(;(i < write_size) && (i < MAX_SHMEM_SIZE - index);++i)
  	{			
        asm volatile("movb %%es:(%1), %0":"=r"(shMem[index+i]):
                     "r"(buffer + i));
  	}
  
  syscallReadShMem:
      int i = 0;
  	for(;(i < read_size) && (i < MAX_SHMEM_SIZE - index);++i)
  	{			
  	  asm volatile("movb %0, %%es:(%1)"::"r"(shMem[index+i]),
                     "r"(buffer + i));
  	}
  ```
  
  <font size=4 face="楷体">	这样，就实现了进程通信相关的系统调用。</font>
  
- <font size=5 face="楷体" color=#00bfff>信号量实现：</font>

  <font size=4 face="楷体">	这一阶段中需要实现以下四个子例程：</font>

  <font size=4 face="楷体">	1、**syscallSemInit**：
  	用于初始化信号量，实现思路就是在信号量数组中寻找一个未使用的信号量进行占用，并将该信号量的初始值设置为传入的参数`value`，实现成功初始化；如果找不到未使用的信号量则设置返回值为-1，否则，需要实现系统调用`sem_init`中的`*sem`指向初始化成功的信号量；由`sem_init`的代码可知，`*sem`的值就是syscallSemInit例程的返回值，而所有信号量又是以数组的形式组织起来的，所以可以将syscallSemInit例程的返回值设置为初始化成功的信号量的下标。这样，`sem`指向的值就是初始化成功的信号量的下标，利用它访问信号量数组即可得到对应的信号量，从而实现“系统调用`sem_init`中的`*sem`指向初始化成功的信号量”；</font>

  <font size=4 face="楷体">	2、**syscallSemWait**：该例程实现的是P操作；首先，通过tf得到要执行P操作的信号量的下标，并对该下标的合法性进行检验：下标不可为负数、不可超出信号量的总个数、同时下标对应的信号量的`state`应该为1，即该信号量必须是被使用的才可以执行P操作；如果下标不合法则返回-1，否则设置返回值为0表示操作成功，然后对该信号量执行P操作：信号量`value`减一，如果`value`小于0，说明需要等待，则利用guide的方法将当前进程阻塞在该信号量上，设置进程状态为STATE_BLOCKED，sleepTime为-1（必须被唤醒，无法自动苏醒）；最后`int $0x20`切换进程；</font>

  <font size=4 face="楷体">	3、**syscallSemPost**：该例程实现的是V操作，具体实现流程与**syscallSemWait**类似，即先验证得到下标的合法性，若合法则设置返回值为0表示操作成功，并执行V操作：信号量`value`加一，如果`value`小于等于0，说明有进程阻塞在该信号量上，则利用guide的方法从该信号量的阻塞列表中取出一个进程唤醒，设置进程状态为STATE_RUNNABLE（sleepTime无需处理，因为STATE_RUNNABLE状态下该值不起作用，之后进程再次阻塞/睡眠时sleepTime又会重新赋值），然后`int $0x20`切换进程；</font>

  <font size=4 face="楷体">	4、**syscallSemDestroy**：该例程用于销毁一个信号量；我对销毁的理解就是将该信号量设置成未使用，并且将该信号量上所有的阻塞进程都丢掉（可能导致未知错误）；所以实现也比较容易：首先仍然要判断下标的合法性(要判断销毁的应该是一个在使用的信号量；父子进程可能先后对一个信号量进行销毁)，若合法则设置该信号量state为0，并利用：</font>

  ```c
  sem[sem_index].pcb.next = &(sem[sem_index].pcb);
  sem[sem_index].pcb.prev = &(sem[sem_index].pcb);
  ```

  <font size=4 face="楷体">	来将该信号量上所有的阻塞进程都丢掉（仿照初始化实现），这样之后再次使用该信号量时，因为信号量上之前阻塞的进程都丢掉了，相当于未使用过，所以直接使用就可以；但如果直接丢掉的话，这些进程都是处于阻塞状态，无法执行，所以应该遍历取掉信号量上的每个进程，并设置它们的状态为RUNNABLE：</font>

  ```c
  while(sem[sem_index].pcb.next != &(sem[sem_index].pcb))
  {
  	ProcessTable* pt = (ProcessTable*)(
          			   (uint32_t)(sem[sem_index].pcb.prev)-
                         (uint32_t)&(((ProcessTable*)0)->blocked)
      				    );
      
  	sem[sem_index].pcb.prev = (sem[sem_index].pcb.prev)->prev;    
  	(sem[sem_index].pcb.prev)->next = &(sem[sem_index].pcb);
  	pt->state = STATE_RUNNABLE;
  }
  ```

  <font size=4 face="楷体">	这样，循环结束时，也满足：</font>

  ```c
  sem[sem_index].pcb.next == &(sem[sem_index].pcb);
  sem[sem_index].pcb.prev == &(sem[sem_index].pcb);
  ```

  <font size=4 face="楷体">	最后，设置返回值、切换进程即可。</font>

- <font size=5 face="楷体" color=#00bfff>进程同步问题实现</font>

  <font size=4 face="楷体">	1、**生产者-消费者问题：**实现`producer`与`consumer`两个函数分别实现生产者与消费者的行为；
  	**`producer`**：函数主体就是一个循环，即不断执行生产者的工作；在循环过程中，首先对信号量`emptyBuffers`执行P操作，来保证如果缓存未满则生产，否则生产者被阻塞；接着对信号量`mutex`执行P操作以保证互斥访问缓存区；如果可以访问，则生产一个产品；生产产品的过程利用一个变量`in`实现：`in`记录了当前可以放产品的一个位置，所以生产者只需在`buffer[in]`放入一个产品，并更新`in`即可；这里由于只是模拟，不用真的生成产品，所以没有`buffer`这样的数组缓存，生产过程用`printf`信息表示：</font>
  
  ```c
  		int ret = sem_wait(&emptyBuffers);
  		sleep;
      	ret = sem_wait(&mutex);
  		sleep;
  		in++;
  		in = in%n;
  		printf("Producer %d: produce\n", id);//id表示该进程的pid
  		sleep;//模拟一段生产时间
  ```

<font size=4 face="楷体">	这样，在生产完成后，`producer`结束了对缓存的操作，故对信号量`mutex`执行V操作，并对信号量`fullBuffers`执行V操作，表示缓存中加入了一个产品：</font>

```c
		ret = sem_post(&mutex);
		sleep;
		ret = sem_post(&fullBuffers);
		sleep;//加入sleep以实现“任意P、V及生产、消费动作之间有sleep”的要求
```

<font size=4 face="楷体">	**`consumer`**：消费者的实现思路与`producer`类似；在我的实现中，使用了一个变量`time`记录消费的次数；当`time`小于5的时候消费者进行消费，否则不消费；这样在5次消费后`time`增为5，消费停止，故生产者将缓存填满后被阻塞，整个生产-消费过程被阻塞停止（方便截图）；每次的消费过程中，消费者首先对信号量`fullBuffers`执行P操作，来保证如果缓存不空则消费，否则被阻塞；接着对信号量`mutex`执行P操作以保证互斥访问缓存区；如果可以访问，则取出一个产品消费；取出产品的过程利用一个变量`out`实现：类似于变量`in`，`out`记录了当前有产品的一个位置，所以生产者只需从`buffer[out]`取一个产品，并更新`out`即可；取出产品后消费者同样要对信号量`mutex`执行V操作，并对信号量`emptyBuffers`执行V操作，表示缓存中产品数减一：</font>

```c
while(time < 5)
	{
		int ret = sem_wait(&fullBuffers);
		sleep;
		ret = sem_wait(&mutex);
    	sleep;
		out++;
		out = out%n;
		time++;
		printf("Consumer : consume\n");
		sleep;
		ret = sem_post(&mutex);
		sleep;
		ret = sem_post(&emptyBuffers);
		sleep;//模拟消费过程
	}
```

<font size=4 face="楷体">	这里变量`time`的增加可以由信号量`mutex`保证互斥增长；同时，消费者对缓存的互斥访问实际在取出产品后就结束了，消费过程可以在之后进行，这里`printf`输出只是说明取产品成功，并不是消费的过程，消费的过程可以用最后的sleep来模拟。</font>

<font size=4 face="楷体">	最后，在`main`函数中对使用到的信号量进行初始化，这里设置缓存区大小为10；并通过`fork`产生五个进程，四个作为生产者，一个作为消费者运行。不同进程的id用`getpid`获得以区分。</font>

<font size=4 face="楷体">	2、**哲学家就餐问题：**本问题中要实现的是5个哲学家共同就餐，实现方法我参照了guide给出的方案3，即：对于某个哲学家，其有一个唯一的、标记自己的编号；每个哲学家不断执行以下操作：</font>

```c
		think;  
		sleep;//模拟思考时间
		if(i%2==0){      
			sem_wait(&forks[i]);
			sleep;  
			sem_wait(&forks[(i+1)%N]);
		}//偶数号哲学家优先拿左边的叉子
		else {      
			sem_wait(&forks[(i+1)%N]);  
			sleep;  
			sem_wait(&forks[i]);          
		}//奇数号哲学家优先拿右边的叉子
		sleep;
		printf("Philosopher %d: eat\n", id); //就餐
		sleep;
		printf("Philosopher %d: putdown one fork\n", id);
		sem_post(&forks[i]);//放下左边的叉子
		sleep;
		printf("Philosopher %d: putdown another fork\n", id);
		sem_post(&forks[(i+1)%N]);//放下右边的叉子
		sleep;
```

<font size=4 face="楷体">	在`main`中，对使用到的信号量进行初始化，并通过`fork`产生五个进程，依次为0-4号哲学家。不同哲学家进程的id用调用`philosopher`函数时传入的参数i获得以区分。</font>

<font size=4 face="楷体">	除了以上方法，还可以通过</font>

<font size=4 face="楷体">	3、**读者-写者问题：**本问题中同样涉及到互斥访问、互斥修改与条件同步操作，相比较于前两个问题，本问题中新增的就是正在read的读者数量`Rcount`这个值会被不同的进程访问，所以需要利用共享内存的方式处理这个值的变化；
	**`reader：`**函数主体内容仍然是一个循环，在循环内部，读者首先对信号量`CountMutex`执行P操作以实现对`Rcount`的互斥修改；接着利用`read()`从共享内存中读取`Rcount`，将计数加一并写入共享内存中，完成对`Rcount`的修改；此时对于`Rcount`的互斥访问还没有结束：如果加一后`Rcount`等于1，则表明当前进程是第一个读者，故必须对信号量`WriteMutex`执行P操作，以保证读写操作的互斥；完成以上工作后，就可以结束对`Rcount`的访问了，故对信号量`CountMutex`执行V操作；</font>

```c
		sem_wait(&CountMutex);
		sleep;
		read(3,(uint8_t*)&Rcount,4);//读共享内存
		Rcount++;
		write(3,(uint8_t*)&Rcount,4);//修改Rcount，并写入共享内存
		if(Rcount == 1)	
		{
			sem_wait(&WriteMutex);
			sleep;
		}		
		sem_post(&CountMutex);//结束Rcount的访问
		sleep;
```

<font size=4 face="楷体">	之后，该读者就可以开始read了；由于开始read的时间不是该读者申请read（即给Rcount加一）的时间（因此可能会出现当第一个reader输出信息时，实际已经有两个reader申请了read，从而打印“有两个reader”的信息），所以还需要利用`read()`从共享内存中读取`Rcount`来作为printf的信息：</font>

```c
		read(3,(uint8_t*)&Rcount,4);
		printf("Reader %d: read, total %d reader\n", id, Rcount); 
		sleep;//模拟read的时间
```

<font size=4 face="楷体">	读者读完后，就要退出了；退出前还需要更新`Rcount`，所以还要对`Rcount`进行互斥修改；同时，在修改`Rcount`后，如果变量值为0，则说明所有的读者都退出了，因此要对信号量`WriteMutex`执行V操作，从而使得被阻塞的写者能够执行写操作（没有阻塞的写者则接下来读写均可）。</font>

```c
		sem_wait(&CountMutex);//互斥修改
		sleep;
		read(3,(uint8_t*)&Rcount,4);
		Rcount--;
		write(3,(uint8_t*)&Rcount,4);//读写共享内存，修改Rcount
		printf("Reader %d: read over\n", id);//read结束
		sleep;
		if(Rcount == 0)
		{			
			sem_post(&WriteMutex);
			sleep;
		}
		sem_post(&CountMutex);
		sleep;
```

<font size=4 face="楷体">	**`writer:`**函数主体也是一个循环，写者要执行的操作很简单：首先对信号量`WriteMutex`执行P操作，以保证“读-写互斥”与“写-写互斥”；之后执行写操作，写完之后对信号量`WriteMutex`执行V操作来让其它写者写或读者读：</font>

```c
		sem_wait(&WriteMutex);
		sleep;
		printf("Writer %d: write\n", id);
		sleep;//模拟写的时间
		printf("Writer %d: write over\n", id); 
		sem_post(&WriteMutex);	
		sleep;
```

<font size=4 face="楷体">	在`main`中，对使用到的信号量进行初始化，同时也要将`Rcount`初始化为0，写入共享内存中；并通过`fork`产生八个进程，其中四个写者，四个读者，不同进程的id用`getpid`获得以区分。</font>

- <font size=5 face="楷体" color=#00bfff>随机数生成实现</font>

  <font size=4 face="楷体">	利用了线性同余法实现随机数生成：</font>
  $$
  R_{n+1}~=(289*R_n+19)mod(1024)
  $$
  <font size=4 face="楷体">	初始种子设置为`seed = R0 = 217`；</font>
  
  <font size=4 face="楷体">	利用该方法得到的前100个随机数值为：</font>
  
  ```
  268 671 402 485 920
  683 798 241 36 183
  682 509 688 195 54
  265 828 719 962 533
  456 731 334 289 596
  231 218 557 224 243
  614 313 364 767 498
  581 1016 779 894 337
  132 279 778 605 784
  291 150 361 924 815
  34 629 552 827 430
  385 692 327 314 653
  320 339 710 409 460
  863 594 677 88 875
  990 433 228 375 874
  701 880 387 246 457
  1020 911 130 725 648
  923 526 481 788 423
  410 749 416 435 806
  505 556 959 690 773
  ```
  
  <font size=4 face="楷体">	可以看出该方法得到的随机数的随机性还是比较好的，只是数值有些偏大，等待时间较长；故这里每次的随机数取的实际是`Rn`的二分之一；</font>
  
  <font size=4 face="楷体">	在`lab4/lib/syscall.c`中将用户进程得到随机数的过程封装为一个系统调用`random()`：</font>
  
  ```c
  int random() {
  	return syscall(SYS_RANDOM, 0, 0, 0, 0, 0);
  }
  ```
  
  <font size=4 face="楷体">	利用`syscall`跳转到`lab4/kernel/kernel/irqHandle.c`中实现的系统调用`syscallGetRandom`:</font>
  
  ```c
  void syscallGetRandom(struct TrapFrame *tf) {
  	int temp = 289 * seed + 19;
  	seed = temp%1024;
  	pcb[current].regs.eax = seed/2;
  	return;
  }
  ```
  
  <font size=4 face="楷体">	其中seed是`lab4/kernel/kernel/irqHandle.c`中的一个全局变量，初始化为217；</font>
  
  <font size=4 face="楷体">	这样就实现了随机数的生成；用户进程只需调用`random()`就可以得到一个随机数，然后睡眠该时长即可：</font>
  
  ```c
  		sleeptime = random();
  		sleep(sleeptime);
  ```
  



### <font face="楷体">五、思考题</font>

<font size=4 face="楷体">	1、生产者-消费者问题中P、V的操作顺序有影响吗？
	答、P操作的顺序有影响；V操作的顺序没有影响；
	对于P操作，应该保证对用于互斥的信号量`mutex`的P操作应该在对信号量`emptyBuffers`和`fullBuffers`的P操作之后；否则可能导致死锁；如果交换生产者与消费者中两个P操作的顺序；那么对于生产者而言，在缓存区填满了之后，生产者又生产了一个产品，由于P操作顺序交换，所以生产者是先占有了缓存区，之后由于缓存区满而阻塞在`emptyBuffers`上；则对于消费者而言，由于`mutex`为0，对它执行P操作会使得消费者阻塞在`mutex`，因而无法取产品，即无法对`emptyBuffers`执行V操作，从而形成生产者等待消费者取产品，但消费者等待生产者退出缓存访问的死锁；类似的，对于消费者来说，如果消费者在缓存区空的时候取产品，则消费者会占有缓存区，然后阻塞在`fullBuffers`上，则之后生产者会阻塞在`mutex`上，无法将产品放入缓存；从而形成消费者等待生产者生产产品，但生产者等待消费者退出缓存访问的死锁。</font>

<font size=4 face="楷体">	2、哲学家就餐的其它解决方式：在哲学家就餐问题中，产生死锁的情形是产生了一个封闭的进程链，使得每一个进程需要的资源都至少被下一个进程占有，形成循环等待；所以解决的方法就是破除这个封闭的进程环；除了手册提到的方法以外，还可以采用以下思路：</font>

- <font size=4 face="楷体">设置一个初值为4的互斥信号量，使得最多有4位哲学家可以同时就餐；</font>
- <font size=4 face="楷体">保证任意一位哲学家只有拿到两把叉子时才eat，否则一把叉子也不取：具体而言，当一位哲学家a要拿起一把叉子却发现叉子已经被另一位哲学家拿起时，a继续think，不取叉子；当a拿起一把叉子后要拿另一把叉子，却发现叉子已经被另一位哲学家b拿起时，如果b在eat则a放下已经拿起的叉子，否则判断一下a、b谁先拿起的第一把叉子，先拿起第一把叉子的哲学家eat，另一位哲学家放下拿到的一把叉子think；</font>



