//===-- DadaoAsmParser.cpp - Parse Dadao assembly to MCInst instructions --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DadaoAluCode.h"
#include "DadaoCondCode.h"
#include "DadaoInstrInfo.h"
#include "MCTargetDesc/DadaoMCExpr.h"
#include "TargetInfo/DadaoTargetInfo.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCParser/MCAsmLexer.h"
#include "llvm/MC/MCParser/MCAsmParser.h"
#include "llvm/MC/MCParser/MCParsedAsmOperand.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/SMLoc.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>

using namespace llvm;

// Auto-generated by TableGen
static unsigned MatchRegisterName(StringRef Name);

namespace {

struct DadaoOperand;

class DadaoAsmParser : public MCTargetAsmParser {
  // Parse operands
  std::unique_ptr<DadaoOperand> parseRegister(bool RestoreOnFailure = false);

  std::unique_ptr<DadaoOperand> parseImmediate();

  std::unique_ptr<DadaoOperand> parseIdentifier();

  unsigned parseAluOperator(bool PreOp, bool PostOp);

  // Split the mnemonic stripping conditional code and quantifiers
  StringRef splitMnemonic(StringRef Name, SMLoc NameLoc,
                          OperandVector *Operands);

  bool parsePrePost(StringRef Type, int *OffsetValue);

  bool ParseDirective(AsmToken DirectiveID) override;

  bool ParseInstruction(ParseInstructionInfo &Info, StringRef Name,
                        SMLoc NameLoc, OperandVector &Operands) override;

  bool parseRegister(MCRegister &RegNum, SMLoc &StartLoc,
                     SMLoc &EndLoc) override;
  OperandMatchResultTy tryParseRegister(MCRegister &RegNo, SMLoc &StartLoc,
                                        SMLoc &EndLoc) override;

  bool MatchAndEmitInstruction(SMLoc IdLoc, unsigned &Opcode,
                               OperandVector &Operands, MCStreamer &Out,
                               uint64_t &ErrorInfo,
                               bool MatchingInlineAsm) override;

// Auto-generated instruction matching functions
#define GET_ASSEMBLER_HEADER
#include "DadaoGenAsmMatcher.inc"

  OperandMatchResultTy parseOperand(OperandVector *Operands,
                                    StringRef Mnemonic);

  OperandMatchResultTy parseMemoryOperand(OperandVector &Operands);

public:
  DadaoAsmParser(const MCSubtargetInfo &STI, MCAsmParser &Parser,
                 const MCInstrInfo &MII, const MCTargetOptions &Options)
      : MCTargetAsmParser(Options, STI, MII), Parser(Parser),
        Lexer(Parser.getLexer()), SubtargetInfo(STI) {
    setAvailableFeatures(
        ComputeAvailableFeatures(SubtargetInfo.getFeatureBits()));
  }

private:
  MCAsmParser &Parser;
  MCAsmLexer &Lexer;

  const MCSubtargetInfo &SubtargetInfo;
};

// DadaoOperand - Instances of this class represented a parsed machine
// instruction
struct DadaoOperand : public MCParsedAsmOperand {
  enum KindTy {
    TOKEN,
    REGISTER,
    IMMEDIATE,
    MEMORY_REG_IMM,
    MEMORY_REG_REG,
  } Kind;

  SMLoc StartLoc, EndLoc;

  struct Token {
    const char *Data;
    unsigned Length;
  };

  struct RegOp {
    unsigned RegNum;
  };

  struct ImmOp {
    const MCExpr *Value;
  };

  struct MemOp {
    unsigned BaseReg;
    unsigned OffsetReg;
    unsigned AluOp;
    const MCExpr *Offset;
  };

  union {
    struct Token Tok;
    struct RegOp Reg;
    struct ImmOp Imm;
    struct MemOp Mem;
  };

  explicit DadaoOperand(KindTy Kind) : Kind(Kind) {}

public:
  // The functions below are used by the autogenerated ASM matcher and hence to
  // be of the form expected.

  // getStartLoc - Gets location of the first token of this operand
  SMLoc getStartLoc() const override { return StartLoc; }

  // getEndLoc - Gets location of the last token of this operand
  SMLoc getEndLoc() const override { return EndLoc; }

  unsigned getReg() const override {
    assert(isReg() && "Invalid type access!");
    return Reg.RegNum;
  }

  const MCExpr *getImm() const {
    assert(isImm() && "Invalid type access!");
    return Imm.Value;
  }

  StringRef getToken() const {
    assert(isToken() && "Invalid type access!");
    return StringRef(Tok.Data, Tok.Length);
  }

  unsigned getMemBaseReg() const {
    assert(isMem() && "Invalid type access!");
    return Mem.BaseReg;
  }

  unsigned getMemOffsetReg() const {
    assert(isMem() && "Invalid type access!");
    return Mem.OffsetReg;
  }

  const MCExpr *getMemOffset() const {
    assert(isMem() && "Invalid type access!");
    return Mem.Offset;
  }

  unsigned getMemOp() const {
    assert(isMem() && "Invalid type access!");
    return Mem.AluOp;
  }

  // Functions for testing operand type
  bool isReg() const override { return Kind == REGISTER; }

  bool isImm() const override { return Kind == IMMEDIATE; }

