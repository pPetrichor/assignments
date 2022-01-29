# <font face="楷体"><center>计网作业6</center></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖
学号：181860092
邮箱：603102048@qq.com</font></p>
------

### <font face="楷体">第4章</font>

<font size=4 face="楷体" color=#00bfff>R19. Give an example from queues you experience in your everyday life of FIFO, priority, RR, and WFQ.</font>

<font size=4 face="楷体">	答、FIFO:食堂排队打饭；priority：楼下理发店，办了会员卡的人可以享受优先理发，所以众人在排队等待理发时，会员可以优先理发；RR：机房助教检查程设实验作业，左边排一队，右边排一队，助教按照一左一右的顺序检查作业；WFQ：游乐园激流勇进项目排队，每次都是VIP通道中8个人与普通通道中4个人乘同一个皮筏。</font>



<font size=4 face="楷体" color=#00bfff>P1：题略。</font>

<font size=4 face="楷体">	答、
	a、如果第一块在t1时刻到达，同时客户开始播放，则第i块应该在 $t1+(i-1)*▲$ 时刻前到达；结合图观察可知第1、4、5、6块是满足条件的，故有这4个视频块将及时到达客户供其播放；
	b、如果第一块在t1到达客户，但客户在 $t1+▲$ 时刻开始播放，则第i块应该在 $t1+i*▲$ 时刻前到达；结合图观察可知第1、2、3、4、5、6块是满足条件的，故有这6个视频块将及时到达客户供其播放；
	c、梳理各个块到达客户以及进缓存、出缓存的过程：块1到达客户被缓存；接着块1出缓存，块2到达进缓存；块2出缓存，块3进缓存；接着在块3出缓存前块4到达客户（块3应该在 $t1+3*▲$ 时刻出缓存）；接着块3出缓存，块5进缓存；块4出缓存，块6进缓存；接着块7一直未到达客户，故块5、6出缓存；最后块7到达客户被播放；故整个过程存储在客户缓存中等待播放的块的最大数量是2；
	d、为了保证每个视频块及时到达客户供其播放，由b、c两问可知t1开始播放与 $t1+▲$ 时刻开始播放均无法满足条件；而且是因为块7到达客户太迟所以不可以；要使得块7满足条件，由图可知块7在 $t1+8*▲$ 时刻后才到达，所以最早也只能在 $t1+9*▲$ 时刻开始播放块7，因此算出最早的开始播放时刻为 $t1+3*▲$ 时刻；最小播放时延为 $t1+3*▲-t0$.</font>



<font size=4 face="楷体" color=#00bfff>P3:题略。</font>

<font size=4 face="楷体">	答、</font>

<font size=4 face="楷体">	a、平均发送速率为：</font>
$$
\frac{0+H}{2}=\frac{H}{2}
$$


<font size=4 face="楷体">	b、Q = 0时，客户在接收到第一个视频块时开始播放，则考虑到r以恒定速率从缓存取数据播放，则在播放初期会有`x(t) < r`的情况，所以会有卡顿；</font>

<font size=4 face="楷体">	c、由题意，即求按照图示情况发送数据，何时可以在缓存累计Q的数据量；记从客户收到第一个数据块开始经过t个T实现，其中t整数部分为x，小数部分为y，则应有：</font>
$$
x=[Q/\frac{HT}{2}]=[\frac{2Q}{HT}](取整)
$$

$$
\frac{H{y}^{2}}{2T}=Q-x*\frac{HT}{2},解得y=\sqrt{\frac{2TQ}{H}-x{T}^2}
$$

<font size=4 face="楷体">	则记从客户收到第一个数据块的时刻为0，首先开始播放时间为：</font>
$$
t=(x+y)*T
$$
<font size=4 face="楷体">	d、首先，在缓存了Q之后开始播放，则此时缓存接收数据的速率（等于发送速率）x(t)等于0，因为Q恰好是一个T发送，也即缓存接收的数据量；故在x(t)增长到r之前，缓存接收数据量是少于取出数据量的，缓存中数据不断减少；这段时间内缓存中减少的数据量为：$\frac{T{r}^{2}}{2H}$;由于`H>2r`，所以代入可知，减少数据量小于$\frac{rT}{4}$，小于$\frac{HT}{8}$，小于Q；所以这一阶段缓存数据量不会减少为0；考虑之后的播放情况：x(t)会从r逐渐增长到H，接收速率大于r，记这一过程缓存区数据量增加a；然后x(t)又会从0开始增长到r，接收速率小于r，记这一过程缓存区数据量减少b；易知如果`r=H/2`则`a=b`，但因为`H>2r`，所以`a>b`；故该减少过程也不会使得缓存区数据量减少到0；之后的播放情况就是上述增加a、减少b的循环，所以整个过程缓存区数据量不会减少到0，所以在初始播放时延之后不会有停滞；</font>

