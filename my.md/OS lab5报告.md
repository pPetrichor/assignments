# <p align="center"><font face="楷体">OS lab5报告</p></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖
学号：181860092
邮箱：603102048@qq.com</font></p>
[TOC]

### <font face="楷体">一、实验进度</font>

<font size=4 face="楷体">	完成了实验涉及的如下内容：1、完善了`genFS`目录下的`rm`与`rmdir`函数并设计了测试；2、在`kernel/kernal/fs.c`中添加了支持系统调用的相关文件操作函数，实现了`open、read、write、lseek、remove`等调用，并在实现进程打开文件表与系统打开文件表的基础上修改了`fork`与`exit`调用的内容；3、基于`open`与`read`两个系统调用函数实现了用户程序`ls`与`cat`并设计相关测试；4、基于`fork`与`exec`实现了一个简单的shell，封装了用户程序`cat_、ls、mkdir、rm、run、touch`，通过`exec`加载。</font>



### <font face="楷体">二、实验结果</font>

<font size=4 face="楷体">	1、`rm`与`rmdir`测试：
	在`genFS/main.c`中添加了以下内容（生成/mytest目录并向其中添加1.txt与2.txt，然后一一删除）以测试：</font>

```c
	//test rm
    stringCpy("/", destFilePath, NAME_LENGTH - 1);
    ls(driver, destFilePath);

    stringCpy("/mytest", destFilePath, NAME_LENGTH - 1);
    mkdir(driver, destFilePath);

    stringCpy("/mytest/1.txt", destFilePath, NAME_LENGTH - 1);
    touch(driver, destFilePath);

    stringCpy("/mytest/2.txt", destFilePath, NAME_LENGTH - 1);
    touch(driver, destFilePath);

    stringCpy("/mytest", destFilePath, NAME_LENGTH - 1);
    ls(driver, destFilePath);

	stringCpy("/", destFilePath, NAME_LENGTH - 1);
    ls(driver, destFilePath);

    stringCpy("/mytest", destFilePath, NAME_LENGTH - 1);
    rmdir(driver, destFilePath);

    stringCpy("/mytest/1.txt", destFilePath, NAME_LENGTH - 1);
    rm(driver, destFilePath);

    stringCpy("/mytest", destFilePath, NAME_LENGTH - 1);
    ls(driver, destFilePath);

    stringCpy("/mytest/2.txt", destFilePath, NAME_LENGTH - 1);
    rm(driver, destFilePath);

    stringCpy("/mytest", destFilePath, NAME_LENGTH - 1);
    ls(driver, destFilePath);

    stringCpy("/mytest/", destFilePath, NAME_LENGTH - 1);
    rmdir(driver, destFilePath);

    stringCpy("/", destFilePath, NAME_LENGTH - 1);
    ls(driver, destFilePath);
```

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_1.png)

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_2.png)

<font size=4 face="楷体">	2&3、文件系统调用与用户程序验证：
	在`app/main.c`中添加内容进行测试：首先调用`ls`函数列出几个目录项的内容；接着利用`open`函数在空文件`/test/`下创建文件`test`并向其中写入字符表，然后显示`/test/`下的内容，再调用`cat`打印`/test/test`中的内容；之后，调用`remove`函数依次删除`/test/test`与`/test/`并利用`ls`查看效果；接着再利用`open`创建`/test/`目录并检验；最后，调用`fork`函数，生成的子进程打开`/dev/shmem`写入字母表，父进程`sleep(128)`后调用`open`打开`/dev/shmem`，并调用`lseek`将文件偏移指针移动到10，再调用`read`读取并打印。
	上述测试执行结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_3.png)

<font size=4 face="楷体">	同时，为了验证基于进程打开文件表与系统打开文件表实现的`fork`与`exit`的正确性，实现了系统调用`show_fcb`来显示当前的系统打开文件表；在上述测试中，首先显示了一次系统打开文件表，接着打开目录`/usr`并再次显示系统打开文件表；之后调用`fork`，在`open`、`close`前后调用`show_fcb`；具体内容为(该内容存放在`app/main.c`中，在提交代码中被注释)：</font>

