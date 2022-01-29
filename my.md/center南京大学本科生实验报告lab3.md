# <font face="楷体"><center>南京大学本科生实验报告</center></font>

<font size=3 face="楷体">课程名称：**计算机网络**							任课教师：李文中
助教：陈衍庆、🕊、田冰川、吴昌容、段建辉、方毓楚、黄晓洁、刘柯鑫、李想</font>

| <font face="楷体">学院</font>  | <font face="楷体">计算机院</font> | <font face="楷体">专业（方向）</font>  | <font face="楷体">计算机科学与技术</font> |
| ------------------------------ | --------------------------------- | -------------------------------------- | ----------------------------------------- |
| <font face="楷体">学号</font>  | 181860092                         | <font face="楷体">姓名</font>          | <font face="楷体">王瀚霖</font>           |
| <font face="楷体">Email</font> | 603102048@qq.com                  | <font face="楷体">开始/完成日期</font> | 2020.3.25  /  2020.4.3                    |



### <font face="楷体">1、实验名称</font> : Respond to ARP

### <font face="楷体">2、实验目的</font>

<font size=4 face="楷体">	完成建立一个IPv4路由的第一阶段任务：实现路由器对ARP请求的应答，并为路由器建立一张cached ARP table；</font>

### <font face="楷体">3、实验内容</font>：

### Task 1: Preparation

<font size=4 face="楷体">	实验要求：在switchyard下创建lab_3，在其中完成文件的复制与更名工作，做好实验准备；
	具体实现：略；</font>

### Task 2: Handle ARP Requests

<font size=4 face="楷体">	实验要求：修改文件`lab_3/myrouter.py`，判断每次接收到的数据报是否为ARP Request而且其目的IP地址是router的interfaces之一，是的话需要从接收到此数据报的interface发送对应的ARP Reply；否则不做任何处理；
	思考与实现：根据实验要求分析，要完成以上任务，需要知道判断一个数据报是否为ARP Request的方法，也要知道路由器所有interface的IP地址；同时，在发送ARP Reply的时候，我们需要知道相应ARP Request数据报中目的IP地址对应的Mac地址是多少；解决了以上问题，任务就很容易实现了；
	首先，**如何判断一个数据报是否为ARP Request**：由手册与reference提示得到如下方法：首先通过`arp = pkt.get_header(Arp)`从收到的数据报中取出ARP header；如果arp类型为None，说明该数据报没有ARP header，自然就不是ARP Request了；如果arp不为None，则进一步判断`arp.operation`的值，其中`ArpOperation.Reply`表示该数据报为ARP Reply，`ArpOperation.Request`表示该数据报为ARP Request；通过这样两次判断就可以完成对ARP Request的判断了；
	**路由器所有interface的IP地址的获取**：利用`interfaces()`方法可以获取与router相连的所有interface项组成的list，每一个interface项又有`name, ethaddr, ipaddr, netmask attributes`这些属性，所以可以通过遍历此list将每一项的ipaddr取出，从而得到所有interface的IP地址；
	**由ARP Request中的目的IP地址得到相应Mac地址**：相应Mac的获取只会发生在进行ARP Reply的时候，这时ARP Request中的目的IP地址为router的interface之一，故可以通过建立一个interface的IP地址与Mac地址一一对应的表来达到目的；每次需要得到相应的Mac地址，就根据目的IP地址查找这张表就可以了；
	综上，实现思路与过程为：
	在`def __init__`中声明要用到的数据结构：首先是获取所有interface的IP地址，利用`interfaces()`方法实现：</font>

```python
       my_interfaces = net.interfaces() 
       self.myips = [intf.ipaddr for intf in my_interfaces]
```

<font size=4 face="楷体">	接下来利用`dict`建立interface的IP地址与Mac地址的一一对应表，考虑到是通过IP地址查询Mac地址，故以IP地址作为key：</font>

```python
		self.mydict = {}
        for intf in my_interfaces:
            self.mydict[intf.ipaddr] = intf.ethaddr
```

<font size=4 face="楷体">	有了这些已知内容，在每接收到一个数据报时，首先执行：</font>

```python
                arp = pkt.get_header(Arp)
```

<font size=4 face="楷体">	然后根据arp进行判断：如果是None则不做处理，进入下一次循环：</font>

```python
                if arp is None:
                    continue
```

<font size=4 face="楷体">	否则判断是ARP Request还是ARP Reply，若为ARP Reply则不作处理，否则判断目的IP是否是本router的interface，如果是则利用`create_ip_arp_reply`生成ARP Reply并对ARP Request进行回复：</font>

