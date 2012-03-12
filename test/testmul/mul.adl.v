module way_select_8_U08(
		i,
		o,
		clock
);

	input [7:0] i;
	output o;
	input clock;

endmodule

module way_select_7_U08(
		i,
		o,
		clock
);

	input [7:0] i;
	output o;
	input clock;

endmodule

module way_select_6_U08(
		i,
		o,
		clock
);

	input [7:0] i;
	output o;
	input clock;

endmodule

module way_select_5_U08(
		i,
		o,
		clock
);

	input [7:0] i;
	output o;
	input clock;

endmodule

module way_select_4_U08(
		i,
		o,
		clock
);

	input [7:0] i;
	output o;
	input clock;

endmodule

module way_select_3_U08(
		i,
		o,
		clock
);

	input [7:0] i;
	output o;
	input clock;

endmodule

module way_select_2_U08(
		i,
		o,
		clock
);

	input [7:0] i;
	output o;
	input clock;

endmodule

module way_select_1_U08(
		i,
		o,
		clock
);

	input [7:0] i;
	output o;
	input clock;

endmodule

module way_join_u08(
		w1,
		w2,
		w3,
		w4,
		w5,
		w6,
		w7,
		w8,
		out,
		clock
);

	input w1;
	input w2;
	input w3;
	input w4;
	input w5;
	input w6;
	input w7;
	input w8;
	output [7:0] out;
	input clock;

endmodule

module shift(
		in,
		high,
		out,
		low,
		clock
);

	input [7:0] in;
	input high;
	output [7:0] out;
	output low;
	input clock;

endmodule

module value_or_zero(
		in,
		set,
		out,
		clock
);

	input [7:0] in;
	input set;
	output [7:0] out;
	input clock;

endmodule

module low(
		o,
		clock
);

	output o;
	input clock;

endmodule

module high(
		o,
		clock
);

	output o;
	input clock;

endmodule

module way_join_8_to_16(
		low,
		high,
		out,
		clock
);

	input [7:0] low;
	input [7:0] high;
	output [15:0] out;
	input clock;

endmodule

module add_c08(
		a,
		b,
		o,
		c,
		clock
);

	input [7:0] a;
	input [7:0] b;
	output [7:0] o;
	output c;
	input clock;

endmodule

module top(
		clock
);

	input clock;

	wire [15:0] mul_out;
	wire [7:0] e_a;
	wire [7:0] e_b;
	dummymul mul (
		.a(e_a),
		.b(e_b),
		.out(mul_out),
		.clock(clock)
	);

	engine e (
		.result(mul_out),
		.a(e_a),
		.b(e_b),
		.clock(clock)
	);

endmodule

module engine(
		result,
		a,
		b,
		clock
);

	input [15:0] result;
	output [7:0] a;
	output [7:0] b;
	input clock;

endmodule

