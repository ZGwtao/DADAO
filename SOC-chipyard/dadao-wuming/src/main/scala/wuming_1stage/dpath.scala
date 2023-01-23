//**************************************************************************
// RISCV Processor 1-Stage Datapath
//--------------------------------------------------------------------------
//
// Christopher Celio
// 2012 Jan 11

package wuming.stage1

import chisel3._
import chisel3.util._

import freechips.rocketchip.config.Parameters
import freechips.rocketchip.rocket.{CSRFile, Causes}
import freechips.rocketchip.tile.CoreInterrupts

import Constants._
import wuming.common._

class DatToCtlIo(implicit val conf: WumingCoreParams) extends Bundle()
{
   val inst   = Output(UInt(BITS_INST.W))
   val imiss  = Output(Bool())
   val br_eq  = Output(Bool())
   val br_n   = Output(Bool())
   val br_z   = Output(Bool())
   val br_p   = Output(Bool())
   val csr_eret = Output(Bool())
   val csr_interrupt = Output(Bool())
   val inst_misaligned = Output(Bool())
   val inst_multi_reg  = Output(Bool())
   val mem_address_low = Output(UInt(3.W))
}

class DpathIo(implicit val p: Parameters, val conf: WumingCoreParams) extends Bundle()
{
   val ddpath = Flipped(new DebugDPath())
   val imem = new MemPortIo(conf.instlen)
   val dmem = new MemPortIo(conf.xprlen)
   val ctl  = Flipped(new CtlToDatIo())
   val dat  = new DatToCtlIo()
   val interrupt = Input(new CoreInterrupts())
   val hartid = Input(UInt())
   val reset_vector = Input(UInt())
}

class DatPath(implicit val p: Parameters, val conf: WumingCoreParams) extends Module
{
   val io = IO(new DpathIo())
   io := DontCare

   // Exception handling values
   val tval_data_ma = Wire(UInt(conf.xprlen.W))
   val tval_inst_ma = Wire(UInt(conf.xprlen.W))

   // Interrupt kill
   val interrupt_edge = Wire(Bool())

   // Instruction Fetch
   val pc_next          = Wire(UInt(conf.xprlen.W))
   val pc_plus4         = Wire(UInt(conf.xprlen.W))
   val br_target12      = Wire(UInt(conf.xprlen.W))
   val br_target18      = Wire(UInt(conf.xprlen.W))
   val jmp_iiii_target  = Wire(UInt(conf.xprlen.W))
   val jmp_rrii_target  = Wire(UInt(conf.xprlen.W))
   val ret_target       = Wire(UInt(conf.xprlen.W))
   val exception_target = Wire(UInt(conf.xprlen.W))

   // PC Register
   pc_next := MuxCase(pc_plus4, Array(
                  (io.ctl.pc_sel === PC_4)   -> pc_plus4,
                  (io.ctl.pc_sel === PC_BR12)  -> br_target12,
                  (io.ctl.pc_sel === PC_BR18)  -> br_target18,
                  (io.ctl.pc_sel === PC_JMPI)  -> jmp_iiii_target,
                  (io.ctl.pc_sel === PC_JMPR)  -> jmp_rrii_target,
                  (io.ctl.pc_sel === PC_RA)  -> ret_target,
                  (io.ctl.pc_sel === PC_EXC) -> exception_target
                  ))

   val pc_reg = RegInit(io.reset_vector)

   when (!io.ctl.stall)
   {
      pc_reg := pc_next
   }

   pc_plus4 := (pc_reg + 4.asUInt(conf.xprlen.W))


   // Instruction memory buffer to store instruction during multicycle data request
   io.dat.imiss := (io.imem.req.valid && !io.imem.resp.valid)
   val reg_dmiss = RegNext(io.ctl.dmiss, false.B)
   val if_inst_buffer = RegInit(0.U(BITS_INST.W))
   when (io.imem.resp.valid) {
      assert(!reg_dmiss, "instruction arrived during data miss")
      if_inst_buffer := io.imem.resp.bits.data
   }

