

# <font face="楷体">

# <center>南京大学本科生实验报告</center></font>

<font size=3 face="楷体">课程名称：**计算机网络**							任课教师：李文中
助教：陈衍庆、🕊、田冰川、吴昌容、段建辉、方毓楚、黄晓洁、刘柯鑫、李想</font>

| <font face="楷体">学院</font>  | <font face="楷体">计算机院</font> | <font face="楷体">专业（方向）</font>  | <font face="楷体">计算机科学与技术</font> |
| ------------------------------ | --------------------------------- | -------------------------------------- | ----------------------------------------- |
| <font face="楷体">学号</font>  | 181860092                         | <font face="楷体">姓名</font>          | <font face="楷体">王瀚霖</font>           |
| <font face="楷体">Email</font> | 603102048@qq.com                  | <font face="楷体">开始/完成日期</font> | 2020.4.8  /  2020.4.16                    |



### <font face="楷体">1、实验名称</font> : Forwarding packets

### <font face="楷体">2、实验目的</font>

<font size=4 face="楷体">	完成建立一个IPv4路由的第二阶段任务：为路由器建立转发表，同时通过**longest prefix match**找出转发表中与目的IP地址达到最长匹配的条目，并利用此条目记录的内容，借助ARP Cache或发送ARP Request得到需要的目的mac地址来完成对当前数据报的转发；</font>

### <font face="楷体">3、实验内容</font>：

### Task 1: Preparation

<font size=4 face="楷体">	实验要求：在switchyard下创建lab_4，在其中完成文件的复制与更名工作，做好实验准备；
	具体实现：略；</font>

### Task 2: IP Forwarding Table Lookup

<font size=4 face="楷体">	实验要求：
	1、修改文件`lab_4/myrouter.py`，在router初始化时利用`net.interfaces()`与`forwarding_table.txt`中的内容建立起router的转发表；
	2、修改文件`lab_4/myrouter.py`，实现当router收到一个需要转发的IP数据报时，利用**longest prefix match**方法查找转发表，正确地找到与IP数据报中目的IP相匹配的条目；
	思考与实现：
	1、首先明确路由器中转发表的作用：转发表中记录了每个子网的prefix、mask、需要当前路由器将此数据报转发到的IP地址next hop以及转发此数据报的port名字；prefix、mask与port很好理解，需要注意的是next hop：如果当前路由器的interfaces没有与目的子网直接相连，那么路由器只能将数据报转发给一个“中转站”，通过它来将数据报传送到目的地，这时next hop就标记了这个“中转站”的IP地址；如果当前路由器的interfaces与目的子网是直接相邻的，那么就不需要“中转站”，这时next hop也就失去了作用，因为这时路由器是知道了数据报中的目的IP地址所在子网与自己的某个interface直接相连，数据报的目的IP地址就相当于next hop；明白了这点，就容易理解`net.interfaces()`与`forwarding_table.txt`在建立转发表时各自的作用了：`net.interfaces()`中包含了本router所有interfaces的各种信息，它对转发表构建的作用就是，当数据报的目的IP地址所在子网是与router的interfaces直接相连时，利用此条目的信息从相应端口转发，转发的目的地就是数据报中的目的IP地址；`forwarding_table.txt`中更多用于目的IP地址所在子网不与router的interfaces直接相连时；这种情况下需要利用此条目的信息从相应端口转发，转发的目的地是条目中的next hop；
	经过以上分析与手册的说明，转发表的内容已经确定了：从`net.interfaces()`得到的条目应包含prefix、mask与port，next hop不需要可以填为None；从`forwarding_table.txt`得到的条目则应该包含prefix、mask、next hop与port；接下来需要考虑用什么数据结构组织这个转发表：因为查找转发表时还是需要遍历查找，所以可以直接用一个list包含所有的条目，每一个条目也组织为一个list，形成list中元素为list的结构来表示转发表；不过考虑到转发表是为去往**不同子网**的数据报指明下一跳的路径，所以还是使用了dict来体现这一点：dict的key为不同的子网（形式为prefix/mask，eg：`172.16.0.0/24`），value则是相应的mask等内容组成的list；具体实现如下：
	在lab3中`my_interfaces`记录了interfaces的所有信息，遍历`my_interfaces`，首先利用`.ipinterface.network`方法获得当前interface直接连接的子网作为key值，然后向value中添加当前interface的mask、next hop（None）、port名字：</font>

```python
		self.forwarding_table = {}
        for intf in my_interfaces:
            prefix = intf.ipinterface.network
            self.forwarding_table[prefix] = []
            self.forwarding_table[prefix].append(intf.netmask)
            self.forwarding_table[prefix].append(None)
            self.forwarding_table[prefix].append(intf.name)
```

<font size=4 face="楷体">	接下来读取`forwarding_table.txt`的内容，考虑到txt文件中的内容为4个一列，而用回车符划分内容又会有多余空格、多余回车的问题，所以直接用空格或回车划分，然后4个一循环读入记录：用i记录读到第几个内容，i除以4余数为0表示prefix，余数为1表示mask，余数为2表示next hop，余数为3表示port名字；利用prefix与mask来计算出子网作为key，然后添加value：（需要注意的就是添加时要与通过`net.interfaces()`得到的条目各部分内容类型要保持一致，详见注释）</font>

```python
		fo = open("forwarding_table.txt","r+")
        s = fo.read()
        txt = s.split()
        # 空格/回车划分
        i = 0
        for entry in txt:
            if (i%4 == 0):
                key = entry
                #prefix，先记录到key中，类型为str
            elif (i%4 == 1):
                key = IPv4Network(key + '/' + entry)
                #组合prefix与mask，形成'172.16.0.0/255.255.255.0'这样格式的str，然后利用IPv4Network得到相应的子网作为key
                self.forwarding_table[key] = []
                self.forwarding_table[key].append(ip_address(entry))
                #向value添加mask，类型为IPv4Address
            else:
                if (i%4 == 2):
                    self.forwarding_table[key].append(ip_address(entry))
                    #next hop要添加为IPv4Address类型
                else:
                    self.forwarding_table[key].append(entry)
                    #port名字存储为str类型
            i = i + 1
```

