module lab05_2(
clk_50,set_time,set_botton_s,set_botton_h,led,set_alarm,
stop_alarm,second_counter,second_l,second_h,
minute_l,minute_h,hour_l,hour_h);
	input clk_50,set_time,set_botton_s,set_botton_h,stop_alarm,second_counter,set_alarm;
	reg clk_second_counter;
	output reg led;
	output reg[6:0]second_l;integer second_ones,second_ones_c,second_ones_a;
	output reg[6:0]second_h;integer second_tens,second_tens_c,second_tens_a;
	output reg[6:0]minute_l;integer minute_ones,minute_ones_c,minute_ones_a;
	output reg[6:0]minute_h;integer minute_tens,minute_tens_c,minute_tens_a;
	output reg[6:0]hour_l;integer hour_ones, hour_ones_c, hour_ones_a;
	output reg[6:0]hour_h;integer hour_tens, hour_tens_c, hour_tens_a;
	reg clk_s;reg [25:0]count_clk;integer count_clk_,num;
	
	always @(posedge clk_50)
		begin
			if(count_clk==25000000)
				begin
				count_clk =0;
				clk_s = ~clk_s;
				end
			else
				count_clk = count_clk+1;
		end
	always @(posedge clk_50)
		begin
			if(count_clk_==250000)
				begin
				count_clk_ =0;
				clk_second_counter = ~clk_second_counter;
				end
			else
				count_clk_ = count_clk_+1;
		end
	always @(posedge clk_s)
		begin
		if(set_time)
		begin
			if(!set_botton_s) 
			begin
			minute_ones = minute_ones + 1;
			if(minute_ones == 10)
				begin
					minute_ones = 0;
					minute_tens = minute_tens + 1;
					if(minute_tens == 6) minute_tens = 0;
					else minute_tens = minute_tens;
				end
			else minute_ones = minute_ones;
			end
			else if(!set_botton_h)
			begin
				hour_ones = hour_ones + 1;
				if(hour_tens == 2 && hour_ones == 4)begin
					hour_tens = 0;hour_ones = 0;end
				else hour_tens = hour_tens;
				if(hour_ones == 10)
					begin
					hour_ones = 0;
					hour_tens = hour_tens + 1;	
					end
				else hour_ones = hour_ones;
			end
			else second_ones = second_ones;
		end
		else 
		begin
		if(second_ones==0&&second_tens==0&&minute_ones == minute_ones_a&&minute_tens==minute_tens_a&&hour_ones==hour_ones_a&&hour_tens==hour_tens_a)
		led = 1;
		else led = led;
		if(stop_alarm) led = 0;else led = led;
		second_ones = second_ones + 1;
		if(second_ones == 10)
		begin
			second_ones = 0;
			second_tens = second_tens + 1;
			if(second_tens == 6)
			begin
				second_tens = 0;
				minute_ones = minute_ones + 1;
				if(minute_ones == 10)
				begin
					minute_ones = 0;
					minute_tens = minute_tens + 1;
					if(minute_tens == 6)
					begin
						minute_tens = 0;
						hour_ones = hour_ones + 1;
						if(hour_tens == 2 && hour_ones == 4)begin
							hour_tens = 0;hour_ones = 0;end
						else hour_tens = hour_tens;
						if(hour_ones == 10)
						begin
							hour_ones = 0;
							hour_tens = hour_tens + 1;	
						end
						else hour_ones = hour_ones;
					end
					else minute_tens = minute_tens;
				end
				else minute_ones = minute_ones;
			end
			else second_tens = second_tens;
		end
		else second_ones = second_ones;
		end
		end
	always @(posedge clk_second_counter)
		begin
		if(!second_counter && !set_alarm)
		begin
		case(second_ones)
				0: second_l = 7'b1000000;
				1: second_l = 7'b1111001;
				2: second_l = 7'b0100100;
				3: second_l = 7'b0110000;
				4: second_l = 7'b0011001;
				5: second_l = 7'b0010010;
				6: second_l = 7'b0000010;
				7: second_l = 7'b1111000;
				8: second_l = 7'b0000000;
				9: second_l = 7'b0010000;
		
			endcase
			case(second_tens)
				0: second_h = 7'b1000000;
				1: second_h = 7'b1111001;
				2: second_h = 7'b0100100;
				3: second_h = 7'b0110000;
				4: second_h = 7'b0011001;
				5: second_h = 7'b0010010;
			
			endcase
			case(minute_ones)
				0: minute_l = 7'b1000000;
				1: minute_l = 7'b1111001;
				2: minute_l = 7'b0100100;
				3: minute_l = 7'b0110000;
				4: minute_l = 7'b0011001;
				5: minute_l = 7'b0010010;
				6: minute_l = 7'b0000010;
				7: minute_l = 7'b1111000;
				8: minute_l = 7'b0000000;
				9: minute_l = 7'b0010000;
		
			endcase
			case(minute_tens)
				0: minute_h = 7'b1000000;
				1: minute_h = 7'b1111001;
				2: minute_h = 7'b0100100;
				3: minute_h = 7'b0110000;
				4: minute_h = 7'b0011001;
				5: minute_h = 7'b0010010;
		
			endcase
			case(hour_ones)
				0: hour_l = 7'b1000000;
				1: hour_l = 7'b1111001;
				2: hour_l = 7'b0100100;
				3: hour_l = 7'b0110000;
				4: hour_l = 7'b0011001;
				5: hour_l = 7'b0010010;
				6: hour_l = 7'b0000010;
				7: hour_l = 7'b1111000;
				8: hour_l = 7'b0000000;
				9: hour_l = 7'b0010000;
		
			endcase
			case(hour_tens)
				0: hour_h = 7'b1000000;
				1: hour_h = 7'b1111001;
				2: hour_h = 7'b0100100;
				3: hour_h = 7'b0110000;
				4: hour_h = 7'b0011001;
				5: hour_h = 7'b0010010;
			endcase
		end
		else if(set_alarm && !second_counter)
		begin
			if(num == 100)
			begin
			num = 0;
			if(!set_botton_s) 
			begin
			minute_ones_a = minute_ones_a + 1;
			if(minute_ones_a == 10)
				begin
					minute_ones_a = 0;
					minute_tens_a = minute_tens_a + 1;
					if(minute_tens_a == 6) minute_tens_a = 0;
					else minute_tens_a = minute_tens_a;
				end
			else minute_ones_a = minute_ones_a;
			end
			else if(!set_botton_h)
			begin
				hour_ones_a = hour_ones_a + 1;
				if(hour_tens_a == 2 && hour_ones_a == 4)begin
					hour_tens_a = 0;hour_ones_a = 0;end
				else hour_tens_a = hour_tens_a;
				if(hour_ones_a == 10)
					begin
					hour_ones_a = 0;
					hour_tens_a = hour_tens_a + 1;	
					end
				else hour_ones_a = hour_ones_a;
			end
			else second_ones_a = second_ones_a;
			end
			else num = num + 1;
			case(second_ones_a)
				0: second_l = 7'b1000000;
				1: second_l = 7'b1111001;
				2: second_l = 7'b0100100;
				3: second_l = 7'b0110000;
				4: second_l = 7'b0011001;
				5: second_l = 7'b0010010;
				6: second_l = 7'b0000010;
				7: second_l = 7'b1111000;
				8: second_l = 7'b0000000;
				9: second_l = 7'b0010000;
		
			endcase
			case(second_tens_a)
				0: second_h = 7'b1000000;
				1: second_h = 7'b1111001;
				2: second_h = 7'b0100100;
				3: second_h = 7'b0110000;
				4: second_h = 7'b0011001;
				5: second_h = 7'b0010010;
			
			endcase
			case(minute_ones_a)
				0: minute_l = 7'b1000000;
				1: minute_l = 7'b1111001;
				2: minute_l = 7'b0100100;
				3: minute_l = 7'b0110000;
				4: minute_l = 7'b0011001;
				5: minute_l = 7'b0010010;
				6: minute_l = 7'b0000010;
				7: minute_l = 7'b1111000;
				8: minute_l = 7'b0000000;
				9: minute_l = 7'b0010000;
		
			endcase
			case(minute_tens_a)
				0: minute_h = 7'b1000000;
				1: minute_h = 7'b1111001;
				2: minute_h = 7'b0100100;
				3: minute_h = 7'b0110000;
				4: minute_h = 7'b0011001;
				5: minute_h = 7'b0010010;
		
			endcase
			case(hour_ones_a)
				0: hour_l = 7'b1000000;
				1: hour_l = 7'b1111001;
				2: hour_l = 7'b0100100;
				3: hour_l = 7'b0110000;
				4: hour_l = 7'b0011001;
				5: hour_l = 7'b0010010;
				6: hour_l = 7'b0000010;
				7: hour_l = 7'b1111000;
				8: hour_l = 7'b0000000;
				9: hour_l = 7'b0010000;
		
			endcase
			case(hour_tens_a)
				0: hour_h = 7'b1000000;
				1: hour_h = 7'b1111001;
				2: hour_h = 7'b0100100;
				3: hour_h = 7'b0110000;
				4: hour_h = 7'b0011001;
				5: hour_h = 7'b0010010;
			endcase
		end
		else 
		begin
		second_ones_c = second_ones_c + 1;
		if(second_ones_c == 10)
		begin
			second_ones_c = 0;
			second_tens_c = second_tens_c + 1;
			if(second_tens_c == 10)
			begin
				second_tens_c = 0;
				minute_ones_c = minute_ones_c + 1;
				if(minute_ones_c == 10)
				begin
					minute_ones_c = 0;
					minute_tens_c = minute_tens_c + 1;
					if(minute_tens_c == 6)
					begin
						minute_tens_c = 0;
						hour_ones_c = hour_ones_c + 1;
						if(hour_ones_c == 10)
						begin
							hour_ones_c = 0;
							hour_tens_c = hour_tens_c + 1;
							if(hour_tens_c == 6) 
								hour_tens_c = 0;
							else hour_tens_c = hour_tens_c;
						end
						else hour_ones_c = hour_ones_c;
					end
					else minute_tens_c = minute_tens_c;
				end
				else minute_ones_c = minute_ones_c;
			end
			else second_tens_c = second_tens_c;
		end
		else second_ones_c = second_ones_c;
		case(second_ones_c)
				0: second_l = 7'b1000000;
				1: second_l = 7'b1111001;
				2: second_l = 7'b0100100;
				3: second_l = 7'b0110000;
				4: second_l = 7'b0011001;
				5: second_l = 7'b0010010;
				6: second_l = 7'b0000010;
				7: second_l = 7'b1111000;
				8: second_l = 7'b0000000;
				9: second_l = 7'b0010000;
		
			endcase
			case(second_tens_c)
				0: second_h = 7'b1000000;
				1: second_h = 7'b1111001;
				2: second_h = 7'b0100100;
				3: second_h = 7'b0110000;
				4: second_h = 7'b0011001;
				5: second_h = 7'b0010010;
				6: second_h = 7'b0000010;
				7: second_h = 7'b1111000;
				8: second_h = 7'b0000000;
				9: second_h = 7'b0010000;
			endcase
			case(minute_ones_c)
				0: minute_l = 7'b1000000;
				1: minute_l = 7'b1111001;
				2: minute_l = 7'b0100100;
				3: minute_l = 7'b0110000;
				4: minute_l = 7'b0011001;
				5: minute_l = 7'b0010010;
				6: minute_l = 7'b0000010;
				7: minute_l = 7'b1111000;
				8: minute_l = 7'b0000000;
				9: minute_l = 7'b0010000;
		
			endcase
			case(minute_tens_c)
				0: minute_h = 7'b1000000;
				1: minute_h = 7'b1111001;
				2: minute_h = 7'b0100100;
				3: minute_h = 7'b0110000;
				4: minute_h = 7'b0011001;
				5: minute_h = 7'b0010010;
		
			endcase
			case(hour_ones_c)
				0: hour_l = 7'b1000000;
				1: hour_l = 7'b1111001;
				2: hour_l = 7'b0100100;
				3: hour_l = 7'b0110000;
				4: hour_l = 7'b0011001;
				5: hour_l = 7'b0010010;
				6: hour_l = 7'b0000010;
				7: hour_l = 7'b1111000;
				8: hour_l = 7'b0000000;
				9: hour_l = 7'b0010000;
		
			endcase
			case(hour_tens_c)
				0: hour_h = 7'b1000000;
				1: hour_h = 7'b1111001;
				2: hour_h = 7'b0100100;
				3: hour_h = 7'b0110000;
				4: hour_h = 7'b0011001;
				5: hour_h = 7'b0010010;
			endcase	
		end
		end
endmodule

	
