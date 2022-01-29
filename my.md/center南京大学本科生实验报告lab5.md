

# <font face="楷体">

# <center>南京大学本科生实验报告</center></font>

<font size=3 face="楷体">课程名称：**计算机网络**							任课教师：李文中
助教：陈衍庆、🕊、田冰川、吴昌容、段建辉、方毓楚、黄晓洁、刘柯鑫、李想</font>

| <font face="楷体">学院</font>  | <font face="楷体">计算机院</font> | <font face="楷体">专业（方向）</font>  | <font face="楷体">计算机科学与技术</font> |
| ------------------------------ | --------------------------------- | -------------------------------------- | ----------------------------------------- |
| <font face="楷体">学号</font>  | 181860092                         | <font face="楷体">姓名</font>          | <font face="楷体">王瀚霖</font>           |
| <font face="楷体">Email</font> | 603102048@qq.com                  | <font face="楷体">开始/完成日期</font> | 2020.4.27  /  2020.5.3                    |



### <font face="楷体">1、实验名称</font> : Respond to ICMP

### <font face="楷体">2、实验目的</font>

<font size=4 face="楷体">	完成建立一个IPv4路由的第三阶段任务：完善路由器，使得路由器可以对发给自己的ICMP ping request作出reply；同时，router应该能够在恰当的时机发送四种ICMP错误信息；这样来得到一个功能比较完善的路由器。</font>

### <font face="楷体">3、实验内容</font>：

### Task 1: Preparation

<font size=4 face="楷体">	实验要求：在switchyard下创建lab_5，在其中完成文件的复制与更名工作，做好实验准备；
	具体实现：略；</font>

### Task 2: Responding to ICMP echo requests

<font size=4 face="楷体">	实验要求：
	修改文件`lab_5/myrouter.py`，增加路由器可以对发给自己的ICMP ping request作出reply的功能；
	思考与实现：
	根据手册提示，这一过程可以被拆解为三步进行：
	首先，需要新建一个ICMP包头与echo reply（实际上echo reply就是ICMP包头中的`icmpdata`部分）：新建ICMP包头即新建一个`ICMP`对象，通过查阅手册与文档可知需要对新建对象的`icmpcode`、`icmpdata`、`icmptype`进行设置；其中`icmptype`自然是`EchoReply`，`icmpcode`可借助`ICMPTypeCodeMap`查找得到；`icmpdata`即echo reply内容；根据手册说明，echo reply的内容实际上就是echo request中`icmpdata`的内容，所以直接利用`from_bytes()`与`to_bytes()`拷贝过来就好了（感谢方毓楚学长的提示）；
	接下来，要构造IP包头与以太网包头以便之后包的转发：新建IPv4类的对象作为IP包头，设置它的ttl、src、dst与protocol，然后新建Ethernet类的对象作为以太网包头，设置它的`ethertype`为IPv4；将上述三部分拼接起来即组成一个可以传输的echo reply；所以，经前两步已经成功构建了一个echo reply，将其封装为函数`creat_echo_reply`，实现如下：（传入的参数ip、icmp分别是echo request的IP包头与ICMP包头）</font>

```python
    def creat_echo_reply(self,ip,icmp):
        myicmp = ICMP()
        myicmp.icmptype = ICMPType.EchoReply
        myicmp.icmpcode = ICMPTypeCodeMap[myicmp.icmptype]
        # 根据ICMPTypeCodeMap得到echo reply对应的icmpcode
        myicmp.icmpdata = ICMPEchoReply()
        # 初始化为一个ICMPEchoReply类的对象
        myicmp.icmpdata.from_bytes(icmp.icmpdata.to_bytes())
        # 从echo request拷贝内容到echo reply
        myip = IPv4()
        myip.ttl = 64
        myip.dst = ip.src
        myip.src = ip.dst
        # 对于echo reply，其源ip地址与目的ip地址可以直接互换
        myip.protocol = ip.protocol
        # 与echo request使用同一协议
        myethernet = Ethernet()
        myethernet.ethertype = EtherType.IPv4
        # 设置ethertype
        myreply = myethernet + myip + myicmp
        # 注意拼接顺序
        return myip,myreply
    	#返回生成的echo reply与echo reply的IP包头，方便之后转发时利用myip查找转发表
```

