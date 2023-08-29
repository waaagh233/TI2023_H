module Multiplier
#(
    parameter Width = 16
)
(
    input                   clk         ,
    input                   rst_n       ,
    input   [Width-1:0]     data_a      ,
    input   [Width-1:0]     data_b      ,
    output  [(Width<<1)-1:0]data_out
);

mult mul (
  // .CLK    (clk      ),    // input wire CLK
  .dataa      (data_a   ),        // input wire [15 : 0] A
  .datab      (data_b   ),        // input wire [15 : 0] B
  .result      (data_out )        // output wire [31 : 0] P
);

endmodule