  bool isMem() const override {
    return isMemRegImm() || isMemRegReg();
  }

  bool isMemRegImm() const { return Kind == MEMORY_REG_IMM; }

  bool isMemRegReg() const { return Kind == MEMORY_REG_REG; }

  bool isToken() const override { return Kind == TOKEN; }

  bool isBrImm() {
    if (!isImm())
      return false;

    // Constant case
    const MCConstantExpr *MCE = dyn_cast<MCConstantExpr>(Imm.Value);
    if (!MCE)
      return true;
    int64_t Value = MCE->getValue();
    // Check if value fits in 25 bits with 2 least significant bits 0.
    return isShiftedUInt<23, 2>(static_cast<int32_t>(Value));
  }

  bool isBrTarget() { return isBrImm() || isToken(); }

  bool isCallTarget() { return isImm() || isToken(); }

  bool isHiImm16() {
    if (!isImm())
      return false;

    // Constant case
    if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Imm.Value)) {
      int64_t Value = ConstExpr->getValue();
      return Value != 0 && isShiftedUInt<16, 16>(Value);
    }

    // Symbolic reference expression
    if (const DadaoMCExpr *SymbolRefExpr = dyn_cast<DadaoMCExpr>(Imm.Value))
      return SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_HI;

    // Binary expression
    if (const MCBinaryExpr *BinaryExpr = dyn_cast<MCBinaryExpr>(Imm.Value))
      if (const DadaoMCExpr *SymbolRefExpr =
              dyn_cast<DadaoMCExpr>(BinaryExpr->getLHS()))
        return SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_HI;

