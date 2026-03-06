//===-- PDP11ISelLowering.cpp - PDP11 DAG Lowering Implementation ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the PDP11TargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "PDP11ISelLowering.h"
#include "PDP11.h"
#include "PDP11MachineFunctionInfo.h"
#include "PDP11Subtarget.h"
#include "PDP11TargetObjectFile.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/TargetCallingConv.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/CodeGenTypes/MachineValueType.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/RuntimeLibcalls.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/KnownBits.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <utility>

#define DEBUG_TYPE "pdp11-lower"

using namespace llvm;

SDValue PDP11TargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  if (IsVarArg)
    report_fatal_error("PDP11 LowerFormalArguments: varargs are not supported");

  for (unsigned I = 0; I < Ins.size(); ++I) {
    if (Ins[I].VT != MVT::i16)
      report_fatal_error(
          "PDP11 LowerFormalArguments: only i16 arguments are supported");

    // Unix/PDP-11 ABI stack layout after prologue:
    // 0(r5) saved old r5, 2(r5) return address, then args at 4(r5), 6(r5), ...
    int64_t ArgOffset = 4 + static_cast<int64_t>(I) * 2;
    SDValue FP = DAG.getRegister(PDP11::R5, MVT::i16);
    SDValue Addr = DAG.getNode(ISD::ADD, DL, MVT::i16, FP,
                               DAG.getConstant(ArgOffset, DL, MVT::i16));
    SDValue ArgValue = DAG.getLoad(MVT::i16, DL, Chain, Addr, MachinePointerInfo());
    InVals.push_back(ArgValue);
    Chain = ArgValue.getValue(1);
  }

  return Chain;
}

SDValue
PDP11TargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                 bool IsVarArg,
                                 const SmallVectorImpl<ISD::OutputArg> &Outs,
                                 const SmallVectorImpl<SDValue> &OutVals,
                                 const SDLoc &DL, SelectionDAG &DAG) const {
  if (Outs.size() > 1)
    report_fatal_error(
        "PDP11 LowerReturn: only a single return value is supported");

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps(1, Chain);
  SDValue PC = DAG.getRegister(PDP11::R7, MVT::i16);
  RetOps.push_back(PC);

  if (!Outs.empty()) {
    if (Outs[0].VT != MVT::i16)
      report_fatal_error("PDP11 LowerReturn: only i16 returns are supported");

    Chain = DAG.getCopyToReg(Chain, DL, PDP11::R0, OutVals[0], Glue);
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(PDP11::R0, MVT::i16));
  }

  RetOps[0] = Chain;
  if (Glue)
    RetOps.push_back(Glue);

  return DAG.getNode(PDP11ISD::RTS, DL, MVT::Other, RetOps);
}

const char *PDP11TargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case PDP11ISD::RTS:
    return "PDP11::RTS";
  default:
    return nullptr;
  }
}

PDP11TargetLowering::PDP11TargetLowering(const TargetMachine &TM,
                                         const PDP11Subtarget &STI)
    : TargetLowering(TM, STI) {
  // Set up the register classes.
  addRegisterClass(MVT::i16, &PDP11::GPRRegClass);

  // Compute derived properties from the register classes
  TRI = STI.getRegisterInfo();
  computeRegisterProperties(TRI);

  setSchedulingPreference(Sched::Source);

  // Booleans always contain 0 or 1.
  setBooleanContents(ZeroOrOneBooleanContent);
}