<font size=4 face="楷体">	通过以上操作，路由器的转发表就建立完成了。
	2、利用**longest prefix match**方法查找转发表，正确地找到与IP数据报中目的IP相匹配的条目：**longest prefix match**方法要求在所有可以匹配的条目中选择一条最长的匹配，所以首先要找到匹配的条目；匹配的方法也很简单，如果目的IP地址与mask按位与之后得到的结果与prefix相同则说明匹配，具体的实现方法手册中给出了</font><font size=4 face="楷体" color=red>（PS：手册中用prefix与dstip与的结果来与prefix比较似乎有点问题？应该是mask与dstip与？）</font><font size=4 face="楷体">；在匹配的前提下，利用`prefixlen`记录匹配的长度，然后选取出最大的一个作为最终的匹配项；考虑到可能没有可以与目的IP匹配的条目，引入一个flag作为是否匹配成功的标志；将上述思路整理为一个函数实现如下，其中ip就是收到的数据报的ipv4 header：</font>

```python
     def longest_match(self,ip):
        flag = False
        match_l = 0
        match = ip_address(0)
        #初始化，match_l记录最长匹配的长度，match记录最长匹配条目
        for entry in self.forwarding_table.keys():
        #遍历转发表
            if (int(self.forwarding_table[entry][0]) & int(ip.dst)) == int(IPv4Address(str(entry).split('/')[0])):
            #self.forwarding_table[entry][0]为mask，ip.dst为数据报的目的IP地址
            #str(entry).split('/')[0]即取子网的prefix部分，然后将其转换为IPv4Address类型，再			 #转换为int类型，来与mask & dstip的结果进行比较
                flag = True
                #匹配成功，有匹配项
                to_match_l = entry.prefixlen
                #记录匹配长度
                if to_match_l > match_l:
                    match = entry
                    match_l = to_match_l
				#取最长匹配
        return flag,match
```

<font size=4 face="楷体">	这样，如果调用此函数，返回flag为False则说明没有匹配项，否则，match就是最长匹配项条目的key值；
	这样，Task2的任务就完成了。</font>

<font size=4 face="楷体">	关于**Testing**与**Deploying**参见文档**实验结果**部分；</font>

### Task 3: Forwarding the Packet and ARP

<font size=4 face="楷体">	实验要求：修改文件`lab_4/myrouter.py`，进一步完善router转发包的功能，具体而言要实现：
	1、将IP报头中header的ttl减一，当前阶段默认ttl值总是为正；
	2、向next hop所指的host发送ARP Request以获得该host的Mac地址；如果没有得到ARP Reply，则每隔1s发送一次ARP Request，最多发送5次；5次Request之后还没有回复则丢弃数据报；之前因为没有得到ARP Reply而无法被转发的数据报应该先放到缓存区中；
	3、如果通过查ARP Cache或收到ARP信息得到了某个host的Mac地址，则将要转发到该host的所有数据报转发过去，这需要router将数据报的以太网header修改后从相应port转发出去；注意这些数据报应该按到达router的顺序依次转发；
	思考与实现：
	1、只需要在收到packet时，判断其为ipv4报文后将报头的ttl减一即可；
	2&3、根据手册提示，实现一个queue作为缓冲来存放未收到ARP Reply的ARP Request与待发送的数据报；最初我的想法是将ARP请求与待发送数据报分别存储，其中ARP部分存储了等待回复的ARP Request：根据FAQ1中的表述，同一时间针对某个目的IP地址发送的ARP请求只会有1个，在5次请求时间内新到来的、发向同一目的IP地址的数据报不会再发ARP请求；基于这些表述，利用dict实现对于ARP Request的管理更为方便：以请求的目的ip地址作为key值，value仍然是一个list，其内容依次为：请求发送的次数times、上一次发送ARP请求的时间time.time()、ARP请求包、发送端口port，即ARP请求的缓存结构为：</font>

| key   | value1 | value2      | value3        | value4 |
| ----- | ------ | ----------- | ------------- | ------ |
| dstip | times  | time.time() | ARP Request包 | port   |

<font size=4 face="楷体">	待发送数据报部分作为等候发送的数据报的缓存，没有一个适合作为key的属性，所以选择利用list来实现：该list中存储的每个元素都是一个list，每个list元素包含了这个数据报的转发信息，有：数据报的next hop地址（用于在收到一个ARP信息时判断此数据报能否被转发）、数据报pkt与要转发此数据报的port，即结构为：</font>

| next hop1 | ipv4包1 | 转发端口port1 |
| --------- | ------- | ------------- |
| next hop2 | ipv4包2 | 转发端口port2 |
| ……        | ……      | ……            |

<font size=4 face="楷体">	这样，ARP请求的处理与数据报的缓存就可以得到实现了：当需要发送一个ARP请求dict中没有的请求时，发送该请求并添加到ARP请求dict中，同时将处理的数据报加入到待发送数据报list中；在main的while循环中对ARP请求dict进行处理，判断其中是否有需要重新发送或发过5次需要删除的项；当router收到一个ARP回复或ARP请求信息，从中获得了某个IP地址的Mac地址时，查看这两个queue，将ARP请求dict中请求此IP地址的Mac地址的项删除，并将待发送数据报list中发往此IP地址的数据报进行发送；
	上述的实现方法没有问题，但再考虑一下，实际上两个queue存储的信息有冗余项：ARP请求dict中的dstip实际上就是待发送数据报list中元素的next hop，转发端口也是相同的（发送ARP请求的port、接收ARP Reply的port、转发数据报的port是同一个）；而且这样实现每次处理待发送数据报时都要遍历整个list，挑选出可发送的数据报进行转发、删除记录；如果将二者合并为1个数据结构，则从时间、空间上都得到了优化；合并的方法就是：在ARP请求dict中添加一个value5，这个value5是一个list，记录了所有发往当前key，即对应dstip的数据报，从而形成以下结构：</font>

