# <font face="楷体"><center>计网作业3</center></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖
学号：181860092
邮箱：603102048@qq.com</font></p>
------

### <font face="楷体">第4章</font>

<font size=4 face="楷体" color=#00bfff>R1. Let’s review some of the terminology used in this textbook. Recall that the name of a transport-layer packet is segment and that the name of a link-layer packet is frame. What is the name of a network-layer packet? Recall that both routers and link-layer switches are called packet switches. What is the fundamental difference between a router and link-layer switch? Recall that we use the term routers for both datagram networks and VC networks.</font>

<font size=4 face="楷体">	答、网络层分组的名字是数据报(datagram)。router与switch的根本区别在于对于数据报文的转发依据不同；router根据数据报文的IP地址进行转发，switch根据数据报文的Mac地址进行转发。</font>



<font size=4 face="楷体" color=#00bfff>R2. What are the two most important network-layer functions in a datagram network? What are the three most important network-layer functions in a virtualcircuit network? </font>

<font size=4 face="楷体">	答、数据报网络中，网络层的两个最重要功能是路由选择与转发数据报；虚电路网络中，网络层的三个最重要功能是路由选择、转发数据报与呼叫建立。</font>



<font size=4 face="楷体" color=#00bfff>R21. Compare and contrast link-state and distance-vector routing algorithms. </font>

<font size=4 face="楷体">	答、二者的区别在于：前者通过广播使得每个节点得到整个网络的完整信息，这样每个节点就可以利用算法计算出从源点到目的地址的least cost path，而且此path是确定且收敛的；后者是通过分布式+迭代的方法计算的：每个节点只能通过接收相邻节点的信息，判断为了达到least cost应该将数据报发送到哪里，进而通过迭代计算出将数据报发往目的地址的least cost path，此方法需要一定的收敛时间。</font>



<font size=4 face="楷体" color=#00bfff>P1. In this question, we consider some of the pros and cons of virtual-circuit and datagram networks. a. Suppose that routers were subjected to conditions that might cause them to fail fairly often.  Would this argue in favor of a VC or datagram architecture?  Why? b. Suppose that a source node and a destination require that a fixed amount of capacity always be available at all routers on the path between the source and destination node, for the exclusive use of traffic flowing between this source and destination node. Would this argue in favor of a VC or datagram architecture?  Why? c. Suppose that the links and routers in the network never fail and that routing paths used between all source/destination pairs remains constant.  In this scenario, does a VC or datagram architecture have more control traffic overhead?  Why?</font>

<font size=4 face="楷体">	答、
	a、对于虚电路网络而言，如果在虚电路建立之后，某一个甚至多个路由器无法正常运转，那么一个路由器的无法正常运转就会引起所有与这个路由器相关的路由无法正常进行，这就要求路由上层的路由器要重新建立一个绕开故障路由器、将数据传输到目的节点的路径；同时，故障路由器下层路由器建立好的连接也无法工作，也需要删除各自的连接重新建立；对于数据报网络而言，如果一个路由器或多个路由器出现了故障，其余路由器只要将转发表通过距离向量算法或链路状态算法进行更新就可以用较小的改动代价更加灵活地避开故障路由器；因此这种情况下数据报网络更好；
	b、虚电路网络是面向连接的，路由器在其中用于维持连接状态的信息；在虚电路网络建立的一个连接中，router是知道这一条线路上的状态的，更容易在源到目的地的连接上的router控制固定的容量；而数据报网络中的router相较之下难以控制容量的固定，所以这种情况下虚电路网络更好；
	c、在这种情况下，连接一旦建立或者router转发表内容一旦计算完毕，就不会再变化了；这时，数据报网络中传输的数据会有数据报的header，这就使得数据报网络会有更多的流量控制开销。</font>



<font size=4 face="楷体" color=#00bfff>P2. Consider a virtual-circuit network. Suppose the VC number is an 8-bit field. a. What is the maximum number of virtual circuits that can be carried over a link? b. Suppose a central node determines paths and VC numbers at connection setup. Suppose the same VC number is used on each link along the VC’s path. Describe how the central node might determine the VC number at connection setup. Is it possible that there are fewer VCs in progress than the maximum as determined in part (a) yet there is no common free VC number? c. Suppose that different VC numbers are permitted in each link along a VC’s path. During connection setup, after an end-to-end path is determined, describe how the links can choose their VC numbers and configure their forwarding tables in a decentralized manner, without reliance on a central node.</font>

