module Save_ram
#(
    parameter   WIDTH = 16-1
)
(
    input                   clk     ,
    input                   rst_n   ,
    input       [WIDTH:0]   data_in ,
    input                   pi_flag ,
    input                   en      ,
    output  reg [WIDTH:0]   Amp1    ,
    output  reg [WIDTH:0]   Amp2    ,
    output  reg [WIDTH:0]   Phase1  ,
    output  reg [WIDTH:0]   Phase2  ,
    output  reg [WIDTH:0]   Fre1    ,
    output  reg [WIDTH:0]   Fre2    ,
    output  reg     [2:0]       idx     
);  

reg [WIDTH:0] ram [5:0];
reg [WIDTH:0] ram_reg [5:0];


integer i,j;

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) begin
        for(i = 0;i <= 5;i = i + 1) ram[i] <= 0;
    end
    else if(en) begin
        if(pi_flag) ram[idx] <= data_in;
    end
end

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) idx <= 0;
    else if(!en) idx <= 0;
    else if(pi_flag) begin
            if(idx < 5)
                idx <= idx + 1;
            else idx <= 0;
    end 
    else idx <= idx;
end

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) begin
        Amp1    <= 0;
        Amp2    <= 0;
        Phase1  <= 0;
        Phase2  <= 0;
        Fre1    <= 0;
        Fre2    <= 0;        
    end
    else if(!en) begin
        Amp1    <= 0;
        Amp2    <= 0;
        Phase1  <= 0;
        Phase2  <= 0;
        Fre1    <= 0;
        Fre2    <= 0;          
    end
    else if(pi_flag & idx == 5) begin
        Amp1 <= ram[0];
        Amp2 <= ram[1];
        Phase1 <= ram[2];
        Phase2 <= ram[3];
        Fre1 <= ram[4];
        Fre2 <= ram[5];
    end 
end

endmodule