```python
                if arp.operation == ArpOperation.Reply:
                    continue
                if arp.operation == ArpOperation.Request:
                    if arp.targetprotoaddr in self.myips:
                        log_info("receive ARP Request packet {} from {}".format(pkt, dev))
                        reply = create_ip_arp_reply(self.mydict[arp.targetprotoaddr], arp.senderhwaddr, arp.targetprotoaddr, arp.senderprotoaddr)
                        self.net.send_packet(dev, reply)
                        log_info("send ARP Reply packet {} to {}".format(reply, dev))
                    else:
                        pass
```

<font size=4 face="楷体" color=green>	生成ARP Reply时，是由router向发起ARP请求的一方发送，故`senderhwaddr`是IP地址为`arp.targetprotoaddr`对应的interface的Mac地址（由mydict查询到），`targethwaddr`是ARP Request中的发送方Mac地址`arp.senderhwaddr`；`senderprotoaddr`是ARP Request中的目的IP地址` arp.targetprotoaddr`；`targetprotoaddr`是ARP Request中的发送方IP地址`arp.senderprotoaddr`；</font>

<font size=4 face="楷体">	通过以上操作，router就具备了识别ARP Request、并对传给自己interface的请求做出应答的功能了；</font>

<font size=4 face="楷体">	关于**Testing**与**Deploying**参见文档**实验结果**部分；</font>

### Task 3: Cached ARP Table

<font size=4 face="楷体">	实验要求：修改文件`lab_3/myrouter.py`，为router建立cached ARP Table；
	思考与实现：根据手册要求，为router建立cached ARP Table就是在router每次接收到一个ARP包时，将ARP包中发送方sender的IP地址与Mac地址作为一个pair记录/更新到ARP Table中；同时，在**Testing**时，要将更新的cached ARP Table打印出来，所以需要判断一下表中内容是否更新；最后，还要实现一个**timeout mechanism**来将超时的条目从表中删除；
	cached ARP Table与timeout mechanism的实现可以参考lab_2中的方法进行实现，即利用`dict`类型的变量`mytable`表示cached ARP Table，其key值为IP地址，value是一个list，list[0]是IP地址对应的Mac地址，list[1]为该条目被加入/上一次更新的时刻，由此可以计算出该条目的`elapsed_time = time.time() - list[1]`；判断cached ARP Table是否被更新通过设置一个bool变量实现：考虑cached ARP Table被更新的情况：
	1、某些过期条目被删除；
	2、加入新的条目；
	3、原有的条目内容被更新（这里elapsed_time的更新不算作条目内容的更新）；
	这样考虑的话，判断表是否被更新就很容易了：声明一个bool型变量flag，初始值为False；如果遍历表一次后发现要删除的条目数不为0，则要删除，就将flag设为True；之后收到一个新的ARP包时，如果表中的`keys()`没有该ARP包的`senderprotoaddr`，说明这是要加入表的新条目，置flag为True；如果`keys`有该ARP包的`senderprotoaddr`但对应条目的value中记录的Mac地址不等于该ARP包的`senderhwaddr`，则要进行更新，也要将flag置为True；添加或更新条目的操作都可以由`mytable[ip] = [mac,time.time()] `来实现（ip、mac分别指ARP包中sender的ip与mac地址）；
	基于以上思考，改写`lab_3/myrouter.py`的处理流程为：
	在类的`__init__`中声明以下结构：</font>

```python
    def __init__(self, net):
        self.net = net
        # other initialization stuff here
        my_interfaces = net.interfaces() 
        self.myips = [intf.ipaddr for intf in my_interfaces]
        # 得到所有interface的IP地址，放在一个list中
        self.mydict = {}
        for intf in my_interfaces:
            self.mydict[intf.ipaddr] = intf.ethaddr
        # 建立interface的ip——mac对应表
        self.mytable = {}
        # router的cached ARP Table
        self.to_be_delete = []
        # 存储每次遍历时要删除的条目
        self.flag = False
        # 判断cached ARP Table是否更新的变量
```

<font size=4 face="楷体">	每次收到一个数据包时，先利用`time.time()`获取当前时间，然后遍历cached ARP Table计算每个条目的elapsed_time，将过期的条目删除（为了在给定的测试用例中显示出表项被删除，设置elapsed_time为0.001s），同时考虑删除条目不为空时，置flag为True：</font>

