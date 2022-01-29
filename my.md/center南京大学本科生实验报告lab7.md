

# <font face="楷体">

# <center>南京大学本科生实验报告</center></font>

<font size=3 face="楷体">课程名称：**计算机网络**							任课教师：李文中
助教：陈衍庆、🕊、田冰川、吴昌容、段建辉、方毓楚、黄晓洁、刘柯鑫、李想</font>

| <font face="楷体">学院</font>  | <font face="楷体">计算机院</font> | <font face="楷体">专业（方向）</font>  | <font face="楷体">计算机科学与技术</font> |
| ------------------------------ | --------------------------------- | -------------------------------------- | ----------------------------------------- |
| <font face="楷体">学号</font>  | 181860092                         | <font face="楷体">姓名</font>          | <font face="楷体">王瀚霖</font>           |
| <font face="楷体">Email</font> | 603102048@qq.com                  | <font face="楷体">开始/完成日期</font> | 2020.6.3  /  2020.6.14                    |



### <font face="楷体">1、实验名称</font> : Firewall

### <font face="楷体">2、实验目的</font>

<font size=4 face="楷体">	根据`firewall_rules.txt`中的内容实现可以检查、控制包流量以及更改数据包内容的防火墙。具体而言，该防火墙需要满足`firewall_rules.txt`中规则的限制、利用令牌桶机制实现某些特定传输的速率限制，并实施一些破坏某些数据包流的措施。</font>

### <font face="楷体">3、实验内容</font>：

### Task 1: Preparation

<font size=4 face="楷体">	实验要求：在switchyard下创建lab_7，在其中完成文件的复制与更名工作，做好实验准备；
	具体实现：略；</font>

### Task 2: Implement firewall rules

<font size=4 face="楷体">	实验要求：
	修改文件`lab_7/firewall.py`，实现一个完全遵循`firewall_rules.txt`中基本内容（不含`ratelimit`和`impair`）的防火墙；具体而言，该防火墙每收到一个包都需要按序比较每一条规则，如果某条规则适用于当前包则执行该规则操作，否则继续比较，如果没有一条规则适用则直接转发；
	思考与实现：
	实现主要分为两个阶段：首先，要处理`firewall_rules.txt`中的内容，记录下所有的规则；之后，每收到一个包就要从头遍历所有的规则，与当前数据包进行匹配与操作。
	对于第一个阶段，根据手册说明，规则的文本内容只有以下三种：</font>

```
[permit|deny] ip src [srcnet|any] dst [dstnet|any]

[permit|deny| icmp src [srcnet|any] dst [dstnet|any]

[permit|deny] [udp|tcp] src [srcnet|any] srcport [portno|any] dst [dstnet|any] dstport [portno|any]
```

<font size=4 face="楷体">	每一条规则的格式都是固定的；此外，要处理的文本内容中还包括注释与空行。因此，我选择利用`readline()`函数一行行处理；同时，规则的内容将会保存在一个名为`rules`的list中，而`rules`的每个元素也是一个list，这些list的内容都整理为以下格式：</font>

| permit/deny | protocol        | srcip      | srcport         |
| ----------- | --------------- | ---------- | --------------- |
| True/False  | ip/icmp/tcp/udp | any/ip地址 | any/端口号/None |

| dstip      | dstport         | ratelimit | impair     |
| ---------- | --------------- | --------- | ---------- |
| any/ip地址 | any/端口号/None | 数值/None | True/False |

<font size=4 face="楷体">	即：`rule`中的每个元素为一个长度为8的list，其内容依次为：第一项表示该规则是`permit`还是`deny`，为前者则存储True，为后者则存储False；第二项表示规则针对的传输协议；第三项表示源ip地址，内容为`any`则表示适用于任何源地址；第四项表示源端口号，对于非`tcp/udp`的规则，其内容中没有端口号，则此项填充为None；第五项表示目的ip地址；第六项表示目的端口号；第七项表示该规则对于流量的限速，如果对应规则有限速则该项内容为具体数值，否则为None；第八项表示是否要对该规则适用的数据包进行破坏，若对应规则末尾有`impair`则置为True，否则为False；
	确认了以上方式，只要利用循环不断读取文件内容就可以了；显然，要读完文本所有内容，循环结束的标志就是读取到空内容；此外，如果读取到空行或注释，应该跳过：</font>

