//===-- PDP11RegisterInfo.cpp - PDP11 Register Information ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the PDP11 implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "PDP11RegisterInfo.h"
#include "PDP11AluCode.h"
#include "PDP11FrameLowering.h"
#include "PDP11InstrInfo.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_REGINFO_TARGET_DESC
#include "PDP11GenRegisterInfo.inc"

using namespace llvm;

PDP11RegisterInfo::PDP11RegisterInfo() : PDP11GenRegisterInfo(PDP11::R7) {}

const MCPhysReg *
PDP11RegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const {
  static const MCPhysReg CalleeSavedRegs[] = {PDP11::R5, PDP11::R4, 0};
  return CalleeSavedRegs;
}

BitVector PDP11RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(PDP11::R6); // SP
  Reserved.set(PDP11::R7); // PC
  return Reserved;
}

bool PDP11RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const {
  // true indicates that we changed the instruction.
  return true;
}

Register
PDP11RegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const {
  return PDP11::R6;
}