<font size=4 face="楷体">	e、由d结论可知Q最小应该小于$\frac{HT}{8}$；在初始播放时延之后，缓存区数据量可能会先增长，也可能先减少；如果缓存Q之后，开始播放时`x(t)>r`，则缓存区数量会先增加；这种情况下，因为`x(t)>r`，所以有$Q>\frac{T{r}^{2}}{2H}$；在接下来x(t)从0增加到r的过程中，缓存区数据量会减少$\frac{T{r}^{2}}{2H}$，所以减少量小于Q，因此这个减少过程不会使得缓存区数据量减少到0；之后的播放情况就是增加a、减少b，所以由d分析可知不会有停滞；如果缓存Q之后，开始播放时`x(t)<r`，则缓存区数量会先减少；设从客户收到第一个视频块经过n时长缓存区累计到Q，则有：</font>
$$
Q=\frac{H{n}^{2}}{2T}
$$
<font size=4 face="楷体">	之后，到x(t)增长到r为止，缓存区数据会一直减少；增长到r的时长为$\frac{rT}{H}$，故减少量c为（此段时间内从缓存取出量减去接收量）：</font>
$$
c=r*(\frac{rT}{H}-n)-\frac{(\frac{Hn}{T}+r)*(\frac{rT}{H}-n)}{2}=\frac{1}{2}(\frac{r^2T}{H}-2rn+\frac{Hn^2}{T})
$$
<font size=4 face="楷体">	则要满足不卡顿，应有$Q>=c$：</font>
$$
\frac{H{n}^{2}}{2T}>=\frac{1}{2}(\frac{r^2T}{H}-2rn+\frac{Hn^2}{T})
$$
<font size=4 face="楷体">	解得：</font>
$$
n>=\frac{rT}{2H}
$$
<font size=4 face="楷体">	代入得：</font>
$$
Q>=\frac{r^2T}{8H}
$$
<font size=4 face="楷体">	即最小为$\frac{r^2T}{8H}$。</font>

<font size=4 face="楷体">	f、由c可得首次开始播放的时间为：</font>
$$
t=(x+y)*T
$$
<font size=4 face="楷体">	记tf等于tf'个T，w为tf'的整数部分，v为tf'的小数部分，则考虑tf时刻缓存区累计收到的数据量为：</font>
$$
w*\frac{HT}{2}+\frac{Hv^2}{2T}
$$
<font size=4 face="楷体">	故应有累计接收数据减去取出数据等于B：</font>
$$
w*\frac{HT}{2}+\frac{Hv^2}{2T}-[(w+v)-(x+y)]*rT=B
$$
<font size=4 face="楷体">	由于缓存区满后发送速率会变，接收速率也相应变化，所以该方程解就是第一次缓存区满的时候；虽然此式中有w、v两个未知数，但w是整数，v是小于1大于0的小数，故给定Q、B、T、H具体的值，可以计算出w、v的一个方程式，改方程式满足是整数，v是小于1大于0的小数的解是唯一的；故tf为：$(w+v)*T$</font>

<font size=4 face="楷体">	eg:假设在开始播放后一个T时间内缓存满了，则w=0，进一步可以推出缓存会在开始播放后一个T时间内达到Q，则x=0，故有：</font>
$$
\frac{Hv^2}{2T}-(v-y)*rT=B
$$
<font size=4 face="楷体">	而</font>
$$
y=\sqrt{\frac{2TQ}{H}-x{T}^2}
$$
<font size=4 face="楷体">	故代入y的值，可解出v，进而得到tf。</font>

<font size=4 face="楷体" color=red>	PS：表示tf应该还需要参数r的值，但题目没有提。</font>



<font size=4 face="楷体" color=#00bfff>P5. Consider a DASH system for which there are N video versions (at N different rates and qualities) and N audio versions (at N different rates and versions). Suppose we want to allow the player to choose at any time any of the N video versions and any of the N audio versions. a. If we create files so that the audio is mixed in with the video, so server sends only one media stream at given time, how many files will the server need to store (each a different URL)? b. If the server instead sends the audio and video streams separately and has the client synchronize the streams, how many files will the server need to store?  </font>

<font size=4 face="楷体">	答、
	a、这种情况下要生成所有匹配可能，故需要存储文件数为：$N*N=N^2$；
	b、单独发送音频流与视频流，然后由用户选择同步的话，只需要将所有音频流和视频流存储下来即可，故需要存储文件数为：$N+N=2*N$；</font>



