

# <font face="楷体">

# <center>南京大学本科生实验报告</center></font>

<font size=3 face="楷体">课程名称：**计算机网络**							任课教师：李文中
助教：陈衍庆、🕊、田冰川、吴昌容、段建辉、方毓楚、黄晓洁、刘柯鑫、李想</font>

| <font face="楷体">学院</font>  | <font face="楷体">计算机院</font> | <font face="楷体">专业（方向）</font>  | <font face="楷体">计算机科学与技术</font> |
| ------------------------------ | --------------------------------- | -------------------------------------- | ----------------------------------------- |
| <font face="楷体">学号</font>  | 181860092                         | <font face="楷体">姓名</font>          | <font face="楷体">王瀚霖</font>           |
| <font face="楷体">Email</font> | 603102048@qq.com                  | <font face="楷体">开始/完成日期</font> | 2020.5.8  /  2020.5.24                    |



### <font face="楷体">1、实验名称</font> : Reliable Communication

### <font face="楷体">2、实验目的</font>

<font size=4 face="楷体">	在IP层实现尽力而为服务的基础上，利用信息ack机制、滑动窗口机制与超时重传机制来实现一个更加安全可靠的信息传输功能。</font>

### <font face="楷体">3、实验内容</font>：

### Task 1: Preparation

<font size=4 face="楷体">	实验要求：在switchyard下创建lab_6，在其中完成文件的复制与更名工作，做好实验准备；
	具体实现：略；</font>

### Task 2: Middlebox

<font size=4 face="楷体">	实验要求：
	修改文件`lab_6/middlebox.py`，实现一个简单的路由器middlebox；该路由器的功能是将从一个端口收到的数据包从另一个端口转发；同时还要有一定的几率将blaster传送给blastee的数据包丢弃；
	思考与实现：
	middlebox的功能十分简单，所以实现起来也很容易：首先，middlebox要完成端口间数据的传输，这里只需要修改数据包的以太网包头，其中middlebox的两个端口的Mac地址可以由`mymacs`得到，blaster与blastee端口的Mac地址利用硬编码实现即可；其次，middlebox要以一定的几率将blaster发送给blastee的数据包丢弃，丢包率通过**middlebox_params.txt**的参数指明；这里实现的方法就是利用随机函数`randint`每次生成一个`0.0、0.1、……、0.9`中的随机数，如果本次产生的随机数值小于丢包率则将该包丢弃；否则就不丢弃；最后，为了更确切地掌握信息传递情况，在middlebox中将数据包的序列号提取出来，如果丢弃了就输出相应的log信息表明改包被丢弃；这样实验要求的middlebox功能就得到了实现。
	除了以上三点，这里还实现了一个结束信息传递的功能：当blaster发送了所有数据包并收到了所有包的ack时，blaster会结束；这时为了让blastee与middlebox也能立即结束，我在blaster结束前发送了一个特殊的“结束信息”（序列号为0的数据包）；这样，当middlebox检测到收到数据包的序列号为0时，就会将该包从另一个端口转发，然后结束；由于正常通信过程中不应该有arp包的出现，所以对于arp包middlebox直接丢弃。</font>

<font size=4 face="楷体">	具体实现代码参见文档**核心代码**部分。</font>

### Task 3: Blastee

<font size=4 face="楷体">	实验要求：修改文件`lab_6/blastee.py`，实现ack blaster发来数据包的blastee；
	思考与实现：
	根据手册要求，blastee要做的工作就是：如果收到了数据包，首先从接收到的数据包中提取出相应的`sequence number`与正确的`Payload`部分，然后利用它们生成一个符合手册规定的ack包，从另一个端口转发回去。同样的，由于正常通信过程中不应该有arp包的出现，所以对于arp包直接丢弃。</font>

