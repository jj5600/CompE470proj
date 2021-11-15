module SHA256_tb();
reg [31:0] e_tb;
reg [31:0] f_tb;
reg [31:0] g_tb;
wire [31:0] ch_tb;

choose dut(.e(e_tb),.f(f_tb),.g(g_tb),.ch(ch_tb));

initial 
begin 
#5
e_tb=32'b00000000000000000000000000111001;
f_tb=32'b00000000000000000000000000011010;
g_tb=32'b00000000000000000000000001101100;
#5
$stop;
end
initial 
begin

$monitor("%b",e_tb);
$monitor("%b",f_tb);
$monitor("%b",g_tb);
$monitor("%b",ch_tb);
end
endmodule
