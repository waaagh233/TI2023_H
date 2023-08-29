module dds
#(
    parameter   WIDTH = 16-1
)
(
    input               clk     ,
    input               rst_n   ,
    input   [WIDTH:0]   Amp     ,
    input   [WIDTH:0]   Phase   ,
    input   [WIDTH:0]   Fre     ,
    input               refresh ,
    input               mode    ,
    input   [31:0]      adjust  ,
    input               A_J     ,
    output  [13:0]      sin_o   ,
    output  [13:0]      cos_o   ,
    output  [13:0]      dds_o
);

reg [31:0] forward;

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) forward <= 0;
    else if(refresh) forward <= 0;
    else begin
        forward <= forward + Fre;
    end 
end

wire [31:0] addr;
wire [31:0] dds_addr;
wire [11:0] sin_addr;
wire [11:0] dds_sin_addr;
wire [11:0] cos_addr;
wire [13:0] dds_sin;
wire [13:0] dds_tri;
wire [13:0] dds_o1;

assign dds_o = A_J ? AP_dds[13:0] : dds_o1;
//  assign dds_o = dds_o1;
assign dds_o1 = mode ? dds_tri : dds_sin;
assign addr = forward + adjust;
assign dds_addr = addr + Phase;
assign sin_addr = addr[31:20];
assign cos_addr = addr[31:20] + 12'h3ff;
assign dds_sin_addr = dds_addr[31:20];
assign dds_tri = dds_addr[31] ? ~dds_addr[30:17]: dds_addr[30:17];

reg [27:0] AP_dds;

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) AP_dds <= {14{1'b1}};
    else AP_dds <= (Amp[31:18] * dds_o1) >> 12; 
end

myrom sin (
  .clock      (clk        ),    // input wire clka
  .address    (sin_addr   ),  // input wire [15 : 0] addra
  .q          (sin_o      )  // output wire [13 : 0] douta
);

myrom cos (
  .clock      (clk            ),    // input wire clka
  .address    (cos_addr       ),  // input wire [15 : 0] addra
  .q          (cos_o          )  // output wire [13 : 0] douta
);

myrom dds_sin_out (
  .clock      (clk            ),    // input wire clka
  .address    (dds_sin_addr   ),  // input wire [15 : 0] addra
  .q          (dds_sin        )  // output wire [13 : 0] douta
);

endmodule
