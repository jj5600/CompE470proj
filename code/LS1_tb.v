module SHA256_tb();
reg[31:0] e_tb;
wire[31:0] S1_tb;
l_sigma_one dut(.e(e_tb),.S1(S1_tb));
initial 
begin 
#5
e_tb=32'b00000000000000000000000000000001;
#5
$stop;
end 
initial
begin 
$monitor("%b",e_tb);
$monitor("%b",S1_tb);
end
endmodule

