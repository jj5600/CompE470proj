module SHA256_tb();
reg [31:0] a_tb;
reg [31:0] b_tb;
reg [31:0] c_tb;
wire [31:0] maj_tb;

majority dut(.a(a_tb),.b(b_tb),.c(c_tb),.maj(maj_tb));

initial 
begin 
#5
a_tb=32'b00000000000000000000000000000001;
b_tb=32'b00000000000000000000000000000010;
c_tb=32'b00000000000000000000000000000100;
#5
$stop;
end
initial 
begin

$monitor("%b",a_tb);
$monitor("%b",b_tb);
$monitor("%b",c_tb);
$monitor("%b",maj_tb);
end
endmodule
