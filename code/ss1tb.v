module SHA256_tb();
reg[31:0] W1_tb;
wire[31:0] s1_tb;
s_sigma_one dut(.W1(W1_tb),.s1(s1_tb));
initial
begin
#5
W1_tb=32'b01101111001000000111011101101111;
$stop;
end
initial 
begin
$monitor("%b",W1_tb);
$monitor("%b",s1_tb);
end
endmodule 