```c
	show_fcb();
	ls("/");
	int t = open("/usr/",O_DIRECTORY | O_RDWR);
	show_fcb();
	ret = fork();
	
	if (ret == 0) {
		show_fcb();//子进程显示系统打开文件表
		fd = open("/dev/shmem",O_RDWR); //子进程打开共享内存文件
		for (i = 0; i < 26; i ++) { // 向"/usr/test"⽂件中写⼊字⺟表
			tmp = (char)(i % 26 + 'A');
			write(fd, (uint8_t*)&tmp, 1);
		}
		show_fcb();//子进程显示系统打开文件表
		exit();
	}
	else if (ret != -1) {
		sleep(128);
		show_fcb();//父进程显示系统打开文件表
		fd = open("/dev/shmem",O_RDWR);//父进程打开共享内存文件
		lseek(fd,10,0);
		char result[100];
		read(fd,(uint8_t *)result,100);
		printf("shmem:%s",result);
		show_fcb();//父进程显示系统打开文件表
		close(fd);
        close(t);//关闭所有打开的文件
		show_fcb();//父进程显示系统打开文件表
		exit();
	}
```

<font size=4 face="楷体">	打印结果如下：可以看出初始时系统未打开任何文件，打开`/usr/`之后系统打开了该文件（inodeoffset为4352）；接着`fork`后，可以看到系统打开文件仍然只有`/usr/`，但指向它的进程打开文件表项变为2(引用计数器命名为linkcount)；然后子进程打开了共享内存进行写，所以系统打开文件多了一项`/dev/shmem`（inodeoffset为6656）；写完后子进程执行`exit`；父进程苏醒后显示系统打开文件表，发现只有`/usr/`（inodeoffset为4352）打开且应用计数器为1，从而验证了`exit`中进程对所有打开的文件成功执行了一次`close`；接着父进程打开`/dev/shmem`读数据、`close`自己打开的所有文件，打印的内容也符合预期，从而验证了实现的正确性。</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_4.png)

<font size=4 face="楷体">	4、shell：
	在2&3中，对于文件的操作都是通过调用封装好的函数实现的，而在shell的实现中，我将对于文件的操作封装为了`/usr/`目录下的一系列用户程序，具体内容包括`ls、run、mkdir、touch、rm、cat`，分别对应于命令行`ls、./、mkdir、touch、rm与cat`；shell的运行流程即为通过`exec`生成子进程调用相应用户程序来执行；简单的shell运行展示如下(蓝色下划线内容是输入的命令行，白色下划线以上为上一张图片中的内容)：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_5.png)

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_6.png)

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_7.png)

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_8.png)

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_9.png)

<font size=4 face="楷体">	对一个文件而非目录文件执行`cd`是非法的命令：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_12.png)



<font size=4 face="楷体">	学长也可以自行测试（PS：实现版本中cd支持`cd a/../b/././../c/……`的命令行；所有用户程序不支持绝对路径，只支持相对路径；`./`、`cat`等程序只支持操作当前目录下的文件，不支持类似`./ a/b/c`的命令行，只支持`./ a`这样的命令行；注意`./`后应该输入一个空格，即支持`./ print`而非`./print`(因为实现版本中用空格分割了命令与命令行参数)）。</font>



### <font face="楷体">三、实验修改的代码</font>

- <font size=4 face="楷体">`rm`与`rmdir`实现：在`utils/genFS/fun.c`中补充完成了`rm`和`rmdir`函数以及它们调用到的一系列函数；</font>
- <font size=4 face="楷体">文件系统调用实现：在`kernel/fs.c`中添加了一系列文件操作相关的函数；在`kernel/irqHandle.c`、`lib/syscall.c`等文件中补充完成了`open、read、write、lseek、remove`等调用；</font>
- <font size=4 face="楷体">用户程序`ls、cat`实现；在`lab4/lib/syscall.c`中实现了ls函数与cat函数，通过调用`read、open`进行实现；并在`app/main.c`中添加了测试代码</font>
- <font size=4 face="楷体">shell实现；在`lab5/`目录下封装了`ls、run、mkdir、touch、rm、cat`这些用户程序，相应地修改了`lab5/Makefile`与`genFS/main.c`与各自的makefile；在`app/main.c`中实现了shell的主流程；在各个用户程序的`main.c`中实现了相应的功能。</font>

### <font face="楷体">四、实验思路以及实验遇到的问题与解决</font>

