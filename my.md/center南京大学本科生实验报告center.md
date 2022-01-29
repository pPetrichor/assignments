# <font face="楷体"><center>南京大学本科生实验报告</center></font>

<font size=3 face="楷体">课程名称：**计算机网络**													任课教师：李文中</font>

| <font face="楷体">学院</font>  | <font face="楷体">计算机院</font> | <font face="楷体">专业（方向）</font>  | <font face="楷体">计算机科学与技术</font> |
| ------------------------------ | --------------------------------- | -------------------------------------- | ----------------------------------------- |
| <font face="楷体">学号</font>  | 181860092                         | <font face="楷体">姓名</font>          | <font face="楷体">王瀚霖</font>           |
| <font face="楷体">Email</font> | 603102048@qq.com                  | <font face="楷体">开始/完成日期</font> | 2020.3.7  /  2020.3.8                     |



### <font face="楷体">1、实验名称</font> : Switchyard & Mininet

### <font face="楷体">2、实验目的</font>

<font size=4 face="楷体">	通过完成本实验来初步学习并逐步掌握计算机网络实验中使用到的环境、语言、工具等内容；结合实验内容理解Mininet、Wireshark、Switchyard的运行规则及使用方法；重点关注Mininet、Wireshark、Switchyard以及python语言的学习。</font>

### <font face="楷体">3、实验内容</font>：Modification

#### Step 1

<font size=4 face="楷体">	实验要求：修改文件`lab_1/start_mininet.py`，在已有的拓扑结构中删除server2或创建一个不同的包含6个节点的拓扑结构；本阶段我选择前者进行实现：即删除已有拓扑结构中的server2节点；
	在进行具体的实现之前，首先对已有的`start_mininet.py`进行分析：阅读文件中的main函数部分可以得知文件的执行思路；首先，实例化`PySwitchTopo`这个类来生成类定义\_\_init\_\_中规定好的拓扑结构，接着通过实例化一个Mininet对象，传入实例化好的topo来生成我们的网络net；完成后调用setup_addressing函数，在其内部通过reset_macs（）函数与set_ip（）函数来设置net中各节点以及hub的mac地址与各节点的ip地址（通过hub相连）；完成这些后，基本的网络设置就完成了，之后main调用了disable_ipv6来说明禁止了ipv6协议生效;最后，执行 net.interact()进入我们的ping、drop等测试环节（目前未配置好路由规则故ping不了）。
	根据上述的分析，修改方法就很简单了：因为关于拓扑结构的设置只出现在`PySwitchTopo`的\_\_init\_\_以及setup_addressing（）中，分别是构建拓扑结构与地址设置，所以要删除server2，只需要在拓扑结构中将server2添加到topo的操作去掉，然后去掉setup_addressing（）中对server2的mac地址和ip地址的设置操作就可以了。具体而言，需要注释掉：</font>

```python
1、self.addHost('server2', **nodeconfig)
# 将server2从__init__中建立的拓扑中去掉

2、reset_macs(net, 'server2', '20:00:00:00:00:{:02x}')
# 取消对于server2的mac地址的设置

3、set_ip(net, 'server2','hub','192.168.100.2/24')
# 取消对于server2的IP地址的设置
```

<font size=4 face="楷体">	注释了这三句后，就完成了Step1的要求（在已有的拓扑结构中删除server2）；</font>

#### Step2

<font size=4 face="楷体">	实验要求：修改文件`lab_1/myhub.py`，实现每收到一个包时都以`in:<ingress packet count> out:<egress packet count>`的形式来输出当前hub累计收到与转发数据包的个数；
	同样的，在开始实验前先分析代码的执行流程：首先，将配置在net上的接口全部都存储到my_interfaces这个list中，再将所有接口的mac地址存储到mymacs变量中；接下来就进入一个循环来开始说明hub的工作方式：首先在try语句下调用函数`net.recv_packet()`来得到一个hub接收到的数据包，接收到此数据包的时间被赋值给`timestamp`，接收的端口名字被赋值给`dev`，数据包内容存到`packet`中；如果检测到NoPackets异常则continue；如果是Shutdown异常则结束hub的工作；否则，就是接收到了一个数据包，因此会对接收到的数据包进行分析；将收到的数据包的header提取出来：如果hearder是空的，则输出提示信息**"Received a non-Ethernet packet?!"**；如果header中目的地址是hub本身，则不需要转发，只输出**"Received a packet intended for me"**；如果都不是的话，就应该是通过hub转发的数据包了；因此，hub需要以flood的方式转发此数据包，即除了接收到数据包的端口以外，其它端口都会发送一次这个数据包，所以会遍历my_interfaces，找到所有除了dev（即接收到数据的端口）的端口，在其中调用send_packet（）发送数据包并输出提示信息**"Flooding packet {} to {}"**；一直重复此过程直到结束（Shutdown）；
	明白了整个代码的流程，要完成实时统计hub吞吐量这一任务就比较简单了：对于接受数据量而言，可以设置一个初始值为0的量，在每接收到一个数据包时自增，也即在调用`recv_packet()`后自增，这样每次输出时就是当前统计到的hub接受packet的数量；对于转发量也一样，代码中是通过`send_packet（）`实现转发数据包的，所以只需在这个函数调用后将对应的量自增即可；最后在一次收发数据包结束，也就是一次while结束时调用`log_info`来输出in、out数据包的统计量即可；此外，对于接收到数据包到来时刻的输出，可以直接输出当前的`timestamp`变量进行完成；具体的代码改动为：</font>