<font size=4 face="楷体">	答、
	a、VC number互不相同，故在8-bit下链路能承载的虚电路的最大数量为：${2}^{8} = 256$；
	b、如果连接由中心节点确定路径和VC号，而且沿某虚电路的路径在每段链路使用相同的VC号，那么中心节点在建立连接时可以从[0,${2}^{8}-1$]中随意挑选一个作为VC号；在这种情况下，进行中的虚电路数目小于256但又没有未使用的VC号（即所有VC号都被使用）是不可能同时达到的；
	c、如果一条path上每一个link都可以使用不同的VC号，那么在建立连接的时候每一个link都可以挑选[0,255]中的一个数作为自己的VC号；而路径上的router则将对应的VC号写入到转发表中，每次接收到特定的VC号的数据报时，都要将其VC号替换为与出口对应的VC号后转发。</font>



<font size=4 face="楷体" color=#00bfff>P26. Consider the following network. With the indicated link costs, use Dijkstra’s shortest-path algorithm to compute the shortest path from x to all network nodes. Show how the algorithm works by computing a table similar to Table 4.3.</font>

| step | N'      | D(z),p(z) | D(y),p(y) | D(w),p(w) | D(v),p(v) | D(u),p(u) | D(t),p(t) |
| ---- | ------- | --------- | --------- | --------- | --------- | --------- | --------- |
| 0    | x       | 8,x       | 6,x       | 6,x       | 3,x       | ∞         | ∞         |
| 1    | xv      | 8,x       | 6,x       | 6,x       | 3,x       | 6,v       | 7,v       |
| 2    | xvy     | 8,x       | 6,x       | 6,x       | 3,x       | 6,v       | 7,v       |
| 3    | xvyw    | 8,x       | 6,x       | 6,x       | 3,x       | 6,v       | 7,v       |
| 4    | xvywu   | 8,x       | 6,x       | 6,x       | 3,x       | 6,v       | 7,v       |
| 5    | xvywut  | 8,x       | 6,x       | 6,x       | 3,x       | 6,v       | 7,v       |
| 6    | xvywutz | 8,x       | 6,x       | 6,x       | 3,x       | 6,v       | 7,v       |



<font size=4 face="楷体" color=#00bfff>P28. Consider the network shown below, and assume that each node initially knows the costs to each of its neighbors. Consider the distance-vector algorithm and show the distance table entries at node z.</font>

<font size=4 face="楷体">	答、初始时，v，x，z只知道到自己邻居的代价，所以它们的距离表内容为</font>

|      | u    | v    | y    | x    | z    |
| ---- | ---- | ---- | ---- | ---- | ---- |
| z    | ∞    | 6    | ∞    | 2    | 0    |
| x    | ∞    | 3    | 3    | 0    | 2    |
| v    | 1    | 0    | ∞    | 3    | 6    |

<font size=4 face="楷体">	z接收到邻居x、v的信息后，会更新自己的内容：</font>

|      | u    | v    | y    | x    | z    |
| ---- | ---- | ---- | ---- | ---- | ---- |
| z    | 7    | 5    | 5    | 2    | 0    |
| x    | ∞    | 3    | 3    | 0    | 2    |
| v    | 1    | 0    | ∞    | 3    | 6    |

<font size=4 face="楷体">	同时，x、v接收到各自邻居的信息后，也会更新自己的内容：</font>

|      | u    | v    | y    | x    | z    |
| ---- | ---- | ---- | ---- | ---- | ---- |
| z    | 7    | 5    | 5    | 2    | 0    |
| x    | 4    | 3    | 3    | 0    | 2    |
| v    | 1    | 0    | 3    | 3    | 5    |

<font size=4 face="楷体">	x、v再将自己更新的信息发送给z，z又会更新自己的内容：</font>

|      | u    | v    | y    | x    | z    |
| ---- | ---- | ---- | ---- | ---- | ---- |
| z    | 6    | 5    | 5    | 2    | 0    |
| x    | 4    | 3    | 3    | 0    | 2    |
| v    | 1    | 0    | 3    | 3    | 5    |

<font size=4 face="楷体">	然后z将自己更新的信息传回给x、v，x、v计算后发现表项无需变化，故得到z最后的距离表表项为：</font>

| u    | v    | y    | x    | z    |
| ---- | ---- | ---- | ---- | ---- |
| 6    | 5    | 5    | 2    | 0    |

<font size=4 face="楷体" color=#00bfff>P30. Consider the network fragment shown below. x has only two attached neighbors, w and y. w has a minimum-cost path to destination u (not shown) of 5, and y has a minimum-cost path to u of 6. The complete paths from w and y to u (and between w and y) are not shown. All link costs in the network have strictly positive integer values.a. Give x’s distance vector for destinations w, y, and u. b. Give a link-cost change for either c(x,w) or c(x,y) such that x will inform its neighbors of a new minimum-cost path to u as a result of executing the distance-vector algorithm. c. Give a link-cost change for either c(x,w) or c(x,y) such that x will not inform its neighbors of a new minimum-cost path to u as a result of executing the distance-vector algorithm. </font>