- <font size=5 face="楷体" color=00bfff>`rm、rmdir`实现：</font>

  <font size=4 face="楷体">	总体来说，这一部分的代码量较大，但难度不是特别大；主要的实现思路就是仿照`touch`、`mkdir`来对称地实现；阅读框架代码总结出`touch`、`mkdir`的整体函数调用流程为：</font>
  
  <img src="C:\Users\njuwhl2019hp\Desktop\os\lab5_10.png"  />

  <font size=4 face="楷体">	故对称地实现`rm、rmdir`调用的相关函数以实现调用流程，内容如下：</font>
	
  ![](C:\Users\njuwhl2019hp\Desktop\os\lab5_11.jpg)
  
  <font size=4 face="楷体">	结合以上流程可以看出，二者的流程基本一致，调用的函数也是基本一样的；
  	对于`deleteInode`，其前半部分与`allocInode`完全一致，同样是在父目录下遍历，只不过寻找的是名字与`destFilename`一致的目录项；如果遍历未找到则返回-1；如果找到，则父目录的第i个block下的第j个目录项为要删除的内容；由此计算出要删除内容的inode偏移量`destInodeOffset`并利用`fread`读取到`destInode`中；读取出来后，判断该inode的类型与传入参数`destFiletype`的一致性；一致性满足后，根据`destInode`的类型进一步操作：
  	如果为目录文件，则只有在目录中无内容时删除；为判断目录是否为空，实现了函数`getDirEntry_num`用以返回传入inode对应的目录下的目录项个数；如果返回值为2，说明只有'.'与'..'两个目录项，则该目录内容为空，否则不为空；若为空，则将父目录的第i个block下的第j个目录项的inode置为0表示删去，再将`destInode`的linkCount减一（同时要将改变后的内容写回父目录inode与`destInodeOffset`处的inode）；如果linkCount自减后成为0，则调用`deleteBlock`删除`destInode`相应的所有data block；最后调用`emptyInode`更新inodeBitmap；如果为普通文件则更为简单，相较于目录文件，只是不必调用函数`getDirEntry_num`判断目录是否为空，其它操作与目录文件完全一样；
  	对于`emptyInode`的实现，就是仿照`getAvailInode`进行；只是判断的是`superBlock->availInodeNum`是否等于inode总数，是则返回-1，否则`availInodeNum`加一；之后根据`getAvailInode`计算j、k的公式`*inodeOffset = inodeTableOffset * SECTOR_SIZE + (j * 8 + k) * sizeof(Inode);`反推出j、k，再利用`inodeBitmap.byte[j] = inodeBitmap.byte[j] ^ (1 << (7 - k));`将inodeBitmap中相应的inode位置零，并写回inodeBitmap；
  	对于`deleteBlock`，就是一个循环，不停调用`deleteLastBlock`直到inode的所有data block均被释放；而`deleteLastBlock`做的就是模仿`allocLastBlock`，每次找到下标为`inode->blockCount`的data block（可能在直接索引，也可能在一级索引）调用`emptyBlock`释放对应data block；要注意的就是如果`blockCount`等于POINTER_NUM，则该block是一级索引的第一项，在删除它的同时还要删除一级索引指向的block；
  	对于`emptyBlock`，类似于`emptyInode`，也是利用`getAvailBlock`中j、k的计算公式反推出j、k，再将相应的`blockBitmap.byte[j]`的第k位置零并写回`blockBitmap`。
  	由此，实现了整个删除流程依赖的函数；在此基础上模仿`mkdir`与`touch`实现`rmdir`与`rm`即可，过程略。</font>
  