    return false;
  }

  bool isHiImm16And() {
    if (!isImm())
      return false;

    const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Imm.Value);
    if (ConstExpr) {
      int64_t Value = ConstExpr->getValue();
      // Check if in the form 0xXYZWffff
      return (Value != 0) && ((Value & ~0xffff0000) == 0xffff);
    }
    return false;
  }

  bool isLoImm16() {
    if (!isImm())
      return false;

    // Constant case
    if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Imm.Value)) {
      int64_t Value = ConstExpr->getValue();
      // Check if value fits in 16 bits
      return isUInt<16>(static_cast<int32_t>(Value));
    }

    // Symbolic reference expression
    if (const DadaoMCExpr *SymbolRefExpr = dyn_cast<DadaoMCExpr>(Imm.Value))
      return SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_LO;

    // Binary expression
    if (const MCBinaryExpr *BinaryExpr = dyn_cast<MCBinaryExpr>(Imm.Value))
      if (const DadaoMCExpr *SymbolRefExpr =
              dyn_cast<DadaoMCExpr>(BinaryExpr->getLHS()))
        return SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_LO;

    return false;
  }

  bool isLoImm16Signed() {
    if (!isImm())
      return false;

    // Constant case
    if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Imm.Value)) {
      int64_t Value = ConstExpr->getValue();
      // Check if value fits in 16 bits or value of the form 0xffffxyzw
      return isInt<16>(static_cast<int32_t>(Value));
    }

    // Symbolic reference expression
    if (const DadaoMCExpr *SymbolRefExpr = dyn_cast<DadaoMCExpr>(Imm.Value))
      return SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_LO;

    // Binary expression
    if (const MCBinaryExpr *BinaryExpr = dyn_cast<MCBinaryExpr>(Imm.Value))
      if (const DadaoMCExpr *SymbolRefExpr =
              dyn_cast<DadaoMCExpr>(BinaryExpr->getLHS()))
        return SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_LO;

    return false;
  }

  bool isLoImm16And() {
    if (!isImm())
      return false;

    const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Imm.Value);
    if (ConstExpr) {
      int64_t Value = ConstExpr->getValue();
      // Check if in the form 0xffffXYZW
      return ((Value & ~0xffff) == 0xffff0000);
    }
    return false;
  }

  bool isImmShift() {
    if (!isImm())
      return false;

    const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Imm.Value);
    if (!ConstExpr)
      return false;
    int64_t Value = ConstExpr->getValue();
    return (Value >= -31) && (Value <= 31);
  }

  bool isImm12() {
    if (!isImm())
      return false;

    const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Imm.Value);
    if (!ConstExpr)
      return false;
    int64_t Value = ConstExpr->getValue();
    return isInt<12>(Value);
  }

  bool isLoImm12() {
    if (!isImm())
      return false;

    // Constant case
    if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Imm.Value)) {
      int64_t Value = ConstExpr->getValue();
      // Check if value fits in 12 bits
      return isUInt<12>(static_cast<int64_t>(Value));
    }

    // Symbolic reference expression
    if (const DadaoMCExpr *SymbolRefExpr = dyn_cast<DadaoMCExpr>(Imm.Value))
      return SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_LO;

    // Binary expression
    if (const MCBinaryExpr *BinaryExpr = dyn_cast<MCBinaryExpr>(Imm.Value))
      if (const DadaoMCExpr *SymbolRefExpr =
              dyn_cast<DadaoMCExpr>(BinaryExpr->getLHS()))
        return SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_LO;

    return false;
  }

  bool isLoImm12And() {
    if (!isImm())
      return false;

    const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Imm.Value);
    if (ConstExpr) {
      int64_t Value = ConstExpr->getValue();
      // Check if in the form 0xfffXYZW
      return ((Value & ~0xFFF) == ~0xFFF);
    }
    return false;
  }

  bool isCondCode() {
    if (!isImm())
      return false;

    const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Imm.Value);
    if (!ConstExpr)
      return false;
    uint64_t Value = ConstExpr->getValue();
    // The condition codes are between 0 (ICC_T) and 15 (ICC_LE). If the
    // unsigned value of the immediate is less than LPCC::UNKNOWN (16) then
    // value corresponds to a valid condition code.
    return Value < LPCC::UNKNOWN;
  }

  void addExpr(MCInst &Inst, const MCExpr *Expr) const {
    // Add as immediates where possible. Null MCExpr = 0
    if (Expr == nullptr)
      Inst.addOperand(MCOperand::createImm(0));
    else if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(Expr))
      Inst.addOperand(
          MCOperand::createImm(static_cast<int32_t>(ConstExpr->getValue())));
    else
      Inst.addOperand(MCOperand::createExpr(Expr));
  }

  void addRegOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    Inst.addOperand(MCOperand::createReg(getReg()));
  }

  void addImmOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getImm());
  }

  void addBrTargetOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getImm());
  }

  void addCallTargetOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getImm());
  }

  void addCondCodeOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getImm());
  }

  void addMemRegImmOperands(MCInst &Inst, unsigned N) const {
    assert(N == 3 && "Invalid number of operands!");
    Inst.addOperand(MCOperand::createReg(getMemBaseReg()));
    const MCExpr *Expr = getMemOffset();
    addExpr(Inst, Expr);
    Inst.addOperand(MCOperand::createImm(getMemOp()));
  }

  void addMemRegRegOperands(MCInst &Inst, unsigned N) const {
    assert(N == 3 && "Invalid number of operands!");
    Inst.addOperand(MCOperand::createReg(getMemBaseReg()));
    assert(getMemOffsetReg() != 0 && "Invalid offset");
    Inst.addOperand(MCOperand::createReg(getMemOffsetReg()));
    Inst.addOperand(MCOperand::createImm(getMemOp()));
  }

  void addImmShiftOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getImm());
  }

  void addImm12Operands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    addExpr(Inst, getImm());
  }

  void addLoImm16Operands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(getImm()))
      Inst.addOperand(
          MCOperand::createImm(static_cast<int32_t>(ConstExpr->getValue())));
    else if (isa<DadaoMCExpr>(getImm())) {
#ifndef NDEBUG
      const DadaoMCExpr *SymbolRefExpr = dyn_cast<DadaoMCExpr>(getImm());
      assert(SymbolRefExpr &&
             SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_LO);
#endif
      Inst.addOperand(MCOperand::createExpr(getImm()));
    } else if (isa<MCBinaryExpr>(getImm())) {
#ifndef NDEBUG
      const MCBinaryExpr *BinaryExpr = dyn_cast<MCBinaryExpr>(getImm());
      assert(BinaryExpr && isa<DadaoMCExpr>(BinaryExpr->getLHS()) &&
             cast<DadaoMCExpr>(BinaryExpr->getLHS())->getKind() ==
                 DadaoMCExpr::VK_Dadao_ABS_LO);
#endif
      Inst.addOperand(MCOperand::createExpr(getImm()));
    } else
      assert(false && "Operand type not supported.");
  }

  void addLoImm16AndOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(getImm()))
      Inst.addOperand(MCOperand::createImm(ConstExpr->getValue() & 0xffff));
    else
      assert(false && "Operand type not supported.");
  }

  void addHiImm16Operands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(getImm()))
      Inst.addOperand(MCOperand::createImm(ConstExpr->getValue() >> 16));
    else if (isa<DadaoMCExpr>(getImm())) {
#ifndef NDEBUG
      const DadaoMCExpr *SymbolRefExpr = dyn_cast<DadaoMCExpr>(getImm());
      assert(SymbolRefExpr &&
             SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_HI);
#endif
      Inst.addOperand(MCOperand::createExpr(getImm()));
    } else if (isa<MCBinaryExpr>(getImm())) {
#ifndef NDEBUG
      const MCBinaryExpr *BinaryExpr = dyn_cast<MCBinaryExpr>(getImm());
      assert(BinaryExpr && isa<DadaoMCExpr>(BinaryExpr->getLHS()) &&
             cast<DadaoMCExpr>(BinaryExpr->getLHS())->getKind() ==
                 DadaoMCExpr::VK_Dadao_ABS_HI);
#endif
      Inst.addOperand(MCOperand::createExpr(getImm()));
    } else
      assert(false && "Operand type not supported.");
  }

  void addHiImm16AndOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(getImm()))
      Inst.addOperand(MCOperand::createImm(ConstExpr->getValue() >> 16));
    else
      assert(false && "Operand type not supported.");
  }

  void addLoImm12Operands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(getImm()))
      Inst.addOperand(
          MCOperand::createImm(static_cast<int64_t>(ConstExpr->getValue())));
    else if (isa<DadaoMCExpr>(getImm())) {
#ifndef NDEBUG
      const DadaoMCExpr *SymbolRefExpr = dyn_cast<DadaoMCExpr>(getImm());
      assert(SymbolRefExpr &&
             SymbolRefExpr->getKind() == DadaoMCExpr::VK_Dadao_ABS_LO);
#endif
      Inst.addOperand(MCOperand::createExpr(getImm()));
    } else if (isa<MCBinaryExpr>(getImm())) {
#ifndef NDEBUG
      const MCBinaryExpr *BinaryExpr = dyn_cast<MCBinaryExpr>(getImm());
      assert(BinaryExpr && isa<DadaoMCExpr>(BinaryExpr->getLHS()) &&
             cast<DadaoMCExpr>(BinaryExpr->getLHS())->getKind() ==
                 DadaoMCExpr::VK_Dadao_ABS_LO);
#endif
      Inst.addOperand(MCOperand::createExpr(getImm()));
    } else
      assert(false && "Operand type not supported.");
  }

  void addLoImm12AndOperands(MCInst &Inst, unsigned N) const {
    assert(N == 1 && "Invalid number of operands!");
    if (const MCConstantExpr *ConstExpr = dyn_cast<MCConstantExpr>(getImm()))
      Inst.addOperand(MCOperand::createImm(ConstExpr->getValue() & 0xfff));
    else
      assert(false && "Operand type not supported.");
  }

  void print(raw_ostream &OS) const override {
    switch (Kind) {
    case IMMEDIATE:
      OS << "Imm: " << getImm() << "\n";
      break;
    case TOKEN:
      OS << "Token: " << getToken() << "\n";
      break;
    case REGISTER:
      OS << "Reg: %r" << getReg() << "\n";
      break;
    case MEMORY_REG_IMM:
      OS << "MemRegImm: " << getMemBaseReg() << "+" << *getMemOffset() << "\n";
      break;
    case MEMORY_REG_REG:
      assert(getMemOffset() == nullptr);
      OS << "MemRegReg: " << getMemBaseReg() << "+"
         << "%r" << getMemOffsetReg() << "\n";
      break;
    }
  }

  static std::unique_ptr<DadaoOperand> CreateToken(StringRef Str, SMLoc Start) {
    auto Op = std::make_unique<DadaoOperand>(TOKEN);
    Op->Tok.Data = Str.data();
    Op->Tok.Length = Str.size();
    Op->StartLoc = Start;
    Op->EndLoc = Start;
    return Op;
  }

  static std::unique_ptr<DadaoOperand> createReg(unsigned RegNum, SMLoc Start,
                                                 SMLoc End) {
    auto Op = std::make_unique<DadaoOperand>(REGISTER);
    Op->Reg.RegNum = RegNum;
    Op->StartLoc = Start;
    Op->EndLoc = End;
    return Op;
  }

  static std::unique_ptr<DadaoOperand> createImm(const MCExpr *Value,
                                                 SMLoc Start, SMLoc End) {
    auto Op = std::make_unique<DadaoOperand>(IMMEDIATE);
    Op->Imm.Value = Value;
    Op->StartLoc = Start;
    Op->EndLoc = End;
    return Op;
  }

  static std::unique_ptr<DadaoOperand>
  MorphToMemRegReg(unsigned BaseReg, std::unique_ptr<DadaoOperand> Op,
                   unsigned AluOp) {
    unsigned OffsetReg = Op->getReg();
    Op->Kind = MEMORY_REG_REG;
    Op->Mem.BaseReg = BaseReg;
    Op->Mem.AluOp = AluOp;
    Op->Mem.OffsetReg = OffsetReg;
    Op->Mem.Offset = nullptr;
    return Op;
  }

  static std::unique_ptr<DadaoOperand>
  MorphToMemRegImm(unsigned BaseReg, std::unique_ptr<DadaoOperand> Op,
                   unsigned AluOp) {
    const MCExpr *Imm = Op->getImm();
    Op->Kind = MEMORY_REG_IMM;
    Op->Mem.BaseReg = BaseReg;
    Op->Mem.AluOp = AluOp;
    Op->Mem.OffsetReg = 0;
    Op->Mem.Offset = Imm;
    return Op;
  }
};

} // end anonymous namespace

