# <font face="楷体"><center>计网作业4</center></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖
学号：181860092
邮箱：603102048@qq.com</font></p>
------

### <font face="楷体">第4章</font>

<font size=4 face="楷体" color=#00bfff>R14. Visit a host that uses DHCP to obtain its IPaddress, network mask, default router, and IPaddress of its local DNS server. List these values. </font>

<font size=4 face="楷体">	答、Linux下查看结果为：主机的ip地址为192.168.5.142，网络掩码为255.255.255.0，默认路由器为192.168.5.2，其本地DNS服务器的IP地址为127.0.0.53。</font>



<font size=4 face="楷体" color=#00bfff>R16. Suppose an application generates chunks of 40 bytes of data every 20 msec, and each chunk gets encapsulated in a TCP segment and then an IP datagram. What percentage of each datagram will be overhead, and what percentage will be application data? </font>

<font size=4 face="楷体">	答、考虑一般情况下，IP数据报具有20字节的首部，TCP首部不计选项和填充部分也是20字节，所以overhead占比为：$\frac{20+20}{20+20+40} *100\%=50\%$；则data占比就是：$100\%-50\% = 50\%$。</font>



<font size=4 face="楷体" color=#00bfff>R19. Compare and contrast the IPv4 and the IPv6 header fields. Do they have any fields in common?</font>

<font size=4 face="楷体">	答、二者的直观比对如下图所示：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\计网\作业1.png" style="zoom: 50%;" />

<font size=4 face="楷体">	从上图可以看出，ipv6相较于ipv4，完全保留了基本的version与srcaddress、dstaddress部分，而且ipv4的type of service整合进了ipv6的traffic class；ipv4的total length整合进了ipv6的payload length；ipv4的protocol整合进了ipv6的next header；ipv4的ttl整合进了ipv6的hop limit；这些都是共同的部分；不同的部分主要有：删去了fragmentation，将分片、重组工作放在端系统（源与目的）处理；删去了checksum，取消了ipv6检验和操作；删除了options部分，同时扩展地址为128 bit，将ipv6的header的长度固定为了40字节；增加了flow lable部分，用于标识一条数据报的流。</font>



<font size=4 face="楷体" color=#00bfff>R20. It has been said that when IPv6 tunnels through IPv4 routers, IPv6 treats the IPv4 tunnels as link-layer protocols. Do you agree with this statement? Why or why not?</font>

<font size=4 face="楷体">	答、当ipv6的数据报通过ipv4的隧道时，ipv4会将ipv6的数据报包在一个ipv4格式的数据报内发送（将整个ipv6数据报，包括header包裹），从这个角度理解，与链路层协议加上链路层的header来传输是一样的，故可以认为是正确的。</font>



<font size=4 face="楷体" color=#00bfff>R23. Is it necessary that every autonomous system use the same intra-AS routing algorithm? Why or why not?</font>

<font size=4 face="楷体">	答、没有必要，因为AS具有内部的路由管理自治权，每个AS内部选择什么路由算法（每个AS内应为一样的路由算法）对于各个AS的boarder router间运行的不同自治系统之间的路由选择协议不会产生任何影响。</font>



<font size=4 face="楷体" color=#00bfff>R29. Define and contrast the following terms: subnet, prefix, and BGP route.</font>

<font size=4 face="楷体">	答、subnet：子网可以看作一个更大网络的一部分；一个较大的网络可以通过路由器或主机端口划分出一部分作为子网；一个子网内部没有路由器；prefix：前缀用来唯一地标识一个CDIR策略化的IP地址的网络部分，具有a.b.c.d/x的形式；BGP route：当一个路由器通过BGP会话通告一个前缀时,它在前缀中包括一些BGP属性，带有属性的前缀就被称为一条BGP路由；三者相比，一个prefix可以标识一个或多个子网，BGP route是在AS互相通信时对一个AS的标识，可看作一种prefix。</font>



<font size=4 face="楷体" color=#00bfff>R35. What are the roles played by the IGMP protocol and a wide-area multicast routing protocol? </font>

<font size=4 face="楷体">	答、IGMP协议的作用就是允许一个主机上运行的某个应用程序选择一个想加入的多播组，然后把这个信息通知给第一条多播路由器；同时多播路由器也能利用IGMP周期性地查询局域网内的组播组成员是否处于活动状态，实现所连网段组成员关系的收集与维护；而多播路由协议的作用就是各个多播路由器运行此协议，来保证各主机可以与所连入多播组的最后一条路由器正常通信。</font>



