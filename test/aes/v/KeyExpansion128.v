`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:06:11 01/14/2009 
// Design Name: 
// Module Name:    KeyExpansion128 
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
module KeyExpansion128(
	input clock,
	input [5:0] round, // 0--176
	input [31:0] in_key0,
	output [31:0] out_expaned
    );
	
	reg  [31:0] wl; // i-4?
	reg  [31:0] w1; // 
	reg  [31:0] w2; // 
	reg  [31:0] wr; // i-1
	
	wire [31:0] t_SubBytes;
	
	wire [31:0] t_rotWord;
	reg  [7:0] t_Rcon;
	wire [31:0] t_final;
	
	wire [31:0] prepare;
	reg prepare_value;
	reg [31:0] result;
	
	SubBytes SubBytes1(
		.clock(clock),
		.in_src(t_rotWord),
		.out_result(t_SubBytes)
		);
	
	assign prepare = (round == 6'b0||prepare_value == 1'b0) ? in_key0 : 
		((round[1:0] == 2'b0) ? wr ^ t_final : wr ^ result);
	
	assign t_rotWord = {prepare[23:0], prepare[31:24]};
	assign t_final = {(t_Rcon ^ t_SubBytes[31:24]), t_SubBytes[23:0]};
	
	assign out_expaned = result;
	always @ (posedge clock)
	begin
		
		case(round[5:2])
		4'h0 : t_Rcon <= 8'h1;
		4'h1 : t_Rcon <= 8'h2;
		4'h2 : t_Rcon <= 8'h4;
		4'h3 : t_Rcon <= 8'h8;
		4'h4 : t_Rcon <= 8'h10;
		4'h5 : t_Rcon <= 8'h20;
		4'h6 : t_Rcon <= 8'h40;
		4'h7 : t_Rcon <= 8'h80;
		4'h8 : t_Rcon <= 8'h1b;
		4'h9 : t_Rcon <= 8'h36;
		default : t_Rcon <= 8'h1;
		endcase
		
		wl <= prepare;
		case(round)
		6'h0 , 6'h1, 6'h2: prepare_value <= 1'b0;
		default : prepare_value <= 1'b1;
		endcase
		
		case(round[1:0])
		2'h0 :    result <= (round[5:2] == 4'b0) ? in_key0 : wr ^ t_final; 
		default : result <= (round[5:2] == 4'b0) ? in_key0 : wr ^ result;
		endcase
		
		w1 <= wl;
		w2 <= w1;
		wr <= w2;
	end
endmodule
