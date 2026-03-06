//===-- PDP11ISelLowering.h - PDP11 DAG Lowering Interface -....-*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that PDP11 uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PDP11_PDP11ISELLOWERING_H
#define LLVM_LIB_TARGET_PDP11_PDP11ISELLOWERING_H

#include "PDP11.h"
#include "PDP11RegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {
namespace PDP11ISD {
enum {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  // Return from subroutine
  RTS,

  /// Wrapper - A wrapper node for TargetConstantPool, TargetExternalSymbol,
  /// and TargetGlobalAddress.
  Wrapper,

  // CALL - These operations represent an abstract call instruction, which
  // includes a bunch of information.
  // CALL
};
} // namespace PDP11ISD

class PDP11Subtarget;

class PDP11TargetLowering : public TargetLowering {
public:
  PDP11TargetLowering(const TargetMachine &TM, const PDP11Subtarget &STI);

  const char *getTargetNodeName(unsigned Opcode) const override;
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;
  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;
  const PDP11RegisterInfo *TRI;
};
} // namespace llvm

#endif // LLVM_LIB_TARGET_PDP11_PDP11ISELLOWERING_H