```python
    rules = []
    file = open("/home/njucs/switchyard/lab_7/firewall_rules.txt")
    while True:
        text = file.readline()
        if text == '': #读取到空，说明读取结束，直接退出循环
            break
        x = text.split() #以空白符分割读取的一行文本内容
        if (len(x)==0) or (x[0].startswith('#')): #分别对应空行与注释
            continue
        # 保证了接下来是处理一条规则的文本内容

```

<font size=4 face="楷体">	这样，我们就保证了处理的是规则的文本内容；由于规则的文本内容只有上述三种形式，而第一、二种形式的除了`protocol`一项不同外全部相同，故可一起处理；对于这样的经过`split()`处理后的文本内容x，转换成的list命名为`rule`；首先处理第一项：判断x[0]，若为`permit`则`rule`添加True，为`deny`则`rule`添加False；对于第二、三项，可以直接添加x[1]、x[3]，（x[2]为'src'，跳过）：</font>

```python
        rule = []
        if x[0] == 'permit':
            rule.append(True)
        elif x[0] == 'deny':
            rule.append(False)

        rule.append(x[1])
        rule.append(x[3])
```

<font size=4 face="楷体">	之后的文本内容，`ip`、`icmp`与`tcp/udp`就会出现分歧了：对于`ip/icmp`，其规则中没有端口号，所以只需添加目的ip，源端口、目的端口项要添加None；对于后者，端口号、目的ip都要添加相应的内容。同时考虑`ratelimit`的处理：对于`ip/icmp`，其文本内容x的长度为6（不包括`ratelimit`和`impair`），所以在包含`ratelimit`时x的长度为8，含`impair`时长度为7；对于`tcp/udp`，其文本内容x的长度为10（不包括`ratelimit`和`impair`），所以在包含`ratelimit`时x的长度为12，含`impair`时长度为8；因此，对于`tcp/udp`，如果x的长度大于11则有`ratelimit`，故添加相应数值；否则添加None；对于`ip/icmp`，如果x的长度大于7则有`ratelimit`，故添加相应数值；否则添加None：</font>

```python
        if (rule[1] == 'udp') or (rule[1] == 'tcp'): #udp/tcp规则，有端口号
            rule.append(x[5]) #添加srcport
            rule.append(x[7]) #添加dstip
            rule.append(x[9]) #添加dstport
            if len(x) > 11: #有ratelimit，添加
                rule.append(x[11])
            else:
                rule.append(None) #没有，ratelimit一项填None
        else: #ip/icmp规则
            rule.append(None)
            rule.append(x[5]) #只有dsrip添加了具体内容，端口号项均添加None
            rule.append(None)
            if len(x) > 7:
                rule.append(x[7]) #有ratelimit，添加
            else:
                rule.append(None) #没有，ratelimit一项填None
```

<font size=4 face="楷体">	之后，判断相应规则是否有`impair`；由手册可知`impair`只会出现在末尾，故只要判断最后一项即可：</font>

```python
        if x[len(x)-1] == 'impair':
            rule.append(True)
        else:
            rule.append(False)
```

<font size=4 face="楷体">	最后，将`rule`加入到`rules`中：</font>

```python
		rules.append(rule)
```

<font size=4 face="楷体">	这样，待循环结束后，所有规则就存储在了`rules`中了。
	考虑到对于有限速的规则需要每隔0.5s添加一次token，所以将所有有限速的规则单独取出来以便于访问：采用一个字典`ratelimit`来存放这些规则，其key为相应规则在`rules`中的下标，value为一个list：[r,现有的token数]。故遍历`rules`，找到`ratelimit`不为None的条目加入：</font>