bool DadaoAsmParser::ParseDirective(AsmToken /*DirectiveId*/) { return true; }

bool DadaoAsmParser::MatchAndEmitInstruction(SMLoc IdLoc, unsigned &Opcode,
                                             OperandVector &Operands,
                                             MCStreamer &Out,
                                             uint64_t &ErrorInfo,
                                             bool MatchingInlineAsm) {
  MCInst Inst;
  SMLoc ErrorLoc;

  switch (MatchInstructionImpl(Operands, Inst, ErrorInfo, MatchingInlineAsm)) {
  case Match_Success:
    Out.emitInstruction(Inst, SubtargetInfo);
    Opcode = Inst.getOpcode();
    return false;
  case Match_MissingFeature:
    return Error(IdLoc, "Instruction use requires option to be enabled");
  case Match_MnemonicFail:
    return Error(IdLoc, "Unrecognized instruction mnemonic");
  case Match_InvalidOperand: {
    ErrorLoc = IdLoc;
    if (ErrorInfo != ~0U) {
      if (ErrorInfo >= Operands.size())
        return Error(IdLoc, "Too few operands for instruction");

      ErrorLoc = ((DadaoOperand &)*Operands[ErrorInfo]).getStartLoc();
      if (ErrorLoc == SMLoc())
        ErrorLoc = IdLoc;
    }
    return Error(ErrorLoc, "Invalid operand for instruction");
  }
  default:
    break;
  }

  llvm_unreachable("Unknown match type detected!");
}