<font size=4 face="楷体">	生成好echo reply之后，就需要查找转发表进行转发了；很遗憾在lab4中转发没有单独的封装成一个函数，所以在lab5中将这一部分单独拿出来作为一个函数`lookup`，其功能就是调用`longest_match`进行查表，然后根据查表结果进行ttl减一、转发工作，实现如下：</font>

```python
    def lookup(self,ip,pkt):
        flag,match = self.longest_match(ip)
        # 查找转发表
        if flag == False:
            pass
        # 无匹配项，不做处理
        else:
            # 匹配成功，ttl减一
            pkt[IPv4].ttl = pkt[IPv4].ttl - 1
            if self.forwarding_table[match][1] is None:
                next_hop = ip.dst
            else:
                next_hop = self.forwarding_table[match][1]
			# 确定next_hop的值
            if next_hop in self.mytable.keys():
            # ARP Cache中已有记录，可以直接设置以太网地址后转发
                pkt[Ethernet].src = self.mydict2
                [self.forwarding_table[match][2]][0]
                pkt[Ethernet].dst = self.mytable[next_hop][0]
                self.net.send_packet(self.forwarding_table[match][2], pkt)
            else:
            # 调用enqueue函数，放入缓存queue中等待发送
                self.enqueue
                (self.forwarding_table[match][2],
                 self.mydict2[self.forwarding_table[match][2]][0],
                 self.mydict2[self.forwarding_table[match][2]][1], 
                 next_hop,
                 pkt)               
```

<font size=4 face="楷体">	这样，在main中只需要对这两个函数进行调用就可以了：</font>

```python
     if gotpkt:                
        arp = pkt.get_header(Arp)
        if arp is None:
            ip = pkt.get_header(IPv4)
            if ip is None:
                pass
            else:                        
                if ip.dst in self.myips:
                    icmp = pkt.get_header(ICMP) 
                    pkt[IPv4].ttl = pkt[IPv4].ttl - 1
           # echo request已经传到，故ttl减或不减都无所谓了，为保持形式一致进行了减一
           # 但即使减之后ttl为0也没有关系，因为echo request已经传到
                    if icmp is None:
                        pass
                    else:
                        if icmp.icmptype == ICMPType.EchoRequest:                    
                            myip,myreply = self.creat_echo_reply(ip,icmp)
                            self.lookup(myip,myreply,False,dev)
                        else:
                            pass                                                                                                      
                 else:
                    self.lookup(ip,pkt,False,dev)                           
                                                                
     else:
        ……
```

<font size=4 face="楷体">	由此，就完成了Task2的任务。</font>

### Task 3: Generating ICMP error messages

<font size=4 face="楷体">	实验要求：修改文件`lab_5/myrouter.py`，使得router可以在四种需要发送error message的情况下发送相应的error message；
	思考与实现：
	在进行实现之前，先整理一下router收到一个IP包的处理流程：如果收到的IP包的目的地址是router本身，要分两种情况处理：如果是ICMP echo request，则就按Task2中的实现处理；否则，应该传回一个**ICMP destination port unreachable** error message；如果收到的IP包的目的地址不是router本身，则router需要转发这个包，即调用`self.lookup`进行转发；如果调用`self.lookup`时查找转发表中有匹配项，则找到了转发方向，下一步应该将ttl减一，如果减一后不为0则可以正常转发，否则就需要发送**ICMP time exceeded** error message；如果调用`self.lookup`时查找转发表中没有匹配项，则需要发送**ICMP destination network unreachable** error message；由此可以看出查找转发表的结果对于main的条件判断是必要的；最后对于ARP Failure，可以直接在lab4的`self.handle_arq_request`中检测到：对于过期的条目，删除前要对其中的每个数据包产生一个**ICMP destination host unreachable** error message并发送；
	根据以上分析，可以将所有的操作分为“生成错误信息”与“转发错误信息”两个原操作，其中后者主要还是依赖`self.lookup`实现；</font>

<font size=4 face="楷体">	对于生成错误信息的实现，手册已经给出具体的方法了，在此不做赘述；将该功能封装为函数`creat_error_message`，实现如下：（传入参数中，生成error message是因为origpkt的错误产生的，error_type,error_code指明了错误的类型与code）</font>