<font size=4 face="楷体">	对于`sequence number`与`Payload`部分的提取是建立在数据包格式确定的基础上的。收到的数据包格式应该为“以太网包头+IP包头+UDP包头+RawPacketContents”，其中`RawPacketContents`中前四个字节为`sequence number`，接下来两个字节为`Payload`部分的长度`length`，后面的就是`Payload`部分；所以得到`sequence number`与`Payload`的方法就是利用`RawPacketContents`的`to_bytes`来以字节流形式提取出`Payload`部分；利用`RawPacketContents`的`to_bytes`与`int.from_bytes`，采用大端方式解码来得到相应的`sequence number`，用来log信息；利用同样的方式可以提取出`length`部分，通过判断其值，可以得知是将`Payload`扩充到8个字节后加到ack的尾部还是将`Payload`的前8个字节加到ack的尾部；故将提取出的`sequence number`与`Payload`字节流合并，调用`RawPacketContents`方法生成ack的后半部分；然后在头部依次添加以太网包头、IP包头和UDP包头即可；以太网的src可以由`mymacs`得到，dst可以用收到数据包的src mac；IP包头的src可以由`myips`得到，dst可以从**blaster_params.txt**得到；协议选择为`UDP`；UDP包头的内容不会用到，随便填写即可；</font>

<font size=4 face="楷体">	除了以上处理，blastee还要对“结束信息”作出回应；处理方法也很简单，如果提取出的`sequence number`等于0，就直接break结束即可。</font>

<font size=4 face="楷体">	具体实现代码参见文档**核心代码**部分。</font>

### Task 4: Blaster

<font size=4 face="楷体">	实验要求：修改文件`lab_6/blaster.py`，实现blaster；
	思考与实现：
	blaster要实现的内容有：维护一个固定大小的滑动窗口与超时重传。对于前者，具体的含义不再赘述，仅谈实现思路：从**blaster_params.txt**中得到窗口的大小后，通过LHS与RHS的值来确定窗口的位置，**在我的实现中没有完全按照手册来，我的实现中每次窗口中的数据包就是[LHS,RHS-1]**，也就是在SW=5时，可能出现LHS=1，RHS=6的情况，这时虽然RHS=6，但6号包还没有发送，只是1-5发送了但1还没有收到ack；这样实现的话，如果`RHS - LHS + 1 ≤ SW`，就说明窗口还没有满，还可以继续发送RHS的包；只有`RHS - LHS + 1 = SW + 1`时才说明窗口满了；每次收到一个ack时判断ack的是否是窗口中某个等待ack的包，是则标记；如果LHS指示的包收到了ack，则窗口应该向后移动，即LHS要增加，直到到达另一个未得到ack的包（如果该包还未被发送，则LHS=RHS）；如果窗口未满（条件为`RHS - LHS + 1 ≤ SW`），可以接着发送未发送的包，即将RHS指向的数据包发送，RHS加一；对于后者，blaster应该维护一个重传list，里面存储的是需要重传的数据包的序号；每一次超时，必然是LHS对应的数据包超时，由于一次循环只能发送一个包，故在重传这个数据包的同时都要将此时窗口中未ack的数据包的序号加入重传队列；之后要在恰当的时间将重传队列中的数据包再次转发。同时，如果某个包在进入重传队列但还没有重传时收到了ack信息，则该包不应该重传，需要从重传队列中删去。</font>

<font size=4 face="楷体">	blaster处理所需要的参数可以从**blaster_params.txt**中获得；同时，在传输工作完成之后，blaster还需要输出**Total TX time**等参数，所以在开始blaster就要读文件并初始化参数，具体内容为：</font>

```python
	my_intf = net.interfaces()
    mymacs = [intf.ethaddr for intf in my_intf]
    myips = [intf.ipaddr for intf in my_intf]
    # 获取blaster的端口ip与mac
    fo = open("blaster_params.txt","r+")
    s = fo.read()
    txt = s.split()
    dstip = txt[1]
    num = int(txt[3])
    length = int(txt[5])
    SW = int(txt[7])
    time_out = int(txt[9])/1000
    rec_timeout = int(txt[11])/1000
    # 读文件，获取blastee的ip地址、传输数量、payload长度等参数
    LHS = 1
    RHS = 1
    starter = 0
    # 记录开始传输时间，初始化为0，发送第一个包时更新
    ender = 100
    # 记录结束传输时间，初始化为100，结束时更新
    timer = time.time()
    # timer作为计时器，初始化为开始时间，用于之后判断是否超时
    window = []
    # 窗口
    for i in range(SW):
        window.append(0)
    # list大小固定为SW
    resend = []
    # 重传列表
    retx = 0 # Number of reTX
    tos = 0 # Number of coarse TOs:
    throughput_bytes = 0 # Throughput*Total TX time，即总吞吐量
    goodput_bytes = 0 # Goodput*Total TX time，即不计重传的总吞吐量
```