```python
            if gotpkt:
                log_debug("Got a packet: {}".format(str(pkt)))
                now = time.time()
                for key in self.mytable.keys():
                    if now - self.mytable[key][1] > 0.001:
                        self.to_be_delete.append(key)
                        # 将待删除条目添加到self.to_be_delete
                l = len(self.to_be_delete)
                if l > 0:
                    # 删除条目不为空，cached ARP Table有更新，置flag为True
                    self.flag =  True
                    log_info("kill {} entries".format(l))
                    # 打印删除信息
                for kill in self.to_be_delete:
                    del self.mytable[kill]
                    # 进行删除
                self.to_be_delete.clear()
                #清空删除列表，为下一次循环作准备
```

<font size=4 face="楷体">	处理完之后，对收到的数据包`pkt`进行分析，判断其是否为ARP 包，如果是则需要更新一下cached ARP Table：</font>

```python
                arp = pkt.get_header(Arp)
                if arp is None:
                    continue
                	#不是ARP包，不做处理
                else:
                    ip = arp.senderprotoaddr
                    mac = arp.senderhwaddr
                    #提取要存到cached ARP Table的IP与Mac地址
                    if(ip not in self.mytable.keys()) or (mac != self.mytable[ip][0]):
                    #需要对cached ARP Table的内容进行更新（不包括elasped_time的更新）
                        self.mytable[ip] = [mac,time.time()]
                        #更新/添加条目
                        self.flag = True
                        #置flag为True
                    else:
                        self.mytable[ip] = [mac,time.time()]    
                        #cached ARP Table的内容没有变化，只需更新elasped_time
```

<font size=4 face="楷体">	在收到ARP包的情况下，进一步判断是请求，还是应答，以进行相应的处理；处理的方式与Task2基本一样，只是为了方便理解cached ARP Table的变化添加了一些打印提示信息的语句；在处理完进入下一次循环前，如果flag是True则还需要将更新后的cached ARP Table内容打印出来：</font>

```python
			if arp.operation == ArpOperation.Reply:
                log_info("receive ARP Reply packet {} from {}".format(str(pkt), dev))
            	 # 输出提示信息：收到ARP Reply，不做处理
            	if self.flag:
                # cached ARP Table有更新，需要打印
                	log_info("Update ARP Table as:")
                # 输出提示信息
                	for key in self.mytable.keys():
                    	log_info("ip: {}    mac: {}".format(key,self.mytable[key][0]))
                    # 打印cached ARP Table内容
                continue
                #进入下一次循环
            if arp.operation == ArpOperation.Request:
                if arp.targetprotoaddr in self.myips:
                    log_info("receive ARP Request packet {} from {}".format(str(pkt), dev))
                    # 输出收到发给自己的ARP Request的提示信息
                    reply = create_ip_arp_reply(self.mydict[arp.targetprotoaddr], arp.senderhwaddr, arp.targetprotoaddr, arp.senderprotoaddr)
                    #生成回复包
                    self.net.send_packet(dev, reply)
                    # 从同一个端口发送回复包
                    log_info("send ARP Reply packet {} to {}".format(str(reply), dev))
                    # 输出提示信息
                else:
                    log_info("receive ARP Request packet {} from {} , not for my interfaces".format(str(pkt), dev))
                    # 输出收到ARP Request但不是发给自己的提示信息，不做处理
            if self.flag:
                # cached ARP Table有更新，需要打印
                log_info("Update ARP Table as:")
                for key in self.mytable.keys():
                    log_info("ip: {}    mac: {}".format(key,self.mytable[key][0]))
                # 打印完毕后进入下一次循环
```

<font size=4 face="楷体">	这样，就成功地为router建立了一个cached ARP Table；</font>

<font size=4 face="楷体">	关于**Testing**参见文档**实验结果**部分；</font>



### <font face="楷体">4、实验结果</font>

#### Task1

<font size=4 face="楷体">	略。</font>

#### Task2

<font size=4 face="楷体">	`routertests1.srpy`的testing结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab3_1.png)

<font size=4 face="楷体">	`routertests1full.srpy`的testing结果如下</font>

<img src="C:\Users\njuwhl2019hp\Desktop\计网\lab3_2.png"  />

<font size=4 face="楷体">	deploying结果如下：
	选取server1来ping router：执行的过程与手册描述基本一致，要改变的地方就只是在server1上运行xterm，然后向router发送ICMP echo request：`server1# ping -c3 192.168.100.2`,得到wireshark捕捉到server1的信息为：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab3_3.png)

<font size=4 face="楷体">	首先，查看wireshark捕捉到的ARP Request信息：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab3_4.png)