```python
    def creat_error_message(self,origpkt_,error_type,error_code):
        origpkt = copy.deepcopy(origpkt_)
        # 深度拷贝origpkt_，用于生成error message的icmpdata部分
        i = origpkt.get_header_index(Ethernet)
        del origpkt[i]
        # 去除以太网包头
        ip = origpkt.get_header(IPv4)
        icmp = ICMP()
        icmp.icmptype = error_type
        icmp.icmpcode = error_code
        icmp.icmpdata.data = origpkt.to_bytes()[:28]
        # 根据手册方法生成ICMP包头
        myip = IPv4()
        myip.protocol = IPProtocol.ICMP
        myip.ttl = 64
        myip.dst = ip.src
        # srcip未确定，要到查找转发表后设置，现在不必设置
        myethernet = Ethernet()
        myethernet.ethertype = EtherType.IPv4
        myreply = myethernet + myip + icmp
        return myip,myreply
```

<font size=4 face="楷体">	错误信息生成后，就可以调用`self.lookup`来进行查表转发了，利用函数`send_error_message`将所有关于错误信息的处理进行封装，实现如下：</font>

```python
   def send_error_message(self,pkt,error_type,error_code,rec_port):
        error_ip,error_message = self.creat_error_message
        						(pkt,error_type,error_code)
        if error_ip.dst in self.myips:
            #如果错误信息的目的地是router自己，则直接丢弃
            #主要是考虑router生成的reply或错误信息收不到ARP Reply时的情况
            pass
        else:
            self.lookup(error_ip,error_message,True,rec_port)
```

<font size=4 face="楷体">	这样，只要在合适的位置针对不同的错误调用`self.send_error_message`就可以完成错误信息的转发；但是这里还是有一个小问题：echo reply与error message的转发都需要调用`self.lookup`，其中error message的srcip是要在`self.lookup`中查找转发表得到的（查表得到的发送端口的ip地址，不一定和收到引起错误的包的端口一样），但echo reply的srcip就是之前被ping的端口的ip地址（在`self.creat_echo_reply`中已经设置好了，无需在`self.lookup`中设置），所以在调用`self.lookup`时，又添加了一个参数`if_error`，值为True表示是一个error message，则`self.lookup`中要在查找转发表后对包的srcip进行设置；否则值为False就没有必要再对包的srcip处理了；
	解决了上述问题后，实际上就可以直接通过调用`send_error_message`完成一个error message的生成与发送了；只要在合适的地方添加对于`self.send_error_message`的调用就可以通过测试样例了；不过考虑这样的情形：router在生成echo reply或者error message后进行转发时，查找转发表没有匹配项；对于这样的情况一般是不会出现的，可以直接丢弃；但还有一种处理方法是从接收端口（接收到echo request或引发错误的包的端口）转发；这里我选择了第二种处理，即如果发生上述情况就从接收端口转发；为此，构造函数`error_lookup_error`用于处理这种情况：</font>

```python
    def error_lookup_error(self,pkt,reply,port):
  #pkt是echo request或引起错误的包，reply是生成的echo reply或error message，port是接收端口
  #只有在router生成了一个echo reply或者error message后找不到转发表的匹配条目时调用
        if reply[ICMP].icmptype != ICMPType.EchoReply:
            #如果是error message，则srcip仍未设置（因为匹配失败）
            reply[IPv4].src = self.mydict2[port][1]
            #设置srcip为接收端口port的ip地址
        reply[IPv4].ttl = reply[IPv4].ttl - 1
        reply[Ethernet].src = self.mydict2[port][0]                    
        reply[Ethernet].dst = pkt[Ethernet].src
        #设置以太网包头，src为接收端口mac，即从接收端口转发回去
        self.net.send_packet(port, reply)  
        #从接收端口转发回去
```

