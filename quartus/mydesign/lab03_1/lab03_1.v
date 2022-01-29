module lab03_1(a,b,cin,carry,result,zero,overflow);
	input [3:0]a;
	input [3:0]b;
	reg [3:0]b_temp;
	input cin;
	output reg carry;
	output reg[3:0]result;
	output reg zero;
	output reg overflow;
	always @(a or b or cin)
	begin
		if(cin==0)//加
		begin
			{result[1],result[0]} = a[0] + b[0];
			{result[2],result[1]} = a[1] + b[1] + result[1];
			{result[3],result[2]} = a[2] + b[2] + result[2];
			{carry,result[3]} = a[3] + b[3] + result[3];
			overflow = (a[3]==b[3])&&(result[3] != a[3]);
		end
		else begin//减
			b_temp = ~b;
			{result[1],result[0]} = a[0] + b_temp[0] + 1;
			{result[2],result[1]} = a[1] + b_temp[1] + result[1];
			{result[3],result[2]} = a[2] + b_temp[2] + result[2];
			{carry,result[3]} = a[3] + b_temp[3] + result[3];
			carry = ~carry;//减法即将a+~b的进位取反
			overflow = (a[3]!=b[3])&&(result[3] != a[3]);		
		end
		zero = ~(|result);
	end
endmodule 