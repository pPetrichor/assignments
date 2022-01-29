# <p align="center"><font face="楷体">大数据处理综合实验报告1</p></font>

<p align="right"><font size=4 face="楷体">姓名：王瀚霖 学号：181860092 邮箱：603102048@qq.com</font></p>


### <font face="楷体">一、实验完成度</font>

<font size=3 face="楷体">	完成了单机Hadoop系统安装与Wordcount实验。</font>

### <font face="楷体">二、系统安装运行情况</font>

<font size=3 face="楷体">	在创建的新用户`hadoop`下完成了`Hadoop`的安装、环境变量的配置、免密`ssh`访问、修改配置文件后，运行情况如下：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\1.png" style="zoom:50%;" />

<img src="C:\Users\njuwhl2019hp\Desktop\7.png" style="zoom:50%;" />

<img src="C:\Users\njuwhl2019hp\Desktop\2.png" style="zoom:50%;" />



### <font face="楷体">三、实验数据说明</font>

<font size=3 face="楷体">	实验数据包含5个测试文件，三个`txt`文件两个`html`文件：</font>

- `bulei.txt`：<font size=3 face="楷体">将网页https://cs.nju.edu.cn/bulei/下的内容全选后复制到了`bulei.txt`中</font>

- `file1.txt`：<font size=3 face="楷体">文档内容为`hello hadoop hello world`</font>

- `file2.txt`：<font size=3 face="楷体">文档内容为`goodbye hadoop`</font>

- `california-wildfire-prevention-536-million-newsom-lawmakers.html`：<font size=3 face="楷体">将如下网页以`html`形式下载得到：</font>

  ```html
  https://www.latimes.com/california/story/2021-04-08/california-wildfire-prevention-536-million-newsom-lawmakers
  ```

- `SKEMA strengthens its position in China by creating a joint school with Nanjing University and a master's degree in entrepreneurship and innovation with Xian'Jiaotong University.html`：<font size=3 face="楷体">将如下网页以`html`形式下载得到：</font>

  ```html
  https://www.prnewswire.com/news-releases/skema-strengthens-its-position-in-china-by-creating-a-joint-school-with-nanjing-university-and-a-masters-degree-in-entrepreneurship-and-innovation-with-xianjiaotong-university-301099106.html
  ```

  

### <font face="楷体">四、实验运行结果</font>

<font size=3 face="楷体">	`Hadoop Web`作业状态查看界面上作业运行状态：</font>

![](C:\Users\njuwhl2019hp\Desktop\3.png)

![](C:\Users\njuwhl2019hp\Desktop\4.png)

<font size=3 face="楷体">	实验输出结果开头部分：</font>

<img src="C:\Users\njuwhl2019hp\Desktop\5.png" style="zoom:50%;" />

<font size=3 face="楷体">	PS：map中间有一次`failed`的原因是虚拟机内存不够大，因占用了太多内存被强制杀死，调整虚拟机内存大小后可以解决；</font>

<img src="C:\Users\njuwhl2019hp\Desktop\6.png" style="zoom:50%;" />



### <font face="楷体">五、实验体会</font>

- <font size=3 face="楷体">`Hadoop`安装过程较为繁琐，其中最棘手的问题是权限问题；</font>
- <font size=3 face="楷体">权限问题主要出现在创建文件、`ssh`自动登录配置时，其中前者一般可以利用`sudo`解决，后者可以通过`chmod`等命令设置权限解决；</font>
- <font size=3 face="楷体">`~/.ssh/`目录权限应该为700，其内部`authorized_keys`权限应该为600；</font>
- <font size=3 face="楷体">执行较大规模数据的`wordcount`时，进程有时会因为占用内存太多被杀死，但是程序最终会成功完成执行；</font>
- <font size=3 face="楷体">配置环境变量时应该在新建的用户目录下新建`.bashrc`或`.bash_profile`，之后进行修改并`source`运行使其生效；</font>
- <font size=3 face="楷体">解决权限问题后，根据实验手册指导，安装与测试过程比较顺利；</font>