<font size=4 face="楷体">	这样，每一次发送错误信息或发送echo reply调用`self.lookup`时，如果匹配转发表条目失败的话，应该调用`self.error_lookup_error`函数进行处理；考虑到调用该函数需要传入接收端口port，所以在调用`self.lookup`也加入了对应的接收端口rec_port；进一步来看，在`send_error_message`中会调用`self.lookup`，所以在`send_error_message`函数中也加入了参数rec_port；此外，最麻烦的就是发生ARP Failure时，需要对过期条目中所有的包依次发送错误信息，所以缓存queue（lab4中记录ARP请求与缓存数据包的队列）中的每个包的接收端口也要记录下来以便在发生ARP Failure、调用`send_error_message`时可以传入相应的接收端口参数；当前缓存queue中一个条目的结构为：（这里value4中的port是转发端口而非接收端口）</font>

| key   | value1 | value2      | value3        | value4 | value5         |
| ----- | ------ | ----------- | ------------- | ------ | -------------- |
| dstip | times  | time.time() | ARP Request包 | port   | [pkt1、pkt2……] |

<font size=4 face="楷体">	将此结构进行微小的改动：将存放等待ARP回复的packet list（value5）中的元素由包改为[包，接收端口]这样的list，得到：</font>

| key   | value1 | value2      | value3        | value4 | value5                                 |
| ----- | ------ | ----------- | ------------- | ------ | -------------------------------------- |
| dstip | times  | time.time() | ARP Request包 | port   | [[pkt1,rec_port1]、[pkt2,rec_port2]……] |

<font size=4 face="楷体">	这样，需要更改`self.enqueue`函数来实现在一个包进入缓存queue中时，接收该包的端口也存储下来；同时在`self.dequeue`取包时也要修改：在取出value5的第i个元素后再取首部元素，才是正确的pkt，具体改动如下图所示：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab5_1.png)

<font size=4 face="楷体">	有了以上结构与函数的支持，就可以完善路由器对这种“不正常情况”的处理了；首先是`self.lookup`可以实现为：</font>

```python
    def lookup(self,ip,pkt,if_error,rec_port):
        flag,match = self.longest_match(ip) #查找转发表进行匹配
        if flag == False:
            pass
        else:
            # 匹配成功
            pkt[IPv4].ttl = pkt[IPv4].ttl - 1
            # ttl减一在匹配成功之后进行
            if pkt[IPv4].ttl == 0:
            # 发生 time exceeded错误，调用send_error_message生成并转发错误信息
            # 错误类型与错误code分别为	
            # ICMPType.TimeExceeded,ICMPCodeTimeExceeded.TTLExpired
                self.send_error_message(pkt,ICMPType.TimeExceeded,
                                        ICMPCodeTimeExceeded.TTLExpired,
                                        rec_port)      
            else:
                #可以正常转发
                if if_error == True:
                    #如果是error message，srcip要填为转发端口的ip地址，利用mydict2找到
                    pkt[IPv4].src = 
                    self.mydict2[self.forwarding_table[match][2]][1]
                if self.forwarding_table[match][1] is None:
                    next_hop = ip.dst
                else:
                    next_hop = self.forwarding_table[match][1]
				# 确定next hop的值
                if next_hop in self.mytable.keys():
                # ARP Cache中已有记录，可以直接设置以太网地址后转发
                    pkt[Ethernet].src = 
                    self.mydict2[self.forwarding_table[match][2]][0]
                    pkt[Ethernet].dst = self.mytable[next_hop][0]
                    self.net.send_packet(self.forwarding_table[match][2], pkt)
                else:
                # 调用enqueue函数，放入缓存queue中等待发送，注意rec_port也要存入缓存queue
                    self.enqueue
                    (self.forwarding_table[match][2],
                     self.mydict2[self.forwarding_table[match][2]][0], 
                     self.mydict2[self.forwarding_table[match][2]][1], 
                     next_hop,pkt,rec_port)                        
        return flag
    	#返回查找转发表的结果，以便于其它函数判断匹配结果
```

<font size=4 face="楷体">	接着是`self.send_error_message`的实现：</font>

```python
    def send_error_message(self,pkt,error_type,error_code,rec_port):
	#传入参数中，生成error message是因为pkt的错误产生的，error_type,error_code指明了错误的类型与code，rec_port是收到pkt的接收端口        
        error_ip,error_message = self.creat_error_message
        						(pkt,error_type,error_code)
        # 调用creat_error_message生成错误包
        if error_ip.dst in self.myips:
            pass
        else:
        	flag = self.lookup(error_ip,error_message,True,rec_port)
        	# 转发生成的错误信息
        	if flag == False:
            	# 如果没有匹配条目，调用error_lookup_error将生成的error_message从rec_port转发
            	self.error_lookup_error(pkt,error_message,rec_port)
```