```python
# 1、在进入while循环之前声明记录hub收发数据包的变量in_与out_
……
my_interfaces = net.interfaces()
mymacs = [intf.ethaddr for intf in my_interfaces]
in_ = 0 # 添加
out_ = 0 # 添加
……

# 2、在try语句后（包括对应的两个except后），即收到数据包后，增加输出时间戳以及in_自增的操作
……
log_debug("In {} received packet {} on {}".format(net.name, packet, dev))
log_info("packet arrival time : {}".format(timestamp)) # 添加
in_ = in_ + 1 # 添加
……

# 3、在调用send_packet之后增加out_自增的操作
……
net.send_packet(intf, packet)
out_ = out_ + 1 # 添加
……

# 4、在每次while结束时输出当前的统计信息：
……
while True:
    ……
    log_info("in:{} out:{}".format(in_, out_)) # 添加
net.shutdown()

# 5、在shutdown前输出最终的统计结果（画蛇添足）
……
except Shutdown:
            log_info("Totally, {} packets pass through a hub in".format(in_)) # 添加
            log_info("Totally, {} packets pass through a hub out".format(out_)) # 添加
            return
……
```

<font size=4 face="楷体">	以上就是Step2中所有的改动即思考过程，log的结果可参看文档**实验结果**部分；</font>

#### Step3

<font size=4 face="楷体">	实验要求：修改文件`lab_1/hubtests.py`，利用给定的函数`mk_pkt`，通过传入不同的参数构建一个新的测试用例或者自己定义一个packet类型变量来构建一个新的测试用例；本阶段我选择前者实现；
	同样的，在开始实验前先分析代码的执行流程：代码框架中定义了函数`mk_pkt`，这里可以先不用管函数的具体内容，只需要知道传入参数的含义依次为：发送方的mac地址、接收方的mac地址、发送方的ip地址、接收方的ip地址以及是否是应答消息；然后定义了`hub_tests()`，也就是实际上的测试用例；在这里最后会通过`scenario = hub_tests()`来将`hub_tests()`中定义好的TestScenario类的变量赋给`scenario`这个特定的变量，所以实际上测试用例都在`hub_tests()`中定义；
	在`hub_tests()`中，首先实例化了一个`TestScenario`类型的变量s，调用`add_interface()`为s中增加了eth0、eth1、eth2三个端口，接下来就是case1到case3的测试用例了；case1模拟的是广播的数据报文，所以接收方的mac要设为"ff:ff:ff:ff:ff:ff"，ip要设为"255.255.255.255"；对于这样的数据包，hub显然应该将此数据包从除了接收到此数据包外的端口转发出去；因此代码框架利用`expect`函数定义了接下来hub的行为，首先调用`PacketInputEvent()`规定hub从eth1接收到这个数据包，然后调用`PacketOutputEvent`说明hub应该从eth0与eth2转发此数据包；case2模拟的是两个机器之间的相互通信；首先eth0接收到一个数据包，检测到目的mac是"30:00:00:00:00:02"，不是广播，应该是要传输到其它地方的机器上，所以hub应该用flood方式发送此数据包；也就是将此数据包从除了接收到此数据包外的端口转发出去；所以添加了预期行为：hub应该从eth1和eth2中转发出此数据包；而接收方接收到此数据包后要进行回复，所以会进行一个基本相同的传输操作（除了mk_pkt时reply置为True），eth1接收（说明接收方与eth1相连），eth0和eth2传出，不再赘述；case3模拟的是一个数据包目的mac为hub本身，那就说明要广播的数据包已经广播到目的机器了，所以hub无需发送此数据包，故代码给出的预期是`PacketInputTimeoutEvent(1.0)`；从而得到一个`NoPackets`异常；
	代码框架给出了三种情况下hub的行为描述，同时也提供了一些机器的mac与ip地址；故可以挑一种类型实现一个新的测试用例；这里我选择了模仿case1实现一个IP地址为"192.168.1.100"，mac地址为"20:00:00:00:00:01"的机器进行信号广播的测试，因此首先调用`mk_pkt`函数生成这样的数据包，然后规定hub的接收端口，预期hub接下来的行为，具体实现为添加如下代码：</font>

