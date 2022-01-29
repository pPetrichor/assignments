module lab01(a,b,s,y);
	input a,b,s; // 声明3个wire 型 输入 变 量 a,b,和s， 其宽度为1位 。
	output y; // 声明1个wire 型 输出 变 量 y， 其宽度为1位 。

	assign y = (~s&a)|(s&b); // 实 现 电 路 的 逻 辑 功 能 。

endmodule 