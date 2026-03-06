//===-- PDP11InstPrinter.cpp - Convert PDP11 MCInst to asm syntax ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints an PDP11 MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "PDP11InstPrinter.h"
#include "MCTargetDesc/PDP11MCTargetDesc.h"
#include "PDP11AluCode.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#define PRINT_ALIAS_INSTR
#include "PDP11GenAsmWriter.inc"

static const char *getUnixRegName(MCRegister Reg) {
  switch (Reg.id()) {
  case PDP11::R6:
    return "sp";
  case PDP11::R7:
    return "pc";
  default:
    return PDP11InstPrinter::getRegisterName(Reg);
  }
}

void PDP11InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                    raw_ostream &OS, const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg())
    OS << getUnixRegName(Op.getReg());
  else if (Op.isImm())
    OS << Op.getImm();
  else {
    assert(Op.isExpr() && "Expected an expression");
    Op.print(OS);
  }
}

void PDP11InstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                 StringRef Annotation,
                                 const MCSubtargetInfo & /*STI*/,
                                 raw_ostream &OS) {

  printInstruction(MI, Address, OS);
}