<font size=4 face="楷体">	明确了以上内容，考虑blaster处理的流程：虽然一次循环只能发送一个数据包，但是在一个循环内也能收一个包发一个包，所以收包与发包是相对独立的两个过程，但二者的顺序不能搞反：要先判断收包，再处理发包。如果一个循环中先发包后收包，则可能在一个包进入重传队列后，某次循环收到了ack，但是因为先处理了发包就将它重发了，造成了浪费。
	对于收包的处理：首先，由于正常通信过程中不应该有arp包的出现，所以对于arp包直接丢弃；收到一个包后，首先提取出其中的`sequence number`（简记为s），如果s小于LHS，则s是一个重复ack，丢掉即可；否则，需要在窗口中将ack的包进行标记（如果已经标记过，即ack过，再次标记仍然表示已经收到ack，也不会有问题）；这里窗口利用一个SW大小的list模拟，1表示已经ack，0表示没有，则标记应该是将`window[s - LHS]`的值置为1；接着就是判断s对应的包是否在重传队列中，是则删去；进行这些操作后，窗口中的第一个包就有可能收到ack了，那么窗口就要移动：故判断`window[0]`是否为1，是则移动窗口，移动的思路就是LHS加1，然后将`window[0:4]`赋值为`window[1:5]`，表示窗口向后移动了一位，再将空出来的`window[4]`置为0；同时，只要窗口移动就要重新计时，所以这时还要重新计时；判断是否超时的方法是用当前时间减去计时变量`timer`（timer记录了上一次重新/开始计时的时刻），检查差值是否大于超时值；故重新计时的方法就是把计时变量的值改为当前值。
	考虑到每次窗口移动可能不只移动一位，但移动的位数最多也只是窗口的大小SW那么多，所以重复上述移动一位的过程SW次，每次都要判断`window[0]`是否为1，如果为1才移动，否则就不移动了；这样就实现了一次移动多位的效果；
	此外，blaster结束是在收到所有数据包的ack后，而一旦blaster收到所有数据包的ack，LHS就会增加为n+1，所以在重复移动一位的过程SW次后，需要判断LHS是否大于n，如果是则发送“错误信息”，然后break结束；具体实现如下：</font>

```python
		if gotpkt:
            if pkt.get_header(Arp) is None:
                # 不是arp包
                log_debug("I got a packet")
                seqnumber1 = int.from_bytes
                (pkt.get_header(RawPacketContents).
                 to_bytes()[0:4],'big')
                # 按大端方式提取序列号
                log_info("get ack : {}".format(seqnumber1))
                if seqnumber1 < LHS:
                    pass
                	#已ack过，丢弃
                else:
                    if seqnumber1 in resend:
                        i = resend.index(seqnumber1)
                        del resend[i]
                        # 收到ack，不必再重传
                    window[seqnumber1 - LHS] = 1
                    # 收到该包ack
                    if window[0] == 1:
                        # 窗口可以移动，进行循环以实现移动多位
                        for i in range(SW):
                            if window[0] == 1:
                                LHS = LHS + 1
                                window[0:4] = window[1:5]
                                window[4] = 0
                        timer = time.time()
                        # 重新计时
                    if LHS > num:
                        # 收到所有ack
                        ender = time.time()
                        # 记录结束时间
                        pkt = Ethernet() + IPv4() + UDP()
                        pkt[1].protocol = IPProtocol.UDP
                        pkt[1].src = myips[0]
                        pkt[1].dst = dstip
                        pkt[0].src = mymacs[0]
                        pkt[0].dst = EthAddr('40:00:00:00:00:01')
                        seqnumber_end = 0 # 序列号为0，作为结束信息
                        content = int.to_bytes(seqnumber_end,4,byteorder='big') + 
                        int.to_bytes(length,2,byteorder='big') + 
                        int.to_bytes(0,length,byteorder='big')
                        myheader = RawPacketContents(content)
                        # 生成序列号+长度+Payload部分
                        pkt = pkt + myheader
                        # 加上包头，转发
                        net.send_packet('blaster-eth0',pkt)
                        break
                        # 结束
            else:
                pass
```

