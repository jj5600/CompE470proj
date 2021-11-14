module SHA256_tb();

reg [31:0] W_tb;
wire [31:0] s0_tb;
//----------
s_sigma_zero dut(.W(W_tb),.s0(s0_tb));
initial 
begin 
#5

W_tb=32'b01101111001000000111011101101111;
#10
$stop;
end
initial 
begin
$monitor("%b",W_tb);
$monitor("%b",s0_tb);
end



Endmodule