   io.imem.req.bits.fcn := M_XRD
   io.imem.req.bits.typ := MT_TU
   io.imem.req.bits.addr := pc_reg
   io.imem.req.valid := !reg_dmiss
   val inst = Mux(reg_dmiss, if_inst_buffer, io.imem.resp.bits.data)

   // Instruction misalign detection
   // In control path, instruction misalignment exception is always raised in the next cycle once the misaligned instruction reaches
   // execution stage, regardless whether the pipeline stalls or not
   io.dat.inst_misaligned :=  (br_target12(1, 0).orR       && io.ctl.pc_sel_no_xept === PC_BR12) ||
                              (br_target18(1, 0).orR       && io.ctl.pc_sel_no_xept === PC_BR18) ||
                              (jmp_iiii_target(1, 0).orR   && io.ctl.pc_sel_no_xept === PC_JMPI)  ||
                              (jmp_rrii_target(1, 0).orR   && io.ctl.pc_sel_no_xept === PC_JMPR)  ||
                              (ret_target(1, 0).orR        && io.ctl.pc_sel_no_xept === PC_RA)
   // TODO: for multi reg insns, only single reg is legal at present
   io.dat.inst_multi_reg := (((io.ctl.reg_grp === REG_MRD) || (io.ctl.reg_grp === REG_MRB)) && inst(5, 0).orR)
   tval_inst_ma := MuxCase(0.U, Array(
                     (io.ctl.pc_sel_no_xept === PC_BR12) -> br_target12,
                     (io.ctl.pc_sel_no_xept === PC_BR18) -> br_target18,
                     (io.ctl.pc_sel_no_xept === PC_JMPI) -> jmp_iiii_target,
                     (io.ctl.pc_sel_no_xept === PC_JMPR) -> jmp_rrii_target,
                     (io.ctl.pc_sel_no_xept === PC_RA)   -> ret_target
                     ))

   // Decode
   val ha_addr  = inst(HA_MSB, HA_LSB)
   val hb_addr  = inst(HB_MSB, HB_LSB)
   val hc_addr  = inst(HC_MSB, HC_LSB)
   val hd_addr  = inst(HD_MSB, HD_LSB)

   val wb_data = Wire(UInt(conf.xprlen.W))
   val wb_data2 = Wire(UInt(conf.xprlen.W))
   val wb_wen = io.ctl.rf_wen && !io.ctl.exception && !interrupt_edge

   // Register File
   val regfileD = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileB = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileF = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileA = Module(new RegFileA())

   regfileA.io.ras_top   := 63.U
   regfileA.io.ras_push  := (wb_wen && (io.ctl.wb_sel === WB_RA))
   regfileA.io.ras_pop   := (io.ctl.pc_sel === PC_RA)
   regfileA.io.push_data := wb_data

   //// DebugModule, TODO: conflict with regfileD setting
   regfileA.io.dm_addr := io.ddpath.addr
   regfileA.io.dm_en := io.ddpath.validreq
   regfileA.io.dm_wdata := io.ddpath.wdata
   ///

   when (wb_wen)
   {
      when ((io.ctl.wb_sel === WB_RBHA) && (ha_addr =/= 0.U)) {
         regfileB(ha_addr) := wb_data
      }
      when ((io.ctl.wb_sel === WB_RBHB) && (hb_addr =/= 0.U)) {
         regfileB(hb_addr) := wb_data
      }
      when ((io.ctl.wb_sel === WB_RBMM) && (ha_addr =/= 0.U)) {
         regfileB(ha_addr) := wb_data
      }
      when ((io.ctl.wb_sel === WB_RDHA) && (ha_addr =/= 0.U)) {
         regfileD(ha_addr) := wb_data
      }
      when ((io.ctl.wb_sel === WB_RDHB) && (hb_addr =/= 0.U)) {
         regfileD(hb_addr) := wb_data
      }
      when ((io.ctl.wb_sel === WB_HAHB) && (hb_addr =/= 0.U)) {
         regfileD(hb_addr) := wb_data
      }
      when ((io.ctl.wb_sel === WB_HAHB) && (ha_addr =/= 0.U)) {
         regfileD(ha_addr) := wb_data2
      }
      when ((io.ctl.wb_sel === WB_RDMM) && (ha_addr =/= 0.U)) {
         regfileD(ha_addr) := wb_data
      }
   }