<font size=4 face="楷体">	最后，就只需要在`main`中合适的位置加入error message的生成与发送；这其中 **time exceeded**错误已经在`self.lookup`中处理了；ARP Failure的处理要在`handle_arq_request`中实现，具体而言就是在删除过期条目时对其中的每个包回复一个**ICMP destination host unreachable**信息：</font>

```python
   def handle_arq_request(self):
        now = time.time()
        to_be_kill = []
        for keys_ in self.myqueue.queue.keys():
            if now - self.myqueue.queue[keys_][1] > 1:
                if self.myqueue.queue[keys_][0] == 5:
                    to_be_kill.append(keys_)
                else:
                    self.net.send_packet(self.myqueue.queue[keys_][3],
                                         self.myqueue.queue[keys_][2])
                    self.myqueue.queue[keys_][1] = time.time()
                    self.myqueue.queue[keys_][0] = 
                    self.myqueue.queue[keys_][0] + 1   
                           
        for k in to_be_kill:
            for packet in self.myqueue.queue[k][4]:
            #遍历过期条目中的每个包，每次发送一个ICMP destination host unreachable信息
            #这里的packet是一个[pkt,rec_port]的list
            #错误信息的类型是ICMPType.DestinationUnreachable，
            #code是 ICMPCodeDestinationUnreachable.HostUnreachable
            	self.send_error_message(packet[0],
                 ICMPType.DestinationUnreachable,
                                        ICMPCodeDestinationUnreachable.HostUnreachable,
                 packet[1])
            del self.myqueue.queue[k]
```

<font size=4 face="楷体">	剩余两种error message的处理要在`main`中实现，增加error message的生成与发送后的`main`如下（缩进太多难以对齐，建议学长学姐直接查看源代码，结合下面的注释理解）：</font>

```python
            if gotpkt:                
                arp = pkt.get_header(Arp)
                if arp is None:
                    ip = pkt.get_header(IPv4)
                    if ip is None:
                        pass
                    else:                        
                        if ip.dst in self.myips:
                            #收到发给自己的包
                            icmp = pkt.get_header(ICMP) 
                            pkt[IPv4].ttl = pkt[IPv4].ttl - 1           
# echo request已经传到，故ttl减或不减都无所谓了，为保持形式一致进行了减一
# 但即使减之后ttl为0也没有关系，因为echo request已经传到
                            if icmp is None:
                            #不是ICMP echo request，发送 ICMP destination port unreachable错误
                                self.send_error_message(pkt,
                                 ICMPType.DestinationUnreachable,
                                 ICMPCodeDestinationUnreachable.
                                 PortUnreachable,dev)
                            else:
                                if icmp.icmptype == ICMPType.EchoRequest:                                    
                                    #回复echo ping reply
                                    myip,myreply = 
                                    self.creat_echo_reply(ip,icmp)
                                    flag = 
                                    self.lookup(myip,myreply,False,dev)
                                    if flag == False:
                                    #如果reply查找不到匹配项则从接收端口转发
                                        self.error_lookup_error(pkt,myreply,dev)
                                else:
                                #不是ICMP echo request，发送 ICMP destination port unreachable错误    
                                	self.send_error_message(pkt,
                                                            ICMPType.DestinationUnreachable,
                                                            ICMPCodeDestinationUnreachable.PortUnreachable,
                                                            dev)                                                                                                       
                        else:
                            #否则进行转发
                            flag = self.lookup(ip,pkt,False,dev)
                            if flag == False:
                                #如果匹配失败，则返回 ICMP destination network unreachable信息
                                self.send_error_message
                                (pkt,
                                 ICMPType.DestinationUnreachable,
                                 ICMPCodeDestinationUnreachable.
                                 NetworkUnreachable,dev)                            
                                                                
                else:
                    ……
```

