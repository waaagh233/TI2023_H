module save_hyper(
    input           clk     ,
    input           rst_n   ,
    input           en      ,
    input           pi_flag ,
    input   [31:0]  pi_data ,
    output  [31:0]  hyper_f1,
    output  [31:0]  hyper_f2
);

assign hyper_f1 = f1;
assign hyper_f2 = f2;

reg [10:0] count;

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) count <= 0;
    else if(!en) count <= 0;
    else if(count >= 1023 & pi_flag) count <= 0;
    else if(pi_flag) count <= count + 1;
end

reg [31:0] f1,f2;

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) begin
        f1 <= 0;
        f2 <= 0;
    end
    else if(!en) begin
        f1 <= 0;
        f2 <= 0;
    end
    else if(count == 0 & pi_flag) f1 <= pi_data;
    else if(count == 1 & pi_flag) f2 <= pi_data;
end
endmodule