   //// DebugModule
   io.ddpath.rdata := regfileD(io.ddpath.addr)
   when(io.ddpath.validreq){
      regfileD(io.ddpath.addr) := io.ddpath.wdata
   }
   ///

   val rdha_data = Mux((ha_addr =/= 0.U), regfileD(ha_addr), 0.asUInt(conf.xprlen.W))
   val rdhb_data = Mux((hb_addr =/= 0.U), regfileD(hb_addr), 0.asUInt(conf.xprlen.W))
   val rdhc_data = Mux((hc_addr =/= 0.U), regfileD(hc_addr), 0.asUInt(conf.xprlen.W))
   val rdhd_data = Mux((hd_addr =/= 0.U), regfileD(hd_addr), 0.asUInt(conf.xprlen.W))

   val rbha_data = Mux((ha_addr =/= 0.U), regfileB(ha_addr), pc_plus4)
   val rbhb_data = Mux((hb_addr =/= 0.U), regfileB(hb_addr), pc_plus4)
   val rbhc_data = Mux((hc_addr =/= 0.U), regfileB(hc_addr), pc_plus4)
   val rbhd_data = Mux((hd_addr =/= 0.U), regfileB(hd_addr), pc_plus4)

   // immediates
   val immu6 = inst(HD_MSB, HD_LSB)
   val immu12 = inst(HC_MSB, HD_LSB)
   val imms12 = Cat(Fill(52, inst(HC_MSB)), inst(HC_MSB, HD_LSB))
   val imms18 = Cat(Fill(46, inst(HB_MSB)), inst(HB_MSB, HD_LSB))
   val imms24 = Cat(Fill(40, inst(HA_MSB)), inst(HA_MSB, HD_LSB))

   val wydeposition = inst(WP_MSB, WP_LSB).asUInt() << 4
   val wyde16 = inst(WYDE_MSB, WYDE_LSB)
   val wydemask = ~(Fill(BITS_WYDE, 1.U) << wydeposition)

   val alu_op1 = MuxCase(0.U, Array(
               (io.ctl.op1_sel === OP1_RDHA) -> rdha_data,
               (io.ctl.op1_sel === OP1_RDHB) -> rdhb_data,
               (io.ctl.op1_sel === OP1_RDHC) -> rdhc_data,
               (io.ctl.op1_sel === OP1_RBHA) -> rbha_data,
               (io.ctl.op1_sel === OP1_RBHB) -> rbhb_data,
               (io.ctl.op1_sel === OP1_RBHC) -> rbhc_data,
               (io.ctl.op1_sel === OP1_PC)  -> pc_reg,
               )).asUInt()

   val alu_op2 = MuxCase(0.U, Array(
               (io.ctl.op2_sel === OP2_RDHD) -> rdhd_data,
               (io.ctl.op2_sel === OP2_RBHD) -> rbhd_data,
               (io.ctl.op2_sel === OP2_IMMU6) -> immu6,
               (io.ctl.op2_sel === OP2_IMMU12) -> immu12,
               (io.ctl.op2_sel === OP2_IMMS12) -> imms12,
               (io.ctl.op2_sel === OP2_IMMS18) -> imms18,
               (io.ctl.op2_sel === OP2_WYDE) -> (wyde16 << wydeposition),
               )).asUInt()



   // ALU
   val alu_out_onemorebit   = Wire(UInt((conf.xprlen + 1).W))

   val alu_shamt = alu_op2(BITS_HEXA-1,0).asUInt()