<font size=4 face="楷体" color=#00bfff>P18.题略。 </font>

<font size=4 face="楷体">	答、</font>

<font size=4 face="楷体">	a、列表如下：</font>

| 分组 | 离开队列时间 | 时延 |
| ---- | ------------ | ---- |
| 2    | 1            | 1    |
| 3    | 2            | 1    |
| 4    | 3            | 2    |
| 5    | 5            | 2    |
| 6    | 4            | 2    |
| 7    | 6            | 3    |
| 8    | 7            | 2    |
| 9    | 8            | 3    |
| 10   | 9            | 2    |
| 11   | 10           | 2    |
| 12   | 11           | 3    |

<font size=4 face="楷体">	分组1的时延为0，故平均时延为(0+1+1+2+2+2+3+2+3+2+2+3)/12 = 1.917;</font>

<font size=4 face="楷体">	b、列表如下：</font>

| 分组 | 离开队列时间 | 时延 |
| ---- | ------------ | ---- |
| 2    | 2            | 2    |
| 3    | 1            | 0    |
| 4    | 6            | 5    |
| 5    | 3            | 0    |
| 6    | 7            | 5    |
| 7    | 4            | 1    |
| 8    | 9            | 4    |
| 9    | 5            | 0    |
| 10   | 10           | 3    |
| 11   | 8            | 0    |
| 12   | 11           | 3    |

<font size=4 face="楷体">	分组1的时延为0，故平均时延为(0+2+0+5+0+5+1+4+0+3+0+3)/12 = 1.917;</font>

<font size=4 face="楷体">	c、列表如下：</font>

| 分组 | 离开队列时间 | 时延 |
| ---- | ------------ | ---- |
| 2    | 2            | 2    |
| 3    | 4            | 3    |
| 4    | 1            | 0    |
| 5    | 3            | 0    |
| 6    | 6            | 4    |
| 7    | 5            | 2    |
| 8    | 7            | 2    |
| 9    | 9            | 4    |
| 10   | 11           | 4    |
| 11   | 8            | 0    |
| 12   | 10           | 2    |

<font size=4 face="楷体">	分组1的时延为0，故平均时延为(0+2+3+0+0+4+2+2+4+4+0+2)/12 = 1.917;</font>

<font size=4 face="楷体">	d、考虑赋类型1权值为2，类型2为1；列表如下：</font>

| 分组 | 离开队列时间 | 时延 | 原因                                                         |
| ---- | ------------ | ---- | ------------------------------------------------------------ |
| 2    | 2            | 2    | 根据WFQ方式选择                                              |
| 3    | 1            | 0    | 根据WFQ方式选择                                              |
| 4    | 5            | 4    | 根据WFQ方式选择                                              |
| 5    | 3            | 0    | 根据WFQ方式选择                                              |
| 6    | 7            | 5    | 按照WFQ，应该服务类型1，但此时等待队列中没有类型1的分组，故发送6 |
| 7    | 4            | 1    | 根据WFQ方式选择                                              |
| 8    | 9            | 4    | 按照WFQ，应该服务类型1，但此时等待队列中没有类型1的分组，故发送8 |
| 9    | 6            | 1    | 根据WFQ方式选择                                              |
| 10   | 10           | 3    | 按照WFQ，应该服务类型1，但此时等待队列中没有类型1的分组，故发送10 |
| 11   | 8            | 0    | 根据WFQ方式选择                                              |
| 12   | 11           | 3    | 按照WFQ，应该服务类型1，但此时等待队列中没有类型1的分组，故发送12 |

<font size=4 face="楷体">	分组1的时延为0，故平均时延为(0+2+0+4+0+5+1+4+1+3+0+3)/12 = 1.917;</font>

<font size=4 face="楷体">	e、不管选择哪种调度方式，平均时延都是相等的。</font>



<font size=4 face="楷体" color=#00bfff>P20.题略。</font>

<font size=4 face="楷体">	答、</font>

<font size=4 face="楷体">	a&b、如下所示：</font>

| t    | 队列中的分组 | 桶中令牌数量 | 输出链路上分组 |
| ---- | ------------ | ------------ | -------------- |
| 0    | 1、2、3      | 2            | 1、2           |
| 1    | 3、4         | 1            | 3              |
| 2    | 4、5         | 1            | 4              |
| 3    | 5、6         | 1            | 5              |
| 4    | 6            | 1            | 6              |
| 5    | 空           | 1            | 空             |
| 6    | 7、8         | 2            | 7、8           |
| 7    | 9、10        | 1            | 9              |
| 8    | 10           | 1            | 10             |

