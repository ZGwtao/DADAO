// See LICENSE for license details.

package wuming.common

import chisel3._
import chisel3.util._


/* Automatically generated by parse-opcodes */
object Instructions {
  def SWYM               = BitPat("b00010000000000??????????????????")

  def CPRD               = BitPat("b01110010????????????????????????")
  def CPWR               = BitPat("b01110011????????????????????????")
  
  def AND                = BitPat("b00010000001000??????????????????")
  def ORR                = BitPat("b00010000001001??????????????????")
  def XOR                = BitPat("b00010000001010??????????????????")

  def SHLUr              = BitPat("b00010000010001??????????????????")
  def SHLUi              = BitPat("b00010000011001??????????????????")
  def SHRSr              = BitPat("b00010000010010??????????????????")
  def SHRSi              = BitPat("b00010000011010??????????????????")
  def SHRUr              = BitPat("b00010000010011??????????????????")
  def SHRUi              = BitPat("b00010000011011??????????????????")

  def ADDrb              = BitPat("b00010000101110??????????????????")
  def SUBrb              = BitPat("b00010000101111??????????????????")

  def ADDIrd             = BitPat("b00011001????????????????????????")
  def ADDIrb             = BitPat("b01001001????????????????????????")

   def ADDrd           = BitPat("b00011010????????????????????????")
   def SUBrd           = BitPat("b00011011????????????????????????")

   def LDBS		= BitPat("b00110000????????????????????????")
   def LDWS		= BitPat("b00110001????????????????????????")
   def LDTS		= BitPat("b00110010????????????????????????")
   def LDO		= BitPat("b00110011????????????????????????")
   def LDBU		= BitPat("b01000000????????????????????????")
   def LDWU		= BitPat("b01000001????????????????????????")
   def LDTU		= BitPat("b01000010????????????????????????")

   def STB		= BitPat("b00111000????????????????????????")
   def STW		= BitPat("b00111001????????????????????????")
   def STT		= BitPat("b00111010????????????????????????")
   def STO		= BitPat("b00111011????????????????????????")

   def LDRB		= BitPat("b01000011????????????????????????")
   def STRB		= BitPat("b01001011????????????????????????")

   def BREQ		= BitPat("b00101110????????????????????????")
   def BRNE		= BitPat("b00101111????????????????????????")

  def BLT                = BitPat("b?????????????????100?????1100011")
  def BGE                = BitPat("b?????????????????101?????1100011")
  def BLTU               = BitPat("b?????????????????110?????1100011")
  def BGEU               = BitPat("b?????????????????111?????1100011")
  def JALR               = BitPat("b?????????????????000?????1100111")
  def JAL                = BitPat("b?????????????????????????1101111")
  def LUI                = BitPat("b?????????????????????????0110111")
  def AUIPC              = BitPat("b?????????????????????????0010111")
  def ADDI               = BitPat("b?????????????????000?????0010011")
  def SLTI               = BitPat("b?????????????????010?????0010011")
  def SLTIU              = BitPat("b?????????????????011?????0010011")
  def SLT                = BitPat("b0000000??????????010?????0110011")
  def SLTU               = BitPat("b0000000??????????011?????0110011")
  def FENCE              = BitPat("b?????????????????000?????0001111")
  def FENCE_I            = BitPat("b?????????????????001?????0001111")
  def MUL                = BitPat("b0000001??????????000?????0110011")
  def MULH               = BitPat("b0000001??????????001?????0110011")
  def MULHSU             = BitPat("b0000001??????????010?????0110011")
  def MULHU              = BitPat("b0000001??????????011?????0110011")
  def DIV                = BitPat("b0000001??????????100?????0110011")
  def DIVU               = BitPat("b0000001??????????101?????0110011")
  def REM                = BitPat("b0000001??????????110?????0110011")
  def REMU               = BitPat("b0000001??????????111?????0110011")
  def ECALL              = BitPat("b00000000000000000000000001110011")
  def EBREAK             = BitPat("b00000000000100000000000001110011")
  def URET               = BitPat("b00000000001000000000000001110011")
  def MRET               = BitPat("b00110000001000000000000001110011")
  def DRET               = BitPat("b01111011001000000000000001110011")
  def SFENCE_VMA         = BitPat("b0001001??????????000000001110011")
  def WFI                = BitPat("b00010000010100000000000001110011")
  def SLLI_RV32          = BitPat("b0000000??????????001?????0010011")
  def SRLI_RV32          = BitPat("b0000000??????????101?????0010011")
  def SRAI_RV32          = BitPat("b0100000??????????101?????0010011")
  def RDCYCLE            = BitPat("b11000000000000000010?????1110011")
  def RDTIME             = BitPat("b11000000000100000010?????1110011")
  def RDINSTRET          = BitPat("b11000000001000000010?????1110011")
  def RDCYCLEH           = BitPat("b11001000000000000010?????1110011")
  def RDTIMEH            = BitPat("b11001000000100000010?????1110011")
  def RDINSTRETH         = BitPat("b11001000001000000010?????1110011")

