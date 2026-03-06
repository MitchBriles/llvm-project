//===-- PDP11InstrInfo.cpp - PDP11 Instruction Information ------*- C++ -*-===//
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

#include "PDP11InstrInfo.h"
#include "PDP11AluCode.h"
#include "PDP11Subtarget.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "PDP11GenInstrInfo.inc"

PDP11InstrInfo::PDP11InstrInfo(const PDP11Subtarget &STI)
    : PDP11GenInstrInfo(STI, RI, PDP11::ADJCALLSTACKDOWN, PDP11::ADJCALLSTACKUP,
                        /*CatchRetOpcode=*/0,
                        /*ReturnOpcode=*/PDP11::RET),
      RI() {}

void PDP11InstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MBBI,
                                 const DebugLoc &DL, Register DestReg,
                                 Register SrcReg, bool KillSrc,
                                 bool RenamableDest,
                                 bool RenamableSrc) const {
  MachineInstrBuilder MIB =
      BuildMI(MBB, MBBI, DL, get(PDP11::MOVrr), DestReg)
          .addReg(SrcReg, getKillRegState(KillSrc));
  MIB->getOperand(0).setIsRenamable(RenamableDest);
  MIB->getOperand(1).setIsRenamable(RenamableSrc);
}
