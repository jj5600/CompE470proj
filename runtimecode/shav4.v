`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:
// Design Name: 
// Module Name: SHA_256
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


module SHA_256(

    );
endmodule


//**----------------------------------CODE FROM EXTERIOR SOURCE-------------------------------
// fractional part of cube root of the first 64 prime numbers.

module SHA256_K_machine (
    input clk,
    input rst,
    output [31:0] K
    );

reg [2047:0] rom_q;
wire [2047:0] rom_d = { rom_q[2015:0], rom_q[2047:2016] };
assign K = rom_q[2047:2016];

always @(posedge clk)
begin
    if (rst) begin
        rom_q <= {
            32'h428a2f98, 32'h71374491, 32'hb5c0fbcf, 32'he9b5dba5,
            32'h3956c25b, 32'h59f111f1, 32'h923f82a4, 32'hab1c5ed5,
            32'hd807aa98, 32'h12835b01, 32'h243185be, 32'h550c7dc3,
            32'h72be5d74, 32'h80deb1fe, 32'h9bdc06a7, 32'hc19bf174,
            32'he49b69c1, 32'hefbe4786, 32'h0fc19dc6, 32'h240ca1cc,
            32'h2de92c6f, 32'h4a7484aa, 32'h5cb0a9dc, 32'h76f988da,
            32'h983e5152, 32'ha831c66d, 32'hb00327c8, 32'hbf597fc7,
            32'hc6e00bf3, 32'hd5a79147, 32'h06ca6351, 32'h14292967,
            32'h27b70a85, 32'h2e1b2138, 32'h4d2c6dfc, 32'h53380d13,
            32'h650a7354, 32'h766a0abb, 32'h81c2c92e, 32'h92722c85,
            32'ha2bfe8a1, 32'ha81a664b, 32'hc24b8b70, 32'hc76c51a3,
            32'hd192e819, 32'hd6990624, 32'hf40e3585, 32'h106aa070,
            32'h19a4c116, 32'h1e376c08, 32'h2748774c, 32'h34b0bcb5,
            32'h391c0cb3, 32'h4ed8aa4a, 32'h5b9cca4f, 32'h682e6ff3,
            32'h748f82ee, 32'h78a5636f, 32'h84c87814, 32'h8cc70208,
            32'h90befffa, 32'ha4506ceb, 32'hbef9a3f7, 32'hc67178f2
        };
    end else begin
        rom_q <= rom_d;
    end
end

endmodule

module sha256_block (
    input clk, rst,
    input [255:0] H_in,
    input [511:0] M_in,
    input input_valid,
    output [255:0] H_out,
    output output_valid
    );

reg [6:0] round;
wire [31:0] a_in = H_in[255:224], b_in = H_in[223:192], c_in = H_in[191:160], d_in = H_in[159:128];
wire [31:0] e_in = H_in[127:96], f_in = H_in[95:64], g_in = H_in[63:32], h_in = H_in[31:0];
reg [31:0] a_q, b_q, c_q, d_q, e_q, f_q, g_q, h_q;
wire [31:0] a_d, b_d, c_d, d_d, e_d, f_d, g_d, h_d;
wire [31:0] W_tm2, W_tm15, s1_Wtm2, s0_Wtm15, Wj, Kj;
assign H_out = {
    a_in + a_q, b_in + b_q, c_in + c_q, d_in + d_q, e_in + e_q, f_in + f_q, g_in + g_q, h_in + h_q
};
assign output_valid = round == 64;

always @(posedge clk)
begin
    if (input_valid) begin
        a_q <= a_in; b_q <= b_in; c_q <= c_in; d_q <= d_in;
        e_q <= e_in; f_q <= f_in; g_q <= g_in; h_q <= h_in;
        round <= 0;
    end else begin
        a_q <= a_d; b_q <= b_d; c_q <= c_d; d_q <= d_d;
        e_q <= e_d; f_q <= f_d; g_q <= g_d; h_q <= h_d;
        round <= round + 1;
    end
end

sha_all sha_al (
    .Kn(Kj), .Tn(Wj),
    .A_i(a_q), .B_i(b_q), .C_i(c_q), .D_i(d_q),
    .E_i(e_q), .F_i(f_q), .G_i(g_q), .H_i(h_q),
    .A_o(a_d), .B_o(b_d), .C_o(c_d), .D_o(d_d),
    .E_o(e_d), .F_o(f_d), .G_o(g_d), .H_o(h_d)
);

s_sigma_zero sha256_s0(.W(W_tm15),.s0(s0_Wtm15));
s_sigma_one sha256_s1(.W1(W_tm2),.s1(s1_Wtm2));
W_messageprep W_machine(
    .clk(clk),
    .M(M_in), .M_stat(input_valid),
    .Wtm2(W_tm2), .Wtm15(W_tm15),
    .ss1_t2(s1_Wtm2),.ss0_t15(s0_Wtm15),
    .W(Wj)
);

SHA256_K_machine sha256_K_machine (
    .clk(clk), .rst(input_valid), .K(Kj)
);

endmodule
//-------------------------------------------------------------------------------------------------
//sha_all pulls all the modules together to and has all their inputs and outputs centralized
//----** END OF code from exterior source----------------------------------------------------------
module sha_all(
    input [31:0] Kn,Tn,A_i,B_i,C_i,D_i,E_i,F_i,G_i,H_i,
    output[31:0] A_o,B_o,C_o,D_o,E_o,F_o,G_o,H_o
                );
     wire [31:0] Chs,Maj,LS1,LS0;
     

     choose Ch(.e(E_i),.f(F_i),.g(G_i),.ch(Chs)); //choose module
     majority Ma(.a(A_i),.b(B_i),.c(C_i),.maj(Maj)); //majority module
     l_sigma_zero lsz(.a(A_i),.S0(LS0)); //large sigma zero module
     l_sigma_one lso(.e(E_i),.S1(LS1)); // large sigma one module
     gen_comp generalcomp( //general compression with appropriate inputs & outputs
        .Kn(Kn),.Tn(Tn),.A_i(A_i),.B_i(B_i),.C_i(C_i),.D_i(D_i),.E_i(E_i),.F_i(F_i),.G_i(G_i),.H_i(H_i),
        .A_o(A_o),.B_o(B_o),.C_o(C_o),.D_o(D_o),.E_o(E_o),.F_o(F_o),.G_o(G_o),.H_o(H_o),
        .Chs(Chs),.Maj(Maj),.LS1(LS1),.LS0(LS0));
        
  endmodule 
//-------------------------------------------------------------------------- 
//------------------------------------------------------------------------------------
//this module is message preparation 
// takes 512 bit block and breaks it into 16 32 bit words 
// uses the function to create 64 total words ... 
//uses formula Wnext=smallsig1(t-2)+(t-7)+smallsig0(t-15)+t-16
// small sig values are calculted in their respective modules and inputted into this one
// for first cycle starts with message inputs , then pulls from old message + new part


module W_messageprep (
    input clk,
    input[511:0] M,//entire 512 bit message
    input M_stat, //status 
    input[31:0] ss1_t2,//input from ss1(t-2) 
    input[31:0] ss0_t15,//input from ss0(t-15)
    output[31:0] Wtm2,//output to ss1- Wtm2,
    output[31:0] Wtm15,//output to ss0- Wtm15,
    output[31:0] W//output- W 
    );
   
    reg[511:0] W_inter;//register for the entire block- W_inter;
    assign Wtm2 =W_inter[63:32];//Gets word #2;
    assign Wtm15=W_inter[479:448];//Gets word #15
    wire[31:0] W_7=W_inter[223:192];//Gets word #7
    wire[31:0] W_16=W_inter[511:480];//Gets word #16
    wire [31:0] W_nxt=ss1_t2 + W_7 + ss0_t15 + W_16; // gets next message W_nxt=ss1_t2 + W_7 + ss0_t15 + W_16;
    assign W=W_inter[511:480];//assigns W to the 16th word
  
    wire[511:0] W_inter2={W_inter[479:0],W_nxt};// intermediary holds the next message shifts down 1 and appends new val
    always @(posedge clk)
    begin 
        if(M_stat) begin
            W_inter <= M; // gets original message during first run
        end else begin 
            W_inter <= W_inter2; // gets new message with added shift
        end
    end 
endmodule
//-------------------------------------------------------------------------------------------------------
 
//----------------------------------------------------------
// generalized compression function 
//T1= LS1(E)+CHoose(e,f,g)+h+k(n)+t(n)
//T2=LS0(A)+Majority(a,b,c)          
//h=g
//g=f
//e=d+T1
//d=c
//c=b
//b=a
//a=t1+t2
//this module is  compression function defiend by the SHA256 standard
//this compression will be done 64 times 
// this calculates T1,T2
// it also defines A->H based on the standard
module gen_comp(
    input [31:0] Kn,Tn,A_i,B_i,C_i,D_i,E_i,F_i,G_i,H_i,Chs,Maj,LS1,LS0,
    output[31:0] A_o,B_o,C_o,D_o,E_o,F_o,G_o,H_o);
    
    wire [31:0] T1= H_i + LS1+Chs+Kn+Tn;
    wire [31:0] T2= LS0+Maj;
    assign A_o=T1+T2;
    assign B_o=A_i;
    assign C_o=B_i;
    assign D_o=C_i;
    assign E_o=D_i+T1;
    assign F_o=E_i;
    assign G_o=F_i;
    assign H_o=G_i;
    
    endmodule
//------------------------------------------------------------ 
//-----------------------------------------------------------------------------
//module s_sigma_zero 
// smallsigmaZero=((W(i-15)RR(7)) XOR ((W(i-15)(RR(18)) XOR ((W(i-15)RSHIFT(3))
// this is implemented by cutting out part of the array and concatinating it 
// on the other end
// Right rotate by 7
// right rotate by 18
// right shift of 3
// xor all together
module s_sigma_zero(input wire [31:0] W ,output wire [31:0] s0);
assign s0=({W[6:0], W[31:7]}^{W[17:0],W[31:18]}^{(W >> 3)});
endmodule
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// module s_sigma_one 
// smallsigmaZero=((W(i-2)RR(17)) XOR ((W(i-2)(RR(19)) XOR ((W(i-2)RSHIFT(10))
// this is implemented by cutting out part of the array and concatinating it 
// on the other end
// performs a right rotate by 17
// performs a right rotate by 19
// performs a right shift of 10
// all are than XOR'd together 
module s_sigma_one(input wire [31:0] W1,output wire [31:0] s1);
assign s1=({W1[16:0],W1[31:17]}^{W1[18:0],W1[31:19]}^{(W1>>10)});
endmodule
//----------------------------------------------------------------------
//-----------------------------------------------------------------
// Large Sigma Zero
//S0 = (a RR(2)) XOR (a RR(13)) XOR (a RR(22))
// Right Rotate 2
// Right Rotate 13
// Right Rotate 22
// XOR all together
module l_sigma_zero(input wire [31:0] a,output wire[31:0] S0);
assign S0= ({a[1:0],a[31:2]}^{a[12:0],a[31:13]}^{a[21:0],a[31:22]});
endmodule
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Large Sigma One
//S1 = (e RR(6)) XOR (e RR(11)) XOR (e RR(25))
// Right Rotate 6
// Right Rotate 11
// Right Rotate 25
// XOR all together 
module l_sigma_one(input wire[31:0] e,output wire[31:0] S1);
    assign S1= ({e[5:0],e[31:6]}^{e[10:0],e[31:11]}^{e[24:0],e[31:25]});
    endmodule
//-----------------------------------------------------------------
// choose the majority of a , b, c
module majority(
    input wire[31:0]a,b,c,
    output wire[31:0]maj);
    assign maj=(a&b)^(a&c)^(b&c);
    endmodule
//--------------------------------------------------------------------
//--------------------------------------------------------------------
// if E if 1 choose bit in F, if E is 0 choose bit in g. ouput in CH
//Choose
//ch = (e & f) XOR ((~ e) & g)

module choose(
    input wire[31:0] e,f,g,
    output wire[31:0] ch);
    assign ch=((e&f)^((~e)&g));
endmodule
//----------------------------------------------------------------------
//Initial Hash Values:
// 8- 32 bit constants that represent the "the first 32 bits of the
//fractional parts of the square roots of the first 8 prime numbers"
// Standard from RFC6234
//hexadecimal notation
module SHA_IHV(output[255:0] IHV);   
assign IHV ={     
                32'h6A09E667, //fractional part of sqrt(2)
                32'hBB67AE85, //fractional part of sqrt(3)
                32'h3C6EF372, //fractional part of sqrt(5)
                32'hA54FF53A, //fractional part of sqrt(7)
                32'h510E527F, //fractional part of sqrt(11)
                32'h9B05688C, //fractional part of sqrt(13)
                32'h1F83D9AB, //fractional part of sqrt(17)
                32'h5BE0CD19  //fractional part of sqrt(19)
                
            };
endmodule
//-------------------------------------------------------------------------