<font size=4 face="楷体" color=#00bfff>R36. What is the difference between a group-shared tree and a source-based tree in the context of multicast routing? </font>

<font size=4 face="楷体">	答、二者的区别在于：前者构建一棵包括了所有具有属于该多播组的相连主机的边缘路由器的树，组中所有的发送方都通过此树来发送多播通信；后者则是为每个独立的发送方建立一棵特定源的路由选择树，特定的发送方会通过专为自己源建立的路由选择树选择路由；这就要求路由器能够了解多个基于源的路由选择树的内容。</font>



<font size=4 face="楷体" color=#00bfff>P13. Consider a router that interconnects three subnets: Subnet 1, Subnet 2, and Subnet 3. Suppose all of the interfaces in each of these three subnets are required to have the prefix 223.1.17/24. Also suppose that Subnet 1 is required to support at least 60 interfaces, Subnet 2 is to support at least 90 interfaces, and Subnet 3 is to support at least 12 interfaces. Provide three network addresses (of the form a.b.c.d/x) that satisfy these constraints.</font>

<font size=4 face="楷体">	答、都具有223.1.17/24的前缀，则3个子网总共可用的范围大小为${2}^{8}=256$，其中子网1要至少占60，则可为其分配0-63的资源，即00000000-00111111的地址，故子网1可为223.1.17.0/26；子网2要至少占90，为方便地址表示为其分配${2}^{7}=128$个地址资源，可以将01000000-01111111分配给子网2，这样子网2就为223.1.17.64/26；最后子网3至少占12，可以给其分配16个地址资源，可以将10000000-10001111分给子网3，故子网3为223.1.17.128/28.（分法不唯一）</font>



<font size=4 face="楷体" color=#00bfff>P17. Consider the topology shown in Figure 4.17. Denote the three subnets with hosts (starting clockwise at 12:00) as Networks A, B, and C. Denote the subnets without hosts as Networks D, E, and F. a. Assign network addresses to each of these six subnets, with the following constraints: All addresses must be allocated from 214.97.254/23; Subnet Ashould have enough addresses to support 250 interfaces; Subnet B should have enough addresses to support 120 interfaces; and Subnet C should have enough addresses to support 120 interfaces. Of course, subnets D, E and F should each be able to support two interfaces. For each subnet, the assignment should take the form a.b.c.d/x or a.b.c.d/x – e.f.g.h/y. b. Using your answer to part (a), provide the forwarding tables (using longest prefix matching) for each of the three routers. </font>

<font size=4 face="楷体">	答、
	a、从214.97.254/23开始，首先为A分配：A要求250，为方便分配给其256个IP地址资源，故A取214.97.254/24;然后为B分配：B要求120，为方便分配给其128个IP地址资源，B取214.97.255.0/25；同样的，为C分配128个IP地址资源，C取214.97.255.128/25；接下来为D、E、F分配：考虑到D、E、F需要的地址资源很少，没有必要从214.98/16中再次划分，所以可以从有空位的B、C中划分出一部分来表示；这里选取从B划分，B只需要120个地址资源，故可以划分214.97.255.0/25 - 214.97.255.0/29这部分为B；空闲出来的8个资源，D取214.97.255.0/31这两个，E取214.97.255.2/31这两个，剩下的4个给F，故F取214.97.255.4/30；
	b、转发表内容如下：</font>

<font size=4 face="楷体">	router1：</font>

| 最长前缀匹配                       | 转发port |
| ---------------------------------- | -------- |
| 11010110 01100001 11111110         | A        |
| 11010110 01100001 11111111 0000000 | D        |
| 11010110 01100001 11111111 000001  | F        |

<font size=4 face="楷体">	router2：</font>

| 最长前缀匹配                       | 转发port |
| ---------------------------------- | -------- |
| 11010110 01100001 11111111 1       | C        |
| 11010110 01100001 11111111 0000001 | E        |
| 11010110 01100001 11111111 000001  | F        |

<font size=4 face="楷体">	router3：</font>