- <font size=5 face="楷体" color=00bfff>文件系统相关调用实现：</font>

  <font size=4 face="楷体">	本阶段的目标就是实现：`open,read,write,lseek,close,remove`这些函数并实现他们对应的系统调用；在实现这些内容之前，首先要在`fs.c`与`fs.h`中添加相应的文件操作函数，包括`deleteLastBlock`、`freeInode`等一系列供`irqHandle.c`中调用的函数；具体的实现就是将`genFS/fun.c`中的相关内容移植到`fs.c`中，利用`diskRead`与`diskWrite`替换`fread`与`fwrite`，在经过一点细节调整即可实现，在此不再赘述。**同时，对`File`结构进行更改，添加一项`linkcount`作为引用计数器；在`irqHandle.c`中添加`extern File fcb[10]`来模拟系统打开文件表，各项包含的元素在`kvm.c`的`initFile`中均初始化为0；在`ProcessTable`结构中添加一项`int fd[10]`模拟进程打开文件表，在`kvm.c`的`initProc`中均初始化为-1表示未填写**；</font>

  <font size=4 face="楷体">	接下来对文件系统相关调用进行实现：</font>

  - <font size=4 face="楷体">`open`：主要内容就是实现系统调用函数`syscallOpen`；首先从`tf`中取出`open`的文件名`path`与参数`flags`，注意`path`的获取方式为`char *path = (char*)(tf->ecx + (current+1) * 0x100000);`而非直接取`tf->ecx`；接着调用`readInode`读取`path`对应的inode，若返回值为-1则说明打开的文件不存在，否则要打开文件的inode信息就存储在了变量`destinode`中，inode的偏移量就存储在了`destoffset`中。
    	如果文件不存在，则要创建文件：首先判断`flags`的`O_CREATE`位是否为1，不是则无法创建，返回-1；否则进行创建文件：通过判断`flags`的`O_DIRECTORY`位是否为1决定要创建的是目录文件还是普通文件；创建的具体过程将`fun.c`中的`mkdir`与`touch`移植过来稍加修改即可；
      	这样，要打开的文件就已经存在了，而且打开文件的inode信息已经存储在`destinode`中，inode的偏移量存储在了`destoffset`中；接着，还要验证一下`flags`在`O_DIRECTORY`位为1的情况下`destinode.type`是否为DIRECTORY_TYPE，以防打开已存在文件的类型与`flags`矛盾的情况出现；
      	以上操作均进行之后，则当前进程可以打开该文件：首先遍历系统打开文件表`fcb`中看是否已打开该文件，若遍历找到fcb第i项的`inodeOffset`等于`destoffset`且该项的`state`为1，则说明该文件已被系统打开，只需要将`fcb[i]`的`linkcount`加一，然后找到当前进程的`fd[]`中空闲的一项填入`i+3`就可以了；否则，遍历找到`fcb`中`state`为0的一项`fcb[i]`，填入`flags`、`destoffset`，置`offset`为0，`linkcount`为1，`state`为1，使得系统打开该文件，再在当前进程的`fd[]`中找到空闲一项填入`i+3`即可；
      	PS：进程打开表中填写`i+3`而非`i`的原因是：考虑到linux下每次`fork`一个打开了`STDIN`、`STDOUT`、`STDERR`的进程时，子进程也会自动打开这写文件，且下标为0、1、2；所以在我的实现中默认这三个文件一直是打开的，且占用下标为0、1、2；因此默认fd为0、1、2时就代表上述三个文件，故之后fcb中记录的打开的文件描述符均要加3，fd同理；</font>
  - <font size=4 face="楷体">`read`：主要内容就是实现系统调用函数`syscallreadfile`；首先要对`syscallRead`进行更改：对于`STDIN`的读取操作不变（`/dev/stdin`文件只是用于占位的），但对于共享内存的读取需要改变：这时的共享内存已经放在了`/dev/shmem`中，进程要通过`open`打开该文件才可对其操作，所以将之前的`uint8_t shMem[MAX_SHMEM_SIZE]`删除，并删除`syscallWriteShMem、syscallReadShMem`两个函数；所以此时读取共享内存就和读取普通的文件没有区别了；只要`fd`在合理范围[3,12]中（`fcb`大小为10，故文件描述符范围在[0,12]，去除前三个特殊文件即为[3,12]）就可以调用`syscallreadfile`进行读取；
    	`syscallreadfile`的实现：从`tf`中取出`read`的文件操作符`current_fd`；接着由`fcb[current_fd].flags`得到`flags`；判断`flags`的`O_READ`位是否为1，不是则返回-1；如果可以读取则调用`diskRead`读取inode信息到`temp_inode`中；之后执行：</font>

  ```c
  	int sel = tf->ds;//用于之后读取数据
  	uint8_t* buffer = (uint8_t*)tf->edx;//存放读取的数据
  	uint8_t temp_buffer[sBlock.blockSize];
  	int read_size = tf->ebx;
  	if(read_size > temp_inode.size - fcb[current_fd].offset) 
          read_size = temp_inode.size - fcb[current_fd].offset;
  	if(read_size <= 0)
  	{
  		pcb[current].regs.eax = 0;
  		return;
  	}
  	//设置读取的字节，如果超出了文件剩余的大小(指减去offset后的大小)则最多
  	//只能读取temp_inode.size - fcb[current_fd].offset大小字节的数据
  
  	int current_block_number = fcb[current_fd].offset/sBlock.blockSize;
  	//应从文件的current_block_number个block开始读
  	int block_offset = fcb[current_fd].offset%sBlock.blockSize;
  	//应从该block中第block_offset个字节开始读
  	readBlock(&sBlock,&temp_inode,current_block_number,temp_buffer);
  	//读取第current_block_number个block的数据到temp_buffer中
  	asm volatile("movw %0, %%es"::"m"(sel));
  ```

  <font size=4 face="楷体">	接下来进行判断：如果要读取的数据都在第`current_block_number`个block上，则直接从`temp_buffer`中读取`read_size`个字节就好了：</font>

  ```c
  	if(read_size <= (int)(sBlock.blockSize - block_offset))
  	{
  		int i = 0;
  		for(;i < read_size; ++i)
  		{
  			asm volatile("movb %0, %%es:(%1)"::"r"
              (temp_buffer[block_offset+i]),"r"(buffer + i));
              
  			fcb[current_fd].offset++;
  		}
  		pcb[current].regs.eax = i;
  		return;
  	}
  ```

  <font size=4 face="楷体">	否则，说明要跨block读取；则第一步要将第`current_block_number`个block读完：</font>

  ```c
  int i = 0;
  	int j = 0;
  	for(;i < sBlock.blockSize - block_offset; ++i)
  	{
  		asm volatile("movb %0, %%es:(%1)"::
                       "r"(temp_buffer[block_offset+i]),"r"(buffer + j));
          
  		fcb[current_fd].offset++;
  		read_size--;
  		j++;
  	}
  ```

  <font size=4 face="楷体">	然后循环读取后面所有占据一整个block的数据：</font>

  ```c
  	while(read_size > sBlock.blockSize)//当还有数据占有一整个block时进行
  	{
  		current_block_number++;//读一下个block
  		readBlock(&sBlock,&temp_inode,current_block_number,
                    temp_buffer);//block的数据读到temp_buffer中
  		for(i = 0;i < sBlock.blockSize; ++i)
  		{
  			asm volatile("movb %0, %%es:(%1)"::
                           "r"(temp_buffer[i]),"r"(buffer + j));	
              
  			fcb[current_fd].offset++;
  			j++;
  		}
          //读取一整个block的数据
  		read_size -= sBlock.blockSize;//读取数据减少sBlock.blockSize
  	}
  ```

  <font size=4 face="楷体">	则循环结束后，剩余数据全部位于下一个block上，读取完即可：</font>

  ```c
  	current_block_number++;//最后一个block
  	readBlock(&sBlock,&temp_inode,current_block_number,
                temp_buffer);//读到temp_buffer中
  	for(i = 0;i < read_size; ++i)
  	{
  		asm volatile("movb %0, %%es:(%1)"::
                       "r"(temp_buffer[i]),"r"(buffer + j));
          
  		fcb[current_fd].offset++;
  		j++;
  	}
  	//读取剩余的所有数据
  	pcb[current].regs.eax = j;
  	return;
  ```

  <font size=4 face="楷体">	由此实现了read；</font>

  - <font size=4 face="楷体">`write`：与`read`的实现基本一样，同样实现了对共享内存的写要先open后写；`syscallWritefile`中，思路与`syscallReadfile`完全一样，操作的方法也基本一样，只是把读改成写；唯一要注意的就是如果写的大小超过了文件的剩余大小(指减去offset后的大小)，则要调用`allocBlock`增大文件大小后写，具体实现为：</font>

    ```c
    	if(write_size < 0)
    	{
    		pcb[current].regs.eax = 0;
    		return;
    	}
    	while(write_size + fcb[current_fd].offset > 
              temp_inode.blockCount * sBlock.blockSize)
    	{
    		int ret = allocBlock
                (&sBlock,&temp_inode,fcb[current_fd].inodeOffset);
    		if(ret == -1)
    		{
    			pcb[current].regs.eax = -1;
    			return;
    		}
    	}
    ```

    <font size=4 face="楷体">	接着的写操作就和`read`中一样，不再赘述；最后，与`read`不同的是，写完后还要调用`writeBlock`与`diskWrite`将`temp_inode`的内容写回去。</font>

  - <font size=4 face="楷体">`lseek`：`lseek`的系统调用`syscallLseek`较为简单，就是通过文件描述符得到系统已打开文件表对应表项的文件指针偏移`fcb[current_fd].offset`，再通过`diskRead`读取相应inode、`tf`得到参数`offset`与`whence`；利用`fcb[current_fd].offset`（当前文件指针偏移`cfo`）、`temp_inode.size`（文件大小）、`offset`（传入参数offset）与`whence`对`fcb[current_fd].offset`进行设置即可，不再赘述。</font>

  - <font size=4 face="楷体">`close`：`close`的系统调用`syscallClose`最为简单：首先由`tf`得到文件描述符`current_fd = tf->ecx - 3`；然后将系统打开文件表中对应项`fcb[current_fd]`的`linkcount`减一，如果减后为0则置`fcb[current_fd].state`为0，由此实现系统关闭该文件；接着在`pcb[current].fd[]`中遍历找到值为`current_fd + 3`的，即找到进程打开文件表中该文件的记录，置为-1即可实现进程对该文件的关闭。</font>

  - <font size=4 face="楷体">`remove`：`remove`的系统调用`syscallRmove`的实现就是移植了函数`rmdir`与`rm`。首先判断是否是`/dev`下的文件，是则不可以删；否则通过`fcb`读取相应的inode，通过其类型判断是按照`rmdir`还是`rm`操作，具体内容不再赘述。</font>

    <font size=4 face="楷体">下图即尝试删除`stdin`失败的例子：</font>

    ![](C:\Users\njuwhl2019hp\Desktop\os\lab5_11.png)

