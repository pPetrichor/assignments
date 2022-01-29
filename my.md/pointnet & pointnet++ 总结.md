# <p align="center"><font face="楷体">pointnet & pointnet++ 总结</p></font>

### pointnet

<font size=4 face="楷体">	pointnet希望对输入的3D点云数据直接进行处理，避免将其先转换为类似图像的规格排列再进行处理。</font>

<img src="C:\Users\njuwhl2019hp\Desktop\pointnet.jpg" style="zoom:150%;" />

<font size=4 face="楷体">	考虑到点云的无序性特征，pointnet使用了对称函数maxpooling解决这个问题，这样即使输入数据的顺序发生改变，最终maxpooling时也可以提取出相同的global feature；论文中作者也尝试了其它的对称函数，还尝试了对输入数据进行排序、利用RNN结构进行序列处理这样的方法，但最终效果还是maxpooling好；</font>

<font size=4 face="楷体">	pointnet网络中还加入了两个TNet，其作用是训练两个坐标变换矩阵对输入数据以及提取的feature进行旋转对齐，至于旋转到什么效果，应该是TNet是根据最后的loss训练的，所以只要能有效降低loss，旋转到什么效果不用特别关注。考虑到TNet是起到维度变换的作用，给TNet加上了一个正交权重，使得训练结果尽量为一个正交矩阵。有一点疑惑是第一个TNet已经在空间维度上进行了旋转，感觉没有必要第二次对齐特征，但考虑到训练还是以降低loss为准，感觉加了也可以。论文里也尝试了不使用TNet进行训练，结果比使用TNet稍低，影响不是非常大；</font>

<font size=4 face="楷体">	pointnet可以同时处理识别与分割任务；对于前者只要利用提取出的global feature进行mlp(代码中使用的是fc)即可得到k个分类的得分；对于分割任务，只利用global feature无法区别不同点，所以pointnet将提取到的global feature与前一次卷积得到的64维局部特征(或者说是每个点自己的特征)拼接到一起，交给后续fc层处理，输出每个点的分类概率；</font>

<font size=4 face="楷体">	论文中使用了共享权重的mlp进行特征提取，在代码中作者写的实际上是用大小为1的kernel卷积操作与fc、bn层代替了mlp，我的理解是特征提取使用的就是大小为1的kernel，所以除了共享kernel权值外，提取局部特征只是考虑了当前点，最终的maxpooling是综合了每个点自己的特征得到全局特征。</font>



### pointnet++

<font size=4 face="楷体">	pointnet是提取了每个点的特征然后综合考虑得到全局特征，没有考虑局部patch的特征，pointnet++基于这一点进行了改进。</font>

![](C:\Users\njuwhl2019hp\Desktop\pointnet++.jpg)

<font size=4 face="楷体">	pointnet++的思想为分层提取特征，每次通过FPS算法进行采样，得到一些中心点，再根据设定的半径与采样上限寻找中心点的近邻点作为patch(或者用KNN也行)，完成对所有点的grouping，然后交由pointnet提取出每个patch的特征，作为下一层的输入，这就是sa abstraction的内容。由于FPS算法以及寻找近邻点要计算点之间的距离，pointnet++要求输入数据可以计算一个距离。这样每经过一层sa处理，就相当于提取了不同patch的局部特征。</font>

<font size=4 face="楷体">	这样处理的另一个问题是如果点云分布不均匀，那么通过采集固定数目的近邻点就不合适了。论文中作者提出了MSG与MRG处理这个问题。前者通过使用不同的范围半径进行分组，并将不同范围内pointnet提取的特征拼接到一起得到新的特征，这样做的计算开销很大；后者则将sa提取的特征与以所有点为输入的pointnet提取的特征进行赋权拼接作为新的特征，如果当前patch点云分布稀疏，则sa提取的特征权重也会降低。</font>

<font size=4 face="楷体">	经过若干次sa处理后，就提取到了几个中心点的特征，这些特征就是pointnet++提取出的局部特征。如果进行识别，则将局部特征送到pointnet中，经fc、dropout层得到结果；如果进行分割任务，还需要进行插值操作。具体而言就是把当前层与sa处理前的层拿来，根据前者计算出来的特征值，按照距离倒数为权重对sa处理前的层中的点进行赋值，然后利用unit pointnet(similar to one-by-one convolution in CNNs)进行一系列处理，恢复上一层每个点的特征值，如此操作直到得到最后的预测结果。</font>

<font size=4 face="楷体">	此外，论文中还采取了random input dropout方法，每次的输入会进行随机丢失，然后进行重新采样，以保证鲁棒性。从实验结果来看，对于稀疏点云，不dropout的话效果很差。最终效果最好的是MSG+DP。</font>