```python
    for i in range(len(rules)):
        if rules[i][6] is not None: #rules[i]有限速，加入
            ratelimit[i] = ([int(rules[i][6]),int(rules[i][6])//2])
            # 初始化令牌值为r/2(取整)
```

<font size=4 face="楷体">	这样就完成了第一阶段的任务。
	对于第二阶段，是在收到包时与规则比对，判断能否转发。具体实现方式为：每收到一个包，首先声明以下变量：</font>

```python
if pkt is not None:

    # This is logically where you'd include some  firewall
    # rule tests.  It currently just forwards the packet
    # out the other port, but depending on the firewall rules
    # the packet may be dropped or mutilated.
    flag = True # 表示当前包是否应该转发
    p_pro = 0 #表示ip包使用的协议，1为icmp；2为tcp/udp
    src_port = None
    dst_port = None #记录当前包的端口号，初始化为None
    rule_number = -1 #记录第一条匹配到的规则号，初始化为-1表示未匹配到任何规则
```

<font size=4 face="楷体">	接着，对收到的包进行分析；根据手册说明，`"The firewall should apply just to IPv4 packets (not to ARP, IPv6, or any other type of packet). In particular, non-IPv4 packets should all be permitted"`，所以首先要对收到的包的类型进行判断：如果为Arp包、非IPv4包或是采用ICMPv6协议的包都不属于防火墙的应用范围，所以可以直接转发：</font>

```python
	if (arp is not None) or (ip is None) or (pkt.get_header(ICMPv6) is not None):
    	pass
		# 不做处理，flag与rule_number都是初始值
	else:
    	log_info("receive pkt from {}".format(ip.src))
        # 接下来要依次匹配所有规则
```

<font size=4 face="楷体">	接下来，要对所有规则依次匹配；在匹配开始前，先对收到的IPv4数据包的协议进行分析：如果是icmp协议则`p_pro`置为1，如果是tcp/udp协议则`p_pro`置为2并从数据包中取出源端口号与目的端口号：</font>

```python
		if pkt.get_header(ICMP) is not None:
    		p_pro = 1 #ICMP

    	if pkt.get_header(UDP) is not None:
            p_pro = 2 #udp
            src_port = pkt.get_header(UDP).src
            dst_port = pkt.get_header(UDP).dst

        if pkt.get_header(TCP) is not None:
            p_pro = 3 #tcp
            src_port = pkt.get_header(TCP).src
            dst_port = pkt.get_header(TCP).dst
```

<font size=4 face="楷体">	循环依次遍历所有规则进行匹配：首先判断当前遍历的规则是`permit`还是`deny`，同时取出规则适用的协议：</font>

```python
         for i in range(len(rules)):               
             permit = rules[i][0]
             protocol = rules[i][1]
```

<font size=4 face="楷体">	若规则适用的协议为ip或icmp，则该规则中没有端口号，只需判断srcip与dstip是否符合；同时如果协议为icmp，还要保证处理的数据包使用icmp协议，否则该规则不适用；对于适用于tcp/udp的规则，同样的要保证处理的数据包使用tcp/udp协议，所以首先要进行检验，并根据检验结果分为两种情况进行处理（ip/icmp没有端口号，归为一类；tcp/udp有端口号，归为一类）；
	如果规则适用且协议为ip/icmp，则取出规则中的源子网与目的子网，并声明两个变量`march_src`与`march_dst`记录源ip地址与目的ip地址的匹配情况：</font>

```python
             if (protocol == 'ip') or (protocol == 'icmp' and p_pro == 1):
        		 #ip或icmp协议
                 rule_src = rules[i][2] #取规则中相关值
                 rule_dst = rules[i][4]
                 march_src = False
                 march_dst = False #变量均初始化为False
```

