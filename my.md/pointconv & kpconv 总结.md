# <p align="center"><font face="楷体">pointconv & kpconv 总结</p></font>

### pointconv

<font size=4 face="楷体">	pointnet希望实现一个针对点云的近似的3D卷积核。</font>

<font size=4 face="楷体">	pointconv流程：输入点云，对一个确定的点，利用K近算法找K个邻居点，接着计算卷积核W与$F_{in}$；这里面使用到的MLP是由1*1卷积与非线性函数实现的，而且在所有处理的点中共享，保证了卷积操作的不变性(个人理解：即使输入发生了位置变换，卷积核仍可以从相应的位置提取出特征)。卷积核W的计算直观上理解就是表征周围点对当前处理点的影响权重，输出的核大小为$K *C_{in} *C_{out}$；针对点云输入分布不均匀的特点，pointconv先通过核密度估计离线计算了每个点所在位置点云的密度，然后用MLP计算一个密度权重S，与K个近邻点的特征对应相乘后，扩展为W的尺寸然后对应相乘，最后相加得到$F_C{out}$；</font>

<img src="C:\Users\njuwhl2019hp\Desktop\点云\pointconv.jpg" style="zoom: 200%;" />

<font size=4 face="楷体">	上述流程是针对一个点的计算，每换一个点，W就会重新计算，但是两个MLP中学习到的参数是共享的，因此具有一定的参数共享性来保证卷积不变性。不同点的计算可以看作是卷积核的移动。而W大小为$K *C_{in} *C_{out}$，特征$F_{in}$大小为$K *C_{in}$，故可以看作是$C_{out}$个卷积核对特征进行提取。所以整体上就是对卷积的模拟，除了学习周围值对自己的影响外，pointconv加入了S反密度估计来适应点云分布不均匀的特点。如果点云分布就像图像一样完全均匀的话，S值都是一样的，W也类似，也就退化为传统的卷积了。</font>

<font size=4 face="楷体">	文章之后整体的实验流程与pointnet++类似，只是用pointconv代替了pointnet来进行特征提取。这时就与CNN网络分层提取特征比较相似了。</font>

<font size=4 face="楷体">	为了更好地处理语义分割任务，pointconv还被设计了反卷积操作。其内容就是先插值，然后与之前卷积阶段对应的特征连接起来，对整体做一次卷积。反卷积的目的就是将高维特征映射到较低一层，插值恢复了卷积之前的大小，再卷积可能相当于同时考虑两个维度的特征来得出结果。</font>

<font size=4 face="楷体">	上述pointconv最关键的就是设计了针对一个点的卷积方案，但是对每一个点都要这样计算，比较低效。论文提出了一种利用GPU并行计算特点的pointconv实现方法，从图上直观来看就是用W做过渡把直接相乘替换为了矩阵相乘。</font>



### kpconv

<font size=4 face="楷体">	在KPConv中，输入点云和核点是没有对齐的，它们各自的点的数量都是可以变化的。因此，每一个点的特征向量 ![[公式]](https://www.zhihu.com/equation?tex=f_%7Bi%7D) 会乘以所有核点的权重矩阵，相关系数 ![[公式]](https://www.zhihu.com/equation?tex=h_%7Bik%7D) 取决于输入点和核点之间的相对位置。这就相当于先根据相关系数计算该点的权重矩阵然后与点的特征向量相乘。</font>

<font size=4 face="楷体">	文章先设计了kernel points的形状、分布(与输入点云位置并不一一对应)，然后邻域(一个球体)中每一个点都要与这些kernel points的权重矩阵相乘后加权相加，来得到这一点对应的权重矩阵。相当于从kernel points中通过线性插值得到邻域空间中每一个点的权重矩阵。</font>

<font size=4 face="楷体">	kernel points的分布设计：设计了一个最优化问题来解，其目的是使得kernel points中各点尽量均匀分布，不要重叠，还能覆盖整个采样区域。为达到这一效果，作者引入了排斥量(两个核点间距离越短排斥量越小)与吸引量(我的理解是为了防止优化得到的结果几乎全分布在远离中心点的位置造成覆盖不完全)，然后最小化这个值对kernel points进行初始化。</font>

<font size=4 face="楷体">	接下来文章尝试了将kernel points视为一个可变可学习的参数，但作者指出如果学习一个全局的kernel points(即通过学习确定一个kernel points的具体形状，然后所有卷积层都用这一形状的卷积核进行学习)，其结果并没有比固定设计的kernel points好很多，因此考虑了为每一个卷积层学习一个kernel points的偏移量来达到更好的效果。偏移量的生成由</font>

<font size=4 face="楷体">	在这个思路下，同时训练偏移量生成与偏移后kernel points的权值矩阵，前者的学习率要小一点。这样学习的结果是kernel points的位置会被推离输入点，导致无法对input point产生影响，卷积也就无法实现。</font>

问题：文章中说kernel points被推离input points的原因是`This phenomenon comes from the sparse nature of point clouds, there is empty space around the points`；没太理解为什么稀疏点云会导致这个现象。

<font size=4 face="楷体">	为了解决上述问题，作者增加了loss来训练，这包括对kernel points与距离其最近的input point的距离太远的惩罚，以及一个对不同kernel points作用域重合太多的惩罚。这样训练后得到的kernel points形状一方面会适应input points的形状，另一方面会尽量均匀的分布在这一空间的“关键位置”。</font>

<font size=4 face="楷体">	网络结构：</font>

![](C:\Users\njuwhl2019hp\Desktop\点云\kpconv.jpg)

<font size=4 face="楷体">	可变与固定版本比较：作者通过进行限制kernel points数量K的方法说明了可变kernel points描述性更强(K值很小时，可变kernel points版本效果下降没有固定版本多)。可视化展示表明底层卷积学习到的主要是一些水平、竖直的低级特征，高层的卷积得到的结果就更加精细一点，在这些可视化结果中固定kernel points与可变kernel points并没有什么差异。作者还对于这两个版本的有效感受野进行了比较，发现在室内数据集上可变kernel points能更好地适应检测物体的形状(对中心有影响的点相对集中在整个物体上)。</font>

<font size=4 face="楷体">	总结这两种点云backbone设计方法，pointconv是对卷积核进行了模仿，主要设计了一个适用于点云的、能保证一些卷积性质的方法；其关键在于密度权重的引入以及邻域点权重矩阵的训练(等同于用CNN训练，有点无脑训练的感觉，不过确实是效仿了图像的卷积操作)。kpconv则是采用了插值的思想来设计卷积核，通过先确定卷积核的形状(当然这个形状也可以微调)，然后用插值的思想生成kernel points范围内每一个点的权重矩阵进行计算。</font>