- <font size=5 face="楷体" color=#00bfff>用户程序ls与cat实现：</font>

  <font size=4 face="楷体">	1、**ls**：
	在`syscall.c`中实现`ls`函数：思路为用过`open`打开传入的目录文件，然后循环调用`read`函数读取目录文件的内容；由于一个`DirEntry`结构的大小为128字节，所以目录文件的大小也必然是128的倍数；故每次`read`128字节，利用`DirEntry`结构体得到每次读取出来的目录项的name，并将不是'.'、'..'的name拼接起来；这样当`read`返回值为0的时候则读完整个目录文件，将拼接的内容打印即可。</font>
  
  <font size=4 face="楷体">	2、**cat**：在`syscall.c`中实现`cat`函数。实现的`cat`函数只是用于显示一个文件的全部内容，所以只要`open`相应文件后循环`read`，直到读完整个文件，并将读取的内容调用`printf`打印即可。</font>

  

- <font size=5 face="楷体" color=#00bfff>简单的shell实现</font>

  <font size=4 face="楷体">	实现shell的思路也比较简单：首先就是在`app/main.c`里写一个总体的shell流程控制；利用二重循环对其进行了实现，同时在里面维护了一个变量`current_dir`作为当前目录，这样`fork`后也能保证父子进程共享同样的当前目录；用变量`command`存储输入的命令行内容：</font>
  
  ```c
  	char current_dir[100];
  	current_dir[0] = '/';	//初始化为根目录
  	current_dir[1] = '\0';
  	char command[100];
  	command[0] = '\0';	//用于存放每次的命令行内容
  	int i = 0;
  	char tmp;
  	int fd = 0;
  ```