| 最长前缀匹配                       | 转发port |
| ---------------------------------- | -------- |
| 11010110 01100001 11111111 0       | B        |
| 11010110 01100001 11111111 0000000 | D        |
| 11010110 01100001 11111111 0000001 | E        |



<font size=4 face="楷体" color=#00bfff>P22. Suppose you are interested in detecting the number of hosts behind a NAT. You observe that the IPlayer stamps an identification number sequentially on each IPpacket. The identification number of the first IPpacket generated by a host is a random number, and the identification numbers of the subsequent IP packets are sequentially assigned. Assume all IPpackets generated by hosts behind the NAT are sent to the outside world. a. Based on this observation, and assuming you can sniff all packets sent by the NAT to the outside, can you outline a simple technique that detects the number of unique hosts behind a NAT? Justify your answer. b. If the identification numbers are not sequentially assigned but randomly assigned, would your technique work? Justify your answer. </font>

<font size=4 face="楷体">	答、
	a、可以将所有发出的IP分组捕获，然后分析：因为每个主机发出的IP分组首先有一个随机数作为标识号，然后顺序分配后面的IP分组标识号，这样就可以区分开每个主机的第一个IP分组的标识号与后续IP分组标识号；所以只需要统计第一个IP分组的标识号并计数，最后得到的结果就是NAT后面的主机数量；
	b、显然，如果标识号都是随机分配的，那就无法区分第一个IP分组与后续IP分组了，a中的方法也就自然失效了。</font>



<font size=4 face="楷体" color=#00bfff>P35. Describe how loops in paths can be detected in BGP. </font>

<font size=4 face="楷体">	答、可以利用BGP中通信传播的AS_PATH来判断：AS_PATH包含了从对应AS到达目的地的完整路径，所以只要检测到当前路由的AS_PATH中包含了自己的AS number，那么选择这条路由就会产生loop。</font>



<font size=4 face="楷体" color=#00bfff>P37. Consider the network shown below. Suppose AS3 and AS2 are running OSPF for their intra-AS routing protocol. Suppose AS1 and AS4 are running RIP for their intra-AS routing protocol. Suppose eBGPand iBGPare used for the inter-AS routing protocol. Initially suppose there is no physical link between AS2 and AS4.a. Router 3c learns about prefix x from which routing protocol: OSPF, RIP, eBGP, or iBGP? b. Router 3a learns about x from which routing protocol? c. Router 1c learns about x from which routing protocol? d. Router 1d learns about x from which routing protocol?</font>

<font size=4 face="楷体">	答、
	a、x在AS4内，所以3c作为AS3的边缘路由器可以通过eBGP协议学习到前缀x；	b、3a是在3c学习到前缀x后又经过iBGP学习到的；所以是iBGP；	c、1c作为AS1的边缘路由器，只能通过eBGP从AS3得到前缀x（AS2、AS4不存在物理链路）；所以是eBGP；
	d、类似b，从1c得到，为iBGP。</font>



<font size=4 face="楷体" color=#00bfff>P45. Consider the two basic approaches identified for achieving broadcast, unicast emulation and network-layer (i.e., router-assisted) broadcast, and suppose spanning-tree broadcast is used to achive network-layer broadcast. Consider a single sender and 32 receivers. Suppose the sender is connected to the receivers by a binary tree of routers. What is the cost of sending a broadcast packet, in the cases of unicast emulation and network-layer broadcast, for this topology? Here, each time a packet (or copy of a packet) is sent over a single link, it incurs a unit of cost. What topology for interconnecting the sender, receivers, and routers will bring the cost of unicast emulation and true network-layer broadcast as far apart as possible? You can choose as many routers as you’d like. </font>

<font size=4 face="楷体">	答、用二叉树结构的路由，连接32个接收方，形成的拓扑就是一棵满二叉树，其中满二叉树高度为5（root的高度为0），所以单组模拟时，每传到一个接收方会经过5跳，所以单组模拟的费用为`32 * 5 = 160`；而网络层广播的费用，首先root传两个分组到1层，1层每个router再传2个分组到第二层……这样进行5次，32个接收方全部接收到该分组，费用为`2 + 4 + 8 + 16 + 32 = 62`;
	如果要考虑单播模拟与网络层广播的最大费用差距，那么拓扑应该为一棵单支树，也就是一条直线，这时单播模拟会重复传播很多跳，二者的差距会达到最大。</font>