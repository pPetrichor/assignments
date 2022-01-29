module lab00(
			input A,
			input B,
			output F
			);
	assign F = ~A&B | A&~B;
endmodule 