// Both '%rN' and 'rN' are parsed as valid registers. This was done to remain
// backwards compatible with GCC and the different ways inline assembly is
// handled.
// TODO: see if there isn't a better way to do this.
std::unique_ptr<DadaoOperand>
DadaoAsmParser::parseRegister(bool RestoreOnFailure) {
  SMLoc Start = Parser.getTok().getLoc();
  SMLoc End = SMLoc::getFromPointer(Parser.getTok().getLoc().getPointer() - 1);
  std::optional<AsmToken> PercentTok;

  unsigned RegNum;
  // Eat the '%'.
  if (Lexer.getKind() == AsmToken::Percent) {
    PercentTok = Parser.getTok();
    Parser.Lex();
  }
  if (Lexer.getKind() == AsmToken::Identifier) {
    RegNum = MatchRegisterName(Lexer.getTok().getIdentifier());
    if (RegNum == 0) {
      if (PercentTok && RestoreOnFailure)
        Lexer.UnLex(*PercentTok);
      return nullptr;
    }
    Parser.Lex(); // Eat identifier token
    return DadaoOperand::createReg(RegNum, Start, End);
  }
  if (PercentTok && RestoreOnFailure)
    Lexer.UnLex(*PercentTok);
  return nullptr;
}

bool DadaoAsmParser::parseRegister(MCRegister &RegNum, SMLoc &StartLoc,
                                   SMLoc &EndLoc) {
  const AsmToken &Tok = getParser().getTok();
  StartLoc = Tok.getLoc();
  EndLoc = Tok.getEndLoc();
  std::unique_ptr<DadaoOperand> Op = parseRegister(/*RestoreOnFailure=*/false);
  if (Op != nullptr)
    RegNum = Op->getReg();
  return (Op == nullptr);
}

OperandMatchResultTy DadaoAsmParser::tryParseRegister(MCRegister &RegNum,
                                                      SMLoc &StartLoc,
                                                      SMLoc &EndLoc) {
  const AsmToken &Tok = getParser().getTok();
  StartLoc = Tok.getLoc();
  EndLoc = Tok.getEndLoc();
  std::unique_ptr<DadaoOperand> Op = parseRegister(/*RestoreOnFailure=*/true);
  if (Op == nullptr)
    return MatchOperand_NoMatch;
  RegNum = Op->getReg();
  return MatchOperand_Success;
}

std::unique_ptr<DadaoOperand> DadaoAsmParser::parseIdentifier() {
  SMLoc Start = Parser.getTok().getLoc();
  SMLoc End = SMLoc::getFromPointer(Parser.getTok().getLoc().getPointer() - 1);
  const MCExpr *Res, *RHS = nullptr;
  DadaoMCExpr::VariantKind Kind = DadaoMCExpr::VK_Dadao_None;

  if (Lexer.getKind() != AsmToken::Identifier)
    return nullptr;

  StringRef Identifier;
  if (Parser.parseIdentifier(Identifier))
    return nullptr;

  // Check if identifier has a modifier
  if (Identifier.equals_insensitive("hi"))
    Kind = DadaoMCExpr::VK_Dadao_ABS_HI;
  else if (Identifier.equals_insensitive("lo"))
    Kind = DadaoMCExpr::VK_Dadao_ABS_LO;

  // If the identifier corresponds to a variant then extract the real
  // identifier.
  if (Kind != DadaoMCExpr::VK_Dadao_None) {
    if (Lexer.getKind() != AsmToken::LParen) {
      Error(Lexer.getLoc(), "Expected '('");
      return nullptr;
    }
    Lexer.Lex(); // lex '('

    // Parse identifier
    if (Parser.parseIdentifier(Identifier))
      return nullptr;
  }

  // If addition parse the RHS.
  if (Lexer.getKind() == AsmToken::Plus && Parser.parseExpression(RHS))
    return nullptr;

  // For variants parse the final ')'
  if (Kind != DadaoMCExpr::VK_Dadao_None) {
    if (Lexer.getKind() != AsmToken::RParen) {
      Error(Lexer.getLoc(), "Expected ')'");
      return nullptr;
    }
    Lexer.Lex(); // lex ')'
  }

  End = SMLoc::getFromPointer(Parser.getTok().getLoc().getPointer() - 1);
  MCSymbol *Sym = getContext().getOrCreateSymbol(Identifier);
  const MCExpr *Expr = MCSymbolRefExpr::create(Sym, getContext());
  Res = DadaoMCExpr::create(Kind, Expr, getContext());

  // Nest if this was an addition
  if (RHS)
    Res = MCBinaryExpr::createAdd(Res, RHS, getContext());

  return DadaoOperand::createImm(Res, Start, End);
}

std::unique_ptr<DadaoOperand> DadaoAsmParser::parseImmediate() {
  SMLoc Start = Parser.getTok().getLoc();
  SMLoc End = SMLoc::getFromPointer(Parser.getTok().getLoc().getPointer() - 1);

  const MCExpr *ExprVal;
  switch (Lexer.getKind()) {
  case AsmToken::Identifier:
    return parseIdentifier();
  case AsmToken::Plus:
  case AsmToken::Minus:
  case AsmToken::Integer:
  case AsmToken::Dot:
    if (!Parser.parseExpression(ExprVal))
      return DadaoOperand::createImm(ExprVal, Start, End);
    [[fallthrough]];
  default:
    return nullptr;
  }
}