<font size=4 face="楷体">	判断收到数据包的源ip地址与目的ip地址是否在规则给出的子网中：如果规则给出的子网是`any`则直接匹配，否则利用`IPv4Network`、`IPv4Address`与操作符`in`来判断数据包的ip地址是否在规则给出的子网中：</font>

```python
                if rule_src == 'any':
                    march_src = True
            		#srcip为any，直接匹配
                else:
                    #否则判断ip.src是否在规则的源子网中，在则匹配
                    src_net = IPv4Network(rule_src,strict=False)
                    if IPv4Address(ip.src) in src_net:
                        march_src = True

                if rule_dst == 'any':
                    march_dst = True
                else:
                    dst_net = IPv4Network(rule_dst,strict=False)
                    if IPv4Address(ip.dst) in dst_net:
                        march_dst = True
                    # 同理
                if march_src and march_dst:
                    # 如果srcip与dstip均匹配，则pkt与该条rule匹配
                    if permit:
                        # 规则为允许转发，则直接设置rule_number
                        # 为匹配到的规则号并跳出循环进行转发
                        rule_number = i
                        break
                    else:
                        # 规则为禁止转发，则设置rule_number
                        # 为匹配到的规则号，更新flag为False
                        # 并跳出循环结束规则遍历                        
                        flag = False
                        rule_number = i
                        break
```

<font size=4 face="楷体">	若规则适用的协议为tcp或udp，则该规则中有端口号，需判断srcip、dstip、srcport与dstport是否符合；如果规则适用且协议为tcp/udp，则取出规则中的源子网、目的子网、源端口号与目的端口号，并声明四个变量`march_src_ip`、`march_dst_ip`、`march_src_port`与`march_dst_port`，记录源ip地址、目的ip地址、源端口号与目的端口号的匹配情况：</font>

```python
			 else:
             	if (protocol == 'tcp' and p_pro == 3) or 
            (protocol == 'udp' and p_pro == 2): #tcp或udp
                    rule_src = rules[i][2]
                    rule_dst = rules[i][4]

                    rule_src_port = rules[i][3]
                    rule_dst_port = rules[i][5] #取规则中相关值
                    march_src_ip = False
                    march_dst_ip = False
                    march_src_port = False
                    march_dst_port = False #变量均初始化为False
```

<font size=4 face="楷体">	判断收到数据包的源ip地址与目的ip地址是否在规则给出的子网中的方法同上；判断端口号是否匹配则是将规则中相应条目的端口号取出后转换为整数与数据包的端口号匹配（即`int(rule_src_port)、int(rule_dst_port)`与数据包的端口号比较）：</font>

```python
                    if rule_src == 'any':
                    	march_src_ip = True
                    else:
                        src_net = IPv4Network(rule_src,strict=False)
                        if IPv4Address(ip.src) in src_net:
                        	march_src_ip = True

                    if rule_src_port == 'any':
                    	march_src_port = True
                    else:
                        if src_port == int(rule_src_port):
                        	march_src_port = True
                    #匹配端口号：转化为int后比较即可
                    if rule_dst == 'any':
                        march_dst_ip = True
                    else:
                        dst_net = IPv4Network(rule_dst,strict=False)
                        if IPv4Address(ip.dst) in dst_net:
							march_dst_ip = True
                            
                    if rule_dst_port == 'any':
                    	march_dst_port = True
                    else:
                        if dst_port == int(rule_dst_port):
                        	march_dst_port = True
					
                   	if march_src_ip and march_src_port and
                    march_dst_ip and march_dst_port:
                    # 全部匹配，操作同ip/icmp
                    	if permit:
                        	rule_number = i
                            break
                        else:
                            flag = False
                            rule_number = i
                            break
```

<font size=4 face="楷体">	经过上述操作，在循环遍历所有规则结束后，匹配到的规则号被记录在`rule_number`中（-1表示没有规则与收到的数据包匹配），能否转发则由`flag`记录；所以只需判断`flag`的值，为True转发，否则不转发。
	由此，完成了task 2的任务。</font>