<font size=4 face="楷体">	为了对`cat`程序进行测试，这里还建立了一个测试文件`cat_`并写入字母表：</font>

```c
	fd = open("/cat_", O_RDWR | O_CREATE); // 创建⽂件"/cat_"
	for (i = 0; i < 26; i ++) { // 向"/cat"⽂件中写⼊字⺟表
	tmp = (char)(i % 26 + 'A');
	write(fd, (uint8_t*)&tmp, 1);
	}
	close(fd);
```

<font size=4 face="楷体">	接下来就是整个shell的运行流程实现了：主体就是一个死循环，用于不断接收命令行；在循环体中，首先利用`scanf`读取命令行并利用`printf`将输入的内容打印出来；</font>

```c
		printf("current_dir:%s\n",current_dir);
		i = 0;
		while(1)
		{
			scanf("%c",&tmp);
			printf("%c",tmp);
			if(tmp == '\n')
			{
				command[i] = '\0';
				break;
			}
			command[i] = tmp;
			i++;
		}
```

<font size=4 face="楷体">	这样，输入的命令行就存储在`command`中了，接下来要将命令与命令内容进行分割（规定输入时命令与命令行内容以空格分割，例如`cd ../a`）；将命令（举例中的`cd`）存放在`command`中，命令行内容(举例中的`../a`)存放在变量`content`中：</font>