   alu_out_onemorebit := MuxCase(0.U, Array(
                  (io.ctl.alu_fun === ALU_ADD)  -> (alu_op1 +& alu_op2).asUInt(),
                  (io.ctl.alu_fun === ALU_SUB)  -> (alu_op1 -& alu_op2).asUInt(),
                  (io.ctl.alu_fun === ALU_AND)  -> (alu_op1 & alu_op2).asUInt(),
                  (io.ctl.alu_fun === ALU_OR)   -> (alu_op1 | alu_op2).asUInt(),
                  (io.ctl.alu_fun === ALU_XOR)  -> (alu_op1 ^ alu_op2).asUInt(),
                  (io.ctl.alu_fun === ALU_CMPS) -> Mux(alu_op1 === alu_op2, 0.U, Mux(alu_op1.asSInt() < alu_op2.asSInt(), ~0.U, 1.U)).asUInt(),
                  (io.ctl.alu_fun === ALU_CMPU) -> Mux(alu_op1 === alu_op2, 0.U, Mux(alu_op1.asUInt() < alu_op2.asUInt(), ~0.U, 1.U)).asUInt(),
                  (io.ctl.alu_fun === ALU_SLL)  -> ((alu_op1 << alu_shamt)(conf.xprlen-1, 0)).asUInt(),
                  (io.ctl.alu_fun === ALU_SRA)  -> (alu_op1.asSInt() >> alu_shamt).asUInt(),
                  (io.ctl.alu_fun === ALU_SRL)  -> (alu_op1 >> alu_shamt).asUInt(),
                  (io.ctl.alu_fun === ALU_SETOW)  -> (alu_op2 | wydemask).asUInt(),
                  (io.ctl.alu_fun === ALU_ANDNW)  -> (alu_op1 & ~alu_op2).asUInt(),
                  (io.ctl.alu_fun === ALU_ADRP)   -> ((((alu_op1 >> 12.U) + alu_op2) << 12.U)).asUInt(),
                  (io.ctl.alu_fun === ALU_COPY2)  -> alu_op2.asUInt(),
                  (io.ctl.alu_fun === ALU_MREG)   -> alu_op1.asUInt(),
                  ))

   val alu_out = alu_out_onemorebit(63, 0)
   val alu_out2 = Cat(Fill(63, 0.U), alu_out_onemorebit(64))

   // Branch/Jump Target Calculation
   br_target12       := pc_reg + (imms12 << 2.U)
   br_target18       := pc_reg + (imms18 << 2.U)
   jmp_iiii_target   := pc_reg + (imms24 << 2.U)
   jmp_rrii_target   := rbha_data + rdhb_data + (imms12 << 2.U)
   ret_target        := regfileA.io.pop_data

   // Control Status Registers
   val csr = Module(new CSRFile(perfEventSets=CSREvents.events))
   csr.io := DontCare
   csr.io.decode(0).inst := inst
   csr.io.rw.addr   := inst(CP_RGRC_ADDR_MSB,CP_RGRC_ADDR_LSB)
   csr.io.rw.cmd    := io.ctl.csr_cmd
   csr.io.rw.wdata  := alu_out

   csr.io.retire    := !(io.ctl.stall || io.ctl.exception)
   csr.io.exception := io.ctl.exception
   csr.io.pc        := pc_reg
   exception_target := csr.io.evec

   csr.io.tval := MuxCase(0.U, Array(
                  (io.ctl.exception_cause === Causes.illegal_instruction.U)     -> inst,
                  (io.ctl.exception_cause === Causes.misaligned_fetch.U)  -> tval_inst_ma,
                  (io.ctl.exception_cause === Causes.misaligned_store.U) -> tval_data_ma,
                  (io.ctl.exception_cause === Causes.misaligned_load.U)  -> tval_data_ma,
                  ))

   // Interrupt rising edge detector (output trap signal for one cycle on rising edge)
   val reg_interrupt_edge = RegInit(false.B)
   when (!io.ctl.stall) {
      reg_interrupt_edge := csr.io.interrupt
   }
   interrupt_edge := csr.io.interrupt && !reg_interrupt_edge