<font size=4 face="楷体">	这样，所有的情况router就都可以处理掉了，大体的处理逻辑为：收到echo ping request时回复reply，reply的srcip为被ping的端口的ip；在合适的时机生成并转发error message，error message的srcip是发送端口的ip；如果生成的reply或error message转发时没有匹配成功则从接收端口返回（这种情况下也可以直接丢弃，只要忽略所有`error_lookup_error`的调用就好了）。</font>

<font size=4 face="楷体">	关于**Testing**与**Deploying**参见文档**实验结果**部分；</font>

### <font face="楷体">4、实验结果</font>

#### Task1&Task2

<font size=4 face="楷体">	略。</font>

#### Task3

<font size=4 face="楷体">	`routertests3.srpy`的testing结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab5_2.png)

![](C:\Users\njuwhl2019hp\Desktop\计网\lab5_3.png)

<font size=4 face="楷体">	在给出的测试用例中，1——4是router收到一个echo ping request后回复一个reply的过程，5——6还是收到一个echo ping request后回复一个reply的过程，只是在之前已经有ARP查询操作，所以此次回复不必进行ARP查询；7——10是一个数据包转发到router后在进行下一步转发时ttl变为0，因此要向源发送一个**ICMP time exceeded**的过程；11——12是转发时查找转发表没有匹配项，因而向源发送**ICMP destination network unreachable**的过程；13——14是收到发给router，但不是ICMP Request，因而向源发送**ICMP destination port unreachable**的过程；15——25模拟了一个等待ARP Reply，但在5次发送ARP Request后仍未得到回复的过程，因此会像26——28所示的向源发送 **ICMP destination host unreachable**。
	从lab5要实现的功能来看，给出的测试用例测试了router对echo ping request作出回复与发送四种错误信息的功能；没有对手册中faq提到的“缓存queue某一条目有多个数据包，该条目过期时要对其所有的数据包各自发送一个 **ICMP destination host unreachable**”与“ttl为1的数据包转发到router，同时查找转发表也没有匹配项，这时应该只会产生查找转发表失败的 **ICMP destination network unreachable**信息”进行测试；另外对于“router发送echo ping reply时如果查找转发表失败，应该从接收端口转发”的测试也没有，所以，结合代码的实现思路，添加以下3个阶段的测试（`routertests.py`中定义）：</font>

<font size=4 face="楷体">	stage1：router首先会收到10.10.0.2发给自己的echo ping request——pkt0，所以router生成了相应的echo ping reply，要发往10.10.0.2，router会将这个reply放入缓存queue中，同时对10.10.0.2发送ARP Request；接下来router会收到发向10.10.0.2的4个数据包pkt1——pkt4，其中pkt1——pkt3的srcip互不相同，pkt3与pkt4由同一个host发出；这四个包也会被放到缓存queue中等待10.10.0.2的ARP Reply；但是发送5次ARP Request后都没有回复；因此router会分别向根据pkt1——pkt4向它们的源发送ICMP destination host unreachable（pkt0的源是router自己，所以直接被丢弃），其中由于pkt3与pkt4由同一个host发出，所以router会向这个host发送两次ICMP destination host unreachable；发送ICMP destination host unreachable的具体过程是router会发送3次ARP Request来得到pkt1——pkt4的sender的Mac地址（pkt3与pkt4一样故只发3次），接着router会收到ARP Reply，每收到一个ARP Reply router就会发送一个ICMP destination host unreachable错误信息（其中pkt3与pkt4的host会重复收到两次错误信息），并且错误信息的srcip是转发端口的ip地址；具体的测试代码实现见`routertests.py`，这里不进行赘述；</font>

<font size=4 face="楷体">	stage2：router会在eth0收到发给自己的eth1的echo ping request，所以会生成一个echo ping reply，但echo ping request的sender的IP地址是1.2.3.4，查找转发表没有匹配项；这种情况下router会直接从eth0将echo ping reply发送出去，同时reply的srcip仍然是eth1的ip地址（被ping的ip地址），而不是转发端口eth0的ip地址；具体的测试代码实现见`routertests.py`，这里不进行赘述；</font>

