//===- PDP11InstrInfo.h - PDP11 Instruction Information ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the PDP11 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PDP11_PDP11INSTRINFO_H
#define LLVM_LIB_TARGET_PDP11_PDP11INSTRINFO_H

#include "MCTargetDesc/PDP11MCTargetDesc.h"
#include "PDP11RegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "PDP11GenInstrInfo.inc"

namespace llvm {
class PDP11Subtarget;

class PDP11InstrInfo : public PDP11GenInstrInfo {
  const PDP11RegisterInfo RI;

public:
  explicit PDP11InstrInfo(const PDP11Subtarget &STI);

  // getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  // such, whenever a client has an instance of instruction info, it should
  // always be able to get register info as well (through this method).
  const PDP11RegisterInfo &getRegisterInfo() const { return RI; }
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_PDP11_PDP11INSTRINFO_H
