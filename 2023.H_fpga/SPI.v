module	SPI
#(
parameter WIDTH = 10 - 1,
parameter W_ADDR = 10 - 1
)
(
    input clk,
    input rst_n,
    input CS_N,
    input SCK,
    input MOSI,
    input [WIDTH:0] po_data,
    output reg MISO,
    output reg [WIDTH:0] pi_data,
    output reg [WIDTH:0] pi_data_tmp,
    output reg sck_p_d4,
    output reg [W_ADDR:0] pi_state,
    output MOSI_r,
    output pi_flag,
    output po_flag,
    output SCK_clk

);

reg [WIDTH:0] po_data_tmp;
//reg [WIDTH:0] pi_data_tmp;
//-------------------------capture the sck-----------------------------
reg sck_r0,sck_r1;
reg CS_N_r0;
reg CS_N_r1;
reg MOSI_r0,MOSI_r1;
reg sck_p_d,sck_p_d1,sck_p_d2,sck_p_d3;

assign MOSI_r=MOSI_r1;

wire CS_N_n;
wire sck_n;
wire sck_p;

always@(posedge clk or negedge rst_n)
begin
    if(!rst_n)
        begin
            sck_r0 <= 1'b0;   //sck of the idle state is high
            sck_r1 <= 1'b0;
            CS_N_r0 <= 1'b1;
            CS_N_r1 <= 1'b1;
            MOSI_r0 <= 1'b1;
            MOSI_r1 <= 1'b1;
            sck_p_d <= 1'b0;
            sck_p_d1 <= 1'b0;
            sck_p_d2 <= 1'b0;
            sck_p_d3 <= 1'b0;
            sck_p_d4 <= 1'b0;
        end
    else
        begin
            sck_r0 <= SCK;
            sck_r1 <= sck_r0;
            CS_N_r0 <= CS_N;
            CS_N_r1 <= CS_N_r0;
            MOSI_r0 <= MOSI;
            MOSI_r1 <= MOSI_r0;
            sck_p_d <= sck_p;
            sck_p_d1 <= sck_p_d;
            sck_p_d2 <= sck_p_d1;
            sck_p_d3 <= sck_p_d2;
            sck_p_d4 <= sck_p_d3;
        end
end
 
assign sck_n = (~sck_r0 & sck_r1)? 1'b1:1'b0;   //capture the sck negedge
assign sck_p = (~sck_r1 & sck_r0)? 1'b1:1'b0;   //capture the sck posedge
assign SCK_clk = sck_r1;
assign CS_N_n = ~CS_N_r0 & CS_N_r1;

//-----------------------spi_slaver read data-------------------------------

reg pi_flag_r;
//reg [W_ADDR:0] pi_state;
always@(posedge clk or negedge rst_n)
begin
    if(!rst_n)
        begin
            pi_data <= 0;
            pi_flag_r <= 1'b0;
            pi_state<= 0;
        end
    else if(CS_N_n) begin
        pi_state <= 1'b0;
        pi_flag_r <= 0;
    end 
    else if(sck_p_d4 && !CS_N)
        begin            
            pi_data[WIDTH-pi_state] <= MOSI_r1;
            if(pi_state < WIDTH)
                begin
                    pi_state <= pi_state + 1;
                    pi_flag_r <= 1'b0;
                end
            else
                begin
                    pi_flag_r <= 1'b1;
                    pi_state <= 0;
                end
        end
end


//--------------------capture spi_flag posedge--------------------------------
reg pi_flag_r0,pi_flag_r1;
always@(posedge clk or negedge rst_n)
begin
    if(!rst_n)
        begin
            pi_flag_r0 <= 1'b0;
            pi_flag_r1 <= 1'b0;
        end
    else
        begin
            pi_flag_r0 <= pi_flag_r;
            pi_flag_r1 <= pi_flag_r0;
        end
end
 
assign pi_flag = ((~pi_flag_r1) & pi_flag_r0)? 1'b1:1'b0;

always@(posedge clk or negedge rst_n)
begin
    if(!rst_n) pi_data_tmp <= 0;
    else if(pi_flag) pi_data_tmp <= pi_data;
    else pi_data_tmp <= pi_data_tmp;
end

//---------------------spi_slaver send data---------------------------
reg po_flag_r;
reg [W_ADDR:0] po_state;
always@(posedge clk or negedge rst_n)
begin
    if(!rst_n)
        begin
            // po_flag_r <= 0;
            po_state <= 0;
            MISO <= 0;
        end
    else if(CS_N_n) begin
        po_state <= 1'b0;
        // po_flag_r <= 0;
        MISO <= 0;
    end 
    else if(sck_p_d && !CS_N)
        begin
            MISO <= po_data[WIDTH-po_state];
            if(po_state < WIDTH)
                begin
                    // if(po_state == 0) po_flag_r <= 1;
                    // else po_flag_r <= 0;
                    po_state <= po_state + 1;
                    // po_flag_r <= 1'b0;
                end
            else
                begin
                    po_state <= 0;
                    // po_flag_r <= 1'b1;          
                end  
        end
end

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) po_flag_r <= 0;
    else if(CS_N_n) po_flag_r <= 0;
    else if(sck_p & !CS_N) 
        if(po_state == 0) po_flag_r <= 1;
        else po_flag_r <= 0;
end

//--------------------capture po_flag posedge--------------------------------
reg po_flag_r0,po_flag_r1;
always@(posedge clk or negedge rst_n)
begin
    if(!rst_n)
        begin
            po_flag_r0 <= 1'b0;
            po_flag_r1 <= 1'b0;
        end
    else
        begin
            po_flag_r0 <= po_flag_r;
            po_flag_r1 <= po_flag_r0;
        end
end

// assign po_flag = ((~po_flag_r1) & po_flag_r0)? 1'b1:1'b0;
assign po_flag = po_flag_r;

endmodule


