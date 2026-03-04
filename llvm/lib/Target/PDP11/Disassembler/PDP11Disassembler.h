//===- PDP11Disassembler.cpp - Disassembler for PDP11 -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is part of the PDP11 Disassembler.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PDP11_DISASSEMBLER_PDP11DISASSEMBLER_H
#define LLVM_LIB_TARGET_PDP11_DISASSEMBLER_PDP11DISASSEMBLER_H

#include "llvm/MC/MCDisassembler/MCDisassembler.h"

#define DEBUG_TYPE "pdp11-disassembler"

namespace llvm {

class PDP11Disassembler : public MCDisassembler {
public:
  PDP11Disassembler(const MCSubtargetInfo &STI, MCContext &Ctx);

  ~PDP11Disassembler() override = default;

  // getInstruction - See MCDisassembler.
  MCDisassembler::DecodeStatus
  getInstruction(MCInst &Instr, uint64_t &Size, ArrayRef<uint8_t> Bytes,
                 uint64_t Address, raw_ostream &CStream) const override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_PDP11_DISASSEMBLER_PDP11DISASSEMBLER_H
