module lab04_2(
	input clk,
	input [3:0] SW,
	output [1:0] LEDR
);
mysynchro m1(clk,SW[0],LEDR[0],SW[1]);
myasynchro m2(clk,SW[2],LEDR[1],SW[3]);
endmodule
