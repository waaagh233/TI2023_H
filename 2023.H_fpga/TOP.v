module TOP(
    input           clk         ,
    input           rst_n       ,
    input           CS_N        ,
    input           SPI_SCK     ,
    input           MOSI        ,
    input           refresh     ,
    input           mode1       ,
    input           mode2       ,
    input           hyper_mode  ,
    input           A_J         ,
    input   [11:0]  data_in     ,
    output          fifo_full   ,
    output          AD_clk      ,
    output          MISO        ,
    output  [13:0]  dds_o1      ,
    output  [13:0]  dds_o2      ,
    output          DA_clk1     ,
    output          DA_clk2     ,
    output  [11:0]  fifo_po_data,
    output          fifo_po_flag

);

assign DA_clk1 = clk;
assign DA_clk2 = clk;
assign AD_clk = clk;


/////////////////////////////////////////////////

//FIFO
wire wrclk;
wire rdclk;
wire SPI_clk;
wire fifo_pi_flag;
// wire fifo_po_flag;
// wire [11:0]fifo_po_data;
wire spi_SCK_clk;
reg tmp_pi_flag;
assign wrclk = AD_clk;
assign fifo_pi_flag = AD_clk;
assign fifo_po_flag = spi_po_flag & hyper_mode;
assign rdclk = spi_SCK_clk;

always @(posedge wrclk or negedge rst_n)
    if(!rst_n)
        tmp_pi_flag <=0;
    else tmp_pi_flag <= rdclk;


fifo dcfifo_10x1024(
    .rst_n      (rst_n          ),
    .clk        (clk            ),
    .wrclk      (wrclk          ), 
    .pi_data    (data_in        ), 
    .rdclk      (rdclk          ), 
    .pi_flag    (fifo_pi_flag   ), 
    .po_flag    (fifo_po_flag   ),
    
    .po_data    (fifo_po_data   ),
    .wrfull     (fifo_full      ),
    .wrusedw    ()
);

//SPI_Slave
wire [31:0] pi_data_tmp;
reg [31:0] data_reg;

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) data_reg <= 0;
    else data_reg <= {20'b0,fifo_po_data};
end

////////////////////////////////////////////////
//SPI

wire [31:0] spi_pi_data;
wire spi_pi_flag;
wire spi_po_flag;

SPI
#(
    .WIDTH      (32-1   ),
    .W_ADDR     (5-1    )
)
spi_inst
(
    //input
    .clk        (clk        ),
    .rst_n      (rst_n      ),
    .CS_N       (CS_N       ),
    .SCK        (SPI_SCK    ),
    .MOSI       (MOSI       ),//32->fpga
    .po_data    (data_reg   ),//fpga->32

    //output
    .MISO       (MISO       ),//fpga->32
    .pi_data    (spi_pi_data),//32->fpga
    .pi_data_tmp(pi_data_tmp),//32->fpga
    .MOSI_r     (MOSI_r     ),//32->fpga
    .pi_state   (pi_state   ),//32->fpga
    .sck_p_d4   (sck_p_d4   ),//32->fpga
    .pi_flag    (spi_pi_flag),//32->fpga
    .po_flag    (spi_po_flag),//fpga->32
    .SCK_clk    (spi_SCK_clk)
);

////////////////////////////////////////////////
//RAM

reg save_flag;

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) save_flag <= 0;
    else save_flag <= spi_pi_flag;
end

wire [31:0]   Amp1  ; 
wire [31:0]   Amp2  ; 
wire [31:0]   Phase1; 
wire [31:0]   Phase2; 
wire [31:0]   Fre1  ; 
wire [31:0]   Fre2  ; 
wire [2:0]    save_idx;

Save_ram
#(
    .WIDTH      (32-1       )
)
saver
(
    .clk        (clk        ),
    .rst_n      (rst_n      ),
    .data_in    (pi_data_tmp),
    .pi_flag    (save_flag  ),
    .en         (!hyper_mode),
    .Amp1       (Amp1       ),
    .Amp2       (Amp2       ),
    .Phase1     (Phase1     ),
    .Phase2     (Phase2     ),
    .Fre1       (Fre1       ),
    .Fre2       (Fre2       ),
    .idx        (save_idx   )
);

////////////////////////////////////////////////
//HYPER_RAM

wire [31:0] hyper_f1;
wire [31:0] hyper_f2;

save_hyper hyper_saver(
    .clk        (clk        ),
    .rst_n      (rst_n      ),
    .en         (hyper_mode ),
    .pi_flag    (save_flag  ),
    .pi_data    (pi_data_tmp),
    .hyper_f1   (hyper_f1   ),
    .hyper_f2   (hyper_f2   )
);

////////////////////////////////////////////////
//dds

wire [13:0] sin_o1;
wire [13:0] cos_o1;
wire [31:0] adjust1;
wire [31:0] dds_f1;

assign dds_f1 = hyper_mode ? hyper_f1 : Fre1;

dds
#(
    .WIDTH     (32-1       )
)
dds_1
(
    .clk        (clk        ),
    .rst_n      (rst_n      ),
    .Amp        (Amp1       ),
    .Phase      (Phase1     ),
    .Fre        (dds_f1     ),
    .mode       (mode1      ),
    .refresh    (refresh    ),
    .A_J        (A_J        ),
    .adjust     (adjust1    ),
    .sin_o      (sin_o1     ),
    .cos_o      (cos_o1     ),
    .dds_o      (dds_o1     )
);

wire [13:0] sin_o2;
wire [13:0] cos_o2;
wire [31:0] adjust2;
wire [31:0] dds_f2;

assign dds_f2 = hyper_mode ? hyper_f2 : Fre2;

dds
#(
    .WIDTH     (32-1       )
)
dds_2
(
    .clk        (clk        ),
    .rst_n      (rst_n      ),
    .Amp        (Amp2       ),
    .Phase      (Phase2     ),
    .Fre        (dds_f2     ),
    .mode       (mode2      ),
    .refresh    (refresh    ),
    .A_J        (A_J        ),
    .adjust     (adjust2    ),
    .sin_o      (sin_o2     ),
    .cos_o      (cos_o2     ),
    .dds_o      (dds_o2     )
);

////////////////////////////////////////////////
//Phase_Locked

wire judge1;

PL_top PL1(
    .clk            (clk        ),
    .rst_n          (rst_n      ),
    .data_in        (data_in    ),
    .sin            (sin_o1     ),
    .cos            (cos_o1     ),
    .Fre            (dds_f1     ),
    .Phase          (adjust1    ),
    .judge          (judge1     )
);

PL_top PL2(
    .clk            (clk        ),
    .rst_n          (rst_n      ),
    .data_in        (data_in    ),
    .sin            (sin_o2     ),
    .cos            (cos_o2     ),
    .Fre            (dds_f2     ),
    .Phase          (adjust2    )
);

endmodule
