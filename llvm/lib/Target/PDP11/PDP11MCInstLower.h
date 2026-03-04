//===-- PDP11MCInstLower.h - Lower MachineInstr to MCInst -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PDP11_PDP11MCINSTLOWER_H
#define LLVM_LIB_TARGET_PDP11_PDP11MCINSTLOWER_H

#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class AsmPrinter;
class MCContext;
class MCInst;
class MCOperand;
class MCSymbol;
class MachineInstr;
class MachineOperand;

// PDP11MCInstLower - This class is used to lower an MachineInstr
// into an MCInst.
class LLVM_LIBRARY_VISIBILITY PDP11MCInstLower {
  typedef MachineOperand::MachineOperandType MachineOperandType;
  MCContext &Ctx;

  AsmPrinter &Printer;

public:
  PDP11MCInstLower(MCContext &CTX, AsmPrinter &AP) : Ctx(CTX), Printer(AP) {}
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;

  MCOperand LowerOperand(const MachineOperand &MO, unsigned offset = 0) const;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_PDP11_PDP11MCINSTLOWER_H
