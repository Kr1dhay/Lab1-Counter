module counter # (parameter WIDTH=8)
(
    //interface signals
    input logic clk,
    input logic rst,
    input logic flag,
    input logic en,     //counter examples
    input logic [WIDTH-1:0]  cin,
    output logic [WIDTH-1:0] count      //count output
);

always_ff @ (posedge clk, posedge rst)
    if(rst) count <= {WIDTH{1'b0}};
    else if(flag) count <= cin;
    else count <= count + {{WIDTH-1{1'0}}, en};

endmodule