### Task 3: Implement the token bucket algorithm

<font size=4 face="楷体">	实验要求：修改文件`lab_7/firewall.py`，实现令牌桶机制以完成某些规则的限速；
	思考与实现：
	根据手册要求，令牌桶机制要做的就是给每一条有`ratelimit`标记的规则维护一个bucket，每个bucket中的tokens数目等价于当前允许传输的数据的字节数；维护bucket中tokens的数量就是每隔0.5s向bucket中添加$\frac{r}{2}$个tokens，同时tokens的数量只能在`0——2r`范围内；每次转发数据包时，要判断数据包除以太网包头外部分的长度是否小于等于当前bucket中tokens的数量；是则转发并减去相应的tokens数，否则丢弃。</font>

<font size=4 face="楷体">	在task2实现的基础上，task3的任务就比较简单了：首先，所有有限速的规则都被记录在了`ratelimit`这一dict中，每一个元素都相当于一个bucket（记录了r与当前的tokens数目），所以添加tokens只需要维护一个时间差，记录距离上次添加tokens的时长；每次循环中判断该时间差是否大于等于0.5s，如果是就添加tokens，添加的次数由时间差的长短决定，具体而言就是添加一次tokens时间差减去0.5，直到时间差小于0.5；具体实现方式为：</font>

<font size=4 face="楷体" color=#adaafa>	PS：`ratelimit`中每一项的结构为：</font>

| key                          | value[0]（value是list） | value[1]       |
| ---------------------------- | ----------------------- | -------------- |
| i（本规则在`rules`中的下标） | r（txt中获取）          | 当前tokens数目 |

```python
        update = False
        time_pass = time.time() - mytime
        
        if time_pass >= 0.5:
            update = True
        
        #print(time.time())  

        while time_pass >= 0.5:
            
            for i in ratelimit.keys():
                ratelimit[i][1] = ratelimit[i][1] + ratelimit[i][0]//2
                if ratelimit[i][1] > ratelimit[i][0]*2:
                    #超出上限2r，调整
                    ratelimit[i][1] = ratelimit[i][0]*2
                #print('r:',ratelimit[i][0],'    tokens:',ratelimit[i][1])
                log_info('rule:{} tokens after add:{}'.format(i,ratelimit[i][1]))
            time_pass = time_pass - 0.5
        
        if update:
            mytime = time.time() - time_pass
```

<font size=4 face="楷体">	对于转发时比较tokens与数据包字节数的实现，直接更改转发逻辑即可：当前的转发逻辑是如果遍历规则结束后`flag`为True则转发，在此基础上进行进一步判断：如果`flag`为True且`rule_number`是`ratelimit`的key值之一，则说明收到的数据包匹配到了一条有限速的rule，而且该数据包要进行转发；故首先计算数据包除去以太网包头的长度`l`，用`l`与匹配的规则对应bucket中当前的tokens数目进行比较，如果`l`更大，说明当前bucket的tokens数目不支持该包的转发，故丢弃；否则进行转发并将bucket中的tokens数量减去`l`；根据以上思路，转发的实现为：</font>

```python
	if flag:
        if rule_number in ratelimit.keys():
        	l = len(pkt) - len(pkt.get_header(Ethernet))
            if l > ratelimit[rule_number][1]:
            	continue
            else:
                ratelimit[rule_number][1] = ratelimit[rule_number][1] - l

        net.send_packet(portpair[input_port], pkt)
```

<font size=4 face="楷体">	由以上，就实现了task 3的任务；一点小问题就是添加tokens因为只能在循环中判断、实现，所以没办法做到真正的每隔0.5s添加一次（例如测试中sleep 1s时就只能睡眠后添加两次，不能在这1s中每过0.5s添加一次）。不过这也是无法避免的，综合下来这种实现方法效果还是最好的。</font>

### Task 4: Implement some other type of network impairment

