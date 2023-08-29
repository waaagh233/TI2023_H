module Lag_filter
#(
    parameter   WIDTH = 24,
    parameter   COEFFICIENT_WIDTH = 24,
    parameter   COEFFICIENT = 1
)
(
    input                               clk         ,
    input                               rst_n       ,
	input       signed [WIDTH-1:0]		data_i      ,
	output reg  signed [WIDTH-1:0]		data_o
);

reg signed [WIDTH-1:0] last_data_o;
reg signed [WIDTH+COEFFICIENT_WIDTH-1:0] cache;

always@(posedge clk or negedge rst_n)
begin
	if(!rst_n) begin
		last_data_o <= {WIDTH{1'b0}};
		data_o <= {WIDTH{1'b0}};
		cache <= {(WIDTH+COEFFICIENT_WIDTH-1){1'b0}};
	end 
    else begin
		last_data_o <= data_o;
		data_o <= cache>>>COEFFICIENT_WIDTH;
		cache <= ((COEFFICIENT*(data_i-last_data_o)))+cache;
	end
end

endmodule