static unsigned AluWithPrePost(unsigned AluCode, bool PreOp, bool PostOp) {
  if (PreOp)
    return LPAC::makePreOp(AluCode);
  if (PostOp)
    return LPAC::makePostOp(AluCode);
  return AluCode;
}

unsigned DadaoAsmParser::parseAluOperator(bool PreOp, bool PostOp) {
  StringRef IdString;
  Parser.parseIdentifier(IdString);
  unsigned AluCode = LPAC::stringToDadaoAluCode(IdString);
  if (AluCode == LPAC::UNKNOWN) {
    Error(Parser.getTok().getLoc(), "Can't parse ALU operator");
    return 0;
  }
  return AluCode;
}

static int SizeForSuffix(StringRef T) {
  return StringSwitch<int>(T).EndsWith(".h", 2).EndsWith(".b", 1).Default(4);
}

bool DadaoAsmParser::parsePrePost(StringRef Type, int *OffsetValue) {
  bool PreOrPost = false;
  if (Lexer.getKind() == Lexer.peekTok(true).getKind()) {
    PreOrPost = true;
    if (Lexer.is(AsmToken::Minus))
      *OffsetValue = -SizeForSuffix(Type);
    else if (Lexer.is(AsmToken::Plus))
      *OffsetValue = SizeForSuffix(Type);
    else
      return false;

    // Eat the '-' '-' or '+' '+'
    Parser.Lex();
    Parser.Lex();
  } else if (Lexer.is(AsmToken::Star)) {
    Parser.Lex(); // Eat the '*'
    PreOrPost = true;
  }

  return PreOrPost;
}

// Matches memory operand. Returns true if error encountered.
OperandMatchResultTy
DadaoAsmParser::parseMemoryOperand(OperandVector &Operands) {
  // Try to match a memory operand.
  // The memory operands are of the form:
  //  (1)  Register|Immediate|'' '[' '*'? Register '*'? ']' or
  //                            ^
  //  (2)  '[' '*'? Register '*'? AluOperator Register ']'
  //      ^
  //  (3)  '[' '--'|'++' Register '--'|'++' ']'
  //
  //  (4) '[' Immediate ']' (for SLS)

  // Store the type for use in parsing pre/post increment/decrement operators
  StringRef Type;
  if (Operands[0]->isToken())
    Type = static_cast<DadaoOperand *>(Operands[0].get())->getToken();

  // Use 0 if no offset given
  int OffsetValue = 0;
  unsigned BaseReg = 0;
  unsigned AluOp = LPAC::ADD;
  bool PostOp = false, PreOp = false;

  // Try to parse the offset
  std::unique_ptr<DadaoOperand> Op = parseRegister();
  if (!Op)
    Op = parseImmediate();

  // Only continue if next token is '['
  if (Lexer.isNot(AsmToken::LBrac)) {
    if (!Op)
      return MatchOperand_NoMatch;

    // The start of this custom parsing overlaps with register/immediate so
    // consider this as a successful match of an operand of that type as the
    // token stream can't be rewound to allow them to match separately.
    Operands.push_back(std::move(Op));
    return MatchOperand_Success;
  }

  Parser.Lex(); // Eat the '['.
  std::unique_ptr<DadaoOperand> Offset = nullptr;
  if (Op)
    Offset.swap(Op);

  // Determine if a pre operation
  PreOp = parsePrePost(Type, &OffsetValue);

  Op = parseRegister();
  if (!Op) {
    if (!Offset) {
      if ((Op = parseImmediate()) && Lexer.is(AsmToken::RBrac)) {
        Parser.Lex(); // Eat the ']'

        if (!Op->isLoImm16Signed()) {
          Error(Parser.getTok().getLoc(),
                "Memory address is not word "
                "aligned and larger than class RM can handle");
          return MatchOperand_ParseFail;
        }
        Operands.push_back(DadaoOperand::MorphToMemRegImm(
            Dadao::R0, std::move(Op), LPAC::ADD));

        return MatchOperand_Success;
      }
    }

    Error(Parser.getTok().getLoc(),
          "Unknown operand, expected register or immediate");
    return MatchOperand_ParseFail;
  }
  BaseReg = Op->getReg();

  // Determine if a post operation
  if (!PreOp)
    PostOp = parsePrePost(Type, &OffsetValue);

  // If ] match form (1) else match form (2)
  if (Lexer.is(AsmToken::RBrac)) {
    Parser.Lex(); // Eat the ']'.
    if (!Offset) {
      SMLoc Start = Parser.getTok().getLoc();
      SMLoc End =
          SMLoc::getFromPointer(Parser.getTok().getLoc().getPointer() - 1);
      const MCConstantExpr *OffsetConstExpr =
          MCConstantExpr::create(OffsetValue, getContext());
      Offset = DadaoOperand::createImm(OffsetConstExpr, Start, End);
    }
  } else {
    if (Offset || OffsetValue != 0) {
      Error(Parser.getTok().getLoc(), "Expected ']'");
      return MatchOperand_ParseFail;
    }

    // Parse operator
    AluOp = parseAluOperator(PreOp, PostOp);

    // Second form requires offset register
    Offset = parseRegister();
    if (!BaseReg || Lexer.isNot(AsmToken::RBrac)) {
      Error(Parser.getTok().getLoc(), "Expected ']'");
      return MatchOperand_ParseFail;
    }
    Parser.Lex(); // Eat the ']'.
  }

  // First form has addition as operator. Add pre- or post-op indicator as
  // needed.
  AluOp = AluWithPrePost(AluOp, PreOp, PostOp);

  // Ensure immediate offset is not too large
  if (Offset->isImm() && !Offset->isLoImm16Signed()) {
    Error(Parser.getTok().getLoc(),
          "Memory address is not word "
          "aligned and larger than class RM can handle");
    return MatchOperand_ParseFail;
  }

  Operands.push_back(
      Offset->isImm()
          ? DadaoOperand::MorphToMemRegImm(BaseReg, std::move(Offset), AluOp)
          : DadaoOperand::MorphToMemRegReg(BaseReg, std::move(Offset), AluOp));

  return MatchOperand_Success;
}