<font size=4 face="楷体">	实验要求：修改文件`lab_7/firewall.py`，实现对某些数据流的破坏；
	思考与实现：破坏数据流的方式有很多种，这里实现了一个`impair`函数用以破坏某些数据流，在转发数据包时，如果发现匹配的rule中有`impair`记录则调用此函数进行破坏；
	</font>

<font size=4 face="楷体">	对于`impair`的实现，这里对不同类型数据包采用了不同破坏方式：如果数据包是tcp包，则将其中的`advertised window`进行缩小以减缓传输速率；如果数据包是udp包并且有payload内容，则修改payload内容；如果数据包为icmp包，则将`icmpdata.data`部分进行修改（修改传输信息），否则，随机丢弃该包；根据以上思路，`impair`实现如下：</font>

```python
def impair(pkt):
    f = True
    #f用于随即丢弃
    if pkt.has_header(TCP):
        #pass
        pkt[TCP].window = pkt[TCP].window//50 #TCP，将窗口缩小为五十分之一
    else:
        if pkt.has_header(UDP) and pkt.has_header(RawPacketContents):
            pkt[pkt.get_header_index(RawPacketContents)] = 
            RawPacketContents('Trudy changes my content.') #UDP
        elif pkt.get_header(ICMP) is not None:
            pkt[ICMP].icmpdata.data = b'Trudy changes my content.' #ICMP
            #UDP与ICMP，都是修改传输内容
        else:
            #进行随机丢弃
            x = randint(0,99)
            if x < 50: #50%的几率丢弃
                f = False
                #置f为False，表示要丢弃该包
    return pkt,f #返回修改后的数据包与丢弃信息
```

<font size=4 face="楷体">	这样，我们需要在转发数据包前检查是否要破坏该数据流，如果要则调用`impair`函数，因此修改转发数据包的实现为：</font>

```python
     if flag:
     	if rule_number in ratelimit.keys():
        	l = len(pkt) - len(pkt.get_header(Ethernet)) #限速实现
            if l > ratelimit[rule_number][1]:
            	continue
            else:
                ratelimit[rule_number][1] = ratelimit[rule_number][1] - l
                log_info("send pkt, l:{} ,current tokens:{}"
                                 .format(l,ratelimit[rule_number][1]))
            #执行impair操作     
            f = True #用于记录丢包信息
            if rules[rule_number][7]: #该规则匹配的数据流要遭到破坏
                pkt,f = impair(pkt) #调用impair
            if f:
                #如果f为True则转发，否则说明此包被随机丢弃，不转发
                net.send_packet(portpair[input_port], pkt)
```

<font size=4 face="楷体">	由此实现了task 4的任务。</font>

### Task 5: Testing

<font size=4 face="楷体">	见文档**实验结果**部分。</font>

### <font face="楷体">4、实验结果</font>

#### Task1&Task2&Task3&Task4

<font size=4 face="楷体">	略。</font>

#### Task5

<font size=4 face="楷体" color=red>	Tip：附图建议放大观看</font>

<font size=4 face="楷体">	测试用例的结果：（选取**方毓楚**学长更改后的测试用例进行测试）</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_1.png)

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_2.png)

------

<font size=4 face="楷体">	mininet下测试rate limitation：</font>

<font size=4 face="楷体">	1、测试rule 13结果如下</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_3.png)

<font size=4 face="楷体">	截取该过程中部分tokens的增加和消耗记录显示如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_4.png)

<font size=4 face="楷体">	根据xterm与终端显示信息可知：在传输开始前，rule 13的bucket中tokens数目已达到最大值300（r为150，如图中蓝色下划线所示）；接着开始传输数据包，注意数据包是每隔1s传输一次：对于第一个数据包，根据绿色下划线可以看出，传输数据包1与对数据包1的reply共消耗了200个token，使得tokens数量减为100；之后由于传输数据包2是在1s后发生的，所以这段时间内会向bucket中添加两次tokens，使得tokens数量变为250（如图中紫色下划线所示）；之后开始传输数据包2，如图中黄色下划线所示，此过程中tokens数量足够，完成了数据包2的发送与对数据包2回复包的传输，tokens减少为50；接下来又过了1s，tokens添加两次变为200（如图中棕色下划线所示），对数据包3及回复进行转发，tokens减少为0（如图中橙色下划线所示）。</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_5.png)

