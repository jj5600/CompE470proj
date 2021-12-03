`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2021 11:48:03 AM
// Design Name: 
// Module Name: SHA256_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////




module SHA256_tb();
reg input_valid = 0;
wire [511:0] M_sha256_abc = {
  256'h6162638000000000000000000000000000000000000000000000000000000000,
  256'h0000000000000000000000000000000000000000000000000000000000000018
};
reg clk = 1'b0;
reg rst = 1'b0;
wire output_valid_256;
wire [255:0] H_0_256, H_out_256;
SHA_IHV sha256_IHV (.IHV(H_0_256));
sha256_block sha256_block (
    .clk(clk), .rst(rst),
    .H_in(H_0_256), .M_in(M_sha256_abc),
    .input_valid(input_valid),
    .H_out(H_out_256),
    .output_valid(output_valid_256)
);

//wire output_valid_512;



// the "abc" test vector, padded
//wire [511:0] M_sha256_abc = {
//  256'h6162638000000000000000000000000000000000000000000000000000000000,
//  256'h0000000000000000000000000000000000000000000000000000000000000018
//};


// the "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq" test vector, padded
wire [1023:0] M_sha256_2block = {
  256'h6162636462636465636465666465666765666768666768696768696A68696A6B,
  256'h696A6B6C6A6B6C6D6B6C6D6E6C6D6E6F6D6E6F706E6F70718000000000000000,
  256'h0000000000000000000000000000000000000000000000000000000000000000,
  256'h00000000000000000000000000000000000000000000000000000000000001C0
};
wire [511:0] M_sha256_2block_a = M_sha256_2block[1023:512];
wire [511:0] M_sha256_2block_b = M_sha256_2block[511:0];

// a null message
wire [511:0] M_sha256_null = {
  256'h8000000000000000000000000000000000000000000000000000000000000000,
  256'h0000000000000000000000000000000000000000000000000000000000000000
};



// driver

reg [31:0] ticks = 0;
//reg clk = 1'b0;
//reg rst = 1'b0;

initial begin
  $display("starting");
  tick;
  input_valid = 1'b1;
  tick;
  input_valid = 1'b0;
  repeat (90) begin
    tick;
  end
  $display("done");
  $finish;
end

task tick;
begin
  #1;
  ticks = ticks + 1;
  clk = 1;
  #1;
  clk = 0;
  dumpstate;
end
endtask

task dumpstate;
begin
  $display("%b %d %h", input_valid, ticks, ticks);
  $display("%b %h", output_valid_256, H_out_256);
  
end
endtask

endmodule

