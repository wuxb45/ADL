`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:07:18 01/14/2009 
// Design Name: 
// Module Name:    ShiftRows 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module ShiftRows(
	input clock,
	input [127:0] in_src, // {a[0][0], a[0][1], ...., a[3][3]
	output [127:0] out_result  // {r[0][0], r[0][1], ...., r[3][3]
    );
	reg [31:0] result[3:0];
	wire[31:0] in_split[3:0];
	
	assign in_split[3] = in_src[127:96];
	assign in_split[2] = in_src[95:64];
	assign in_split[1] = in_src[63:32];
	assign in_split[0] = in_src[31:0];
	
	assign out_result = {result[3], result[2], result[1], result[0]};
	
	always @ (posedge clock)
	begin
		result[3] <= in_split[3];
		result[2] <= {in_split[2][23:0], in_split[2][31:24]} ;
		result[1] <= {in_split[1][15:0],in_split[1][31:16]} ;
		result[0] <= {in_split[0][7:0],in_split[0][31:8]} ;
	end
endmodule