```python
# my test case : a frame with broadcast destination should get sent out
    # all ports except ingress
mypkt = mk_pkt("20:00:00:00:00:01", "ff:ff:ff:ff:ff:ff", '192.168.1.100','255.255.255.255')

s.expect(PacketInputEvent("eth0", mypkt, display=Ethernet), "An Ethernet frame with a broadcast destination address should arrive on eth0")

s.expect(PacketOutputEvent("eth1", mypkt, "eth2", mypkt, display=Ethernet), "The Ethernet frame with a broadcast destination address should be forwarded out ports eth1 and eth2")
```

<font size=4 face="楷体">	测试过程：在终端进入switchyard文件夹，输入`source ./syenv/bin/activate`指令，然后输入指令`swyard -t lab_1/hubtests.py lab_1/myhub.py`，即可用hubtests.py中写好的测试用例对myhub.py中的hub进行测试；测试结果可参看**实验结果**部分；</font>

#### Step4

<font size=4 face="楷体">	实验要求：在修改后的拓扑结构中运行修改后的myhub.py，确保其能够正常工作；
	操作过程：保存已更改的`lab_1/start_mininet.py`与`lab_1/muhub.py`，接着在终端进入switchyard文件夹，输入`sudo python lab_1/start_mininet.py`指令，进入修改后的拓扑结构中；利用`xterm hub`指令以更好地观察输出；在打开的窗口中输入`source ./syenv/bin/activate`指令，然后执行`swyard lab_1/myhub.py`，使hub在当前拓扑下工作起来；接下来就可以通过ping、drop等命令进行测试了；测试的结果可参看**实验结果**部分；</font>

#### Step5

<font size=4 face="楷体">	实验要求：利用wireshark捕捉此拓扑结构中除了hub以外的一个主机的通信数据包并解释，这里选择捕捉client的通信数据；
	操作过程：在与Step4进行相同的操作，使hub在当前拓扑下工作起来后，键入指令`client wireshark &`，选择client-eth0，然后在mininet中输入`client ping -c 1 server1`制造一些traffic，则wireshark中就捕捉到了这次通信的数据包；对于捕捉结果与对结果的解释参见**实验结果**。</font>

### <font face="楷体">4、实验结果</font>

#### Step1

<font size=4 face="楷体">	在修改后的拓扑结构中删除了server2这一节点，修改后的拓扑结构为：</font>

```mermaid
graph LR
  B-.-C[client]
  A(server1)===B((hub))

```

#### Step2

<font size=4 face="楷体">	在mininet下运行，依次输入`client ping -c 1 server1`与`pingall`后hub的窗口输出结果为（建议pdf下放大查看）：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\计网3.png">

<font size=4 face="楷体">	在删除了server2的拓扑下运行`myhub.py`，可以看到因为此时的拓扑结构如Step1中所示，所以结果显示hub每从一个端口收到1个帧，就会广播此帧（目的mac为"ff:ff:ff:ff:ff:ff"）或者采用flooding的方式转发，所以每次都是“in 1 out 1”的情况；正如结果显示的，每一次接收到包都是“in k out k”的输出形式（一开始还会以“packet arrival time ：xxxxx”的形式输出时间）；</font>

#### Step3

<font size=4 face="楷体">	利用hubtest.py(修改后的)来测试muhub.py的结果如下图所示(截图包含了本人信息“181860092_whl”、时间戳输出以及in、out实时统计)；可以看到hub的行为与预期完全一致；</font>

![](C:\Users\njuwhl2019hp\Desktop\计网1.png)

#### Step4

<font size=4 face="楷体">	在mininet下运行，依次输入`dump`、`net`与`pingall`后hub的窗口输出结果如下图所示（建议pdf下放大查看）；根据结果来看，修改后的拓扑与hub是work的；</font>

![](C:\Users\njuwhl2019hp\Desktop\计网4.png)

#### Step5

<font size=4 face="楷体">	经实验内容Step5中的操作后，wireshark捕捉到的内容为：</font>

![](C:\Users\njuwhl2019hp\Desktop\计网6.png)

