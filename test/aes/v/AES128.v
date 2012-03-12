`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    07:29:40 01/15/2009 
// Design Name: 
// Module Name:    AES128 
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
module AES128(
	input clock,
	input reset,
	input [127:0] in_src,
	input [127:0] in_key,
	output [127:0] out_result,
	output write
    );
	
	reg write_out;
	
	reg [31:0] temp_src[3:0];
	reg [31:0] temp_key[3:0];
	
	reg [5:0] counter;
	reg [1:0] key_input_flag;
	
	wire [127:0] initial_src;
	wire [31:0] input_of_SubBytes[3:0];
	wire [31:0] input_of_MixColumes[3:0];
	wire [31:0] input_of_KeyExpansion;
	
	
	wire [31:0] output_of_SubBytes[3:0];
	wire [31:0] output_of_MixColumes[3:0];
	wire [31:0] output_of_KeyExpansion;
	wire [5:0] key128_round;
	
	SubBytes sub3(
		.clock(clock),
		.in_src(input_of_SubBytes[3]),
		.out_result(output_of_SubBytes[3])
	);
	SubBytes sub2(
		.clock(clock),
		.in_src(input_of_SubBytes[2]),
		.out_result(output_of_SubBytes[2])
	);
	SubBytes sub1(
		.clock(clock),
		.in_src(input_of_SubBytes[1]),
		.out_result(output_of_SubBytes[1])
	);
	SubBytes sub0(
		.clock(clock),
		.in_src(input_of_SubBytes[0]),
		.out_result(output_of_SubBytes[0])
	);
	
	MixColumes mix3(
		.clock(clock),
		.in_columes(input_of_MixColumes[3]),
		.out_result(output_of_MixColumes[3])
	);
	MixColumes mix2(
		.clock(clock),
		.in_columes(input_of_MixColumes[2]),
		.out_result(output_of_MixColumes[2])
	);
	MixColumes mix1(
		.clock(clock),
		.in_columes(input_of_MixColumes[1]),
		.out_result(output_of_MixColumes[1])
	);
	MixColumes mix0(
		.clock(clock),
		.in_columes(input_of_MixColumes[0]),
		.out_result(output_of_MixColumes[0])
	);
	
	KeyExpansion128 key128(
		.clock(clock),
		.round(key128_round),
		.in_key0(input_of_KeyExpansion),
		.out_expaned(output_of_KeyExpansion)
	);
	assign write = write_out;
	assign initial_src = in_src ^ in_key;
	
	assign input_of_SubBytes[3] = {temp_src[3][31:24], temp_src[2][23:16], temp_src[1][15:8], temp_src[0][7:0]};
	assign input_of_SubBytes[2] = {temp_src[2][31:24], temp_src[1][23:16], temp_src[0][15:8], temp_src[3][7:0]};
	assign input_of_SubBytes[1] = {temp_src[1][31:24], temp_src[0][23:16], temp_src[3][15:8], temp_src[2][7:0]};
	assign input_of_SubBytes[0] = {temp_src[0][31:24], temp_src[3][23:16], temp_src[2][15:8], temp_src[1][7:0]};
	
	assign input_of_MixColumes[3] = output_of_SubBytes[3];
	assign input_of_MixColumes[2] = output_of_SubBytes[2];
	assign input_of_MixColumes[1] = output_of_SubBytes[1];
	assign input_of_MixColumes[0] = output_of_SubBytes[0];
	
	
	assign key128_round = (counter[5:0] == 6'b111111)? 6'b000000 : counter[5:0];
	
	assign input_of_KeyExpansion = 
			key_input_flag == 2'b00 ? (in_key[127:96]) :
			(key_input_flag == 2'b01 ? (in_key[95:64]) :
			(key_input_flag == 2'b10 ? (in_key[63:32]) :
			(in_key[31:0])));
	
	assign out_result = {temp_src[3], temp_src[2], temp_src[1], temp_src[0]};
	
	always @ (posedge clock)
	begin
		counter <= (reset == 1'b1) ? 32'hffffffff : counter + 32'h1;
		case (counter) // set 
		6'h3f : key_input_flag <= 2'b00;
		6'h0 : key_input_flag <= 2'b01; 
		6'h1 : key_input_flag <= 2'b10;
		6'h2 : key_input_flag <= 2'b11;
		default : key_input_flag <= 2'b11;
		endcase
		
		case (counter[1:0])
		2'h1 : temp_key[3] <= output_of_KeyExpansion;
		2'h2 : temp_key[2] <= output_of_KeyExpansion;
		2'h3 : temp_key[1] <= output_of_KeyExpansion;
		2'h0 : temp_key[0] <= output_of_KeyExpansion;
		endcase
		
		// xor operation
		temp_src[3] <= (counter[1:0] == 2'b1) ? 
		(counter == 6'h5 ? initial_src[127:96] : temp_key[3] ^ (counter == 6'h2d ? output_of_SubBytes[3]:output_of_MixColumes[3])) : 
		temp_src[3];
		
		temp_src[2] <= (counter[1:0] == 2'b1) ? 
		(counter == 6'h5 ? initial_src[95:64] : temp_key[2] ^ (counter == 6'h2d ? output_of_SubBytes[2]:output_of_MixColumes[2])) : 
		temp_src[2];
		
		temp_src[1] <= (counter[1:0] == 2'b1) ? 
		(counter == 6'h5 ? initial_src[63:32] : temp_key[1] ^ (counter == 6'h2d ? output_of_SubBytes[1]:output_of_MixColumes[1])) : 
		temp_src[1];
		
		temp_src[0] <= (counter[1:0] == 2'b1) ? 
		(counter == 6'h5 ? initial_src[31:0] : temp_key[0] ^ (counter == 6'h2d ? output_of_SubBytes[0]:output_of_MixColumes[0])) : 
		temp_src[0];
		
		case (counter[5:1])
		5'h17 : write_out <= 1'b1;
		default : write_out <=1'b0;
		endcase
	end

endmodule