module dummymul(
		a,
		b,
		out,
		clock
);

	input [7:0] a;
	input [7:0] b;
	output [15:0] out;
	input clock;

	wire [7:0] ad1_o;
	wire ad1_c;
	wire [7:0] ad2_o;
	wire ad2_c;
	wire [7:0] ad3_o;
	wire ad3_c;
	wire [7:0] ad4_o;
	wire ad4_c;
	wire [7:0] ad5_o;
	wire ad5_c;
	wire [7:0] ad6_o;
	wire ad6_c;
	wire [7:0] ad7_o;
	wire ad7_c;
	wire [7:0] sh1_out;
	wire sh1_low;
	wire [7:0] sh2_out;
	wire sh2_low;
	wire [7:0] sh3_out;
	wire sh3_low;
	wire [7:0] sh4_out;
	wire sh4_low;
	wire [7:0] sh5_out;
	wire sh5_low;
	wire [7:0] sh6_out;
	wire sh6_low;
	wire [7:0] sh7_out;
	wire sh7_low;
	wire [7:0] sh8_out;
	wire sh8_low;
	wire [7:0] in1_out;
	wire [7:0] in2_out;
	wire [7:0] in3_out;
	wire [7:0] in4_out;
	wire [7:0] in5_out;
	wire [7:0] in6_out;
	wire [7:0] in7_out;
	wire [7:0] in8_out;
	wire [7:0] join_out;
	wire sel1_o;
	wire sel2_o;
	wire sel3_o;
	wire sel4_o;
	wire sel5_o;
	wire sel6_o;
	wire sel7_o;
	wire sel8_o;
	wire zero_o;
	wire [15:0] w8_16_out;
	add_c08 ad1 (
		.a(sh1_out),
		.b(in2_out),
		.o(ad1_o),
		.c(ad1_c),
		.clock(clock)
	);

	add_c08 ad2 (
		.a(sh2_out),
		.b(in3_out),
		.o(ad2_o),
		.c(ad2_c),
		.clock(clock)
	);

	add_c08 ad3 (
		.a(sh3_out),
		.b(in4_out),
		.o(ad3_o),
		.c(ad3_c),
		.clock(clock)
	);

	add_c08 ad4 (
		.a(sh4_out),
		.b(in5_out),
		.o(ad4_o),
		.c(ad4_c),
		.clock(clock)
	);

	add_c08 ad5 (
		.a(sh5_out),
		.b(in6_out),
		.o(ad5_o),
		.c(ad5_c),
		.clock(clock)
	);

	add_c08 ad6 (
		.a(sh6_out),
		.b(in7_out),
		.o(ad6_o),
		.c(ad6_c),
		.clock(clock)
	);

	add_c08 ad7 (
		.a(sh7_out),
		.b(in8_out),
		.o(ad7_o),
		.c(ad7_c),
		.clock(clock)
	);

	shift sh1 (
		.in(in1_out),
		.high(zero_o),
		.out(sh1_out),
		.low(sh1_low),
		.clock(clock)
	);

	shift sh2 (
		.in(ad1_o),
		.high(ad1_c),
		.out(sh2_out),
		.low(sh2_low),
		.clock(clock)
	);

	shift sh3 (
		.in(ad2_o),
		.high(ad2_c),
		.out(sh3_out),
		.low(sh3_low),
		.clock(clock)
	);

	shift sh4 (
		.in(ad3_o),
		.high(ad3_c),
		.out(sh4_out),
		.low(sh4_low),
		.clock(clock)
	);

	shift sh5 (
		.in(ad4_o),
		.high(ad4_c),
		.out(sh5_out),
		.low(sh5_low),
		.clock(clock)
	);

	shift sh6 (
		.in(ad5_o),
		.high(ad5_c),
		.out(sh6_out),
		.low(sh6_low),
		.clock(clock)
	);

	shift sh7 (
		.in(ad6_o),
		.high(ad6_c),
		.out(sh7_out),
		.low(sh7_low),
		.clock(clock)
	);

	shift sh8 (
		.in(ad7_o),
		.high(ad7_c),
		.out(sh8_out),
		.low(sh8_low),
		.clock(clock)
	);

	value_or_zero in1 (
		.in(a),
		.set(sel1_o),
		.out(in1_out),
		.clock(clock)
	);

	value_or_zero in2 (
		.in(a),
		.set(sel2_o),
		.out(in2_out),
		.clock(clock)
	);

	value_or_zero in3 (
		.in(a),
		.set(sel3_o),
		.out(in3_out),
		.clock(clock)
	);

	value_or_zero in4 (
		.in(a),
		.set(sel4_o),
		.out(in4_out),
		.clock(clock)
	);

	value_or_zero in5 (
		.in(a),
		.set(sel5_o),
		.out(in5_out),
		.clock(clock)
	);

	value_or_zero in6 (
		.in(a),
		.set(sel6_o),
		.out(in6_out),
		.clock(clock)
	);

	value_or_zero in7 (
		.in(a),
		.set(sel7_o),
		.out(in7_out),
		.clock(clock)
	);

	value_or_zero in8 (
		.in(a),
		.set(sel8_o),
		.out(in8_out),
		.clock(clock)
	);

	way_join_u08 join (
		.w1(sh1_low),
		.w2(sh2_low),
		.w3(sh3_low),
		.w4(sh4_low),
		.w5(sh5_low),
		.w6(sh6_low),
		.w7(sh7_low),
		.w8(sh8_low),
		.out(join_out),
		.clock(clock)
	);

	way_select_1_U08 sel1 (
		.i(b),
		.o(sel1_o),
		.clock(clock)
	);

	way_select_2_U08 sel2 (
		.i(b),
		.o(sel2_o),
		.clock(clock)
	);

	way_select_3_U08 sel3 (
		.i(b),
		.o(sel3_o),
		.clock(clock)
	);

	way_select_4_U08 sel4 (
		.i(b),
		.o(sel4_o),
		.clock(clock)
	);

	way_select_5_U08 sel5 (
		.i(b),
		.o(sel5_o),
		.clock(clock)
	);

	way_select_6_U08 sel6 (
		.i(b),
		.o(sel6_o),
		.clock(clock)
	);

	way_select_7_U08 sel7 (
		.i(b),
		.o(sel7_o),
		.clock(clock)
	);

	way_select_8_U08 sel8 (
		.i(b),
		.o(sel8_o),
		.clock(clock)
	);

	low zero (
		.o(zero_o),
		.clock(clock)
	);

	way_join_8_to_16 w8_16 (
		.low(join_out),
		.high(sh8_out),
		.out(w8_16_out),
		.clock(clock)
	);

endmodule

