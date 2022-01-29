module scan_HEX(input clk, input write, input [7:0] sc,
	output	[7:0]ascii, output wren);

	reg [7:0]ascii_big [255:0];
	
	initial
	begin
		$readmemh("./ascii_big.txt", ascii_big, 0, 255);
	end
	
	assign ascii = ascii_big[sc];
	assign wren = write && (ascii != 8'h00);
	
endmodule 