module fifo (
    input rst_n,
    input clk,
    input wire wrclk , //同步于FIFO写数据的时钟
    input wire [11:0] pi_data , //输入顶层模块的数据，要写入到FIFO中的数据同步于wrclk时钟
    input wire pi_flag , //输入数据有效标志信号，也作为FIFO的写请求信号，同步于wrclk时钟
    input wire po_flag,
    input wire rdclk , //同步于FIFO读数据的时钟

    output wire wrfull,
    output wire wrempty,
    output wire [9:0]  wrusedw,
    output wire [11:0] po_data
    // output reg po_flag
);

reg wrreq;
reg rdreq;          //rdreq:FIFO读请求信号同步于rdclk时钟

wire rdempty;       //FIFO读端口空标志信号，高有效，同步于rdclk时钟
wire rdfull;        //FIFO读端口满标志信号,高有效,同步于rdclk时钟
wire [9:0]  rdusedw;//FIFO读端口中存在的数据个数，同步于rdclk时钟
//wire wrempty;       //FIFO写端口空标志信号，高有效，同步于wrclk时钟
// wire wrfull;        //FIFO写端口满标志信号，高有效，同步于wrclk时钟
//wire [9:0]  wrusedw;//FIFO写端口中存在的数据个数，同步于wrclk时钟


always@(posedge rdclk or negedge rst_n)
    if(!rst_n)
        rdreq <= 1'b0;
    else if(po_flag)
            rdreq <= 1'b1;
        // else if(rdempty == 1'b1)//当FIFO中的数据被读空时停止读取FIFO中的数据
        //     rdreq <= 1'b0;
    else 
            rdreq <= 1'b0;

always@(posedge wrclk or negedge rst_n)
    if(!rst_n)
        wrreq <= 1'b1;
    else if(wrempty)
        wrreq <= 1'b1;
    else if(wrfull == 1'b1)//当FIFO中的数据被读空时停止读取FIFO中的数据
        wrreq <= 1'b0;

dcfifo_10x1024to10x1024	inst (
	.data    ( pi_data  ),
	.rdclk   ( rdclk    ),
	.rdreq   ( rdreq    ),
	.wrclk   ( wrclk    ),
	.wrreq   ( wrreq    ),

	.q       ( po_data  ),
	.rdempty ( rdempty  ),
	.rdfull  ( rdfull   ),
	.rdusedw ( rdusedw  ),
	.wrempty ( wrempty  ),
	.wrfull  ( wrfull   ),
	.wrusedw ( wrusedw  )
);

endmodule