<font size=4 face="楷体">	tokens减少为0后，在之后的1s中会增加两次变为150（如上图蓝色下划线所示），接着firewall会收到数据包4并将其转发，使得tokens减少为50；所以接着firewall收到192.168.0.2发送的对数据包4的回复时，tokens数量小于100，无法转发，从而丢弃该包，因此终端上显示没有收到数据包4的reply；丢弃该包后，经过1s，tokens增加两次变为200（如绿色下划线所示），足够转发数据包5与对数据包5的reply，所以终端显示从192.168.0.2收到了数据包5的回复；而tokens经过这两次转发数量减为0；所以之后就是上述过程的循环（如上图黄色下划线所示）；故整体看来，之后`internal`只能隔一个包收一个reply，符合终端显示的结果。</font>

<font size=4 face="楷体">	wireshark的抓包结果也证实了上述推理：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_12.png)

------

<font size=4 face="楷体">	2、测试rule 7&8，结果&分析如下：</font>

<font size=4 face="楷体">	按照手册说明得到测试结果为：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_6.png)

<font size=4 face="楷体">	作为对照，将ratelimit的处理注释掉再次测试：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_7.png)

<font size=4 face="楷体">	根据以上两张截图可以看出加入ratelimit处理之后，吞吐率有明显的下降，而且传输速率被限制小于了12.5KB/s；由此可以看出`rule 7&8`的限速确实起到了作用。</font>

<font size=4 face="楷体" color=red>	PS：在我的机器上测试，如果先执行`firewall.py`，使bucket中的token填满的话，会出现传输速率大于12.5KB/s的情况，所以为保险起见，请学长学姐测试时先执行`internal wget http://192.168.0.2/bigfile -O /dev/null`，再在xterm上执行`swyard firewall.py`.</font>

------

<font size=4 face="楷体">	mininet下测试impairments：</font>

<font size=4 face="楷体">	按照手册说明进行测试，得到的结果为：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_8.png)

<font size=4 face="楷体">	通过打印收到的数据包得知该测试中传输的数据包均为tcp数据包，因此相应的破坏就是将tcp包中的`advertised window`值减小；这样的破坏方式导致的结果就是使得传输速率下降；因此跳过这一部分的破坏再次测试(注释掉减小`advertised window`值的语句，改为打印"impair pkt")：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_9.png)

<font size=4 face="楷体">	对比以上两个结果不难看出，通过`impair`函数对tcp数据包中`advertised window`的修改，使得对应传输流的传输速率明显的下降。</font>

<font size=4 face="楷体">	从抓包结果来看，window值确实在impair中被减小：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_13.png)

------

<font size=4 face="楷体">	在mininet中测试`impair`只对tcp数据包进行了破坏，因此编写了测试用例`impairmenttest.py`测试`impair`：</font>

<font size=4 face="楷体">	在测试用例中，为了测试到所有的情形，对`firewall_rules`的内容进行了更改：在rule 5末尾添加了`impair`，以测试对udp包的更改；在rule 13末尾添加了`impair`，以测试对icmp包的更改；利用原本的rule 11、12对tcp包进行测试。</font>

<font size=4 face="楷体">	首先，对tcp包进行测试：构建一个目的端口号为8000的tcp数据包与rule 11匹配；其`advertised window`值为1000；转发时经`impair`处理，`advertised window`应该为`1000/50 = 20`：</font>

