//= DadaoInstPrinter.h - Convert Dadao MCInst to asm syntax -------*- C++ -*--//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints a Dadao MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOINSTPRINTER_H
#define LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOINSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"

namespace llvm {
class StringRef;

class DadaoInstPrinter : public MCInstPrinter {
public:
  DadaoInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                   const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                 const MCSubtargetInfo &STI, raw_ostream &O) override;
  void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O,
                    const char *Modifier = nullptr);
  void printPredicateOperand(const MCInst *MI, unsigned OpNum, raw_ostream &O);
  void printMemRRIIOperand(const MCInst *MI, int OpNo, raw_ostream &O,
                         const char *Modifier = nullptr);
  void printMemRRRIOperand(const MCInst *MI, int OpNo, raw_ostream &O,
                         const char *Modifier = nullptr);
  void printCCOperand(const MCInst *MI, int OpNo, raw_ostream &O);
  void printHi16ImmOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
  void printHi16AndImmOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
  void printLo16AndImmOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
  void printLo12AndImmOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);

  // Autogenerated by tblgen.
  std::pair<const char *, uint64_t> getMnemonic(const MCInst *MI) override;
  void printInstruction(const MCInst *MI, uint64_t Address, raw_ostream &O);
  bool printAliasInstr(const MCInst *MI, uint64_t Address, raw_ostream &OS);
  void printCustomAliasOperand(const MCInst *MI, uint64_t Address,
                               unsigned OpIdx, unsigned PrintMethodIdx,
                               raw_ostream &O);
  static const char *getRegisterName(MCRegister Reg);
  void printRegName(raw_ostream &OS, MCRegister Reg) const override;

private:
  bool printInst(const MCInst *MI, raw_ostream &Ostream, StringRef Alias,
                 unsigned OpNo0, unsigned OpnNo1);
  bool printMemoryLoadIncrement(const MCInst *MI, raw_ostream &Ostream,
                                StringRef Opcode, int AddOffset);
  bool printMemoryStoreIncrement(const MCInst *MI, raw_ostream &Ostream,
                                 StringRef Opcode, int AddOffset);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOINSTPRINTER_H
