# <font color=green face="楷体">			leetcode——两数之和</font>

### <font face="楷体">题目</font>

<font face="楷体">给定一个整数数组 `nums` 和一个目标值 `target`，请你在该数组中找出和为目标值的那**两个**整数，并返回他们的数组下标。你可以假设每种输入只会对应一个答案。但是，你不能重复利用这个数组中同样的元素。</font>

示例:

```c++
给定 nums = [2, 7, 11, 15], target = 9
因为 nums[0] + nums[1] = 2 + 7 = 9
所以返回 [0, 1]
```





### <font face="楷体">第一次题解</font>

<font size=4 color=#48d1cc  face="楷体">最简单的思路，即遍历，每次找`nums`中是否有target - 当前元素的值，若有且两个元素不是同一个，则输出，附代码：</font>

```c++
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int need=0;
        for(int i=0;i<nums.size();i++)
        {
            need = target - nums[i];
            for(int j=0;j<nums.size();++j)
                {
                    if(nums[j] == need && i != j)
                        return {i,j};
                }
        }
        return {-1,-1};
    }
};
```





### <font face="楷体">官方题解</font>

1、<font size=4 color=#48d1cc  face="楷体">暴力法:</font>

<font size=4 color=#48d1cc  face="楷体">	同样是暴力法，不过参考代码为：</font>

```c++
class Solution {
    public int[] twoSum(int[] nums, int target) {
        for (int i = 0; i < nums.length; i++) {
            for (int j = i + 1; j < nums.length; j++) {
                if (nums[j] == target - nums[i]) {
                    return new int[] { i, j };
                }
            }
        }
        throw new IllegalArgumentException("No two sum solution");
    }
}

```

​	<font size=4 color=#48d1cc  face="楷体">可以看到第一次题解的方法还是有不当，即a与b相加不是target就无需再次比较了。</font>

2、 <font size=4 color=#00bfff  face="楷体">利用哈希表进行实现:</font>

<font size=4 color=#00bfff  face="楷体">简单来说，就是建立一个哈希表，将nums中的n个元素放到此哈希表中，使得每次查找元素`target - 当前元素`时可以在O(1)左右的时间内完成，这样整个的时间复杂度就会下降到O(n)；**这里在leetcode上有一点，就是如果存在nums为{2，2，3……}且target为4这样的话，答案应该为{2，2}；利用stl自带的hashmap好像会出现问题？这里没有太多了解，不过按照数据结构学过的处理冲突的方法，往后放就可以了，不会有冲突**；所以问题就是要找一个好一点的hash函数来进行转换了;</font>

<font size=4 color=#9932cc face="楷体">官方解答为（貌似是java写法？）：</font>

```c++
class Solution {
    public int[] twoSum(int[] nums, int target) {
        Map<Integer, Integer> map = new HashMap<>();
        for (int i = 0; i < nums.length; i++) {
            map.put(nums[i], i);
        }
        for (int i = 0; i < nums.length; i++) {
            int complement = target - nums[i];
            if (map.containsKey(complement) && map.get(complement) != i) {
                return new int[] { i, map.get(complement) };
            }
        }
        throw new IllegalArgumentException("No two sum solution");
    }
}
```

<font size=4 color=#9932cc face="楷体">	这样，相当于遍历了两遍；当然，也可以边放边查，遍历一次:</font>

```java
class Solution {
    public int[] twoSum(int[] nums, int target) {
        Map<Integer, Integer> map = new HashMap<>();
        for (int i = 0; i < nums.length; i++) {
            int complement = target - nums[i];
            if (map.containsKey(complement)) {
                return new int[] { map.get(complement), i };
            }
            map.put(nums[i], i);
        }
        throw new IllegalArgumentException("No two sum solution");
    }
}
```

<font size=4 color=#9932cc face="楷体">	按照自己理解来说，先尝试了一下除留取余法，hash函数就简单地选取了：构建一个长度为n的hash数组，然后利用nums[i]%n的方式放置元素，如果冲突就加一，这样去执行；这里由于需要返回原数组的元素所在位置，而在hash里反向计算不一定会正确对应，所以改用结构体进行存储；</font>

<font size=4  face="楷体">	之后的思路就很好说明了，直接上代码：</font>

```c++
struct hash_value
{
    int val;
    int loc;
};
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int need=0,n = nums.size();
        hash_value *hash = new hash_value[2*n];
        hash_value temp;
        temp.val = -1;temp.loc = -1;
        for(int i=0;i<(2*n);i++)
        {
            /*need = target - nums[i];
            for(int j=0;j<nums.size();++j)
                {
                    if(nums[j] == need && i != j)
                        return {i,j};
                }
            */
            hash[i] = temp;
        }
        for(int i=0;i<n;++i)
        {
            int loc = nums[i]%(2*n);
            if(loc < 0) while(loc < 0) loc += n;
            if(hash[loc].loc == -1) {hash[loc].val = nums[i];hash[loc].loc = i;}
            else 
            {
                loc = (loc + 1) % (2*n);
                while(hash[loc].loc != -1) loc = (loc + 1) % (2*n);
                hash[loc].val = nums[i];hash[loc].loc = i;
            }
        }
        for(int i=0;i<n;++i)
        {
            need = target - nums[i];
            int x = need%(2*n);
            if(x < 0) while(x < 0) x += n;
            int x0 = x;
            if(hash[x].val == need && hash[x].loc != i && hash[x].loc != -1)
                return {i,hash[x].loc};
            x = (x + 1) % (2*n);
            while(hash[x].val != need && x != x0) 
            {
                 x = (x + 1) % (2*n);
            }
            if(x != x0 && hash[x].loc != i && hash[x].loc != -1) return {i,hash[x].loc}; 
        }
        return {-1,-1};
    }
};
```

<font size=4 face="楷体">这里采用的直接是长度为2n的哈希数组，需要注意的有这么几点</font>：

1、<font size=4 face="楷体">初始化时将val与loc均置为-1，判断hash中暂无元素存放时应该用loc为-1判断，因为nums中也可能有值为-1；
2、注意如果hash数组存储到尾部了，需要返回到首部存储，也即不是简单的++，而是 loc = (loc + 1) % (2*n);
3、注意负数在取余运算时结果可能还是负数，所以可能导致数组越界的问题，所以加上了`if(loc < 0) while(loc < 0) loc += n;`这样的操作；</font>

<font face="楷体" size=4>**选用2n是在选用n后尝试的，或许可以空间换时间？？？**，然而被打脸：</font>

![image-20200208214834525](C:\Users\njuwhl2019hp\AppData\Roaming\Typora\typora-user-images\image-20200208214834525.png)

<font size=4 face="楷体">从下到上通过的分别是暴力、n与2n,发现自己的hash构造好慢……可能的原因是数据复杂度较小，数据量小，体现不出hash的优势？？？于是我按照教程的方法测了一下，结果用时3ms……
	我觉得这个应该是java中自带hashmap实现的优化，于是又了解了一下：https://blog.csdn.net/qingtian_1993/article/details/80763381，大概意思就是如果直接命中则访问，否则改为红黑树存储访问，大概就是这样的意思……</font>

<font size=4 color=green face="楷体"><u>总结</u>：二次循环时注意不要做重复的配对工作；了解一些已经封装好的工具例如hashmap，C++中也要尝试使用map</font>