| key   | value1 | value2      | value3        | value4 | value5         |
| ----- | ------ | ----------- | ------------- | ------ | -------------- |
| dstip | times  | time.time() | ARP Request包 | port   | [pkt1、pkt2……] |

<font size=4 face="楷体">	利用此结构进行实现，消除了记录的冗余项，而且转发数据报时，可以直接将对应value5的所有报依次发送，无需挑选，删除也可以直接由dict条目的删除直接实现；利用此结构进行实现的思路为：
	a、当需要发送一个dict中没有的请求时，发送该请求，并将处理的数据报作为新增条目的value5添加到dict中；
	b、在main的while循环中对dict进行处理，判断其中是否有需要重新发送或发过5次需要删除的项（这也意味着如果因发送5次没有Reply而删除某个条目时，之前所有等待发送到对应dstip的数据报都会被丢弃）；
	c、当router收到一个ARP回复或ARP请求信息，从中获得了某个IP地址的Mac地址时，查看dict，如果该IP地址在dict的keys()中，则说明该条目的请求信息已得到，故将该条目中待发送的数据报依次发送后删除该条目；下面依次介绍这3点的实现思路：
	a、将此功能封装为函数enqueue，需要传入的参数有：当前ARP请求对应的转发口port，用于之后发送ARP Request以及数据报的源Mac地址srchw、源IP地址srcip、目的IP地址dstip，等待转发的数据报pkt（不难看出srchw与srcip实际就是port的Mac地址与IP地址）；首先利用dstip判断一下对于该地址是否已经发送过ARP Request，如果是则不需要再次发送ARP请求了，只需要通过`queue[dstip][4].append(pkt)`将pkt添加到缓存就可以了；反之，如果之前没有发送过，就需要发送一次ARP Request并且添加到queue中；发送ARP Request可通过`request = create_ip_arp_request(srchw, srcip, dstip)`得到请求包后转发；添加到queue则是通过`queue[dstip] = [1,time.time(),request,port,[pkt]]`实现（第一次发送，故times为1）；
	b、将此功能封装为函数handle_arq_request，由于所有的相关信息都已经记录在queue里了，所以只需要遍历queue，找到时间差大于1s的条目；如果找到的条目times为5，则说明已经发送了5次请求，而且在第5次请求过后1s未收到回复，所以要删除此条目；如果条目的times小于5，则根据条目中的信息再次发送ARP Request，同时将条目中times加一，time.time更新为当前时间；
	c、将此功能封装为函数dequeue，需要传入的参数有：从ARP信息中得到的IP地址ip与它的Mac地址mac（即ARP包中源的ip与mac地址）；如果queue的keys()中包含ip，则说明该条目所需要的Mac信息已经收到，就是mac，所以需要依次将条目中value5里记录的数据报发送出去；发送的具体过程需要将每个pkt中以太网层的src更改为转发端口的Mac地址，dst改为目的IP的Mac地址；后者容易实现，目的IP的Mac地址就是传入的mac，而前者要找到还需要通过条目中记录的信息去查找，不太方便；而实际上一个数据报在进入queue时，就已经在匹配转发表时确定了自己的转发端口，也就确定了自己被转发时的src Mac，所以pkt中以太网层src的更改放在enqueue中实现，每次enqueue都要执行`pkt[Ethernet].src = srchw`来提前填好转发时数据报的src Mac；</font>



<font size=4 face="楷体">	至此，实现思路叙述完毕，具体代码实现为：
	首先，在`__init__`中声明一个dict作为queue，为了突出其作用，将其封装为一个类：</font>

```python
class QUEUE(object):
    def __init__(self):
        self.queue = {}

class Router(object):
    def __init__(self, net):
        ……
        self.myqueue = QUEUE()
```

<font size=4 face="楷体">	接着是三个函数的实现：</font>

```python
    def enqueue(self,port,srchw,srcip,dstip,pkt):
    #用于处理思路中的a情况
        pkt[Ethernet].src = srchw
        #先提前填好转发时的src mac
        if dstip in self.myqueue.queue.keys():
            #已在queue中，不必再次发送ARP Request，只需将pkt添加到相应待发送数据报list中
            self.myqueue.queue[dstip][4].append(pkt)
        else:
            #之前未发送过此ARP Request，需要发送并添加到queue中
            request = create_ip_arp_request(srchw, srcip, dstip)
            self.net.send_packet(port, request)
            #发送请求
            self.myqueue.queue[dstip] = [1,time.time(),request,port,[pkt]]
            #进行添加，刚添加时只发送了一次，待发送的数据报也只有pkt一个
```

```python
    def handle_arq_request(self):
    #用于处理思路中的b情况
        now = time.time() #获取当前时间
        to_be_kill = [] #记录待删除条目
        for keys_ in self.myqueue.queue.keys():
            if now - self.myqueue.queue[keys_][1] > 1:
                #某条目距离上次发送请求超过了1s
                if self.myqueue.queue[keys_][0] == 5:
                    to_be_kill.append(keys_)
                    #已经发送了5次请求，且没有一次请求收到了回复，需要删除
                else:
                    self.net.send_packet(self.myqueue.queue[keys_][3],self.myqueue.queue[keys_][2]) #参数分别为port和request包
                    self.myqueue.queue[keys_][1] = time.time()
                    self.myqueue.queue[keys_][0] = self.myqueue.queue[keys_][0] + 1   
                    #否则再次发送请求并更新条目
                    #需要注意要先判断是否已发送了5次后次数再增加，不可以在次数增加后立刻判断，否则					会导致即使最后一次请求得到了回复也没用了
        for k in to_be_kill:
            del self.myqueue.queue[k]
            #删除
```

