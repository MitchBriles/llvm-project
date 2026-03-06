//===-- PDP11FrameLowering.cpp - PDP11 Frame Information ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the PDP11 implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "PDP11FrameLowering.h"

#include "PDP11AluCode.h"
#include "PDP11InstrInfo.h"
#include "PDP11Subtarget.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Function.h"

using namespace llvm;

void PDP11FrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {
  const PDP11InstrInfo &TII = *STI.getInstrInfo();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL;
  if (MBBI != MBB.end())
    DL = MBBI->getDebugLoc();

  // Save old frame pointer and establish r5 as the new frame pointer.
  BuildMI(MBB, MBBI, DL, TII.get(PDP11::PUSHr)).addReg(PDP11::R5);
  BuildMI(MBB, MBBI, DL, TII.get(PDP11::MOVrr), PDP11::R5).addReg(PDP11::R6);

  uint64_t StackSize = MF.getFrameInfo().getStackSize();
  if (StackSize != 0) {
    BuildMI(MBB, MBBI, DL, TII.get(PDP11::ADDri), PDP11::R6)
        .addReg(PDP11::R6)
        .addImm(-static_cast<int>(StackSize));
  }
}

MachineBasicBlock::iterator PDP11FrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction & /*MF*/, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  // Discard ADJCALLSTACKDOWN, ADJCALLSTACKUP instructions.
  return MBB.erase(I);
}

void PDP11FrameLowering::emitEpilogue(MachineFunction & /*MF*/,
                                      MachineBasicBlock &MBB) const {
  const PDP11InstrInfo &TII = *STI.getInstrInfo();
  MachineBasicBlock::iterator MBBI = MBB.getFirstTerminator();
  DebugLoc DL;
  if (MBBI != MBB.end())
    DL = MBBI->getDebugLoc();

  // Tear down frame: SP <- FP; restore old FP; pop the saved slot.
  BuildMI(MBB, MBBI, DL, TII.get(PDP11::MOVrr), PDP11::R6).addReg(PDP11::R5);
  BuildMI(MBB, MBBI, DL, TII.get(PDP11::POPr), PDP11::R5);
}

void PDP11FrameLowering::determineCalleeSaves(MachineFunction &MF,
                                              BitVector &SavedRegs,
                                              RegScavenger *RS) const {}

bool PDP11FrameLowering::hasFPImpl(const MachineFunction & /*MF*/) const {
  return true;
}