​	<font size=4 face="楷体">	对于捕捉内容的解释：由start_mininet.py中的定义可知，client的mac地址为'30:00:00:00:00:01'，IP地址为'192.168.100.3'；server1的mac地址为'10:00:00:00:00:01'，IP地址为'192.168.100.1'；捕捉到的内容是对`client ping -c 1 server1`产生的通信的捕捉，该指令说明client要向server1来ping一个消息；结合每个帧的source与Destination以及info进行对这一过程的分析：由于最初始时所有机器的ARP表都是空的，client想传一个帧到server1，但不知道其mac地址，无法传输，所以client会先以广播方式发送一个ARP请求，这就是第一个帧的内容；该请求被server1捕获后，检测到目的ip与自己的匹配，就会向client发送一个应答消息，告诉client自己的mac地址，这就是第二个帧的内容；该帧被client接收后，client的ARP表会更新，添加server的ip、mac地址；随后client向server1发送信息，根据ICMP协议server1会回复client，回复时server1已经知道了client的mac地址了，所以直接回复，无需ARP请求；经过一段时间后，server1会再次发一个ARP请求命令来得到client的mac地址，更新自己的ARP表，从而在自己的本地机的ARP表上记录与自己通信过的client的当前mac地址；这就是第5、6个帧的内容；</font>

### <font face="楷体">5、核心代码</font>

​	<font size=4 face="楷体">	本实验中各阶段主要修改的代码在**实验内容**部分已经提到；在这里集中呈现一下：</font>

#### Step1

<font size=4 face="楷体">	本阶段的核心就是注释掉以下三行代码：</font>

```python
1、self.addHost('server2', **nodeconfig)
# 将server2从__init__中建立的拓扑中去掉

2、reset_macs(net, 'server2', '20:00:00:00:00:{:02x}')
# 取消对于server2的mac地址的设置

3、set_ip(net, 'server2','hub','192.168.100.2/24')
# 取消对于server2的IP地址的设置
```

#### Step2

<font size=4 face="楷体">	本阶段的核心就是在框架合适位置添加以下代码：</font>

```python
# 1、在进入while循环之前声明记录hub收发数据包的变量in_与out_
……
my_interfaces = net.interfaces()
mymacs = [intf.ethaddr for intf in my_interfaces]
in_ = 0 # 添加
out_ = 0 # 添加
……

# 2、在try语句后（包括对应的两个except后），即收到数据包后，增加输出时间戳以及in_自增的操作
……
log_debug("In {} received packet {} on {}".format(net.name, packet, dev))
log_info("packet arrival time : {}".format(timestamp)) # 添加
in_ = in_ + 1 # 添加
……

# 3、在调用send_packet之后增加out_自增的操作
……
net.send_packet(intf, packet)
out_ = out_ + 1 # 添加
……

# 4、在每次while结束时输出当前的统计信息：
……
while True:
    ……
    log_info("in:{} out:{}".format(in_, out_)) # 添加
net.shutdown()

# 5、在shutdown前输出最终的统计结果（画蛇添足）
……
except Shutdown:
            log_info("Totally, {} packets pass through a hub in".format(in_)) # 添加
            log_info("Totally, {} packets pass through a hub out".format(out_)) # 添加
            return
……
```

#### Step3

<font size=4 face="楷体">	本阶段的核心就是添加以下代码，增加一个测试用例：</font>

```python
# my test case : a frame with broadcast destination should get sent out
    # all ports except ingress
mypkt = mk_pkt("20:00:00:00:00:01", "ff:ff:ff:ff:ff:ff", '192.168.1.100','255.255.255.255')

s.expect(PacketInputEvent("eth0", mypkt, display=Ethernet), "An Ethernet frame with a broadcast destination address should arrive on eth0")

s.expect(PacketOutputEvent("eth1", mypkt, "eth2", mypkt, display=Ethernet), "The Ethernet frame with a broadcast destination address should be forwarded out ports eth1 and eth2")
```

#### Step4&Step5

<font size=4 face="楷体">	无核心代码。</font>

### <font face="楷体">6、总结与感想</font>

<font size=4 face="楷体">	完成本次实验让我对于Mininet、Wireshark和Switchyard的作用与使用方法有了比较全面的理解，同时对于之前完全没接触过的python与接触过一点点的git也有了更多的了解；另外，完成实验内容也让我对课程学习的一些内容，例如ARP协议、hub集线器等内容有了更直接的接触，得到了更好的理解；此外，观察理解框架代码，利用应用工具来进行理解都是有效的办法；例如在执行pingall过程中hub给出的log为"in 8 out 8"，就很难理解；但结合Wireshark捕获的同时给出的info信息再来理解就轻松了很多；今后学习、实验中也会注意这些方法的应用。</font>