```python
    def dequeue(self,ip,mac):
    #用于处理思路中的c情况
        if ip in self.myqueue.queue.keys():
        #ip与mac是某条目需要的信息
            for i in range(len(self.myqueue.queue[ip][4])):
                mypacket = self.myqueue.queue[ip][4][i]
                mypacket[Ethernet].dst = mac
                self.net.send_packet(self.myqueue.queue[ip][3],mypacket)
                #依次转发此条目中等待转发的数据报，转发前每个数据报的src mac与dst mac都得到了更新

            del self.myqueue.queue[ip]
            # 所有数据报转发完毕，可以删除
```



<font size=4 face="楷体">	有了以上函数，整个router逻辑的实现就比较容易了；在实现之前首先明确一下已有的“工具”：
	lab3中实现的self.myips（记录router所有端口的IP地址）、self.mydict（记录router每个端口IP地址与Mac地址的对应关系）、self.mytable（ARP Cache）、self.flag（判断ARP Cache是否有更新）、lab4中的转发表self.forwarding_table、缓存队列self.myqueue与4个函数（longest_match、enqueue、handle_arq_request与dequeue）；考虑到在lab4中port的IP地址、Mac地址会在enqueue时使用到（作为srcip与srchw），而已知的就是port的名字，所以为了方便查询，构建self.mydict2描述这种对应关系：</font>

```python
        self.mydict2 = {}
        for intf in my_interfaces:
            self.mydict2[intf.name] = [intf.ethaddr,intf.ipaddr]
```

<font size=4 face="楷体">	同时，在lab3中实现的ARP Cache中的timeout机制在lab4中不予考虑，所以这里将这一内容封装为函数arp_table_timeout，再将过期时长设置为100s来使得这一内容实际上不会被执行到：</font>

```python
    def arp_table_timeout(self):
        now = time.time()
        to_be_delete = []
        for key in self.mytable.keys():
            if now - self.mytable[key][1] > 100:
                to_be_delete.append(key)
        l = len(to_be_delete)
        if l > 0:
            self.flag =  True
            log_info("kill {} entries".format(l))
        for kill in to_be_delete:
            del self.mytable[kill]
```

<font size=4 face="楷体">	有了这些基础，进入`router_main`的实现：
	函数`arp_table_timeout`与`handle_arq_request`不是只在接收到包时才会执行，所以要将它们放在while循环里执行；剩余的处理就是对于收到的Ipv4数据报与ARP包进行操作了：首先提取ARP header，如果为None则进一步提取ipv4 header，提取到则是ipv4数据报；若ARP header不为None则为ARP信息；否则既不是Ipv4数据报也不是ARP包，直接丢弃；对于ARP包，处理在lab3中基本都已实现，只需要在每收到一个ARP包时调用`dequeue`来发送数据报就好；对于ipv4数据报，先将其header的ttl减一，然后判断：目的IP地址为router本身的报要丢弃；如果不是，调用`longest_match`寻找最长匹配项，若得到flag为0，说明没有匹配项，则丢弃；否则根据匹配到的转发条目选取next hop，然后尝试转发：如果查询ARP Cache发现有next hop对应host的Mac信息，则直接转发，如果没有，则调用`enqueue`来缓存，等待后续转发。具体实现代码可参见文档**核心代码部分**。</font>

<font size=4 face="楷体">	关于**Testing**与**Deploying**参见文档**实验结果**部分；</font>

### <font face="楷体">4、实验结果</font>

#### Task1&Task2

<font size=4 face="楷体">	略。</font>

#### Task3

<font size=4 face="楷体">	`routertests2.srpy`的testing结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab4_1.png)

![](C:\Users\njuwhl2019hp\Desktop\计网\lab4_2.png)

<font size=4 face="楷体">	在给出的测试用例中，1——8是两次“router收到数据报，发送ARP Request并立刻收到回复，然后将数据报进行转发”的过程；9——12是两次“router收到数据报，查询ARP Cache后得到目的地址信息，直接转发”的过程；13——18是一次“router收到发往172.16.64.35的数据报，匹配转发条目得知next hop为10.10.1.254，然后向它发送了2次ARP Request才收到回复，然后完成转发”的过程；19是未与转发表条目完成匹配的数据报被直接丢弃的过程；20——31则是一次发送了5次ARP Request都未得到回复的过程。
	从lab4要实现的功能来看，给出的测试用例测试了对ARP Request缓存队列的管理以及最简单的查询转发表然后转发的操作；没有对有多个数据报可以发送时按序转发的测试；而在deploying中也无法做到暂时没有ARP Reply，所以，结合代码的实现思路，添加以下测试（`routertests.py`中定义）：</font>

<font size=4 face="楷体">	首先，router会收到5个目的IP地址为10.10.0.2的数据报，但没有收到任何ARP Reply；按照实验要求，router只应该在收到第一个数据报时发送一个ARP Request，之后的4个都只是依次进入缓存队列等待：</font>