```python
    t = TCP()
    t.ACK = 1
    t.ack = random.randrange(0,2**32)
    t.seq = random.randrange(0,2**32)
    t.src = random.randrange(2**12,2**16)
    t.dst = 8000 #用以匹配rule 11
    t.window = 1000
    ip = IPv4()
    ip.src = '192.168.0.13'
    ip.dst = IPv4Address(random.randrange(2**16, 2**32))
    ip.protocol = IPProtocol.TCP
    pkt = Ethernet() + ip + t + "This is some TCP data!".encode()
    # fill in any other packet headers or data to the constructed packet
    s.expect(PacketInputEvent('eth0',pkt), 
        'tcp Packet that should be impaired arrives on eth0')

    # Modify the packet in the same way that you expect the firewall
    # to modify the packet.  
    pkt = deepcopy(pkt) # make a full copy of the packet before modifying
    pkt[TCP].window = 20 #窗口大小变为原来的1/50

    s.expect(PacketOutputEvent('eth1',pkt),
        'for tcp pkt, advertised window size should be smaller.')
```

<font size=4 face="楷体">	接着对udp数据包进行测试：</font>

```python
    t = UDP()
    t.src = random.randrange(2**12,2**16)
    t.dst = 53 #目的端口号设置为53用以匹配rule 5
    ip.protocol = IPProtocol.UDP #更改ip协议
    pkt = Ethernet() + ip + t + "This is some UDP data!".encode()
    s.expect(PacketInputEvent('eth0',pkt), 
        'udp Packet that should be impaired arrives on eth0')

    pkt = deepcopy(pkt) # make a full copy of the packet before modifying
    pkt[pkt.get_header_index(RawPacketContents)] = 
    RawPacketContents('Trudy changes my content.')
	#经impair处理，payload被改变
    s.expect(PacketOutputEvent('eth1',pkt),
        'for udp pkt, payload should be changed.')
```

<font size=4 face="楷体">	icmp数据包的测试与udp数据包类似，只不过更改的是`icmpdata.data`：</font>

```python
    ip.protocol = IPProtocol.ICMP
    icmp_pkt = ICMP()
    icmp_pkt.icmpdata.data = b'This is some UDP data!'
    pkt = Ethernet() + ip + icmp_pkt
    s.expect(PacketInputEvent('eth0',pkt), 
        'udp Packet that should be impaired arrives on eth0')
	#将与rule 13匹配
    pkt = deepcopy(pkt) # make a full copy of the packet before modifying
    pkt[ICMP].icmpdata.data = b'Trudy changes my content.'

    s.expect(PacketOutputEvent('eth1',pkt),
        'for icmp pkt, icmpdata should be changed.')
```

<font size=4 face="楷体">	最后，生成一个没有payload的udp包，该包在`impair`函数中将会被随机丢弃，所以无法设置是否发送该包；在测试文件中最后一个包是否发送不予设置，该包的转发情况由运行结果验证：</font>

```python
    t = UDP()
    t.src = random.randrange(2**12,2**16)
    t.dst = 53
    ip.protocol = IPProtocol.UDP
    pkt = Ethernet() + ip + t
    s.expect(PacketInputEvent('eth0',pkt), 
        'udp Packet that should be impaired arrives on eth0, the pkt has no payload ,so it should be dropped randomly.')
```

<font size=4 face="楷体">	运行该测试文件，得到的结果是随机的；有以下两种情况，分别代表最后一个包被丢弃与不被丢弃：</font>

<font size=4 face="楷体">	被丢弃：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_11.png)

<font size=4 face="楷体">	不被丢弃：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab7_10.png)

<font size=4 face="楷体">	由此，证明了`impair`实现的正确性。</font>

### <font face="楷体">5、核心代码</font>

​	<font size=4 face="楷体">	`firewall.py`的代码实现可参见文档**实验内容**部分，考虑到代码量较大，故在此不再赘述。</font>

### <font face="楷体">6、总结与感想</font>

<font size=4 face="楷体">	完成本次实验让我了解到了防火墙涉及到的规则与内容，同时对于令牌桶方法实现限流的过程有了更为直观的理解，也接触到了破坏数据包传输流的几种方法。</font>

