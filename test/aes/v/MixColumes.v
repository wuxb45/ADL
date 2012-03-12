`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: wu xing bo
// 
// Create Date:    14:45:22 01/14/2009 
// Design Name: 
// Module Name:    MixColumes 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: MixColumes operation, need 1 cycle
//
//////////////////////////////////////////////////////////////////////////////////
module MixColumes(
	input clock,
	input [31:0] in_columes, // {a[0][i], a[1][i], a[2][i], a[3][i]}
	output [31:0] out_result // {r[0][i], r[1][i], r[2][i], r[3][i]}
    );
	
	wire[7:0] in_split[3:0];
	wire[7:0] b[3:0];
	reg[7:0] result[3:0];
	
	assign in_split[0] = in_columes[7:0];
	assign in_split[1] = in_columes[15:8];
	assign in_split[2] = in_columes[23:16];
	assign in_split[3] = in_columes[31:24];
	
	assign b[0] = {in_split[0][6:0], 1'b0} ^ ({8{in_split[0][7]}} & 8'h1b);
	assign b[1] = {in_split[1][6:0], 1'b0} ^ ({8{in_split[1][7]}} & 8'h1b);
	assign b[2] = {in_split[2][6:0], 1'b0} ^ ({8{in_split[2][7]}} & 8'h1b);
	assign b[3] = {in_split[3][6:0], 1'b0} ^ ({8{in_split[3][7]}} & 8'h1b);
	
	assign out_result = {result[3], result[2], result[1], result[0]};
	
	always @ (posedge clock)
	begin
		result[0] <= b[0] ^ in_split[1] ^ in_split[2] ^ b[3] ^ in_split[3];
		result[1] <= b[1] ^ in_split[2] ^ in_split[3] ^ b[0] ^ in_split[0];
		result[2] <= b[2] ^ in_split[3] ^ in_split[0] ^ b[1] ^ in_split[1];
		result[3] <= b[3] ^ in_split[0] ^ in_split[1] ^ b[2] ^ in_split[2];
	end
	
endmodule
