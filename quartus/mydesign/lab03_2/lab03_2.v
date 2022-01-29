module lab03_2(cin,a,b,result,carry,overflow,zero);
	input [2:0]cin;
	input [3:0]a, b;
	reg [3:0]b_temp;
	output reg [3:0]result;
	output reg carry,overflow,zero;
	always @(cin or a or b) begin
		overflow = 0;
		carry = 0;
		case(cin)
			0:begin
				{result[1],result[0]} = a[0] + b[0];
				{result[2],result[1]} = a[1] + b[1] + result[1];
				{result[3],result[2]} = a[2] + b[2] + result[2];
				{carry,result[3]} = a[3] + b[3] + result[3];
				overflow = (a[3]==b[3])&&(result[3] != a[3]);
			end
			1:begin
				b_temp = ~b;
				{result[1],result[0]} = a[0] + b_temp[0] + 1;
				{result[2],result[1]} = a[1] + b_temp[1] + result[1];
				{result[3],result[2]} = a[2] + b_temp[2] + result[2];
				{carry,result[3]} = a[3] + b_temp[3] + result[3];
				carry = ~carry;//减法即将a+~b的进位取反
				overflow = (a[3]!=b[3])&&(result[3] != a[3]);
			end
			2:result = ~a;
			3:result = a & b;
			4:result = a | b;
			5:result = a ^ b;
			6:begin
				if(a[3] > b[3]) result = 0;
				else if(a[3] < b[3]) result = 1;
				else result = (a > b);
				
			end
			7:begin
				if(a == b) result = 1;
				else result = 0;
			end

			default:result = 0;//不会出现 
		endcase
		zero = ~(|result);
	end
endmodule 