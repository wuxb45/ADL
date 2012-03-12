`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: BUAA
// Engineer: wu xing bo
// 
// Create Date:    21:48:35 01/13/2009 
// Design Name: 
// Module Name:    SubBytes 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: SubBytes operation, need 1 cycle
//
//////////////////////////////////////////////////////////////////////////////////
module SubBytes(
    input clock,
    input [31:0] in_src,
    output [31:0] out_result
    );
	
	integer i;
	
	wire[7:0] in_split[3:0];
	reg[7:0] result [3:0];

	assign in_split[3] = in_src[31:24];
	assign in_split[2] = in_src[23:16];
	assign in_split[1] = in_src[15:8];
	assign in_split[0] = in_src[7:0];
	
	assign  out_result[31:24]=result[3];
	assign  out_result[23:16]=result[2];
	assign  out_result[15:8]=result[1];
	assign  out_result[7:0]=result[0];
	
	always @ (posedge clock)
	begin
		for(i =0; i< 4; i=i+1)
		begin
			case(in_split[i])
			8'h0: result[i]<= 8'h63; 8'h1: result[i]<= 8'h7C; 8'h2: result[i]<= 8'h77; 8'h3: result[i]<= 8'h7B;
			8'h4: result[i]<= 8'hF2; 8'h5: result[i]<= 8'h6B; 8'h6: result[i]<= 8'h6F; 8'h7: result[i]<= 8'hC5;
			8'h8: result[i]<= 8'h30; 8'h9: result[i]<= 8'h01; 8'ha: result[i]<= 8'h67; 8'hb: result[i]<= 8'h2B;
			8'hc: result[i]<= 8'hFE; 8'hd: result[i]<= 8'hD7; 8'he: result[i]<= 8'hAB; 8'hf: result[i]<= 8'h76;
			8'h10: result[i]<= 8'hCA; 8'h11: result[i]<= 8'h82; 8'h12: result[i]<= 8'hC9; 8'h13: result[i]<= 8'h7D;
			8'h14: result[i]<= 8'hFA; 8'h15: result[i]<= 8'h59; 8'h16: result[i]<= 8'h47; 8'h17: result[i]<= 8'hF0;
			8'h18: result[i]<= 8'hAD; 8'h19: result[i]<= 8'hD4; 8'h1a: result[i]<= 8'hA2; 8'h1b: result[i]<= 8'hAF;
			8'h1c: result[i]<= 8'h9C; 8'h1d: result[i]<= 8'hA4; 8'h1e: result[i]<= 8'h72; 8'h1f: result[i]<= 8'hC0;
			8'h20: result[i]<= 8'hB7; 8'h21: result[i]<= 8'hFD; 8'h22: result[i]<= 8'h93; 8'h23: result[i]<= 8'h26;
			8'h24: result[i]<= 8'h36; 8'h25: result[i]<= 8'h3F; 8'h26: result[i]<= 8'hF7; 8'h27: result[i]<= 8'hCC;
			8'h28: result[i]<= 8'h34; 8'h29: result[i]<= 8'hA5; 8'h2a: result[i]<= 8'hE5; 8'h2b: result[i]<= 8'hF1;
			8'h2c: result[i]<= 8'h71; 8'h2d: result[i]<= 8'hD8; 8'h2e: result[i]<= 8'h31; 8'h2f: result[i]<= 8'h15;
			8'h30: result[i]<= 8'h04; 8'h31: result[i]<= 8'hC7; 8'h32: result[i]<= 8'h23; 8'h33: result[i]<= 8'hC3;
			8'h34: result[i]<= 8'h18; 8'h35: result[i]<= 8'h96; 8'h36: result[i]<= 8'h05; 8'h37: result[i]<= 8'h9A;
			8'h38: result[i]<= 8'h07; 8'h39: result[i]<= 8'h12; 8'h3a: result[i]<= 8'h80; 8'h3b: result[i]<= 8'hE2;
			8'h3c: result[i]<= 8'hEB; 8'h3d: result[i]<= 8'h27; 8'h3e: result[i]<= 8'hB2; 8'h3f: result[i]<= 8'h75;
			8'h40: result[i]<= 8'h09; 8'h41: result[i]<= 8'h83; 8'h42: result[i]<= 8'h2C; 8'h43: result[i]<= 8'h1A;
			8'h44: result[i]<= 8'h1B; 8'h45: result[i]<= 8'h6E; 8'h46: result[i]<= 8'h5A; 8'h47: result[i]<= 8'hA0;
			8'h48: result[i]<= 8'h52; 8'h49: result[i]<= 8'h3B; 8'h4a: result[i]<= 8'hD6; 8'h4b: result[i]<= 8'hB3;
			8'h4c: result[i]<= 8'h29; 8'h4d: result[i]<= 8'hE3; 8'h4e: result[i]<= 8'h2F; 8'h4f: result[i]<= 8'h84;
			8'h50: result[i]<= 8'h53; 8'h51: result[i]<= 8'hD1; 8'h52: result[i]<= 8'h00; 8'h53: result[i]<= 8'hED;
			8'h54: result[i]<= 8'h20; 8'h55: result[i]<= 8'hFC; 8'h56: result[i]<= 8'hB1; 8'h57: result[i]<= 8'h5B;
			8'h58: result[i]<= 8'h6A; 8'h59: result[i]<= 8'hCB; 8'h5a: result[i]<= 8'hBE; 8'h5b: result[i]<= 8'h39;
			8'h5c: result[i]<= 8'h4A; 8'h5d: result[i]<= 8'h4C; 8'h5e: result[i]<= 8'h58; 8'h5f: result[i]<= 8'hCF;
			8'h60: result[i]<= 8'hD0; 8'h61: result[i]<= 8'hEF; 8'h62: result[i]<= 8'hAA; 8'h63: result[i]<= 8'hFB;
			8'h64: result[i]<= 8'h43; 8'h65: result[i]<= 8'h4D; 8'h66: result[i]<= 8'h33; 8'h67: result[i]<= 8'h85;
			8'h68: result[i]<= 8'h45; 8'h69: result[i]<= 8'hF9; 8'h6a: result[i]<= 8'h02; 8'h6b: result[i]<= 8'h7F;
			8'h6c: result[i]<= 8'h50; 8'h6d: result[i]<= 8'h3C; 8'h6e: result[i]<= 8'h9F; 8'h6f: result[i]<= 8'hA8;
			8'h70: result[i]<= 8'h51; 8'h71: result[i]<= 8'hA3; 8'h72: result[i]<= 8'h40; 8'h73: result[i]<= 8'h8F;
			8'h74: result[i]<= 8'h92; 8'h75: result[i]<= 8'h9D; 8'h76: result[i]<= 8'h38; 8'h77: result[i]<= 8'hF5;
			8'h78: result[i]<= 8'hBC; 8'h79: result[i]<= 8'hB6; 8'h7a: result[i]<= 8'hDA; 8'h7b: result[i]<= 8'h21;
			8'h7c: result[i]<= 8'h10; 8'h7d: result[i]<= 8'hFF; 8'h7e: result[i]<= 8'hF3; 8'h7f: result[i]<= 8'hD2;
			8'h80: result[i]<= 8'hCD; 8'h81: result[i]<= 8'h0C; 8'h82: result[i]<= 8'h13; 8'h83: result[i]<= 8'hEC;
			8'h84: result[i]<= 8'h5F; 8'h85: result[i]<= 8'h97; 8'h86: result[i]<= 8'h44; 8'h87: result[i]<= 8'h17;
			8'h88: result[i]<= 8'hC4; 8'h89: result[i]<= 8'hA7; 8'h8a: result[i]<= 8'h7E; 8'h8b: result[i]<= 8'h3D;
			8'h8c: result[i]<= 8'h64; 8'h8d: result[i]<= 8'h5D; 8'h8e: result[i]<= 8'h19; 8'h8f: result[i]<= 8'h73;
			8'h90: result[i]<= 8'h60; 8'h91: result[i]<= 8'h81; 8'h92: result[i]<= 8'h4F; 8'h93: result[i]<= 8'hDC;
			8'h94: result[i]<= 8'h22; 8'h95: result[i]<= 8'h2A; 8'h96: result[i]<= 8'h90; 8'h97: result[i]<= 8'h88;
			8'h98: result[i]<= 8'h46; 8'h99: result[i]<= 8'hEE; 8'h9a: result[i]<= 8'hB8; 8'h9b: result[i]<= 8'h14;
			8'h9c: result[i]<= 8'hDE; 8'h9d: result[i]<= 8'h5E; 8'h9e: result[i]<= 8'h0B; 8'h9f: result[i]<= 8'hDB;
			8'ha0: result[i]<= 8'hE0; 8'ha1: result[i]<= 8'h32; 8'ha2: result[i]<= 8'h3A; 8'ha3: result[i]<= 8'h0A;
			8'ha4: result[i]<= 8'h49; 8'ha5: result[i]<= 8'h06; 8'ha6: result[i]<= 8'h24; 8'ha7: result[i]<= 8'h5C;
			8'ha8: result[i]<= 8'hC2; 8'ha9: result[i]<= 8'hD3; 8'haa: result[i]<= 8'hAC; 8'hab: result[i]<= 8'h62;
			8'hac: result[i]<= 8'h91; 8'had: result[i]<= 8'h95; 8'hae: result[i]<= 8'hE4; 8'haf: result[i]<= 8'h79;
			8'hb0: result[i]<= 8'hE7; 8'hb1: result[i]<= 8'hC8; 8'hb2: result[i]<= 8'h37; 8'hb3: result[i]<= 8'h6D;
			8'hb4: result[i]<= 8'h8D; 8'hb5: result[i]<= 8'hD5; 8'hb6: result[i]<= 8'h4E; 8'hb7: result[i]<= 8'hA9;
			8'hb8: result[i]<= 8'h6C; 8'hb9: result[i]<= 8'h56; 8'hba: result[i]<= 8'hF4; 8'hbb: result[i]<= 8'hEA;
			8'hbc: result[i]<= 8'h65; 8'hbd: result[i]<= 8'h7A; 8'hbe: result[i]<= 8'hAE; 8'hbf: result[i]<= 8'h08;
			8'hc0: result[i]<= 8'hBA; 8'hc1: result[i]<= 8'h78; 8'hc2: result[i]<= 8'h25; 8'hc3: result[i]<= 8'h2E;
			8'hc4: result[i]<= 8'h1C; 8'hc5: result[i]<= 8'hA6; 8'hc6: result[i]<= 8'hB4; 8'hc7: result[i]<= 8'hC6;
			8'hc8: result[i]<= 8'hE8; 8'hc9: result[i]<= 8'hDD; 8'hca: result[i]<= 8'h74; 8'hcb: result[i]<= 8'h1F;
			8'hcc: result[i]<= 8'h4B; 8'hcd: result[i]<= 8'hBD; 8'hce: result[i]<= 8'h8B; 8'hcf: result[i]<= 8'h8A;
			8'hd0: result[i]<= 8'h70; 8'hd1: result[i]<= 8'h3E; 8'hd2: result[i]<= 8'hB5; 8'hd3: result[i]<= 8'h66;
			8'hd4: result[i]<= 8'h48; 8'hd5: result[i]<= 8'h03; 8'hd6: result[i]<= 8'hF6; 8'hd7: result[i]<= 8'h0E;
			8'hd8: result[i]<= 8'h61; 8'hd9: result[i]<= 8'h35; 8'hda: result[i]<= 8'h57; 8'hdb: result[i]<= 8'hB9;
			8'hdc: result[i]<= 8'h86; 8'hdd: result[i]<= 8'hC1; 8'hde: result[i]<= 8'h1D; 8'hdf: result[i]<= 8'h9E;
			8'he0: result[i]<= 8'hE1; 8'he1: result[i]<= 8'hF8; 8'he2: result[i]<= 8'h98; 8'he3: result[i]<= 8'h11;
			8'he4: result[i]<= 8'h69; 8'he5: result[i]<= 8'hD9; 8'he6: result[i]<= 8'h8E; 8'he7: result[i]<= 8'h94;
			8'he8: result[i]<= 8'h9B; 8'he9: result[i]<= 8'h1E; 8'hea: result[i]<= 8'h87; 8'heb: result[i]<= 8'hE9;
			8'hec: result[i]<= 8'hCE; 8'hed: result[i]<= 8'h55; 8'hee: result[i]<= 8'h28; 8'hef: result[i]<= 8'hDF;
			8'hf0: result[i]<= 8'h8C; 8'hf1: result[i]<= 8'hA1; 8'hf2: result[i]<= 8'h89; 8'hf3: result[i]<= 8'h0D;
			8'hf4: result[i]<= 8'hBF; 8'hf5: result[i]<= 8'hE6; 8'hf6: result[i]<= 8'h42; 8'hf7: result[i]<= 8'h68;
			8'hf8: result[i]<= 8'h41; 8'hf9: result[i]<= 8'h99; 8'hfa: result[i]<= 8'h2D; 8'hfb: result[i]<= 8'h0F;
			8'hfc: result[i]<= 8'hB0; 8'hfd: result[i]<= 8'h54; 8'hfe: result[i]<= 8'hBB; 8'hff: result[i]<= 8'h16;
			endcase
		end
	end
endmodule