<font size=4 face="楷体">	答、
	a、分别为2、4、7；
	b、c、未变化前的最低费用路径是`x->w->u`，最低费用为7；
	如果是c(x,w)变化：1、若c(x,w)变小，则最低费用路径不变但费用会减小，即c(x,w) < 2时，x会通知邻居最低费用为`c(x,w)+5`，最低费用路径不变；2、若c(x,w)变大，则当2 < c(x,w) <= 6时，最低费用路径不会改变但费用会增加，x会通知邻居最低费用为`c(x,w)+5`，最低费用路径不变；当c(x,w) > 6时，路径`x->y->u`的费用为11，比之前路径费用7小，故x通知邻居最低费用为11，最低费用路径为`x->y->u`；
	如果是c(x,y)发生变化：1、若c(x,y)变小，考虑到费用都是正整数，所以c(x,y) >= 1，此时`x->y->u`路径的费用 >= 7，所以最低费用路径不会改变而且费用不变，x不会通知邻居最低费用路径有变化；2、若c(x,y)变大，则路径`x->y->u`的费用更高了，故最低费用路径不会改变而且费用不变，x不会通知邻居最低费用路径有变化；
	综上所述，c(x,y)无论怎么变化都不会引起最低费用路径变化；故b的答案为：c(x,w) > 6时；c的答案为：c(x,w) <= 6或c(x,y)变化时。</font>

<font size=4 face="楷体" color=red>	PS:上述作答仅指最低费用路径不变，最低费用可能变化；如果最低费用变化也算作更新，那么b答案为：当c(x,w)变化时；c答案为：当c(x,y)变化时。</font>



<font size=4 face="楷体" color=#00bfff>P34. Consider Figure 4.31. Suppose there is another router w, connected to router y and z. The costs of all links are given as follows: c(x,y) = 4, c(x,z) = 50, c(y,w) = 1, c(z,w) = 1, c(y,z) = 3. Suppose that poisoned reverse is used in the distance-vector routing algorithm. a. When the distance vector routing is stabilized, router w, y, and z inform their distances to x to each other. What distance values do they tell each other? b. Now suppose that the link cost between x and y increases to 60. Will there be a count-to-infinity problem even if poisoned reverse is used? Why or why not? If there is a count-to-infinity problem, then how many iterations are needed for the distance-vector routing to reach a stable state again? Justify your answer. c. How do you modify c(y,z) such that there is no count-to-infinity problem at all if c(y,x) changes from 4 to 60? </font>

<font size=4 face="楷体">	答、
	a、	在毒性逆转下达到稳定后，y会通知w、z从y到x的距离为4；z会通知y从z到x的距离为6，通知w从z到x的距离为∞；w会通知y从w到x的距离为∞，通知z从w到x的距离为5；
	b、	会存在无穷计数问题：当c(x,y)变为60后，y会重新计算；y从z得知从z到x距离为6 < 60，故y会通知w从y到x的距离为6 + 3 = 9,通知z从y到z的距离为∞；z收到信息后，选择不会发生变化，w则会更新从w到x的距离为9 + 1 = 10；然后w将这一信息通知给z（通知y从w到x距离为∞），z会更新z到x的距离为10 + 1 = 11，然后通知y这一信息；y收到这一通知后更新y到x的距离为 11 + 3 = 14，并将这一信息通知给w；这时就可以发现通信过程会导致y、w、z构成的环路中产生无穷计数问题；同时迭代一轮y通知给w y到x的距离会增加5；这样迭代8轮，即迭代24次后，y会通知w y到x的距离为49，w由此计算出 w到x的距离为50，通知z后z会发现50 + 1 = 51高过由z直接到x的代价50，所以z会更新z到x距离为50，并将这一消息通知给y、w；w收到信息后，因为50 = 50，所以不会改变；y收到后会更新 y到x的距离为53，然后将这一信息通知到w，w接收后更新w到x的距离为51，然后会将这一信息通知给y；y收到这一信息后再更新y到x的距离为52；最终达到稳定状态：</font>

| y    | c(y->w->x)=52；（   c(y->x)=60；c(y->z->x)=53   ） |
| ---- | -------------------------------------------------- |
| z    | c(z->x)=50；（   c(z->w->x)=∞；c(z->y->x)=55   ）  |
| w    | c(w->z->x)=51；（   c(w->x)=∞；c(w->y->x)=∞   ）   |

<font size=4 face="楷体">	因此，需要迭代的次数为：`24 + 6 = 30`次；
	c、	可以将c(x,z)设为一个很大的值，例如100；这样发生变化后，y从w得知`y->w->x`距离为∞，从z得知`y->z->x`距离为100 + 6 = 106，故会选择`y->x`这条路径，代价为60；更新后w、z得到消息，会分别更新自己到x的距离为61、50；然后z、w相互通信后，w更新w到x距离为51，达到稳定；从而避免了无限计数。</font>

