module PL_top (
    input               clk             ,
    input               rst_n           ,
    input   [11:0]      data_in         ,
    input   [13:0]      sin             ,
    input   [13:0]      cos             ,
    input   [31:0]      Fre             ,
    // output reg [13:0]   data_o          ,
    // output              AD_clk          ,
    output  [31:0]      Phase           ,
    output              judge
    // output  [35:0]      fir1_truncation ,
    // output  [35:0]      fir2_truncation 
);

assign Phase = Phase_move;
// assign AD_clk = clk;

// wire [15:0] sin,cos;

// parameter phase_forward = 4294967;//100k

// reference_source reference(
//     .clk             (clk_100M      ),
//     .rst_n           (rst_n         ),
//     .phase_forward   (phase_forward ),
//     .sin             (sin           ),
//     .cos             (cos           )
// );


wire [11:0] data_a1,data_a2,data_b1,data_b2;
wire [23:0] mul_data_out1,mul_data_out2;

// assign data_o = {!sin[15],sin[14:2]};
assign data_a1 = {!sin[13],sin[12:2]};
assign data_a2 = {!cos[13],cos[12:2]};
assign data_b1 = {!data_in[11],data_in[10:0]}+40;
assign data_b2 = {!data_in[11],data_in[10:0]}+40;

// always @(posedge clk or negedge rst_n) begin
//     if(!rst_n) data_o <= 0;
//     else data_o <= {!sin[15],sin[14:2]};
// end

Multiplier
#(
    .Width          (12             )
)
Multiplier1
(
    .clk            (clk            ),
    .rst_n          (rst_n          ),
    .data_a         (data_a1        ),
    .data_b         (data_b1        ),
    .data_out       (mul_data_out1  )
);

Multiplier
#(
    .Width          (12             )
)
Multiplier2
(
    .clk            (clk            ),
    .rst_n          (rst_n          ),
    .data_a         (data_a2        ),
    .data_b         (data_b2        ),
    .data_out       (mul_data_out2  )
);

// wire [31:0] fir_data_out1,fir_data_out2;

// FIR fir1
// (   
//     .clk            (clk            ),
//     .rst_n          (rst_n          ),
//     .data_in        (mul_data_out1  ),
//     .data_out       (fir_data_out1  )
// );

// FIR fir2
// (   
//     .clk            (clk            ),
//     .rst_n          (rst_n          ),
//     .data_in        (mul_data_out2  ),
//     .data_out       (fir_data_out2  )
// );
wire [23:0] Lag_filter_o1;

Lag_filter
#(
    .WIDTH              (24             ),
    .COEFFICIENT_WIDTH  (24             ),
    .COEFFICIENT        (64             )
)
Lag_filter1
(
    .clk                (clk            ),
    .rst_n              (rst_n          ),
    .data_i             (mul_data_out1  ),
    .data_o             (Lag_filter_o1  )
);

wire [23:0] Lag_filter_o2;

Lag_filter
#(
    .WIDTH              (24             ),
    // .COEFFICIENT_WIDTH  (24             ),
    .COEFFICIENT_WIDTH  (24             ),
    .COEFFICIENT        (64             )
    // .COEFFICIENT        (1024             )
)
Lag_filter2
(
    .clk                (clk            ),
    .rst_n              (rst_n          ),
    .data_i             (mul_data_out2  ),
    .data_o             (Lag_filter_o2  )
);

// parameter LOW_move = 32'h800;
parameter LOW_move = 32'h100;
parameter HIGH_move = 32'h1000;

wire [31:0] move_word;

reg [31:0] Phase_move;

assign move_word = Fre > 17_180_000 ? HIGH_move : LOW_move;
// assign judge = fir_data_out2[31] ^ fir_data_out1[31] == 0;
assign judge = Lag_filter_o2[23]== 0;

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) Phase_move <= 0;
    else if(judge) 
        Phase_move <= Phase_move + move_word;
    else Phase_move <= Phase_move - move_word;
end
// arctan cordic(
//     .clk            (clk            ),
//     .rst_n          (rst_n          ),
//     .IMAG           (fir_data_out2  ),
//     .REAL           (fir_data_out1  ),
//     .Phase          (Phase          ),
//     .arc_valid      (arc_valid      )
// );

endmodule