  /* Custom complex instructions */
  def MOVN               = BitPat("b?????????????????????????1110111")
  /* TODO: Add your custom instruction encoding here */
}
object Causes {
  val misaligned_fetch = 0x0
  val fetch_access = 0x1
  val illegal_instruction = 0x2
  val breakpoint = 0x3
  val misaligned_load = 0x4
  val load_access = 0x5
  val misaligned_store = 0x6
  val store_access = 0x7
  val user_ecall = 0x8
  val machine_ecall = 0xb
  val all = {
    val res = collection.mutable.ArrayBuffer[Int]()
    res += misaligned_fetch
    res += fetch_access
    res += illegal_instruction
    res += breakpoint
    res += misaligned_load
    res += load_access
    res += misaligned_store
    res += store_access
    res += user_ecall
    res += machine_ecall
    res.toArray
  }
}
object CSRs {
  val cycle = 0xc00
  val instret = 0xc02
  val hpmcounter3 = 0xc03
  val hpmcounter4 = 0xc04
  val hpmcounter5 = 0xc05
  val hpmcounter6 = 0xc06
  val hpmcounter7 = 0xc07
  val hpmcounter8 = 0xc08
  val hpmcounter9 = 0xc09
  val hpmcounter10 = 0xc0a
  val hpmcounter11 = 0xc0b
  val hpmcounter12 = 0xc0c
  val hpmcounter13 = 0xc0d
  val hpmcounter14 = 0xc0e
  val hpmcounter15 = 0xc0f
  val hpmcounter16 = 0xc10
  val hpmcounter17 = 0xc11
  val hpmcounter18 = 0xc12
  val hpmcounter19 = 0xc13
  val hpmcounter20 = 0xc14
  val hpmcounter21 = 0xc15
  val hpmcounter22 = 0xc16
  val hpmcounter23 = 0xc17
  val hpmcounter24 = 0xc18
  val hpmcounter25 = 0xc19
  val hpmcounter26 = 0xc1a
  val hpmcounter27 = 0xc1b
  val hpmcounter28 = 0xc1c
  val hpmcounter29 = 0xc1d
  val hpmcounter30 = 0xc1e
  val hpmcounter31 = 0xc1f
  val mstatus = 0x300
  val misa = 0x301
  val medeleg = 0x302
  val mideleg = 0x303
  val mie = 0x304
  val mtvec = 0x305
  val mscratch = 0x340
  val mcounteren = 0x306
  val mepc = 0x341
  val mcause = 0x342
  val mtval = 0x343
  val mip = 0x344
  val tselect = 0x7a0
  val tdata1 = 0x7a1
  val tdata2 = 0x7a2
  val tdata3 = 0x7a3
  val dcsr = 0x7b0
  val dpc = 0x7b1
  val dscratch = 0x7b2
  val mcycle = 0xb00
  val minstret = 0xb02
  val mhpmcounter3 = 0xb03
  val mhpmcounter4 = 0xb04
  val mhpmcounter5 = 0xb05
  val mhpmcounter6 = 0xb06
  val mhpmcounter7 = 0xb07
  val mhpmcounter8 = 0xb08
  val mhpmcounter9 = 0xb09
  val mhpmcounter10 = 0xb0a
  val mhpmcounter11 = 0xb0b
  val mhpmcounter12 = 0xb0c
  val mhpmcounter13 = 0xb0d
  val mhpmcounter14 = 0xb0e
  val mhpmcounter15 = 0xb0f
  val mhpmcounter16 = 0xb10
  val mhpmcounter17 = 0xb11
  val mhpmcounter18 = 0xb12
  val mhpmcounter19 = 0xb13
  val mhpmcounter20 = 0xb14
  val mhpmcounter21 = 0xb15
  val mhpmcounter22 = 0xb16
  val mhpmcounter23 = 0xb17
  val mhpmcounter24 = 0xb18
  val mhpmcounter25 = 0xb19
  val mhpmcounter26 = 0xb1a
  val mhpmcounter27 = 0xb1b
  val mhpmcounter28 = 0xb1c
  val mhpmcounter29 = 0xb1d
  val mhpmcounter30 = 0xb1e
  val mhpmcounter31 = 0xb1f
  val mucounteren = 0x320
  val mhpmevent3 = 0x323
  val mhpmevent4 = 0x324
  val mhpmevent5 = 0x325
  val mhpmevent6 = 0x326
  val mhpmevent7 = 0x327
  val mhpmevent8 = 0x328
  val mhpmevent9 = 0x329
  val mhpmevent10 = 0x32a
  val mhpmevent11 = 0x32b
  val mhpmevent12 = 0x32c
  val mhpmevent13 = 0x32d
  val mhpmevent14 = 0x32e
  val mhpmevent15 = 0x32f
  val mhpmevent16 = 0x330
  val mhpmevent17 = 0x331
  val mhpmevent18 = 0x332
  val mhpmevent19 = 0x333
  val mhpmevent20 = 0x334
  val mhpmevent21 = 0x335
  val mhpmevent22 = 0x336
  val mhpmevent23 = 0x337
  val mhpmevent24 = 0x338
  val mhpmevent25 = 0x339
  val mhpmevent26 = 0x33a
  val mhpmevent27 = 0x33b
  val mhpmevent28 = 0x33c
  val mhpmevent29 = 0x33d
  val mhpmevent30 = 0x33e
  val mhpmevent31 = 0x33f
  val mvendorid = 0xf11
  val marchid = 0xf12
  val mimpid = 0xf13
  val mhartid = 0xf14
  val cycleh = 0xc80
  val instreth = 0xc82
  val hpmcounter3h = 0xc83
  val hpmcounter4h = 0xc84
  val hpmcounter5h = 0xc85
  val hpmcounter6h = 0xc86
  val hpmcounter7h = 0xc87
  val hpmcounter8h = 0xc88
  val hpmcounter9h = 0xc89
  val hpmcounter10h = 0xc8a
  val hpmcounter11h = 0xc8b
  val hpmcounter12h = 0xc8c
  val hpmcounter13h = 0xc8d
  val hpmcounter14h = 0xc8e
  val hpmcounter15h = 0xc8f
  val hpmcounter16h = 0xc90
  val hpmcounter17h = 0xc91
  val hpmcounter18h = 0xc92
  val hpmcounter19h = 0xc93
  val hpmcounter20h = 0xc94
  val hpmcounter21h = 0xc95
  val hpmcounter22h = 0xc96
  val hpmcounter23h = 0xc97
  val hpmcounter24h = 0xc98
  val hpmcounter25h = 0xc99
  val hpmcounter26h = 0xc9a
  val hpmcounter27h = 0xc9b
  val hpmcounter28h = 0xc9c
  val hpmcounter29h = 0xc9d
  val hpmcounter30h = 0xc9e
  val hpmcounter31h = 0xc9f
  val mcycleh = 0xb80
  val minstreth = 0xb82
  val mhpmcounter3h = 0xb83
  val mhpmcounter4h = 0xb84
  val mhpmcounter5h = 0xb85
  val mhpmcounter6h = 0xb86
  val mhpmcounter7h = 0xb87
  val mhpmcounter8h = 0xb88
  val mhpmcounter9h = 0xb89
  val mhpmcounter10h = 0xb8a
  val mhpmcounter11h = 0xb8b
  val mhpmcounter12h = 0xb8c
  val mhpmcounter13h = 0xb8d
  val mhpmcounter14h = 0xb8e
  val mhpmcounter15h = 0xb8f
  val mhpmcounter16h = 0xb90
  val mhpmcounter17h = 0xb91
  val mhpmcounter18h = 0xb92
  val mhpmcounter19h = 0xb93
  val mhpmcounter20h = 0xb94
  val mhpmcounter21h = 0xb95
  val mhpmcounter22h = 0xb96
  val mhpmcounter23h = 0xb97
  val mhpmcounter24h = 0xb98
  val mhpmcounter25h = 0xb99
  val mhpmcounter26h = 0xb9a
  val mhpmcounter27h = 0xb9b
  val mhpmcounter28h = 0xb9c
  val mhpmcounter29h = 0xb9d
  val mhpmcounter30h = 0xb9e
  val mhpmcounter31h = 0xb9f
  val all = {
    val res = collection.mutable.ArrayBuffer[Int]()
    res += cycle
    res += instret
    res += hpmcounter3
    res += hpmcounter4
    res += hpmcounter5
    res += hpmcounter6
    res += hpmcounter7
    res += hpmcounter8
    res += hpmcounter9
    res += hpmcounter10
    res += hpmcounter11
    res += hpmcounter12
    res += hpmcounter13
    res += hpmcounter14
    res += hpmcounter15
    res += hpmcounter16
    res += hpmcounter17
    res += hpmcounter18
    res += hpmcounter19
    res += hpmcounter20
    res += hpmcounter21
    res += hpmcounter22
    res += hpmcounter23
    res += hpmcounter24
    res += hpmcounter25
    res += hpmcounter26
    res += hpmcounter27
    res += hpmcounter28
    res += hpmcounter29
    res += hpmcounter30
    res += hpmcounter31
    res += mstatus
    res += misa
    res += medeleg
    res += mideleg
    res += mie
    res += mtvec
    res += mscratch
    res += mepc
    res += mcause
    res += mtval
    res += mip
    res += tselect
    res += tdata1
    res += tdata2
    res += tdata3
    res += dcsr
    res += dpc
    res += dscratch
    res += mcycle
    res += minstret
    res += mhpmcounter3
    res += mhpmcounter4
    res += mhpmcounter5
    res += mhpmcounter6
    res += mhpmcounter7
    res += mhpmcounter8
    res += mhpmcounter9
    res += mhpmcounter10
    res += mhpmcounter11
    res += mhpmcounter12
    res += mhpmcounter13
    res += mhpmcounter14
    res += mhpmcounter15
    res += mhpmcounter16
    res += mhpmcounter17
    res += mhpmcounter18
    res += mhpmcounter19
    res += mhpmcounter20
    res += mhpmcounter21
    res += mhpmcounter22
    res += mhpmcounter23
    res += mhpmcounter24
    res += mhpmcounter25
    res += mhpmcounter26
    res += mhpmcounter27
    res += mhpmcounter28
    res += mhpmcounter29
    res += mhpmcounter30
    res += mhpmcounter31
    res += mucounteren
    res += mhpmevent3
    res += mhpmevent4
    res += mhpmevent5
    res += mhpmevent6
    res += mhpmevent7
    res += mhpmevent8
    res += mhpmevent9
    res += mhpmevent10
    res += mhpmevent11
    res += mhpmevent12
    res += mhpmevent13
    res += mhpmevent14
    res += mhpmevent15
    res += mhpmevent16
    res += mhpmevent17
    res += mhpmevent18
    res += mhpmevent19
    res += mhpmevent20
    res += mhpmevent21
    res += mhpmevent22
    res += mhpmevent23
    res += mhpmevent24
    res += mhpmevent25
    res += mhpmevent26
    res += mhpmevent27
    res += mhpmevent28
    res += mhpmevent29
    res += mhpmevent30
    res += mhpmevent31
    res += mvendorid
    res += marchid
    res += mimpid
    res += mhartid
    res.toArray
  }
  val all32 = {
    val res = collection.mutable.ArrayBuffer(all:_*)
    res += cycleh
    res += instreth
    res += hpmcounter3h
    res += hpmcounter4h
    res += hpmcounter5h
    res += hpmcounter6h
    res += hpmcounter7h
    res += hpmcounter8h
    res += hpmcounter9h
    res += hpmcounter10h
    res += hpmcounter11h
    res += hpmcounter12h
    res += hpmcounter13h
    res += hpmcounter14h
    res += hpmcounter15h
    res += hpmcounter16h
    res += hpmcounter17h
    res += hpmcounter18h
    res += hpmcounter19h
    res += hpmcounter20h
    res += hpmcounter21h
    res += hpmcounter22h
    res += hpmcounter23h
    res += hpmcounter24h
    res += hpmcounter25h
    res += hpmcounter26h
    res += hpmcounter27h
    res += hpmcounter28h
    res += hpmcounter29h
    res += hpmcounter30h
    res += hpmcounter31h
    res += mcycleh
    res += minstreth
    res += mhpmcounter3h
    res += mhpmcounter4h
    res += mhpmcounter5h
    res += mhpmcounter6h
    res += mhpmcounter7h
    res += mhpmcounter8h
    res += mhpmcounter9h
    res += mhpmcounter10h
    res += mhpmcounter11h
    res += mhpmcounter12h
    res += mhpmcounter13h
    res += mhpmcounter14h
    res += mhpmcounter15h
    res += mhpmcounter16h
    res += mhpmcounter17h
    res += mhpmcounter18h
    res += mhpmcounter19h
    res += mhpmcounter20h
    res += mhpmcounter21h
    res += mhpmcounter22h
    res += mhpmcounter23h
    res += mhpmcounter24h
    res += mhpmcounter25h
    res += mhpmcounter26h
    res += mhpmcounter27h
    res += mhpmcounter28h
    res += mhpmcounter29h
    res += mhpmcounter30h
    res += mhpmcounter31h
    res.toArray
  }
}