<font size=4 face="楷体">	收包结束后，进行发包工作。因为每次循环只能发一个包，所以会有优先级。blaster发包的优先级为：如果超时，则说明LHS的包还没有收到ack，需要重传，故重发LHS的包，同时将窗口中未ack的包加入重传list中（加入前需要先将重传队列清空）并更新超时数`tos`、重传包数`retx`与总吞吐量`throughput_bytes`；如果没有超时，则先处理需要重传的包：从重发list中取出第一个序列号，生成相应的包发送；同时重传包数`retx`、总吞吐量`throughput_bytes`更新；如果没有需要重传的包，则判断能否继续发送后面仍未发送的包，不能则进入下一循环；否则发送RHS指向序号的包（首次发送该包），更新RHS（加一）、总吞吐量`throughput_bytes`与`goodput_bytes`；判断能否继续发送后面未发送的包的方法就是看窗口是否满：`RHS - LHS + 1 <= SW`与是否发送完所有的包：`RHS <= num`；只有窗口不满并且没有发送完所有的包时才可以继续发包。同时如果是1号包首次传输，则是从该时刻开始传输，所以要更新`starter`与`timer`（将判断是否是1号包首次传输的语句放在`if time.time() - timer < time_out`中是因为手册指出可以认为`recv_timeout < timeout`，所以1号包首次传输时一定会执行到该判断语句）；具体实现如下：</font>

```python
        pkt = Ethernet() + IPv4() + UDP()
        pkt[1].protocol = IPProtocol.UDP
        pkt[1].src = myips[0]
        pkt[1].dst = dstip
        pkt[1].ttl = 64
        pkt[0].ethertype = EtherType.IPv4
        pkt[0].src = mymacs[0]
        pkt[0].dst = EthAddr('40:00:00:00:00:01')
        # 生成数据包的包头
        if time.time() - timer < time_out:
        # 如果未超时
            if len(resend) > 0:
            # 有等待重传的包则优先重传
                seqnumber2 = resend[0]
                # 从重传队列首部取出重传的数据包
                
                content = int.to_bytes(seqnumber2,4,byteorder='big')
                + int.to_bytes(length,2,byteorder='big')
                + int.to_bytes(0,length,byteorder='big')
                
                # 生成序列号、length与Payload部分（字节流形式），采用大端模式
                myheader = RawPacketContents(content)
                # 转化为RawPacketContents
                pkt = pkt + myheader
                # 生成数据包
                net.send_packet('blaster-eth0',pkt)
                del resend[0]
                # 重发该数据
                log_info("resend packet : {}".format(seqnumber2))
                retx = retx + 1
                throughput_bytes = throughput_bytes + length
                # 更新重传数与总吞吐量
            else:
                # 没有等待重传的包，判断能否发送后面仍未发送的包
                if (RHS - LHS + 1 <= SW) and (RHS <= num):    
                # 可以发送
                    seqnumber2 = RHS # 发送下一个包，即RHS指向的包
                    if seqnumber2 == 1: # 是一号包首次传输
                        starter = time.time()
                        timer = time.time()
                    content = int.to_bytes(seqnumber2,4,byteorder='big')
                    + int.to_bytes(length,2,byteorder='big')
                    + int.to_bytes(0,length,byteorder='big')
                    
                    # 同上，不再赘述
                    myheader = RawPacketContents(content)
                    pkt = pkt + myheader
                    net.send_packet('blaster-eth0',pkt)
                    RHS = RHS + 1 # 发送了一个包，窗口移动
                    log_info("first send {}".format(seqnumber2))
                    throughput_bytes = throughput_bytes + length
                    goodput_bytes = goodput_bytes + length

        else:
        # 出现超时，说明LHS指向的包仍未被ack，则重传LHS指向的包，并更新重传队列
            tos = tos + 1
            # 超时次数加一
            seqnumber3 = LHS
            # 要重传LHS指向的数据包
            
            content = int.to_bytes(seqnumber3,4,byteorder='big') 
            + int.to_bytes(length,2,byteorder='big')
            + int.to_bytes(0,length,byteorder='big')
            
            # 同上，不再赘述
            myheader = RawPacketContents(content)
            pkt = pkt + myheader
            net.send_packet('blaster-eth0',pkt)
            log_info("timeout on {}, resend {}".format(LHS,LHS))
            retx = retx + 1
            # 重传数加一
            throughput_bytes = throughput_bytes + length
            resend.clear()
            for i in range(1,RHS - LHS):
            # RHS指向的是窗口的后一位，而LHS已经被重传，不必加入重传队列
            # 所以只考虑window[range(1,RHS - LHS)]中的未ack包
            # 例如LHS=3,RHS=6，窗口中包均未ack，则重传时3已经重传，
            # 只需要4、5进入重传list，即window[1]、window[2]
                if window[i] == 0:
                    resend.append(i + LHS)
            # 更新重传队列
            timer = time.time()
            # 重新计时
```