   io.dat.csr_eret := csr.io.eret

   csr.io.interrupts := io.interrupt
   csr.io.hartid := io.hartid
   io.dat.csr_interrupt := interrupt_edge
   csr.io.cause := Mux(io.ctl.exception, io.ctl.exception_cause, csr.io.interrupt_cause)
   csr.io.ungated_clock := clock

   // Add your own uarch counters here!
   csr.io.counters.foreach(_.inc := false.B)

   // WB Mux
   wb_data := MuxCase(alu_out, Array(
                  (io.ctl.wb_sel === WB_RDHA) -> alu_out,
                  (io.ctl.wb_sel === WB_RDHB) -> alu_out,
                  (io.ctl.wb_sel === WB_HAHB) -> alu_out,
                  (io.ctl.wb_sel === WB_RBHA) -> alu_out,
                  (io.ctl.wb_sel === WB_RBHB) -> alu_out,
                  (io.ctl.wb_sel === WB_RDMM) -> io.dmem.resp.bits.data,
                  (io.ctl.wb_sel === WB_RBMM) -> io.dmem.resp.bits.data,
                  (io.ctl.wb_sel === WB_RA)   -> pc_plus4,
                  (io.ctl.wb_sel === WB_CSR) -> csr.io.rw.rdata
                  ))
   wb_data2 := MuxCase(alu_out2, Array(
                  (io.ctl.wb_sel === WB_HAHB) -> alu_out2
                  ))

   // datapath to controlpath outputs
   io.dat.inst   := inst
   io.dat.br_eq  := (rdha_data === rdhb_data)
   io.dat.br_n   := (rdha_data.asSInt() < 0.S)
   io.dat.br_z   := (rdha_data === 0.U)
   io.dat.br_p   := (rdha_data.asSInt() > 0.S)

   // datapath to data memory outputs
   io.dmem.req.bits.addr := alu_out
   io.dmem.req.bits.data := MuxCase(0.U, Array(
                  (io.ctl.reg_grp === REG_MRD) -> rdha_data,
                  (io.ctl.reg_grp === REG_MRB) -> rbha_data,
                  (io.ctl.reg_grp === REG_RD) -> rdha_data,
                  (io.ctl.reg_grp === REG_RB) -> rbha_data
                  ))


   io.dat.mem_address_low := alu_out(2, 0)
   tval_data_ma := alu_out

   // Printout
   // pass output through the spike-dasm binary (found in riscv-tools) to turn
   // the DASM(%x) into a disassembly string.
   printf("Cyc= %d [%d] pc=[%x] inst=[%x] W[%x][%d] Op1=[%x] Op2=[%x] %c%c%c DASM(%x)\n",
      csr.io.time(31,0),
      csr.io.retire,
      pc_reg,
      inst,
      wb_data,
      wb_wen,
      alu_op1,
      alu_op2,
      Mux(io.ctl.stall, Str("S"), Str(" ")),
      MuxLookup(io.ctl.pc_sel, Str("?"), Seq(
         PC_BR12 -> Str("B"),
         PC_BR18 -> Str("B"),
         PC_JMPI -> Str("J"),
         PC_JMPR -> Str("R"),
         PC_RA -> Str("A"),
         PC_EXC -> Str("E"),
         PC_4 -> Str(" "))),
      Mux(csr.io.exception, Str("X"), Str(" ")),
      inst)


   if (PRINT_COMMIT_LOG)
   {
      when (!io.ctl.stall)
      {
         // use "sed" to parse out "@@@" from the other printf code above.
         when (io.ctl.rf_wen)
         {
            printf("@@@ 0x%x (0x%x) %d %d 0x%x\n", pc_reg, inst, ha_addr, hb_addr, Cat(Fill(32,wb_data(31)),wb_data))
         }
         .otherwise
         {
            printf("@@@ 0x%x (0x%x)\n", pc_reg, inst)
         }
      }
   }
}