// Looks at a token type and creates the relevant operand from this
// information, adding to operands.
// If operand was parsed, returns false, else true.
OperandMatchResultTy
DadaoAsmParser::parseOperand(OperandVector *Operands, StringRef Mnemonic) {
  // Check if the current operand has a custom associated parser, if so, try to
  // custom parse the operand, or fallback to the general approach.
  OperandMatchResultTy Result = MatchOperandParserImpl(*Operands, Mnemonic);

  if (Result == MatchOperand_Success)
    return Result;
  if (Result == MatchOperand_ParseFail) {
    Parser.eatToEndOfStatement();
    return Result;
  }

  // Attempt to parse token as register
  std::unique_ptr<DadaoOperand> Op = parseRegister();

  // Attempt to parse token as immediate
  if (!Op)
    Op = parseImmediate();

  // If the token could not be parsed then fail
  if (!Op) {
    Error(Parser.getTok().getLoc(), "Unknown operand");
    Parser.eatToEndOfStatement();
    return MatchOperand_ParseFail;
  }

  // Push back parsed operand into list of operands
  Operands->push_back(std::move(Op));

  return MatchOperand_Success;
}

// Split the mnemonic into ASM operand, conditional code and instruction
// qualifier (half-word, byte).
StringRef DadaoAsmParser::splitMnemonic(StringRef Name, SMLoc NameLoc,
                                        OperandVector *Operands) {
  size_t Next = Name.find('.');

  StringRef Mnemonic = Name;

  bool IsBRR = false;
  if (Name.endswith(".r")) {
    Mnemonic = Name.substr(0, Name.size() - 2);
    IsBRR = true;
  }

  // Match b?? and s?? (BR, BRR, and SCC instruction classes).
  if (Mnemonic[0] == 'b' ||
      (Mnemonic[0] == 's' && !Mnemonic.startswith("sel") &&
       !Mnemonic.startswith("st"))) {
    // Parse instructions with a conditional code. For example, 'bne' is
    // converted into two operands 'b' and 'ne'.
    LPCC::CondCode CondCode =
        LPCC::suffixToDadaoCondCode(Mnemonic.substr(1, Next));
    if (CondCode != LPCC::UNKNOWN) {
      Mnemonic = Mnemonic.slice(0, 1);
      Operands->push_back(DadaoOperand::CreateToken(Mnemonic, NameLoc));
      Operands->push_back(DadaoOperand::createImm(
          MCConstantExpr::create(CondCode, getContext()), NameLoc, NameLoc));
      if (IsBRR) {
        Operands->push_back(DadaoOperand::CreateToken(".r", NameLoc));
      }
      return Mnemonic;
    }
  }

  // Parse other instructions with condition codes (RR instructions).
  // We ignore .f here and assume they are flag-setting operations, not
  // conditional codes (except for select instructions where flag-setting
  // variants are not yet implemented).
  if (Mnemonic.startswith("sel") ||
      (!Mnemonic.endswith(".f") && !Mnemonic.startswith("st"))) {
    LPCC::CondCode CondCode = LPCC::suffixToDadaoCondCode(Mnemonic);
    if (CondCode != LPCC::UNKNOWN) {
      size_t Next = Mnemonic.rfind('.', Name.size());
      // 'sel' doesn't use a predicate operand whose printer adds the period,
      // but instead has the period as part of the identifier (i.e., 'sel.' is
      // expected by the generated matcher). If the mnemonic starts with 'sel'
      // then include the period as part of the mnemonic, else don't include it
      // as part of the mnemonic.
      if (Mnemonic.startswith("sel")) {
        Mnemonic = Mnemonic.substr(0, Next + 1);
      } else {
        Mnemonic = Mnemonic.substr(0, Next);
      }
      Operands->push_back(DadaoOperand::CreateToken(Mnemonic, NameLoc));
      Operands->push_back(DadaoOperand::createImm(
          MCConstantExpr::create(CondCode, getContext()), NameLoc, NameLoc));
      return Mnemonic;
    }
  }

  Operands->push_back(DadaoOperand::CreateToken(Mnemonic, NameLoc));
  if (IsBRR) {
    Operands->push_back(DadaoOperand::CreateToken(".r", NameLoc));
  }

  return Mnemonic;
}

