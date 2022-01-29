module mydesign(clk_50,PS,if_shift,sound,off);
input [7:0] PS;
input if_shift,clk_50,off;
output reg [15:0] sound;

always @(posedge clk_50)
begin
	
	if(off) sound = 0;
	else
	begin
	case(PS)
	8'h15:
	begin
	if(if_shift) sound = 189;
	else sound = 178;
	end
	8'h1d:
	begin
	if(if_shift) sound = 212;
	else sound = 200;
	end
	8'h24:
	sound = 225;
	8'h2d:
	begin
	if(if_shift) sound = 252;
	else sound = 238;
	end
	8'h2c:
	begin
	if(if_shift) sound = 283;
	else sound = 267;
	end
	8'h35:
	begin
	if(if_shift) sound = 318;
	else sound = 300;
	end
	8'h3c:
	sound = 337;
	8'h1c:
	begin
	if(if_shift) sound = 378;
	else sound = 357;
	end
	8'h1b:
	begin
	if(if_shift) sound = 424;
	else sound = 400;
	end
	8'h23:
	sound = 450;
	8'h2b:
	begin
	if(if_shift) sound = 505;
	else sound = 476;
	end
	8'h34:
	begin
	if(if_shift) sound = 567;
	else sound = 535;
	end
	8'h33:
	begin
	if(if_shift) sound = 636;
	else sound = 600;
	end
	8'h3b:
	sound = 674;
	8'h1a:
	begin
	if(if_shift) sound = 756;
	else sound = 714;
	end
	8'h22:
	begin
	if(if_shift) sound = 849;
	else sound = 801;
	end
	8'h21:
	sound = 900;
	8'h2a:
	begin
	if(if_shift) sound = 1010;
	else sound = 953;
	end
	8'h32:
	begin
	if(if_shift) sound = 1134;
	else sound = 1070;
	end
	8'h31:
	begin
	if(if_shift) sound = 1272;
	else sound = 1201;
	end
	8'h3a:
	sound = 1348;
	8'h16:
	begin
	if(if_shift) sound = 1513;
	else sound = 1428;
	end
	8'h1e:
	begin
	if(if_shift) sound = 1699;
	else sound = 1603;
	end
	8'h26:
	sound = 1800;
	8'h25:
	begin
	if(if_shift) sound = 2020;
	else sound = 1907;
	end
	8'h2e:
	begin
	if(if_shift) sound = 2268;
	else sound = 2140;
	end
	8'h36:
	begin
	if(if_shift) sound = 2545;
	else sound = 2402;
	end
	8'h3d:
	sound = 2697;
	default:sound = 0;
	endcase
	end
end
endmodule