<font size=4 face="楷体">	可以看出，server1要向router ping消息，会先向router发送一个ARP Request来得到router对应interface的mac地址，该请求由server1发出，故sender的ip、mac地址就是server1的信息，目的ip地址是router的interface之一，目的mac未知，故为“00:00:00:00:00:00”；按照`myrouter.py`实现的逻辑，router应该正确识别出这个目标为自己的ARP Request，并作出Reply，在Reply中Sender与Target反转，同时要查询的router的interface的mac地址会被填到Reply的Sender Mac address；从wireshark捕捉到的ARP Reply信息来看，测试表现是符合预期的：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab3_5.png)

<font size=4 face="楷体">	完成了对ARP Request的回复后，server1会向router发送ICMP协议下的Request，由于本阶段的router不会对收到的网络层数据报进行处理，所以server1连续发送了3次ping request但都没有回应；
	通过以上server1 ping router的模拟，验证了router对ARP Request的处理与应答是正确的。</font>

#### Task3

<font size=4 face="楷体">	`routertests1.srpy`的testing结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab3_6.png)

<font size=4 face="楷体">	（手动划线分割，绿线划分阶段，黄线划出每次更新的ARP表内容；条目过期时间为0.001s）从log的信息来看，该测试有3个阶段：第一阶段IP地址为192.168.1.100的host向router发送了针对router的ARP Request，所以router会从eth0发送一个Reply，同时在收到ARP Request时将其中sender的IP与Mac地址记录到表中，故更新后表的内容中添加了`<192.168.1.100 30:00:00:00:00:01>`这一内容；cached ARP Table更新为：</font>

| IP            | MAC                         |
| ------------- | --------------------------- |
| 192.168.1.100 | 30 : 00 : 00 : 00 : 00 : 01 |

<font size=4 face="楷体">	第二阶段，首先由于条目超时过期，所以被删除(kill 1 entries)；然后router收到了一个不是发给自己的Request，所以router会drop掉这个包，但在收到这个ARP Request时，router也要更新表的内容，所以cached ARP Table删了一条又添加了一条，最后更新完为：</font>

| IP        | MAC                         |
| --------- | --------------------------- |
| 10.10.1.1 | 60 : 00 : de : ad : be : ef |

<font size=4 face="楷体">	第三阶段，同样的，一个条目超时过期，所以被删除(kill 1 entries)；然后router收到了一个IP地址为10.10.5.5的host向自己发送的ARP Request，所以router会从eth1发送一个Reply，同时在收到ARP Request时将其中sender的IP与Mac地址记录到表中，故更新后表的内容为：</font>

| IP        | MAC                         |
| --------- | --------------------------- |
| 10.10.5.5 | 70 : 00 : ca : fe : c0 : de |

<font size=4 face="楷体">	`routertests1full.srpy`的testing结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\la3_7.png)