```python
    pkt = mk_ping("11:00:00:00:00:01","10:00:00:00:00:01","192.168.1.2","10.10.0.2")
    s.expect(PacketInputEvent("router-eth0", pkt, display=IPv4),
        "IP packet to be forwarded to 10.10.0.2 should arrive on router-eth0.")
    otroarp = mk_arpreq("10:00:00:00:00:02", "10.10.0.1", "10.10.0.2")
    s.expect(PacketOutputEvent("router-eth1",otroarp,display=Arp),
        "Router should send ARP request for 10.10.0.2 out router-eth1 interface.")
    # 第一个数据报，发送ARP Request
    pkt = mk_ping("11:00:00:00:00:02","10:00:00:00:00:03", "172.16.42.2","10.10.0.2")
    s.expect(PacketInputEvent("router-eth0", pkt, display=IPv4),
        "IP packet to be forwarded to 10.10.0.2 should arrive on router-eth2, but the arp request already exits, so just enqueue this packet.")
    # 第二个数据报
    pkt = mk_ping("11:00:00:00:00:03","10:00:00:00:00:03", "172.16.42.3","10.10.0.2")
    s.expect(PacketInputEvent("router-eth0", pkt, display=IPv4),
        "IP packet to be forwarded to 10.10.0.2 should arrive on router-eth2, but the arp request already exits, so just enqueue this packet.")
    # 第三个数据报
    pkt = mk_ping("11:00:00:00:00:04","10:00:00:00:00:03", "172.16.42.4","10.10.0.2")
    s.expect(PacketInputEvent("router-eth0", pkt, display=IPv4),
        "IP packet to be forwarded to 10.10.0.2 should arrive on router-eth2, but the arp request already exits, so just enqueue this packet.")
    # 第四个数据报
    pkt = mk_ping("11:00:00:00:00:05","10:00:00:00:00:03", "172.16.42.5","10.10.0.2")
    s.expect(PacketInputEvent("router-eth0", pkt, display=IPv4),
        "IP packet to be forwarded to 10.10.0.2 should arrive on router-eth2, but the arp request already exits, so just enqueue this packet.")
    # 第五个数据报
```

<font size=4 face="楷体">	接下来，router会收到一个发往192.168.1.2的数据报，router应该发送相应的ARP请求并缓存：</font>

```python
    pkt = mk_ping("11:00:00:00:00:06","10:00:00:00:00:03", "172.16.42.6","192.168.1.2")
    s.expect(PacketInputEvent("router-eth0", pkt, display=IPv4),
        "IP packet to be forwarded to 192.168.1.2 should arrive on router-eth2.")
    otroarp = mk_arpreq("10:00:00:00:00:01", "192.168.1.1", "192.168.1.2")
    s.expect(PacketOutputEvent("router-eth0",otroarp,display=Arp),
        "Router should send ARP request for 192.168.1.2 out router-eth0 interface.")
```

<font size=4 face="楷体">	接着是对待发送数据报**按序转发**的测试：router会收到IP地址为10.10.0.2的host的ARP Reply，则按照实验要求，router应该按照之前数据报的到达顺序按序转发：</font>

```python
    # start to send 5 enqueued pkt to 10.10.0.2
    otroarp = mk_arpreq("10:00:00:00:00:02", "10.10.0.1", "10.10.0.2")   
    otroarpresponse = mk_arpresp(otroarp, "11:22:33:44:55:66")
    s.expect(PacketInputEvent("router-eth1", otroarpresponse, display=Arp),
        "Router should receive an ARP response for 10.10.0.2 on router-eth1.")
    #收到ARP Reply
    pkt = mk_ping("10:00:00:00:00:02","11:22:33:44:55:66",
                  "192.168.1.2","10.10.0.2",ttl=63) 
    s.expect(PacketOutputEvent("router-eth1",pkt,display=IPv4),
        "1st enqueued IP packet should be forwarded to 10.10.0.2 out router-eth1.")
    
    pkt = mk_ping("10:00:00:00:00:02","11:22:33:44:55:66",
                  "172.16.42.2","10.10.0.2",ttl=63)
    s.expect(PacketOutputEvent("router-eth1",pkt,display=IPv4),
        "2nd enqueued IP packet should be forwarded to 10.10.0.2 out router-eth1.")
    
    pkt = mk_ping("10:00:00:00:00:02","11:22:33:44:55:66",
                  "172.16.42.3","10.10.0.2",ttl=63)
    s.expect(PacketOutputEvent("router-eth1",pkt,display=IPv4),
        "3rd enqueued IP packet should be forwarded to 10.10.0.2 out router-eth1.")
    
    pkt = mk_ping("10:00:00:00:00:02","11:22:33:44:55:66",
                  "172.16.42.4","10.10.0.2",ttl=63)
    s.expect(PacketOutputEvent("router-eth1",pkt,display=IPv4),
        "4th enqueued IP packet should be forwarded to 10.10.0.2 out router-eth1.")
    
    pkt = mk_ping("10:00:00:00:00:02","11:22:33:44:55:66",
                  "172.16.42.5","10.10.0.2",ttl=63)
    s.expect(PacketOutputEvent("router-eth1",pkt,display=IPv4),
        "5th enqueued IP packet should be forwarded to 10.10.0.2 out router-eth1.")
```

<font size=4 face="楷体">	至此，缓存队列中还有一个发往192.168.1.2的数据报在等待；接下来的测试用例起初是为了测试：当缓存队列中有ARP请求需要被再次发送，同时又有一个新的ARP请求产生，需要发送，代码实现的是先发送新的ARP请求，接着再次发送已有的ARP请求（即代码中`self.handle_arq_request() `在循环最后被调用）；但是实际写测试用例时这两个事件很难通过`PacketInputTimeoutEvent`一段时间使得二者撞在同一时间发生，所以接下来的测试只是router又接收到了一个新的数据报，发送ARP Request后等待1s，使得两个ARP请求第二次被发送的测试：</font>

```python
    # now there's still an arp request waiting for reply

    pkt = mk_ping("11:00:00:00:00:07","10:00:00:00:00:03", "172.16.42.7","10.10.0.3")
    s.expect(PacketInputEvent("router-eth0", pkt, display=IPv4),
        "IP packet to be forwarded to 10.10.0.3 should arrive on router-eth2.")
    # 收到新的数据报
    # send current request first
    otroarp = mk_arpreq("10:00:00:00:00:02", "10.10.0.1", "10.10.0.3")
    s.expect(PacketOutputEvent("router-eth1",otroarp,display=Arp),
        "Router should send ARP request for 10.10.0.3 out router-eth1 interface before send the arp request that timeout again.")
    
    # wait for 1 second
    s.expect(PacketInputTimeoutEvent(1.0),
            "Application should try to receive a packet, but then timeout.")
    
    # 2 timeout entries send arp request again
    otroarp = mk_arpreq("10:00:00:00:00:01", "192.168.1.1", "192.168.1.2")
    s.expect(PacketOutputEvent("router-eth0",otroarp,display=Arp),
        "Router should send ARP request for 192.168.1.2 again.")
    otroarp = mk_arpreq("10:00:00:00:00:02", "10.10.0.1", "10.10.0.3")
    s.expect(PacketOutputEvent("router-eth1",otroarp,display=Arp),
        "Router should send ARP request for 10.10.0.3 again.")
```

