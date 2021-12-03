`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 
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
reg input_v = 0;
reg [31:0] count = 0;
wire[511:0]Inputbinary= {
    512'b01001010011011110111001101100101011100000110100010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000110000
    };

reg clk = 1'b0;
reg rst = 1'b0;
wire output_v;
wire [255:0] InitialHash,Out_256;


SHA_IHV sha256_IHV (.IHV(InitialHash));
sha256_block sha256_block (
    .clk(clk), .rst(rst),
    .H_in(InitialHash), .M_in(Inputbinary),
    .input_valid(input_v),
    .H_out(Out_256),
    .output_valid(output_v)
);



initial begin
  $display("-----Begin-----");
  counter;
  input_v = 1'b1;
  counter;
  input_v = 1'b0;
  repeat (64) begin
  counter;
  end
  $display("done");
  $finish;
end

task counter;
begin
  #1;
  count =count + 1;
  clk = 1;
  #1;
  clk = 0;
  Output;
end
endtask

task Output;
begin
   $display("%b",input_v);
   $display("%b %h", output_v, Out_256);
end
endtask

endmodule