<font size=4 face="楷体">	(手动划线分割，绿线划分阶段，黄线划出每次更新的ARP表内容；更改条目过期时间为0.006s）从log的信息来看，本次测试包含了5个阶段的通信：
	第一阶段，IP地址为192.168.1.100的host向router发送了针对router的ARP Request，所以router会从eth0发送一个Reply，同时在收到ARP Request时将其中sender的IP与Mac地址记录到表中，更新后表内容为：</font>

| IP            | MAC                         |
| ------------- | --------------------------- |
| 192.168.1.100 | 30 : 00 : 00 : 00 : 00 : 01 |

<font size=4 face="楷体">	第二阶段，IP地址为172.16.42.2的host向router发送了针对router的ARP Request，所以router会从eth2发送一个Reply，同时在收到ARP Request时将其中sender的IP与Mac地址记录到表中，更新后表内容为：</font>

| IP            | MAC                         |
| ------------- | --------------------------- |
| 192.168.1.100 | 30 : 00 : 00 : 00 : 00 : 01 |
| 172.16.42.2   | 50 : 00 : 00 : 00 : 00 : 01 |

<font size=4 face="楷体">	第三阶段，第一个条目超时过期被删除，接着router收到了一个不是发给自己的Request，所以router会drop掉这个包，但在收到这个ARP Request时，router也要更新表的内容，所以cached ARP Table删了一条又添加了一条，最后更新完为：</font>

| IP          | MAC                         |
| ----------- | --------------------------- |
| 172.16.42.2 | 50 : 00 : 00 : 00 : 00 : 01 |
| 10.10.1.1   | 60 : 00 : de : ad : be : ef |

<font size=4 face="楷体">	第四阶段，router收到了一个不是发给自己的Request（目的IP为10.10.1.1），所以router会drop掉这个包，但在收到这个ARP Request时，router也要更新表的内容，所以cached ARP Table删了一条又添加了一条，最后更新完为：</font>

| IP          | MAC                         |
| ----------- | --------------------------- |
| 172.16.42.2 | 50 : 00 : 00 : 00 : 00 : 01 |
| 10.10.1.1   | 60 : 00 : de : ad : be : ef |
| 10.10.5.5   | 70 : 00 : ca : fe : c0 : de |

<font size=4 face="楷体">	第五阶段，表中的第一个条目超时过期被删除，接着IP地址为10.10.5.5的host向router发送了针对router的ARP Request（目的IP为10.10.1.1），所以router会从eth2发送一个Reply，同时在收到ARP Request时cached ARP Table要将其中sender的IP与Mac地址记录下来，但注意到之前第四阶段已经记录了这一个pair，所以只会更新这一条目的timestamp，更新后表内容为：</font>

| IP        | MAC                         |
| --------- | --------------------------- |
| 10.10.1.1 | 60 : 00 : de : ad : be : ef |
| 10.10.5.5 | 70 : 00 : ca : fe : c0 : de |

### <font face="楷体">5、核心代码</font>

​	<font size=4 face="楷体">	本实验中各阶段主要修改的代码在**实验内容**部分已经提到；在这里将最后实现的版本呈现一下：</font>

```python
class Router(object):
    def __init__(self, net):
        self.net = net
        # other initialization stuff here
        my_interfaces = net.interfaces() 
        self.myips = [intf.ipaddr for intf in my_interfaces]
        self.mydict = {}
        for intf in my_interfaces:
            self.mydict[intf.ipaddr] = intf.ethaddr
        self.mytable = {}
        self.to_be_delete = []
        self.flag = False


    def router_main(self):    
        '''
        Main method for router; we stay in a loop in this method, receiving
        packets until the end of time.
        '''
        while True:
            gotpkt = True
            try:
                timestamp,dev,pkt = self.net.recv_packet(timeout=1.0)
            except NoPackets:
                log_debug("No packets available in recv_packet")
                gotpkt = False
            except Shutdown:
                log_debug("Got shutdown signal")
                break

            if gotpkt:
                log_debug("Got a packet: {}".format(str(pkt)))
                now = time.time()
                for key in self.mytable.keys():
                    if now - self.mytable[key][1] > 0.006:
                        self.to_be_delete.append(key)
                l = len(self.to_be_delete)
                if l > 0:
                    self.flag =  True
                    log_info("kill {} entries".format(l))
                for kill in self.to_be_delete:
                    del self.mytable[kill]
                self.to_be_delete.clear()
                # 删除过期条目，根据更新情况给flag赋值
                
                arp = pkt.get_header(Arp)
                # 解析ARP header
                if arp is None:
                    continue
                    # 不为arp header，直接drop
                else:
                    ip = arp.senderprotoaddr
                    mac = arp.senderhwaddr
                    if (ip not in self.mytable.keys()) or (mac != self.mytable[ip][0]):
                        self.mytable[ip] = [mac,time.time()]
                        self.flag = True
                    else:
                        self.mytable[ip] = [mac,time.time()]    

                if arp.operation == ArpOperation.Reply:
                    log_info("receive ARP Reply packet {} from {}".format(str(pkt), dev))
                    if self.flag:
                        log_info("Update ARP Table as:")
                        for key in self.mytable.keys():
                            log_info("ip: {}    mac: {}".format(key,self.mytable[key][0]))
                    continue
                    # 更新cached ARP Table，根据更新情况给flag赋值
                    
                if arp.operation == ArpOperation.Request:
                    if arp.targetprotoaddr in self.myips:
                        log_info("receive ARP Request packet {} from {}".format(str(pkt), dev))
                        reply = create_ip_arp_reply(self.mydict[arp.targetprotoaddr], arp.senderhwaddr, arp.targetprotoaddr, arp.senderprotoaddr)
                        self.net.send_packet(dev, reply)
                        log_info("send ARP Reply packet {} to {}".format(str(reply), dev))
                # 对给自己的request作出回复
                    else:
                        log_info("receive ARP Request packet {} from {} , not for my interfaces".format(str(pkt), dev))
                # 否则drop
                if self.flag:
                    log_info("Update ARP Table as:")
                    for key in self.mytable.keys():
                        log_info("ip: {}    mac: {}".format(key,self.mytable[key][0]))
				# 表内容有更新，打印cached ARP Table
```



### <font face="楷体">6、总结与感想</font>

<font size=4 face="楷体">	本次实验让我初步了解了router在网络中对ARP Request的处理方式，也知道了手册与guide中提供的很多实用方便的API；同时，构建cached ARP Table与timeout Mechanism的实现也帮助我复习了lab_2中的知识内容。</font>