<font size=4 face="楷体">	stage3：router从eth2收到了一个需要转发的数据包，其ttl值为1，在router查找转发表时没有匹配项；正如手册所说，因为ttl减一是在查找转发表找到匹配项后进行的。所以这种情况router只会生成一个 **ICMP destination network unreachable**信息发送回去，不会产生**ICMP time exceeded**错误信息；这里错误信息的srcip是转发端口eth0的ip地址而非接收端口eth2的ip地址。</font>

<font size=4 face="楷体">	stage4：router从eth2收到了一个需要转发的数据包，其ttl值为1，因此在查表找到匹配项后会产生**ICMP time exceeded**错误信息；但是在发送这个错误信息时查找转发表没有匹配成功，这时router应该直接从eth2将**ICMP time exceeded**错误信息转发出去；这里错误信息的srcip就是eth2的ip地址。</font>

<font size=4 face="楷体">	`routertests.py`的testing结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab5_4.png)

![](C:\Users\njuwhl2019hp\Desktop\计网\lab5_5.png)

<font size=4 face="楷体">	由此证明了实现的正确性。</font>

<font size=4 face="楷体">	`routertests3_template.py`的testing为空，不做展示。</font>

<font size=4 face="楷体">	**deploying**过程如下：</font>

<font size=4 face="楷体">	deploying模拟的过程就是server1向router进行ping以及产生各种error message的过程；依照手册顺序依次验证：</font>

<font size=4 face="楷体">	server1 ping 10.10.1.2的结果如下；可以看出针对这个ping request router正确地回复了ping reply；</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab5_6.png)

<font size=4 face="楷体">	server1 ping server2，ping request的ttl=1的结果如下；可以看出针对这个ping request router正确地从eth0向server1回复了**ICMP time exceeded**错误信息；</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab5_7.png)

<font size=4 face="楷体">	server1会ping一个不存在的地址‘1.2.3.4’，在这之前需要在`start_mininet`中添加server1对于这一地址的一个route，来将这个ping request转发到router上：</font>

```python
set_route(net, 'server1', '1.2.0.0/16', '192.168.100.2')
```

<font size=4 face="楷体">	这样，server1 ping一个不存在的地址‘1.2.3.4’的结果如下，可以看出router针对这一ping request正确地从eth0向server1发送了一个**ICMP destination network unreachable**信息；</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab5_8.png)

<font size=4 face="楷体">	在server1上执行traceroute的结果为：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab5_9.png)

<font size=4 face="楷体">	根据server1显示的traceroute结果，结合wireshark抓包结果可以实现符合测试要求。</font>

### <font face="楷体">5、核心代码</font>

​	<font size=4 face="楷体">	本实验中各阶段主要修改的代码在**实验内容**部分已经提到；这里不再赘述：主要修改或实现的代码有（从上到下）：`handle_arq_request`、`enqueue`、`lookup`、`creat_echo_reply`、`creat_error_message`、`send_error_message`与`error_lookup_error`以及`router_main`函数；</font>

### <font face="楷体">6、总结与感想</font>

<font size=4 face="楷体">	本次实验让我对于router在网络通信过程中的工作流程有了比较全面的了解，同时也了解到了在通信过程中产生的不同种类的错误信息以及router的处理方式。
	**实验遇到的问题与解决**：
	本次实验的实现过程其实没有遇到问题，因为都是在lab4的基础上不断增加函数和函数调用的修修补补的工作，最核心的生成echo ping reply与error message的方式手册也已经给出了，所以实现过程没有问题；遇到问题是在编写测试样例的stage4的时候，每次执行都会出现`error:KeyError: 'No such header type exists.'`这样的错误提示；即使更改错误类型（由**ICMP time exceeded**改为**ICMP destination network unreachable**或**ICMP destination port unreachable**）也仍然会报这个错误；经过调试发现错误原因是：在收到引起错误的pkt包之后要利用pkt生成一个error message，但是在初始的`self.creat_error_message`中是直接将pkt的以太网包头删除后来填充error message的icmpdata的；因此，在之后调用`self.error_lookup_error`执行到`reply[Ethernet].dst = pkt[Ethernet].src`时，pkt的以太网包头已经被删除了，因此`pkt[Ethernet]`是错误的；找到了错误后，修正就很简单了：引入`copy`模块，利用`copy.deepcopy`来得到pkt的一个副本，利用副本生成error message，就不会影响pkt的内容了。</font>