<font size=4 face="楷体">	然后，router收到了来自192.168.1.2的ARP Request，在我的实现中这也会调用dequeue进行数据报转发（即使不是Reply），所以router应该将对应数据报转发,并且将缓存队列中相应的条目删除，测试如下：</font>

```python
    # an arp request from 192.168.1.2 arrive
    otroarp = mk_arpreq("10:00:00:00:00:05", "192.168.1.2", "10.10.0.5")
    s.expect(PacketInputEvent("router-eth0", otroarp, display=Arp),
        "IP packet to be forwarded to 10.10.0.5 should arrive on router-eth0.")
    
    # arp table update
    pkt = mk_ping("10:00:00:00:00:01","10:00:00:00:00:05", "172.16.42.6","192.168.1.2",ttl=63)
    s.expect(PacketOutputEvent("router-eth0",pkt,display=IPv4),
        "The queued packet can be forward and the corresponding arp request should be killed.")
    # 相应缓存条目中的数据报可以转发，转发后删除该条目

    # prove the corresponding arp request has been killed
    # wait for 1 second
    s.expect(PacketInputTimeoutEvent(1.0),
            "Application should try to receive a packet, but then timeout.")
    
    #only 1 timeout entry send arp request again
    otroarp = mk_arpreq("10:00:00:00:00:02", "10.10.0.1", "10.10.0.3")
    s.expect(PacketOutputEvent("router-eth1",otroarp,display=Arp),
        "Router should send ARP request for 10.10.0.3 again.")
    # 只有发往10.10.0.3的数据报再次发送ARP 请求，说明192.168.1.2的条目确实被删除了
```

<font size=4 face="楷体">	最后，发往10.10.0.3的数据报在第5次发送ARP请求后收到了回复，也可以成功转发：</font>

```python
    # reply reaches at 5th sending
    s.expect(PacketInputTimeoutEvent(1.0),
            "Application should try to receive a packet, but then timeout.")
    otroarp = mk_arpreq("10:00:00:00:00:02", "10.10.0.1", "10.10.0.3")
    s.expect(PacketOutputEvent("router-eth1",otroarp,display=Arp),
        "Router should send ARP request for 10.10.0.3 again.")
    #第4次请求
    
    s.expect(PacketInputTimeoutEvent(1.0),
            "Application should try to receive a packet, but then timeout.")
    otroarp = mk_arpreq("10:00:00:00:00:02", "10.10.0.1", "10.10.0.3")
    s.expect(PacketOutputEvent("router-eth1",otroarp,display=Arp),
        "Router should send ARP request for 10.10.0.3 again.")
	#第5次请求
    
    otroarpresponse = mk_arpresp(otroarp, "11:22:33:44:55:67")
    s.expect(PacketInputEvent("router-eth1", otroarpresponse, display=Arp),
        "Router should receive an ARP response for 10.10.0.3 on router-eth1.")
    #第5次请求后得到了回复
    
    pkt = mk_ping("10:00:00:00:00:02","11:22:33:44:55:67",
                  "172.16.42.7","10.10.0.3",ttl=63) 
    s.expect(PacketOutputEvent("router-eth1",pkt,display=IPv4),
        "IP packet should be forwarded to 10.10.0.3 out router-eth1.")
    # 成功转发
```

<font size=4 face="楷体">	`routertests.py`的testing结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab4_3.png)

![](C:\Users\njuwhl2019hp\Desktop\计网\lab4_4.png)

<font size=4 face="楷体">	由此证明了实现的正确性。</font>

<font size=4 face="楷体">	`routertests2_template.py`的testing结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab4_7.png)

<font size=4 face="楷体">	**deploying**过程如下：</font>

<font size=4 face="楷体">	deploying模拟的过程就是server1向server2 ping两次的过程，在这个过程中router接收、转发信息的端口为eth0与eth1，下图是分别从eth0与eth1捕捉到的此过程中的数据包：
	eth0：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab4_5.png)

<font size=4 face="楷体">	eth1：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab4_6.png)

<font size=4 face="楷体">	根据以上捕捉到的信息，可以看出通信的整个流程是（根据时间戳大致整理）：首先，server1向router的eth0发送一个ARP Request来请求eth0的Mac地址，router进行相应的答复；接着server1从eth0向router发送ICMP请求，router收到后正确判断出next hop为192.168.200.1，且应从eth1转发，故router从eth1发送ARP Request来请求192.168.200.1的Mac地址，得到Reply后，将数据报从eth1转发，从而完成了第一次ping的request转发；接着server2对ping request进行回复，由于此通信过程中涉及到的Mac信息已经记录下来，所以无必要进行ARP Request；故之后的通信就是ping reply从server2发往router，router再转发给server1；接着又是一次ping request与ping reply通信，最后server2又发送ARP Request对eth1的Mac地址进行了确定。整个处理过程中router工作正常，转发正确。</font>

### <font face="楷体">5、核心代码</font>

​	<font size=4 face="楷体">	本实验中各阶段主要修改的代码在**实验内容**部分已经提到；在这里将最后实现的版本呈现一下：</font>

<font size=4 face="楷体">	初始化：</font>