```c
		int k = 0;
		while(command[k]!=' ' && command[k]!= '\0')
		{
			k++;
		}//k标记了命令结束的位置
		command[k] = '\0';k++;
		//设置第k位为'\0'，相当于清空后面的内容
		//k++后指向命令行内容的起始处
		
		int t = 0;
		char content[100];
		while(command[k]!='\0')
		{
			content[t] = command[k];
			t++;
			k++;
		}
		content[t] = '\0';//利用循环将命令行内容存储到content中
```

<font size=4 face="楷体">	接下来就要根据`command`的内容执行相应操作：如果是`cd`的话，则要利用`content`中的内容对`current_dir`进行更新；为方便验证当前目录下是否存在要进入的目录，实现了函数`if_exit`：</font>

```c
int if_exit(const char* str,const char* current_dir,int dir_or_file){
	//current_dir是当前目录，str是命令行内容；
    //dir_or_file表示str指代的是文件还是目录，前者为0，后者为1
    char f[100];
	int i = 0;
	int j = 0;
	while(current_dir[j] != '\0')
	{
		f[i] = current_dir[j];
		i++;j++;
	}
	j = 0;
	while(str[j] != '\0')
	{
		f[i] = str[j];
		i++;
		j++;
	}
	f[i] = '\0';
    //将str与current_dir拼接为f
	int fd = -1;
	if(dir_or_file == 0)
	{
		fd = open(f,O_RDWR);
	}
	else
	{
		fd = open(f,O_DIRECTORY | O_RDWR);
	}
    //利用open是否成功判断是否存在该文件，传入flags的O_CREAT位为0
	if(fd == -1) return 0;
	else
	{
		close(fd);
		return 1;
	}
	
```

<font size=4 face="楷体">	对于`cd`，要考虑的情形只有三种：`'cd ..'、'cd .'`与其它；为了处理类似`../a/b/./c`这样的复杂内容，在`cd`处理的内部对`content`继续分割，每次分割出一级放入`cur_content`中并处理，从而依次处理所有内容；`cd`的具体实现为：</font>

```c
		if(command[0]=='c'&&command[1]=='d'&&command[2]=='\0')
		{
			int s = 0;
			while(content[s]!='\0')//对整个content进行分割，依次处理
			{
				int r = 0;
				char cur_content[10];
				while(content[s]!='/' && content[s]!='\0')
				{
					cur_content[r] = content[s];
					r++;s++;
				}
                //以'/'分割，将取出的内容存到cur_content中
				cur_content[r] = '/';
				cur_content[r+1] = '\0';
				//printf("%s\n",cur_content);
				if(content[s] == '/') s++;
                //说明后面还有内容，s指向下一个内容的起始
				if(cur_content[0] == '.' && cur_content[1] == '.'
                   && cur_content[2] == '/')
                //当前处理的是cd ..
				{
					if(current_dir[1] == '\0')
					{
						printf("invaild command\n");
						break;
                        //根目录下不能返回上一级
					}
					else
					{
						int a = 0;
						while(current_dir[a]!='\0') a++;
						a--;
						if(current_dir[a]=='/') a--;
						while(current_dir[a]!='/') a--;
						current_dir[a+1] = '\0';
                        //将current_dir进行更新
					}
					
				}
				else if(cur_content[0] == '.' && cur_content[1] == '/')
				{
					;
                    //当前处理'cd .'  不必执行任何操作
				}
				else
				{
                    //处理的是cd某个文件目录
					if(if_exit(cur_content,current_dir,1))
                    //判断当前目录下是否存在要进入的目录
					{
						int a = 0;
						while(current_dir[a]!='\0') a++;
						k = 0;
						while(cur_content[k]!='\0')
						{
							current_dir[a] = cur_content[k];
							a++;
							k++;
						}
						if(current_dir[a-1] != '/')
						{
							current_dir[a] = '/';
							a++;
						} 
						current_dir[a] = '\0';
						//存在，将内容拼接到current_dir上
					}
					else
					{
						printf("invaild command\n");
						break;
					}
					
				}
			}
			
			
		}
```