<font size=4 face="楷体">	最后，在循环结束，即传输结束后，利用已有信息计算要打印的关于本次传输的一些信息输出即可：</font>

```python
    tx_time = ender - starter
    throughput = throughput_bytes / tx_time
    goodput = goodput_bytes / tx_time
    log_info("Total TX time : {}".format(tx_time))
    log_info("Number of reTX : {}".format(retx))
    log_info("Number of coarse TOs : {}".format(tos))
    log_info("throughput : {}".format(throughput))
    log_info("goodput : {}".format(goodput))
```

<font size=4 face="楷体">	由此，完成了blaster的设计。</font>

### Task 5: Running your code

<font size=4 face="楷体">	见文档**实验结果**部分。</font>

### <font face="楷体">4、实验结果</font>

#### Task1&Task2&Task3&Task4

<font size=4 face="楷体">	略。</font>

#### Task5

<font size=4 face="楷体" color=red>	Tip：附图建议放大观看</font>

<font size=4 face="楷体">	取blaster参数为：`-b 192.168.200.1 -n 100 -l 100 -w 5 -t 300 -r 100`，运行的结果如下：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_1.png)

<font size=4 face="楷体">	选取其中部分传输过程进行分析：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_2.png)

<font size=4 face="楷体">	该过程从第一次传输28号数据包开始，这时LHS=24，RHS=29，即24——28号包在窗口中；由middlebox的log信息可知28、29号包会被丢弃；之后blaster依次收到24——27号包的ack，每收到一次ack也会传一个新的包，但由于28、29号包被丢弃，所以一直收不到28号的ack，此时LHS=28，RHS=33，窗口大小为5，已满，所以会停止发后面的包；在收到30、31号包的ack后，因为超时，blaster会重传28、29号数据包，而重传29的时候还收到了32号包的ack，所以此时窗口中28、29号包未得到ack，30——32号包已被ack；</font>

<font size=4 face="楷体">	接着，在等待了一段时间后（该时间段内没有超时发生超时，也没有要重传的包，同时窗口已满也不会发送新的包，所以会等待），blaster收到了28号包的ack，从而可以继续接下来的发包工作。</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_3.png)

<font size=4 face="楷体">	另外一个丢包多次的例子：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_4.png)

<font size=4 face="楷体">	从middlebox的log信息可以看出70号包被丢弃了4次，所以blaster会在70号包上超时4次，重传4次后才收到70的ack；而且在重传后期blaster只是执行等待——超时——重传这样的流程。</font>

<font size=4 face="楷体">	再举一个传输比较流畅，不存在丢包的过程：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_5.png)

<font size=4 face="楷体">	在这个过程中，每次循环会收到一个ack并发送一个包。</font>

------

<font size=4 face="楷体">	更改参数后继续测试，具体来说就是将重传时间变长一点：</font>

```python
-b 192.168.200.1 -n 100 -l 100 -w 5 -t 2000 -r 100
```

<font size=4 face="楷体">	同时把丢包率由0.19改为0.3：</font>

```python
-d 0.3
```

<font size=4 face="楷体">	再次测试</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_6.png)

<font size=4 face="楷体">	显然传输完成所需的时间长了很多，而且要等待很多轮循环之后才能超时重传，同时可以看到由于丢包率的提高和超时时间的增加，吞吐率明显下降，goodput占比也下降很多。</font>

------

<font size=4 face="楷体">	利用wireshark测试：方便起见，将传输包的数量减少为10进行分析（其它参数就按给定的txt文件内容来），结果&分析如下：</font>

<font size=4 face="楷体">	发送数据包1，但是被丢弃（middlebox中显示）：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_7.png)

<font size=4 face="楷体">	从blastee的抓包结果也可以看出第一次传输的数据包1确实被丢弃了：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_13.png)

<font size=4 face="楷体">	数据包1超时重发，此时数据包2还没有收到ack，故2也会重传：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_8.png)