```python
class QUEUE(object):
    def __init__(self):
        self.queue = {}

class Router(object):
    def __init__(self, net):
        self.net = net
        # other initialization stuff here
        my_interfaces = net.interfaces() 
        self.myips = [intf.ipaddr for intf in my_interfaces] #记录router所有端口ip
        self.mydict = {}
        for intf in my_interfaces:
            self.mydict[intf.ipaddr] = intf.ethaddr #记录各端口ip与mac对应关系
        self.mydict2 = {}
        for intf in my_interfaces:
            self.mydict2[intf.name] = [intf.ethaddr,intf.ipaddr] 
            #记录各端口名字与ip、mac对应关系
        self.mytable = {} #ARP Cache
        self.flag = False
        self.forwarding_table = {} #转发表
        for intf in my_interfaces:
            prefix = intf.ipinterface.network
            self.forwarding_table[prefix] = []
            self.forwarding_table[prefix].append(intf.netmask)
            self.forwarding_table[prefix].append(None)
            self.forwarding_table[prefix].append(intf.name)
        fo = open("forwarding_table.txt","r+")
        s = fo.read()
        txt = s.split()
        i = 0
        for entry in txt:
            if (i%4 == 0):
                key = entry
            elif (i%4 == 1):
                key = IPv4Network(key + '/' + entry)
                self.forwarding_table[key] = []
                self.forwarding_table[key].append(ip_address(entry))
            else:
                if (i%4 == 2):
                    self.forwarding_table[key].append(ip_address(entry))
                else:
                    self.forwarding_table[key].append(entry)
            i = i + 1
        self.myqueue = QUEUE() #缓存队列
```

<font size=4 face="楷体">	函数封装：</font>

```python
    def handle_arq_request(self):
        now = time.time()
        to_be_kill = []
        for keys_ in self.myqueue.queue.keys():
            if now - self.myqueue.queue[keys_][1] > 1:
                if self.myqueue.queue[keys_][0] == 5:
                    to_be_kill.append(keys_)
                else:
                    self.net.send_packet(self.myqueue.queue[keys_][3],self.myqueue.queue[keys_][2])
                    self.myqueue.queue[keys_][1] = time.time()
                    self.myqueue.queue[keys_][0] = self.myqueue.queue[keys_][0] + 1   
                           
        for k in to_be_kill:
            del self.myqueue.queue[k]

    def arp_table_timeout(self):
        now = time.time()
        to_be_delete = []
        for key in self.mytable.keys():
            if now - self.mytable[key][1] > 100:
                to_be_delete.append(key)
        l = len(to_be_delete)
        if l > 0:
            self.flag =  True
            log_info("kill {} entries".format(l))
        for kill in to_be_delete:
            del self.mytable[kill]

    def longest_match(self,ip):
        flag = False
        match_l = 0
        match = ip_address(0)
        for entry in self.forwarding_table.keys():
            if (int(self.forwarding_table[entry][0]) & int(ip.dst)) == int(IPv4Address(str(entry).split('/')[0])):
                flag = True
                to_match_l = entry.prefixlen
                if to_match_l > match_l:
                    match = entry
                    match_l = to_match_l

        return flag,match

    def dequeue(self,ip,mac):
        if ip in self.myqueue.queue.keys():
            for i in range(len(self.myqueue.queue[ip][4])):
                mypacket = self.myqueue.queue[ip][4][i]
                mypacket[Ethernet].dst = mac
                self.net.send_packet(self.myqueue.queue[ip][3],mypacket)

            del self.myqueue.queue[ip]
        
    def enqueue(self,port,srchw,srcip,dstip,pkt):
        pkt[Ethernet].src = srchw
        if dstip in self.myqueue.queue.keys():
            self.myqueue.queue[dstip][4].append(pkt)
        else:
            request = create_ip_arp_request(srchw, srcip, dstip)
            self.net.send_packet(port, request)          
            self.myqueue.queue[dstip] = [1,time.time(),request,port,[pkt]]
```

<font size=4 face="楷体">	主函数流程：</font>

```python
    def router_main(self):    
        '''
        Main method for router; we stay in a loop in this method, receiving
        packets until the end of time.
        '''
        while True:
            gotpkt = True
            self.arp_table_timeout() 
            #实现ARP Cache timeout机制（实际上不会删除任何内容）
            try:
                timestamp,dev,pkt = self.net.recv_packet(timeout=1.0)
            except NoPackets:
                log_debug("No packets available in recv_packet")
                gotpkt = False
            except Shutdown:
                log_debug("Got shutdown signal")
                break

            if gotpkt:
                
                arp = pkt.get_header(Arp) #提取ARP header
                if arp is None:
                    ip = pkt.get_header(IPv4) #提取IPv4 header
                    if ip is None:
                        pass # 不是IPv4数据报，丢弃
                    else:
                        pkt[IPv4].ttl = pkt[IPv4].ttl - 1 #ttl减一
                        if ip.dst in self.myips:
                            pass
                        	# 目的IP为router，丢弃
                        else:
                            flag,match = self.longest_match(ip)
                            # 调用longest_match在转发表中进行匹配
                            if flag == False:
                                pass
                            	# 无匹配项，丢弃
                            else:
                                # 匹配到match条目
                                if self.forwarding_table[match][1] is None:
                                    next_hop = ip.dst
                                    # 匹配到interfaces建立的条目，即目的IP所在子网与router直										接相连，这种情况下next hop就是最终的目的地址，即ip.dst
                                else:
                                    next_hop = self.forwarding_table[match][1]
                                    # 否则next hop要从对应条目中获取

                                if next_hop in self.mytable.keys():
                                # ARP Cache已经有目的地址的记录，可以直接转发
                                    pkt[Ethernet].src = self.mydict2[self.forwarding_table[match][2]][0]
                                    # 利用self.mydict2获取转发port的Mac地址
                                    pkt[Ethernet].dst = self.mytable[next_hop][0]
                                    # 更改数据报src mac与dst mac后进行转发
                                    self.net.send_packet(self.forwarding_table[match][2], pkt)
                                else:
                                # 否则需要enqueue
                                    self.enqueue(self.forwarding_table[match][2],self.mydict2[self.forwarding_table[match][2]][0], self.mydict2[self.forwarding_table[match][2]][1], next_hop,pkt)                       
# enqueue的参数依次为port、port的Mac地址、port的IP地址、目的IP地址与待发送的数据报                           
                                       
                else:
            	# 为ARP信息
                    ip = arp.senderprotoaddr
                    mac = arp.senderhwaddr
                    # 获取发送ARP信息方的IP地址与Mac地址
                    if (ip not in self.mytable.keys()) or (mac != self.mytable[ip][0]):
                        self.mytable[ip] = [mac,time.time()]
                        self.flag = True
                    else:
                        self.mytable[ip] = [mac,time.time()]    
                    self.dequeue(ip,mac)
					# 更新ARP Cache，同时利用得到的ip与mac信息执行dequeue操作
                    if arp.operation == ArpOperation.Reply:
                        log_info("receive ARP Reply packet {} from {}".format(str(pkt), dev))
                                              
                        if self.flag:
                            log_info("Update ARP Table as:")
                            for key in self.mytable.keys():
                                log_info("ip: {}    mac: {}".format(key,self.mytable[key][0]))
                        continue
                    if arp.operation == ArpOperation.Request:
                        if arp.targetprotoaddr in self.myips:
                            log_info("receive ARP Request packet {} from {}".format(str(pkt), dev))
                            reply = create_ip_arp_reply(self.mydict[arp.targetprotoaddr], arp.senderhwaddr, arp.targetprotoaddr, arp.senderprotoaddr)
                            self.net.send_packet(dev, reply)
                            log_info("send ARP Reply packet {} to {}".format(str(reply), dev))
                            
                        else:
                            log_info("receive ARP Request packet {} from {} , not for my interfaces".format(str(pkt), dev))
                    if self.flag:
                        log_info("Update ARP Table as:")
                        for key in self.mytable.keys():
                            log_info("ip: {}    mac: {}".format(key,self.mytable[key][0]))
            self.handle_arq_request() 
            # 循环最后处理缓存队列的ARP请求信息
```



