# <font face="楷体"><center>操作系统文件系统一章作业</center></font>

<p align="right"><font size=4 face="楷体">学号：181860092
姓名：王瀚霖
邮箱：603102048@qq.com</font></p> 
<font size=4 face="楷体">4、
	答、`open`操作的目的在于将磁盘中的inode复制到活动inode表中并建立用户打开文件表项和系统打开文件表项以便于后续访问；如果没有`open`操作的话，在读写文件的时候就必须要提供文件名等信息来获取相应的inode位置；如果没有`open`，那每读写一次就要访问磁盘inode一次，效率太低；引入Cache可以一定程度缓解此问题，但这样实际上与`open`的工作类似；另外如果没有`open`，那`close`也要相应调整；如果去掉`close`，则修改inode信息后应该写回磁盘；若每次写之后都写回磁盘则效率太低；总而言之，`open`操作是可以被其它操作替换的，但引入`open`操作对于实现效率的提高有一定作用。</font>



<font size=4 face="楷体">6、
	答、有区别：首先如果磁盘剩余空间不足，则无法执行复制操作；另外，rename只是改了文件名字，不会更改其他信息；但复制新文件再删除原文件的话，除了文件名被更改，文件的创建、修改、访问时间也会变化；而且rename不会更改inode信息，但先复制后删除的话新文件的inode信息必然与原文件不同。</font>



<font size=4 face="楷体">19、
	答、如果要直接在此inode中保存一个短文件，则必须有一个信息表明此时inode中各个块中保存的不是指针而是文件数据；假设每个地址都是x字节，指示信息占用y字节；则该inode中最多可以存放`9x-y`字节的数据；要注意的是该信息必须与正常情况下的指针有区分，例如可以用一个特殊的、正常指针绝对指不到的地址来表示；可以将该信息存放在“磁盘块0的地址”中，则此时该inode中最多可以存放$9x-y=9x-x=8x$字节的数据。</font>



<font size=4 face="楷体">22、
	答、在inode分配上，硬链接不会申请新的inode存放信息，只是会新建一个指向该已存在文件inode节点的目录项，并将该inode的连接计数i_node值加一，所以所有硬链接的目录条目都指向一个inode；而软链接会申请一个新的inode节点，其指向的数据块中存储了被链接的文件（已存在）的路径指向信息。</font>



<font size=4 face="楷体">40、
	答、inode的10个直接目录指向了10个块；1个一级目录指向了1个块，里面存放了$\frac{4KB}{4byte}=2^{10}$个指针，指向$2^{10}$个数据块；1个二级目录指向了1个块，里面存放了$\frac{4KB}{4byte}=2^{10}$个一级目录，共指向$2^{20}$个数据块；1个三级目录指向了1个块，里面存放了$\frac{4KB}{4byte}=2^{10}$个二级目录，指向$2^{30}$个数据块；故最大文件大小为：</font>
$$
4KB*(10+2^{10}+2^{20}+2^{30})=4299165736KB≈4TB
$$




<font size=4 face="楷体">41、
	答、需要执行以下10个读磁盘操作：</font>

- <font size=4 face="楷体">首先根据根目录inode读取磁盘，将对应的物理块（存储着文件目录）读入，并搜索到/usr；</font>
- <font size=4 face="楷体">搜索到/usr后，得到/usr的inode号，然后为/usr分配一个活动inode，读取磁盘中/usr的inode，复制到活动inode中；</font>
- <font size=4 face="楷体">根据/usr的inode读取磁盘，将对应的物理块读入，并搜索到/usr/ast；</font>
- <font size=4 face="楷体">搜索到/usr/ast后，得到/usr/ast的inode号，然后为/usr/ast分配一个活动inode，读取磁盘中/usr/ast的inode，复制到活动inode中；</font>
- <font size=4 face="楷体">根据/usr/ast的inode读取磁盘，将对应的物理块读入，并搜索到/usr/ast/courses；</font>
- <font size=4 face="楷体">搜索到/usr/ast/courses后，得到/usr/ast/courses的inode号，然后为/usr/ast/courses分配一个活动inode，读取磁盘中/usr/ast/courses的inode，复制到活动inode中；</font>
- <font size=4 face="楷体">根据/usr/ast/courses的inode读取磁盘，将对应的物理块读入，并搜索到/usr/ast/courses/os；</font>
- <font size=4 face="楷体">搜索到/usr/ast/courses/os后，得到/usr/ast/courses/os的inode号，然后为/usr/ast/courses/os分配一个活动inode，读取磁盘中/usr/ast/courses/os的inode，复制到活动inode中；</font>

- <font size=4 face="楷体">根据/usr/ast/courses/os的inode读取磁盘，将对应的物理块读入，并搜索到/usr/ast/courses/os/handout.t；</font>
- <font size=4 face="楷体">搜索到/usr/ast/courses/os/handout.t后，得到/usr/ast/courses/os/handout.t的inode号，然后为/usr/ast/courses/os/handout.t分配一个活动inode，读取磁盘中/usr/ast/courses/os/handout.t的inode，复制到活动inode中，即完成了/usr/ast/courses/os/handout.t的inode的调入。</font>