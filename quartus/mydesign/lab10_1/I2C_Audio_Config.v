module I2C_Audio_Config ( clk_i2c,
                          reset_n,
								  vol_cnt,
								  I2C_SCLK,
								  I2C_SDAT,
								  testbit);
   parameter  total_cmd = 9;

	input clk_i2c;  //10k I2C clock
   input reset_n;
   
   output I2C_SCLK;
	output [2:0] testbit;
   inout  I2C_SDAT;

	reg [23:0] mi2c_data;
	reg  mi2c_go;
	wire mi2c_end;
	reg  [1:0] mi2c_state; //state 0: stop, state 1: sendnext;
	                       //state 2: wait for finish, state 3:move index
	wire [2:0] mi2c_ack;
	wire [7:0] audio_addr;
	
	reg [3:0] cmd_count;
	reg [6:0] audio_reg [15:0]; //register to write
	reg [8:0] audio_cmd [15:0]; //register content
	
	input [2:0]vol_cnt;
	reg [9:0]vol;
	
initial
begin
   audio_reg[0]= 7'h0f; audio_cmd[0]=9'h0;  //reset
	audio_reg[1]= 7'h06; audio_cmd[1]=9'h0;  //Disable Power Down
	audio_reg[2]= 7'h08; audio_cmd[2]=9'h2;  //Sampling Control
	audio_reg[3]= 7'h02; audio_cmd[3]=9'd76; //Left Volume  1111111(127) max 1111001 0dB 0110000(48) min less:mute
	audio_reg[4]= 7'h03; audio_cmd[4]=9'd76; //Right Volume
	audio_reg[5]= 7'h07; audio_cmd[5]=9'h1;  //I2S format
	audio_reg[6]= 7'h09; audio_cmd[6]=9'h1;  //Active
	audio_reg[7]= 7'h04; audio_cmd[7]=9'h16; //Analog path
	audio_reg[8]= 7'h05; audio_cmd[8]=9'h06;  //Digital path
	vol = 9'd77;
end


assign audio_addr={7'b0011010,1'b0}; //WM8731 addr, always write
//assign testbit = cmd_count[2:0];
assign testbit = vol_cnt;

I2C_Controller 	u0	(	.CLOCK(clk_i2c),		//	Controller Work Clock
						.I2C_SCLK(I2C_SCLK),		//	I2C CLOCK
 	 	 	 	 	 	.I2C_SDAT(I2C_SDAT),		//	I2C DATA
						.I2C_DATA(mi2c_data),		//	DATA:[SLAVE_ADDR,SUB_ADDR,DATA]
						.GO(mi2c_go),      			//	GO transfor
						.END(mi2c_end),				//	END transfor 
						.ACK(mi2c_ack),				//	ACK
						.RESET_N(reset_n)	);			

always @ (posedge clk_i2c)
begin
	case(vol_cnt)
	0:vol = 9'd46;
	1:vol = 9'd56;
	2:vol = 9'd66;
	3:vol = 9'd76;
	4:vol = 9'd86;
	5:vol = 9'd96;
	6:vol = 9'd106;
	7:vol = 9'd116;
	endcase 
end

always @ (posedge clk_i2c or negedge reset_n)
begin
	  
     if(!reset_n)
	  begin
	     cmd_count  <= 4'b0;
		  mi2c_state <= 4'b0;
	     mi2c_go    <= 1'b0;
	  end
	  else
	  begin
		  if(cmd_count == 4'd3 || cmd_count == 4'd4)
				audio_cmd[cmd_count] <= vol;
	     case(mi2c_state)
		  2'd0: begin  //stop
		            if(cmd_count ==4'b0)
						   mi2c_state <= 2'd1;
		        end
		  2'd1: begin
		            mi2c_data <= {audio_addr, audio_reg[cmd_count], audio_cmd[cmd_count]};
						mi2c_go   <= 1'b1;
						mi2c_state<= 2'd2;
		        end
		  2'd2: begin
		             if(mi2c_end)
						 begin
						        mi2c_state <= 2'd3;
								  mi2c_go    <= 1'b0;
						 end
		        end
		  2'd3: begin	
		            cmd_count <= cmd_count + 4'd1;
						if(cmd_count + 4'd1 < total_cmd)
						   mi2c_state <= 2'd1;  //start next
					   else
						   mi2c_state <= 2'd0;  //last cmd
		        end
		  endcase
		  
	  end
	  
end						  
endmodule 