### <font face="楷体">6、总结与感想</font>

<font size=4 face="楷体">	本次实验让我进一步了解了router在网络中对ARP信息与IPv4数据报的处理方式，也对router大致的工作流程有了基本的了解。
	**实验遇到的问题与思考**：
	1、关于缓存队列利用dict实现，选择什么作为key值的问题：最开始的实现中，我是利用prefix作为key值的，即我认为转发表中不应该有两个prefix相同的条目；一开始这样实现可以通过测试用例，没有问题；第一次对这样处理产生疑问是在运行start_mininet.py时，发现通过interfaces方法与阅读forwarding_table.txt都会得到192.168.200.0这一prefix，但它们的mask不一样；那么对于这两个信息，是否需要分开存储？对于start_mininet.py中的情况，两个信息对应的转发端口是一样的，所以处理为同一条目不影响；但会不会对应不同的port呢？在搜索了一些信息之后，没有找到将这样的信息分开处理的例子；都是认为prefix相同的子网A,B（假设A的int(mask) > B的int(mask)），A应该是从B中划分出来的子网；假设转发表中有a.0.0.0/16，a.1.0.0/16，a.2.0.0/16……这些子网，那么a.0.0.0/24应该是a.0.0.0/16的一部分，port也不应该不同；但之后写计网作业P17时，与同学讨论得知似乎从一个大子网中划分出一部分分给另一个port也可以（就像题目中说的a.b.c.d/x – e.f.g.h/y这样的表示方法所暗示的）；所以最后为了保证实现的正确性，还是用a.b.c.d/x这样的子网表示来作为dict的key值；这样对于上述情况，如果对应port相同则没有影响，如果不一样则longest prefix match方法也会返回int(mask)值较大，即规模较小子网的条目，这也是正确的实现；
	2、关于longest prefix match的实现：这个问题是与1相联系的；一开始如果key只取prefix的话，如果有匹配项，那么利用`int(prefix) ^ int(ip.dst)`，也就是prefix与目的IP地址异或，得到的值越小，就说明目的IP地址与prefix从开头匹配的长度最长（一开始是利用异或后取反，取最大作为最长匹配项实现，但python的按位取反实际上就是取-(x+1)，所以取最大不可取，直接取最小就好了），故利用异或实现可以找到最长匹配项；具体实现如下：</font>

```python
    def longest_match(self,ip):
        flag = False
        match_l = 4294967295 #定义32位均为1的数作为一个极大值
        match = ip_address(0)
        for entry in self.forwarding_table.keys():
            if (int(self.forwarding_table[entry][0]) & int(ip.dst)) == int(entry):
                flag = True
                to_match_l = int(entry) ^ int(ip.dst)
                if to_match_l < match_l:
                    match = entry
                    match_l = to_match_l
```

<font size=4 face="楷体">	之后更改key为子网后，就存在`int(entry) ^ int(ip.dst)`得到相同值的可能性（例如192.168.200.0/24与192.168.200.0/30在匹配192.168.200.0时得到结果一样）；这时还需要进一步判断取出规模较小的子网来匹配，故实现为：</font>

```python
    def longest_match(self,ip):
        flag = False
        match_l = 4294967295
        match = ip_address(0)
        for entry in self.forwarding_table.keys():
            if (int(self.forwarding_table[entry][0]) & int(ip.dst)) == int(IPv4Address(str(entry).split('/')[0])):
                flag = True
                to_match_l = int(IPv4Address(str(entry).split('/')[0])) ^ int(ip.dst)
                if to_match_l < match_l:
                    match = entry
                    match_l = to_match_l
                elif to_match_l == match_l:
                # 等于时，判断mask，int(mask)较大则说明子网较小，应该匹配
                    if int(self.forwarding_table[entry][0]) > int(self.forwarding_table[match][0]):
                        match = entry
```

<font size=4 face="楷体">	这样实现之后，反过来看，实现较为麻烦……还不如直接取prefixlen来作为匹配长度，故最终更改为提交版本的实现。</font>