// TODO: use the riscv-dis binary instead. Requires change to Makefile (and the installation of riscv-dis).
// Using Chisel is not very efficient to get a disassemble string. :(
//object Disassemble
//{
//  def apply(insn: UInt, is_mini: Boolean = false) = {
//    val name :: fmt :: Nil = ListLookup(insn, default, table)
//    if (is_mini)
//       sprintf("%s", name)
//    else
//       sprintf("%s %s", name, operands(insn, fmt))
//  }
//
//
//  private def operands(insn: Bits, fmt: Bits): Bits = {
//    val x = Vec(Str("zero"), Str("  ra"), Str("  sp"), Str("  gp"),
//                Str("  tp"), Str("  t0"), Str("  t1"), Str("  t2"),
//                Str("  s0"), Str("  s1"), Str("  a0"), Str("  a1"),
//                Str("  a2"), Str("  a3"), Str("  a4"), Str("  a5"),
//                Str("  a6"), Str("  a7"), Str("  s2"), Str("  s3"),
//                Str("  s4"), Str("  s5"), Str("  s6"), Str("  s7"),
//                Str("  s8"), Str("  s9"), Str(" s10"), Str(" s11"),
//                Str("  t3"), Str("  t4"), Str("  t5"), Str("  t6"))
//    val f = Vec(Str(" ft0"), Str(" ft1"), Str(" ft2"), Str(" ft3"),
//                Str(" ft4"), Str(" ft5"), Str(" ft6"), Str(" ft7"),
//                Str(" fs0"), Str(" fs1"), Str(" fa0"), Str(" fa1"),
//                Str(" fa2"), Str(" fa3"), Str(" fa4"), Str(" fa5"),
//                Str(" fa6"), Str(" fa7"), Str(" fs2"), Str(" fs3"),
//                Str(" fs4"), Str(" fs5"), Str(" fs6"), Str(" fs7"),
//                Str(" fs8"), Str(" fs9"), Str("fs10"), Str("fs11"),
//                Str(" ft8"), Str(" ft9"), Str("ft10"), Str("ft11"))
//    val p = Vec(Str(" cr0"), Str(" cr1"), Str(" cr2"), Str(" cr3"),
//                Str(" cr4"), Str(" cr5"), Str(" cr6"), Str(" cr7"),
//                Str(" cr8"), Str(" cr9"), Str("cr10"), Str("cr11"),
//                Str("cr12"), Str("cr13"), Str("cr14"), Str("cr15"),
//                Str("cr16"), Str("cr17"), Str("cr18"), Str("cr19"),
//                Str("cr20"), Str("cr21"), Str("cr22"), Str("cr23"),
//                Str("cr24"), Str("cr25"), Str("cr26"), Str("cr27"),
//                Str("cr28"), Str("cr29"), Str("cr30"), Str("cr31"))
//
//
//    def hex(x: SInt, plus: Char = ' ') =
//      Cat(Mux(x < SInt(0), Str("-0x"), Str(plus + "0x")), Str(x.abs, 16))
//
//    val comma = Str(',')
//    val lparen = Str('(')
//    val rparen = Str(')')
//
//    val rd = insn(11,7)
//    val rs1 = insn(19,15)
//    val rs2 = insn(24,20)
//    val immv = insn(31,20).toSInt
//    val smmv = Cat(insn(31, 25), insn(11,7)).toSInt
//    val bmmv = Cat(insn(31), insn(7), insn(30,25), insn(11,8)).toSInt
//    val jmmv = Cat(insn(31,7), insn(19,12), insn(20), insn(30,21)).toSInt
//
//    val imm = hex(immv)
//    val bmm = hex(bmmv << UInt(1))
//    val jmm = hex(jmmv << UInt(1))
//    val smm = hex(smmv)
//    val umm = Cat(Str("0x"), Str(insn(31,12).toUInt, 16))
//
//    val laddr = Cat(Str(immv), lparen, x(rs1), rparen)
//    val saddr = Cat(Str(bmmv), lparen, x(rs1), rparen)
//
//    val r0 = x(rd)
//    val r1 = Cat(r0, comma, x(rs1))
//    val r2 = Cat(r1, comma, x(rs2))
//    val f1 = Cat(f(rd), comma, f(rs1))
//    val f2 = Cat(f1, comma, f(rs2))
//    val fx = Cat(f(rd), comma, x(rs1))
//    val xf1 = Cat(x(rd), comma, f(rs1))
//    val xf2 = Cat(xf1, comma, f(rs2))
//    val z = Str(' ')
//    val i = Cat(r1, comma, imm)
//    val b = Cat(x(rs1), comma, x(rs2), comma, bmm)
//    val j = jmm
//    val l = Cat(x(rd), comma, umm)
//    val ld = Cat(x(rd), comma, laddr)
//    val st = Cat(x(rs2), comma, saddr)
//    val fld = Cat(f(rd), comma, laddr)
//    val fst = Cat(f(rs2), comma, saddr)
//    val amo = r2
//    val r2_p = Cat(r0, comma, p(rs1), comma, x(rs2))
//
//    val opts = Seq(r0, r1, r2, f1, f2, UInt(0), fx, xf1, xf2, z, i, b, j, l,  ld, st,
//                   fld, fst, amo, r2_p)
//    val maxLen = opts.map(_.getWidth).reduce(_ max _)
//    val padded = opts.map(x => x.toUInt << UInt(maxLen - x.getWidth))
//    Vec(padded)(fmt.toUInt)
//  }
//
//  private def FMT_R0  = Bits(0, 5)
//  private def FMT_R1  = Bits(1, 5)
//  private def FMT_R2  = Bits(2, 5)
//  private def FMT_F1  = Bits(3, 5)
//  private def FMT_F2  = Bits(4, 5)
//  private def FMT_F3  = Bits(5, 5)
//  private def FMT_FX  = Bits(6, 5)
//  private def FMT_XF1 = Bits(7, 5)
//  private def FMT_XF2 = Bits(8, 5)
//  private def FMT_0   = Bits(9, 5)
//  private def FMT_I   = Bits(10, 5)
//  private def FMT_B   = Bits(11, 5)
//  private def FMT_J   = Bits(12, 5)
//  private def FMT_L   = Bits(13, 5)
//  private def FMT_LD  = Bits(14, 5)
//  private def FMT_ST  = Bits(15, 5)
//  private def FMT_FLD = Bits(16, 5)
//  private def FMT_FST = Bits(17, 5)
//  private def FMT_AMO = Bits(18, 5)
//  private def FMT_R2_P= Bits(19, 5)
//
//  private def default = List(Str("unknown   "), FMT_0)
//
//  import Instructions._
//  private def table = Array(
////    NOP->       List(Str("nop       "), FMT_0),
//    Bits(0x4033,32)-> // machine generated bubble
//                List(Str(" -        "), FMT_0),
//
//    BNE->       List(Str("bne       "), FMT_B),
//    BEQ->       List(Str("beq       "), FMT_B),
//    BLT->       List(Str("blt       "), FMT_B),
//    BLTU->      List(Str("bltu      "), FMT_B),
//    BGE->       List(Str("bge       "), FMT_B),
//    BGEU->      List(Str("bgeu      "), FMT_B),
//
//    JAL->       List(Str("jal       "), FMT_J),
//    JALR->      List(Str("jalr      "), FMT_LD),
//    AUIPC->     List(Str("auipc     "), FMT_L),
//
//    LB->        List(Str("lb        "), FMT_LD),
//    LH->        List(Str("lh        "), FMT_LD),
//    LW->        List(Str("lw        "), FMT_LD),
//    LD->        List(Str("ld        "), FMT_LD),
//    LBU->       List(Str("lbu       "), FMT_LD),
//    LHU->       List(Str("lhu       "), FMT_LD),
//    LWU->       List(Str("lwu       "), FMT_LD),
//    SB->        List(Str("sb        "), FMT_ST),
//    SH->        List(Str("sh        "), FMT_ST),
//    SW->        List(Str("sw        "), FMT_ST),
//    SD->        List(Str("sd        "), FMT_ST),
//
//    AMOADD_W->  List(Str("amoadd.w  "), FMT_AMO),
//    AMOSWAP_W-> List(Str("amoswap.w "), FMT_AMO),
//    AMOAND_W->  List(Str("amoand.w  "), FMT_AMO),
//    AMOOR_W->   List(Str("amoor.w   "), FMT_AMO),
//    AMOMIN_W->  List(Str("amomin.w  "), FMT_AMO),
//    AMOMINU_W-> List(Str("amominu.w "), FMT_AMO),
//    AMOMAX_W->  List(Str("amomax.w  "), FMT_AMO),
//    AMOMAXU_W-> List(Str("amomaxu.w "), FMT_AMO),
//    AMOADD_D->  List(Str("amoadd.d  "), FMT_AMO),
//    AMOSWAP_D-> List(Str("amoswap.d "), FMT_AMO),
//    AMOAND_D->  List(Str("amoand.d  "), FMT_AMO),
//    AMOOR_D->   List(Str("amoor.d   "), FMT_AMO),
//    AMOMIN_D->  List(Str("amomin.d  "), FMT_AMO),
//    AMOMINU_D-> List(Str("amominu.d "), FMT_AMO),
//    AMOMAX_D->  List(Str("amomax.d  "), FMT_AMO),
//    AMOMAXU_D-> List(Str("amomaxu.d "), FMT_AMO),
//
//    LR_W->      List(Str("lr.w      "), FMT_AMO),
//    LR_D->      List(Str("lr.d      "), FMT_AMO),
//    SC_W->      List(Str("sc.w      "), FMT_AMO),
//    SC_D->      List(Str("sc.d      "), FMT_AMO),
//
//    LUI->       List(Str("lui       "), FMT_L),
//    ADDI->      List(Str("addi      "), FMT_I),
//    SLTI ->     List(Str("slti      "), FMT_I),
//    SLTIU->     List(Str("sltiu     "), FMT_I),
//    ANDI->      List(Str("andi      "), FMT_I),
//    ORI->       List(Str("ori       "), FMT_I),
//    XORI->      List(Str("xori      "), FMT_I),
//    SLLI->      List(Str("slli      "), FMT_I),
//    SRLI->      List(Str("srli      "), FMT_I),
//    SRAI->      List(Str("srai      "), FMT_I),
//    ADD->       List(Str("add       "), FMT_R2),
//    SUB->       List(Str("sub       "), FMT_R2),
//    SLT->       List(Str("slt       "), FMT_R2),
//    SLTU->      List(Str("sltu      "), FMT_R2),
//    AND->       List(Str("and       "), FMT_R2),
//    OR->        List(Str("or        "), FMT_R2),
//    XOR->       List(Str("xor       "), FMT_R2),
//    SLL->       List(Str("sll       "), FMT_R2),
//    SRL->       List(Str("srl       "), FMT_R2),
//    SRA->       List(Str("sra       "), FMT_R2),
//
//    ADDIW->     List(Str("addiw     "), FMT_I),
//    SLLIW->     List(Str("slliw     "), FMT_I),
//    SRLIW->     List(Str("srliw     "), FMT_I),
//    SRAIW->     List(Str("sraiw     "), FMT_I),
//    ADDW->      List(Str("addw      "), FMT_R2),
//    SUBW->      List(Str("subw      "), FMT_R2),
//    SLLW->      List(Str("sllw      "), FMT_R2),
//    SRLW->      List(Str("srlw      "), FMT_R2),
//    SRAW->      List(Str("sraw      "), FMT_R2),
//
//    MUL->       List(Str("mul       "), FMT_R2),
//    MULH->      List(Str("mulh      "), FMT_R2),
//    MULHU->     List(Str("mulhu     "), FMT_R2),
//    MULHSU->    List(Str("mulhsu    "), FMT_R2),
//    MULW->      List(Str("mulw      "), FMT_R2),
//
//    DIV->       List(Str("div       "), FMT_R2),
//    DIVU->      List(Str("divu      "), FMT_R2),
//    REM->       List(Str("rem       "), FMT_R2),
//    REMU->      List(Str("remu      "), FMT_R2),
//    DIVW->      List(Str("divw      "), FMT_R2),
//    DIVUW->     List(Str("divuw     "), FMT_R2),
//    REMW->      List(Str("remw      "), FMT_R2),
//    REMUW->     List(Str("remuw     "), FMT_R2),
//
//    SCALL->     List(Str("scall     "), FMT_0),
//    CSRRW->     List(Str("csrrw     "), FMT_I),
//    CSRRS->     List(Str("csrrs     "), FMT_I),
//    CSRRC->     List(Str("csrrc     "), FMT_I),
//    CSRRWI->    List(Str("csrrwi    "), FMT_I),
//    CSRRSI->    List(Str("csrrsi    "), FMT_I),
//    CSRRCI->    List(Str("csrrci    "), FMT_I),
//    SRET->      List(Str("sret      "), FMT_0),
//    FENCE->     List(Str("fence     "), FMT_0),
//    FENCE_I->   List(Str("fence.i   "), FMT_0)
///*
//    FCVT_S_D->  List(Str("fcvt.s.d  "), FMT_F1),
//    FCVT_D_S->  List(Str("fcvt.d.s  "), FMT_F1),
//    FSGNJ_S->   List(Str("fsgnj.s   "), FMT_F2),
//    FSGNJ_D->   List(Str("fsgnj.d   "), FMT_F2),
//    FSGNJX_S->  List(Str("fsgnx.s   "), FMT_F2),
//    FSGNJX_D->  List(Str("fsgnx.d   "), FMT_F2),
//    FSGNJN_S->  List(Str("fsgnjn.s  "), FMT_F2),
//    FSGNJN_D->  List(Str("fsgnjn.d  "), FMT_F2),
//    FMIN_S->    List(Str("fmin.s    "), FMT_F2),
//    FMIN_D->    List(Str("fmin.d    "), FMT_F2),
//    FMAX_S->    List(Str("fmax.s    "), FMT_F2),
//    FMAX_D->    List(Str("fmax.d    "), FMT_F2),
//    FADD_S->    List(Str("fadd.s    "), FMT_F2),
//    FADD_D->    List(Str("fadd.d    "), FMT_F2),
//    FSUB_S->    List(Str("fsub.s    "), FMT_F2),
//    FSUB_D->    List(Str("fsub.d    "), FMT_F2),
//    FMUL_S->    List(Str("fmul.s    "), FMT_F2),
//    FMUL_D->    List(Str("fmul.d    "), FMT_F2),
//    FMADD_S->   List(Str("fmadd.s   "), FMT_F3),
//    FMADD_D->   List(Str("fmadd.d   "), FMT_F3),
//    FMSUB_S->   List(Str("fmsub.s   "), FMT_F3),
//    FMSUB_D->   List(Str("fmsub.d   "), FMT_F3),
//    FNMADD_S->  List(Str("fnmadd.s  "), FMT_F3),
//    FNMADD_D->  List(Str("fnmadd.d  "), FMT_F3),
//    FNMSUB_S->  List(Str("fnmsub.s  "), FMT_F3),
//    FNMSUB_D->  List(Str("fnmsub.d  "), FMT_F3),
//    FCVT_W_S->  List(Str("fcvt.w.s  "), FMT_XF1),
//    FCVT_W_D->  List(Str("fcvt.w.d  "), FMT_XF1),
//    FCVT_WU_S-> List(Str("fcvt.wu.s "), FMT_XF1),
//    FCVT_WU_D-> List(Str("fcvt.wu.d "), FMT_XF1),
//    FCVT_L_S->  List(Str("fcvt.l.s  "), FMT_XF1),
//    FCVT_L_D->  List(Str("fcvt.l.d  "), FMT_XF1),
//    FCVT_LU_S-> List(Str("fcvt.lu.s "), FMT_XF1),
//    FCVT_LU_D-> List(Str("fcvt.lu.d "), FMT_XF1),
//    FEQ_S->     List(Str("feq.s     "), FMT_XF2),
//    FEQ_D->     List(Str("feq.d     "), FMT_XF2),
//    FLT_S->     List(Str("flt.s     "), FMT_XF2),
//    FLT_D->     List(Str("flt.d     "), FMT_XF2),
//    FLE_S->     List(Str("fle.s     "), FMT_XF2),
//    FLE_D->     List(Str("fle.d     "), FMT_XF2),
//    FCVT_S_W->  List(Str("fcvt.s.w  "), FMT_FX),
//    FCVT_D_W->  List(Str("fcvt.d.w  "), FMT_FX),
//    FCVT_S_WU-> List(Str("fcvt.s.wu "), FMT_FX),
//    FCVT_D_WU-> List(Str("fcvt.d.wu "), FMT_FX),
//    FCVT_S_L->  List(Str("fcvt.s.l  "), FMT_FX),
//    FCVT_D_L->  List(Str("fcvt.d.l  "), FMT_FX),
//    FCVT_S_LU-> List(Str("fcvt.s.lu "), FMT_FX),
//    FCVT_D_LU-> List(Str("fcvt.d.lu "), FMT_FX),
//    FLW->       List(Str("flw       "), FMT_FLD),
//    FLD->       List(Str("fld       "), FMT_FLD),
//    FSW->       List(Str("fsw       "), FMT_FST),
//    FSD->       List(Str("fsd       "), FMT_FST),
//
//    VVCFGIVL->  List(Str("vecInst   "), FMT_0),
//    VVCFG->     List(Str("vecInst   "), FMT_0),
//    VSETVL->    List(Str("vecInst   "), FMT_0),
//    VF->        List(Str("vecInst   "), FMT_0),
//    VMVV->      List(Str("vecInst   "), FMT_0),
//    VMSV->      List(Str("vecInst   "), FMT_0),
//    VFMVV->     List(Str("vecInst   "), FMT_0),
//    FENCE_V_L-> List(Str("vecInst   "), FMT_0),
//    FENCE_V_G-> List(Str("vecInst   "), FMT_0),
//    VLD->       List(Str("vecInst   "), FMT_0),
//    VLW->       List(Str("vecInst   "), FMT_0),
//    VLWU->      List(Str("vecInst   "), FMT_0),
//    VLH->       List(Str("vecInst   "), FMT_0),
//    VLHU->      List(Str("vecInst   "), FMT_0),
//    VLB->       List(Str("vecInst   "), FMT_0),
//    VLBU->      List(Str("vecInst   "), FMT_0),
//    VSD->       List(Str("vecInst   "), FMT_0),
//    VSW->       List(Str("vecInst   "), FMT_0),
//    VSH->       List(Str("vecInst   "), FMT_0),
//    VSB->       List(Str("vecInst   "), FMT_0),
//    VFLD->      List(Str("vecInst   "), FMT_0),
//    VFLW->      List(Str("vecInst   "), FMT_0),
//    VFSD->      List(Str("vecInst   "), FMT_0),
//    VFSW->      List(Str("vecInst   "), FMT_0),
//    VLSTD->     List(Str("vecInst   "), FMT_0),
//    VLSTW->     List(Str("vecInst   "), FMT_0),
//    VLSTWU->    List(Str("vecInst   "), FMT_0),
//    VLSTH->     List(Str("vecInst   "), FMT_0),
//    VLSTHU->    List(Str("vecInst   "), FMT_0),
//    VLSTB->     List(Str("vecInst   "), FMT_0),
//    VLSTBU->    List(Str("vecInst   "), FMT_0),
//    VSSTD->     List(Str("vecInst   "), FMT_0),
//    VSSTW->     List(Str("vecInst   "), FMT_0),
//    VSSTH->     List(Str("vecInst   "), FMT_0),
//    VSSTB->     List(Str("vecInst   "), FMT_0),
//    VFLSTD->    List(Str("vecInst   "), FMT_0),
//    VFLSTW->    List(Str("vecInst   "), FMT_0),
//    VFSSTD->    List(Str("vecInst   "), FMT_0),
//    VFSSTW->    List(Str("vecInst   "), FMT_0),
//
//    VENQCMD->   List(Str("vecInst   "), FMT_0),
//    VENQIMM1->  List(Str("vecInst   "), FMT_0),
//    VENQIMM2->  List(Str("vecInst   "), FMT_0),
//    VENQCNT->   List(Str("vecInst   "), FMT_0),
//    VXCPTEVAC-> List(Str("vecInst   "), FMT_0),
//    VXCPTKILL-> List(Str("vecInst   "), FMT_0),
//    VXCPTHOLD-> List(Str("vecInst   "), FMT_0)
//  */
//  )
//}
