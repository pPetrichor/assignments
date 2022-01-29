# Bpex

<font size=4 face="楷体">核心：在apex基于执行顺序、迭代次数的基础上引入概率来进行对齐；
做法：简单来说，就是在对A版本的某一句进行匹配时，通过分析它的语句类型选定一些候选，将这些候选语句赋为相等的概率，然后在后续的迭代中通过迭代次数、执行等内容更新概率，最后分析完后选取最大概率的语句作为对齐项；
个人感觉BPEX相较于APEX就是先不根据当前已有的信息判断一个对齐项，而是暂存下来直到最后再确定；</font>



## details

1. <font size=4 face="楷体">先进行一次切片对齐，并加上初始的概率：</font>

   <font size=4 face="楷体">利用Needleman-Wunsch算法（https://blog.csdn.net/aldenphy/article/details/45057133）进行最初始的对齐操作，关键内容就是基于符号表达式进行最大匹配，使得匹配后大部分的对齐项都是相同的符号表达式；在这个基础上进行概率的初始化：一一比较每一个匹配好的pair，如果在这个pair中出现了变量，那么由于它们又有相同的符号表达式，那么我们就有更大的把握说这两句是匹配的，所以它们之间匹配的概率初值赋为0.99；而如果这一个pair中没有变量只是有些具体的数值计算，那可能这个数值计算出现在不同地方很多次，相对来说可能不匹配的概率大些，所以初值赋为0.7；当然如果没有变量但是是简单的输出语句，输出内容匹配，那基本就匹配到了，也赋值为0.99；</font>

2. <font size=4 face="楷体">迭代完善</font>

   <font size=4 face="楷体">迭代完善的流程为：根据当前的对齐情况建立一个概率model，然后解这个model并计算新的对齐概率分布，在新计算出来的概率中，如果有一些值超过了设置的某个阙值，就把这个对齐情况加入到新的对齐中；最后将新得到的对齐与旧的对齐情况作并集运算（这就可能导致某个语句与多个语句匹配），得到新的对齐情况；这个过程可以重复多次；</font>

   <font size=4 face="楷体">通过一个对齐情况建立一个概率model的过程利用了worklist算法（或许可以找一找有无代码实现）为：</font>

   ```
    procedure PROPAGATE(alignments, n)
   	 worklist ← alignments 
   	 #将当前的对齐情况放入到worklist中
    	while worklist is not empty do #遍历
    		I`i,tj,distance ← pop(worklist) 
    		#pop出一个内容，Ii、tj是旧的alignments匹配的一对
   		 ApplyRule(I`i,tj) 
   		 #用propagation规则处理这个pair，没找到该规则……，不过函数的作用是通过与之前该pair的距离来限制worklist？？？
    		if distance <= threshold then 
    		#如果当前pair与之前的对齐pair的依赖距离小于某个阙值，说明当前pair的对齐情况还是比较可靠的
    			for I`p,tq ∈ dep(I`i,n)×dep(tj,n) do 
    				worklist ← worklist ∪ 
    					{(I`p,tq,distance + 1)} 
    					#直接相关的话，距离增大1
    					
   		#将这一个pair中所有依赖距离小于n的依赖项也作为对齐的可能项加入到worklist中 
   		 end for 
   		 end if 
   	 end while 
    end procedure
    
    #总而言之，这一部分做的就是通过依赖距离这个关系把一切有可能是对齐的项加入到当前的对齐中，加大搜索空间
    #有n的限制是为了防止搜索空间增加的太快
   ```

3. <font size=4 face="楷体">基于概率进行推断</font>

<font size=4 face="楷体">基于概率进行推断的基本规则</font>

<img src="C:\Users\njuwhl2019hp\AppData\Roaming\Typora\typora-user-images\image-20200309162307782.png" alt="image-20200309162307782" style="zoom:67%;" />

<font size=4 face="楷体">解释：
1、如果li与tj有相同的符号表达式，那么它们很有可能匹配；
2、3、如果li和tj匹配，那么li的依赖项也有可能与tj的依赖项匹配；这里的dependencies包括直接依赖与过渡性的依赖，例如同样是计算中间值，可能有`mid = (low + high) / 2; `和`diff = high - low; mid = low + diff / 2; `两种写法；前者mid直接low、high，后者mid直接依赖low，过渡性依赖high；但二者是等价的，应该都考虑到；但显然没有办法考虑到所有的过渡性依赖（如果考虑的话，源于同一个值的诸多变量都可能被视为有很高的概率匹配），故选择限制distance来解决，同时不断地增大这个值；具体的做法就是一开始只考虑直接依赖项的对齐，之后会增大distance考虑更大范围的匹配对齐；自然，直接匹配到的是优先考虑的；
4、如果li与tj匹配，那么它们所对应的语句l、t也可能匹配,反之亦然；由此规则可以推出，如果两个执行实例（instance）匹配了，那么这两个实例对应的语句的其它实例也大概率匹配；</font>

<font size=4 face="楷体">模型计算中使用到的函数：</font>

1、<img src="C:\Users\njuwhl2019hp\AppData\Roaming\Typora\typora-user-images\image-20200309170022583.png" alt="image-20200309170022583" style="zoom:67%;" />

<font size=4 face="楷体">对应于rule1；描述了li和tj匹配的概率；</font>

2、

<img src="C:\Users\njuwhl2019hp\AppData\Roaming\Typora\typora-user-images\image-20200309170442582.png" alt="image-20200309170442582" style="zoom:67%;" />

<font size=4 face="楷体">对应于rule4；描述了实例与语句的一致性；</font>

3、<img src="C:\Users\njuwhl2019hp\AppData\Roaming\Typora\typora-user-images\image-20200309171028510.png" alt="image-20200309171028510" style="zoom:67%;" />

<font size=4 face="楷体">对应于rule2、3；对依赖项的匹配给出估计；例如在当前的对齐情况下，li和tj对齐且li的数据依赖项ld1与tj的数据依赖项td1对齐，那么此函数就给出0.99，否则0.01；分析可知，此函数没有考虑数据依赖项和控制依赖项匹配的情况，因为这种匹配几乎不可能出现，所以忽略。</font>



<font size=4 face="楷体">论文中在这一部分介绍完后就没有介绍了，进入了例子介绍；结合上下文的话，这些函数应该是用于在已有的对齐情况下利用2中的PROPAGATE扩大搜索范围，即加入了一些新的对齐项后，调用这些函数计算新的对齐项的估计值，然后用</font>

<img src="C:\Users\njuwhl2019hp\AppData\Roaming\Typora\typora-user-images\image-20200309172544778.png" alt="image-20200309172544778" style="zoom:67%;" />

<font size=4 face="楷体">（这个式子是在文章中的factor graph提到的，graph就是用画图形式来表示概率模型，不过似乎也适合这里用）
来计算新的对齐项的概率，然后与一个阙值比较，大于则加入到候选对齐项中；最后选较大概率匹配就好了；
整个流程可用下图概述（solve应该就是利用上述函数计算）：</font>

<img src="C:\Users\njuwhl2019hp\AppData\Roaming\Typora\typora-user-images\image-20200309173101096.png" alt="image-20200309173101096" style="zoom:67%;" />

