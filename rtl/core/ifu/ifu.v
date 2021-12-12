/*-------------------------------------------------------------------------
// Module:  ifu
// File:    ifu.v
// Author:  shawn Liu
// E-mail:  shawn110285@gmail.com
// Description: generate the pc for instruction fetching
--------------------------------------------------------------------------*/

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//-----------------------------------------------------------------

`include "defines.v"

module ifu(
    input   wire                  clk_i,
    input   wire                  n_rst_i,

    /* ------- signals from the ctrl unit --------*/
    input wire[5:0]               stall_i,
    input wire                    flush_i,
    input wire[`RegBus]           new_pc_i,

    //bypass from exu
    input wire                    branch_i,           //the instruction in execution stage is a branch or jump
    input wire[`RegBus]           branch_addr_i,      //the target address of the branch or jump

	/* ------- signals to inst_rom and decode unit --------*/
    output reg[`InstAddrBus]      pc_o, // the pc, to the inst_rom and decode module
    output reg                    ce_o,  // to inst_rom
    output reg                    branch_slot_end_o,

    /* ---stall the pipeline, waiting for the rom to response with instruction ----*/
    output wire                   stall_req_o
);

    // if the rom can not response in the same cycle, need to set the stall_req_o
    assign stall_req_o = 0;

    always @ (posedge clk_i) begin
        if (n_rst_i == `RstEnable) begin
            ce_o <= `ChipDisable;
        end else begin
            ce_o <= `ChipEnable;
        end
    end

    always @ (posedge clk_i) begin
        if (ce_o == `ChipDisable) begin  //delay one tap,
            pc_o <= `REBOOT_ADDR;
            branch_slot_end_o <= 1'b0;
        end else begin
            if(flush_i == 1'b1) begin
                pc_o <= new_pc_i;
                branch_slot_end_o <= 1'b0;
            end else if(stall_i[0] == `NoStop) begin
                if(branch_i == `Branch) begin
                    branch_slot_end_o <= 1'b1;
                    pc_o <= branch_addr_i;  //fetch the instruction from the branch target address
                end else begin
                    branch_slot_end_o <= 1'b0;
                    pc_o <= pc_o + 32'h4;
                end
            end else begin
                // if stall[0] == `Stop，the pc value will be kept
                branch_slot_end_o <= 1'b0;
                pc_o <= pc_o;
            end
        end
    end
endmodule