<font size=4 face="楷体">	数据包1收到ack（在2收到ack之后）：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_9.png)

<font size=4 face="楷体">	之后正常地发包、收ack，其中又经历了4号包、8号包与10号包的丢包与重传；这里仅对8号包的传输进行分析，4、10号包分析略去；
	由blastee的log与抓包信息可知，8号包只在传输结束的前一个循环成功传输到了blastee，在blastee的其余信息中没有第二次出现过8号包：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\6_14.png)

<font size=4 face="楷体">	所以，在8号包第一次传输到最后一次重传之前，所有的包都会被丢弃；了解这个之后从blaster角度分析8号包的传输：
	首先8号包第一次传输会被丢弃，第一次重传是由于4号包也被丢弃，引发超时重传导致的，但是重传的8号包也被丢弃；之后blaster收到4号包的ack，LHS变为8，并首次发送了最后两个包9、10号包；</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_10.png)

<font size=4 face="楷体">	由于重传的包被丢弃，所以blaster收不到8号包的ack，故再次引发了超时重传，8号包第二次重传，再次被丢弃，所以blaster又等不到8的ack，再次超时重传……直到最后收到8的ack，结束传输。</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_11.png)

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_12.png)

<font size=4 face="楷体">	最后blaster会发送结束信息（序列号为0的包）：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_14.png)

<font size=4 face="楷体">	根据wireshark与log信息的比对，可以看出随机丢包、ack正确的序列号、维护窗口的大小、超时重传与重传队列内容的维护等都得到了实现，而且故可以说明`blaster, blastee 与 middlebox`是正确运行的。</font>

<font size=4 face="楷体">	附加一个对于数据包组成结构的验证：再上述测试中，blaster发送的数据包内容中，Data长度为106字节，即为4个字节的序列号、2个字节的length与100字节的Payload；例如上图中8号包的Data为：</font>

```python
Data:00 00 00 08 00 64 000000000000000000000000000000000000…
```

<font size=4 face="楷体">	可以看出前四个字节表示序列号，5-6个字节为`0x64 = 100`，即length，之后的Payload内容都是0；而对于ack来说，Data长度为12字节，即4字节序列号加上8字节Payload；例如8号包的ack的Data为：</font>

```python
Data:00 00 00 08 00 00 00 00 00 00 00 00
```

<font size=4 face="楷体">	即序列号加上8号包Payload部分的前8个字节。</font>

<font size=4 face="楷体">	当包的Payload长度小于8时，情况为(更改txt文件中length为6)：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_15.png)

![](C:\Users\njuwhl2019hp\Desktop\计网\lab6_16.png)

<font size=4 face="楷体">	可以看出blaster发送的包长度为12，前四个字节表示序列号，5-6个字节为`0x06 = 6`，即length，之后的Payload内容都是0，为6个字节；而对于ack来说，Data长度仍然为12字节，即4字节序列号加上6字节Payload，再加上(`8-6=`)2字节Payload的扩充内容；</font>

### <font face="楷体">5、核心代码</font>

​	<font size=4 face="楷体">	**middlebox**：</font>

```python
	my_intf = net.interfaces()
    mymacs = [intf.ethaddr for intf in my_intf]
    myips = [intf.ipaddr for intf in my_intf]
    fo = open("middlebox_params.txt","r+")
    s = fo.read()
    txt = s.split()
    drop_rate = float(txt[1]) #读取丢包率
    ……
		if dev == "middlebox-eth0": #收到blaster的包
            log_debug("Received from blaster")
            '''
            Received data packet
            Should I drop it?
            If not, modify headers & send to blastee
            '''
            seqnumber = int.from_bytes
            (pkt.get_header(RawPacketContents).to_bytes()[0:4],byteorder='big')
            # 获取序列号
            if seqnumber == 0: # end signal
                pkt[Ethernet].src = mymacs[1]
                pkt[Ethernet].dst = EthAddr('20:00:00:00:00:01')
                net.send_packet("middlebox-eth1", pkt)
                break
            temp = randint(0,9)/10
            if temp <= drop_rate: # 随机数小于drop_rate，丢弃该包
                log_info("drop packet {}".format(seqnumber))
                
            else: # 不丢弃，传输该包
                pkt[Ethernet].src = mymacs[1]
                pkt[Ethernet].dst = EthAddr('20:00:00:00:00:01')
                net.send_packet("middlebox-eth1", pkt)
        elif dev == "middlebox-eth1": #收到ack，直接传输
            log_debug("Received from blastee")
            '''
            Received ACK
            Modify headers & send to blaster. Not dropping ACK packets!
            net.send_packet("middlebox-eth0", pkt)
            '''
            pkt[Ethernet].src = mymacs[0]
            pkt[Ethernet].dst = EthAddr('10:00:00:00:00:01')
            net.send_packet("middlebox-eth0", pkt)
```

