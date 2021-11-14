module SHA256_tb();
reg[31:0] a_tb;
wire[31:0] S0_tb;
l_sigma_zero dut(.a(a_tb),.S0(S0_tb));
initial 
begin 
#5
a_tb=32'b01101111001000000111011101101111;
#10
a_tb=32'b00000000000000000000000000000001;
#5
$stop;
end 
initial
begin 
$monitor("%b",a_tb);
$monitor("%b",S0_tb);
end
endmodule