<font size=4 face="楷体">	对于其它的`ls`、`mkdir`、`./`、`touch`、`rm`、`cat`指令，实现的方法都是一样的：利用`fork`生成子进程，在子进程中设置好参数，调用`exec`使子进程运行相应程序，父进程则睡眠一段时间。以`mkdir`为例：</font>

```c
		else if(stringCmp("mkdir",command,5) == 0 && command[5] == '\0')
		{
			int ret = fork();
			if (ret == 0) {
				char* argv[4];
				argv[0] = "/usr/mkdir";
				argv[1] = current_dir;
				argv[2] = content;
				argv[3] = NULL;
                //设置exec要传递的参数，argv[1]、argv[2]分别是当前目录与命令行内容
				exec("/usr/mkdir\0", argv);
			}
			else if (ret != -1) {
				sleep(128);
			}	
		}
```

<font size=4 face="楷体">	因此就需要实现`exec`的传参；传参的方式有很多种，例如可以在`exec`中将`argv`压栈后，在用户程序里取出；总之只要将参数存放到`main`函数可以找到的地址就好了；这里我的实现是建立了一个`/argv`文件，并更改`syscall.c`中的`exec`为：</font>

```c
int exec(const char *filename, char * const argv[]) {
	int i = 1;//argv[0]是filename，故从1开始存
	int fd = open("/argv",O_RDWR);//打开文件
	char end[1];end[0] = '\0';参数之间以'\0'分隔
	while(argv[i]!=NULL)
	{
		//printf("content:%s\n",argv[i]);
		write(fd,(uint8_t*)argv[i],stringLen(argv[i]));
		write(fd,(uint8_t*)end,1);
        //向文件中写入argv[i]并写入一个'\0'
		i++;
	}
	
	close(fd);//关闭文件
	return syscall(SYS_EXEC, (uint32_t)filename, (uint32_t)argv, 0, 0, 0);
}
```

<font size=4 face="楷体">	这样，在用户程序的`main`中，只要打开`/argv`文件读取需要的参数就可以了。基于此，实现了`ls`、`mkdir`、`./`、`touch`、`rm`、`cat`这些用户程序（添加用户程序的方式在lab3中有提）。</font>

<font size=4 face="楷体">	`ls`与`cat`的实现之前已经说明了，而`mkdir`、`touch`的实现思路均为先检测当前目录下是否存在该文件，若不存在则调用`open`创建；`rm`的实现思路则是先判断当前目录下是否存在该文件，若存在则调用`remove`删除；对于`./`，实现了用户程序`run`，在`main.c`中先检测是否存在该文件，存在则调用`exec`加载该程序执行。</font>

<font size=4 face="楷体">	这样就实现了一个简易版的shell。</font>

<font size=4 face="楷体">	PS：由于`stringCmp`只是比较了前size个字节，所以如果目录下有文件`mytest`，那么用`my`也会检索到，即执行`rm my`、`mkdir my`等操作也会检索到`mytest`，从而导致不可预知的错误；因此我对`stringCmp`进行了修改：</font>

```c
int stringCmp (const char *srcString, const char *destString,
               int size) { 
    int i = 0;
    if (srcString == NULL || destString == NULL)
        return -1;
    while (i != size) {
        if (srcString[i] != destString[i])
            return -1;
        else if (srcString[i] == 0)
            return 0;
        else
            i ++;
    }
    if(srcString[i] == 0)
    {
        if(destString[i] != 0 && destString[i] != '/')
            return -1;
    }
    if(destString[i] == 0)
    {
        if(srcString[i] != 0 && srcString[i] != '/')
            return -1;
    }
    return 0;
}
```

<font size=4 face="楷体">	考虑到`stringCmp`比较的两个字符串如果返回0，一定是一个字符串包含了另外一个且会以'\0'或'/'结尾，因此添加了末尾的再次判断，从而解决了以上问题。测试结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\os\lab5_13.png)