<font size=4 face="楷体">	**blastee**：</font>

```python
    my_interfaces = net.interfaces()
    mymacs = [intf.ethaddr for intf in my_interfaces]
    myips = [intf.ipaddr for intf in my_interfaces]
    fo = open("blastee_params.txt","r+")
    s = fo.read()
    txt = s.split()
    dstip = txt[1]
    num = int(txt[3]) # 读取文本信息
    ……
            if gotpkt:
            if pkt.get_header(Arp) is not None:
                continue
            if int.from_bytes(pkt.get_header(RawPacketContents)
                              .to_bytes()[0:4],byteorder='big') == 0:
            # 序列号为0，是结束信号，直接结束
                break
            log_debug("I got a packet from {}".format(dev))
            log_debug("Pkt: {}".format(pkt))
            seqnumber = pkt.get_header(RawPacketContents).to_bytes()[0:4]
            length = int.from_bytes(pkt.get_header(RawPacketContents)
                                    .to_bytes()[4:6],byteorder='big')
            payload = pkt.get_header(RawPacketContents).to_bytes()[6:]
            # 提取序列号、长度与Payload内容
            if(length < 8):
                payload = payload + int.to_bytes(0,8-length,byteorder='big')
                myheader = RawPacketContents(seqnumber+payload)
                # Payload长度小于8，将Payload扩充后加到包的尾部
            else:
                myheader = RawPacketContents(seqnumber+payload[0:8])
                # 否则只将Payload的前8个字节加到包的尾部
            ip = IPv4()
            ip.dst = dstip
            ip.src = myips[0]
            ip.ttl = 64
            ip.protocol = IPProtocol.UDP
            ethernet = Ethernet()
            ethernet.ethertype = EtherType.IPv4
            ethernet.src = mymacs[0]
            ethernet.dst = pkt[Ethernet].src
            udp = UDP()
            udp.src = 0
            udp.dst = 1
            # 生成以太网、ip、udp包头
            ack = ethernet + ip + udp + myheader
            log_info("ack : {}".format(int.from_bytes(seqnumber,byteorder='big')))
            net.send_packet(dev, ack)
			# 传输
```

<font size=4 face="楷体">	**blaster**：</font>

<font size=4 face="楷体">	参看**Task 4：Blaster**部分。</font>

### <font face="楷体">6、总结与感想</font>

<font size=4 face="楷体">	完成本次实验加深了我对传输层协议中sliding window、超时重传等机制的理解，让我对于“可靠传输”建立的方法原理有了进一步的了解。
	**实验遇到的问题与解决**：
	本次实验的实现过程比较简单，最需要理解的内容——传输包的格式以及超时机制的实现也在手册中详细说明了，所以在实现过程中并没有遇到问题。问题的出现是在测试阶段：
	1、在利用wireshark抓包的过程中发现，在log信息与抓包结果中，blaster收到ack和传输数据包的顺序经常会不一致；例如数据包1被丢弃，重传后抓包结果显示是先收到了2、1的ack后传输3号包，但log信息显示是收到2的ack、传输3号包，最后收到1的ack；对于这个问题，可能的解释是wireshark会显示收到或发送的所有包，但是在程序处理时每次只能处理一个数据包，所以收到2的ack后先执行程序，发送了3，下一个循环才收到1的ack；
	2、在一个mininet下运行结束或因为异常中止后，不退出mininet，重新打开xterm窗口再次运行，middlebox与blastee会出现异常丢包与异常ack的情况；根据讨论群的提示，问题出现的原因之一是没有对arp包进行丢弃处理；但增加了对arp包的处理后还是会有异常情况出现，经过尝试后发现只有blastee与middlebox异常的原因是传输结束时blaster结束了，但它们没有结束，再次启动时会一直有异常包产生。因此加入了end signal的生成与传输，解决了这个问题。</font>