static bool IsMemoryAssignmentError(const OperandVector &Operands) {
  // Detects if a memory operation has an erroneous base register modification.
  // Memory operations are detected by matching the types of operands.
  //
  // TODO: This test is focussed on one specific instance (ld/st).
  // Extend it to handle more cases or be more robust.
  bool Modifies = false;

  int Offset = 0;

  if (Operands.size() < 5)
    return false;
  else if (Operands[0]->isToken() && Operands[1]->isReg() &&
           Operands[2]->isImm() && Operands[3]->isImm() && Operands[4]->isReg())
    Offset = 0;
  else if (Operands[0]->isToken() && Operands[1]->isToken() &&
           Operands[2]->isReg() && Operands[3]->isImm() &&
           Operands[4]->isImm() && Operands[5]->isReg())
    Offset = 1;
  else
    return false;

  int PossibleAluOpIdx = Offset + 3;
  int PossibleBaseIdx = Offset + 1;
  int PossibleDestIdx = Offset + 4;
  if (DadaoOperand *PossibleAluOp =
          static_cast<DadaoOperand *>(Operands[PossibleAluOpIdx].get()))
    if (PossibleAluOp->isImm())
      if (const MCConstantExpr *ConstExpr =
              dyn_cast<MCConstantExpr>(PossibleAluOp->getImm()))
        Modifies = LPAC::modifiesOp(ConstExpr->getValue());
  return Modifies && Operands[PossibleBaseIdx]->isReg() &&
         Operands[PossibleDestIdx]->isReg() &&
         Operands[PossibleBaseIdx]->getReg() ==
             Operands[PossibleDestIdx]->getReg();
}

static bool IsRegister(const MCParsedAsmOperand &op) {
  return static_cast<const DadaoOperand &>(op).isReg();
}

static bool MaybePredicatedInst(const OperandVector &Operands) {
  if (Operands.size() < 4 || !IsRegister(*Operands[1]) ||
      !IsRegister(*Operands[2]))
    return false;
  return StringSwitch<bool>(
             static_cast<const DadaoOperand &>(*Operands[0]).getToken())
      .StartsWith("add", true)
      .StartsWith("and", true)
      .StartsWith("sh", true)
      .StartsWith("sub", true)
      .StartsWith("or", true)
      .StartsWith("xor", true)
      .Default(false);
}

bool DadaoAsmParser::ParseInstruction(ParseInstructionInfo & /*Info*/,
                                      StringRef Name, SMLoc NameLoc,
                                      OperandVector &Operands) {
  // First operand is token for instruction
  StringRef Mnemonic = splitMnemonic(Name, NameLoc, &Operands);

  // If there are no more operands, then finish
  if (Lexer.is(AsmToken::EndOfStatement))
    return false;

  // Parse first operand
  if (parseOperand(&Operands, Mnemonic) != MatchOperand_Success)
    return true;

  // If it is a st instruction with one 1 operand then it is a "store true".
  // Transform <"st"> to <"s">, <LPCC:ICC_T>
  if (Lexer.is(AsmToken::EndOfStatement) && Name == "st" &&
      Operands.size() == 2) {
    Operands.erase(Operands.begin(), Operands.begin() + 1);
    Operands.insert(Operands.begin(), DadaoOperand::CreateToken("s", NameLoc));
    Operands.insert(Operands.begin() + 1,
                    DadaoOperand::createImm(
                        MCConstantExpr::create(LPCC::ICC_T, getContext()),
                        NameLoc, NameLoc));
  }

  // If the instruction is a bt instruction with 1 operand (in assembly) then it
  // is an unconditional branch instruction and the first two elements of
  // operands need to be merged.
  if (Lexer.is(AsmToken::EndOfStatement) && Name.startswith("bt") &&
      Operands.size() == 3) {
    Operands.erase(Operands.begin(), Operands.begin() + 2);
    Operands.insert(Operands.begin(), DadaoOperand::CreateToken("bt", NameLoc));
  }

  // Parse until end of statement, consuming commas between operands
  while (Lexer.isNot(AsmToken::EndOfStatement) && Lexer.is(AsmToken::Comma)) {
    // Consume comma token
    Lex();

    // Parse next operand
    if (parseOperand(&Operands, Mnemonic) != MatchOperand_Success)
      return true;
  }

  if (IsMemoryAssignmentError(Operands)) {
    Error(Parser.getTok().getLoc(),
          "the destination register can't equal the base register in an "
          "instruction that modifies the base register.");
    return true;
  }

  // Insert always true operand for instruction that may be predicated but
  // are not. Currently the autogenerated parser always expects a predicate.
  if (MaybePredicatedInst(Operands)) {
    Operands.insert(Operands.begin() + 1,
                    DadaoOperand::createImm(
                        MCConstantExpr::create(LPCC::ICC_T, getContext()),
                        NameLoc, NameLoc));
  }

  return false;
}

#define GET_REGISTER_MATCHER
#define GET_MATCHER_IMPLEMENTATION
#include "DadaoGenAsmMatcher.inc"

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDadaoAsmParser() {
  RegisterMCAsmParser<DadaoAsmParser> x(getTheDadaoTarget());
}
