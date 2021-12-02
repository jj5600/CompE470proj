`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/20/2021 10:57:22 AM
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
// fractional part of cube root of the first 64 prime numbers.
// module sha256_K_machine taken from github.com/sha256.v

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
//--end of code taken from github

//module s_sigma_zero has a rotate right portion
//s0 = (w[i-15] rightrotate 7) xor (w[i-15] rightrotate 18) xor (w[i-15] rightshift 3)
// this is implemented by cutting out part of the array and concatinating it 
// on the other end
//
module s_sigma_zero(input wire [31:0] W ,output wire [31:0] s0);

assign s0=({W[6:0], W[31:7]}^{W[17:0],W[31:18]}^{(W >> 3)});

endmodule
//s1 = (w[i- 2] rightrotate 17) xor (w[i- 2] rightrotate 19) xor (w[i- 2] rightshift 10)
// performs a right rotate by 17
//performs a right rotate by 19
// performs a right shift of 10
// all are than XOR'd together 
module s_sigma_one(input wire [31:0] W1,output wire [31:0] s1);

assign s1=({W1[16:0],W1[31:17]}^{W1[18:0],W1[31:19]}^{(W1>>10)});

endmodule
// Large Sigma Zero
//S0 = (a rightrotate 2) xor (a rightrotate 13) xor (a rightrotate 22)

module l_sigma_zero(input wire [31:0] a,output wire[31:0] S0);
assign S0= ({a[1:0],a[31:2]}^{a[12:0],a[31:13]}^{a[21:0],a[31:22]});
endmodule

//Large Sigma One
//S1 = (e rightrotate 6) xor (e rightrotate 11) xor (e rightrotate 25)
module l_sigma_one(input wire[31:0] e,output wire[31:0] S1);
assign S1= ({e[5:0],e[31:6]}^{e[10:0],e[31:11]}^{e[24:0],e[31:25]});
endmodule

//choose the majority of the 3 inputs (majority)
//maj = (a and b) xor (a and c) xor (b and c)
module majority #(parameter WSIZE=0)(
    input wire[WSIZE:0]a,b,c,
    output wire[WSIZE:0] maj);
    assign maj=(a&b)^(a&c)^(b&c);
    endmodule
//Choose
//ch = (e and f) xor ((not e) and g)
//if e is 1 it chooses f if e is 0 it chooses g
module choose #(parameter WSIZE=0)(
input wire[WSIZE:0]e,f,g,
output wire[WSIZE:0]ch);
assign ch=((e&f)^((~e)&g));

endmodule
//this module is message preparation 
// takes 512 bit block and breaks it into 16 32 bit words 
// uses the function to create 64 total words ... 
//uses formula Wnext=smallsig1(t-2)+(t-7)+smallsig0(t-15)+t-16
// small sig values are calculted in their respective modules and inputted into this one
// for first cycle starts with message inputs , then pulls from old message + new part

module W_messageprep #(parameter WSIZE=1)(
    input clk,
    input[WSIZE*16-1:0]M,
    input M_stat,
    input[WSIZE-1:0] ss1_t2,
    input[WSIZE-1:0] ss0_t15,
    output[WSIZE-1:0] Wtm2,
    output[WSIZE-1:0] Wtm15,
    output[WSIZE-1:0] W 
    );
   

   assign Wtm2 =W_inter[WSIZE*2-1:WSIZE*1];
   assign Wtm15=W_inter[WSIZE*15-1:WSIZE*14];
   wire [WSIZE-1:0] W_7=W_inter[WSIZE*7-1:WSIZE*6];
   wire [WSIZE-1:0] W_16=W_inter[WSIZE*16-1:WSIZE*15];
   
   reg [WSIZE*16-1:0] W_inter;
   wire [WSIZE-1:0] W_nxt=ss1_t2 + W_7 + ss0_t15 + W_16;
   assign W=W_inter[WSIZE*16-1:WSIZE*15];// w_inter == w_stack_q
  
   wire[WSIZE*16-1:0] W_inter2={W_inter[WSIZE*15-1:0],W_nxt};

    always @(posedge clk)
    begin 
        if(M_stat) begin
            W_inter <= M; 
        end else begin 
            W_inter <= W_inter2;
        end
    end 
endmodule
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
module gen_comp #(parameter WORDS=0)(
    input [WORDS-1:0] Kn,Tn,A_i,B_i,C_i,D_i,E_i,F_i,G_i,H_i,Chs,Maj,LS1,LS0,
    output[WORDS-1:0] A_o,B_o,C_o,D_o,E_o,F_o,G_o,H_o);
    
    wire [WORDS-1:0] T1= H_i + LS1+Chs+Kn+Tn;
    wire [WORDS-1:0] T2= LS0+Maj;
    
    assign A_o=T1+T2;
    assign B_o=A_i;
    assign C_o=B_i;
    assign D_o=C_i;
    assign E_o=D_i+T1;
    assign F_o=E_i;
    assign G_o=F_i;
    assign H_o=G_i;
    
    endmodule
//pulls in various modules and connects them 
// via instantiation 
 
module sha_all(
    input [31:0] Kn,Tn,A_i,B_i,C_i,D_i,E_i,F_i,G_i,H_i,
    output[31:0] A_o,B_o,C_o,D_o,E_o,F_o,G_o,H_o
                );
     wire [31:0] Chs,Maj,LS1,LSO;
     
     choose #(.WORDSIZE(32)) Ch(.e(E_i),.f(F_i),.g(H_i),.ch(Chs));
     majority #(.WORDSIZE(32)) Ma(.a(A_i),.b(B_i),.c(C_i),.maj(Maj));
     l_sigma_zero #(.WORDSIZE(32)) lsz(.a(A_i),.S0(LS0));
     l_sigma_one #(.WORDSIZE(32)) lso(.e(E_i),.S1(LS1));
     
     
     gen_comp #(.WORDSIZE(32)) generalcomp(
        .Kn(Kn),.Tn(Tn),.A_i(A_i),.B_i(B_i),.C_i(C_i),.D_i(D_i),.E_i(E_i),.F_i(F_i),.G_i(G_i),.H_i(H_i),
        .A_o(A_o),.B_o(B_o),.C_o(C_o),.D_o(D_o),.E_o(E_o),.F_o(F_o),.G_o(G_o),.H_o(H_o),
        .Chs(Chs),.Maj(Maj),.LS1(LS1),.LS0(LS0));
        
  endmodule 
     
         
    

