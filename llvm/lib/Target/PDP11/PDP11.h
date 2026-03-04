//===-- PDP11.h - Top-level interface for PDP11 representation --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// PDP11 back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PDP11_PDP11_H
#define LLVM_LIB_TARGET_PDP11_PDP11_H

#include "llvm/Pass.h"

namespace llvm {
class FunctionPass;
class PDP11TargetMachine;
class PassRegistry;

// createPDP11ISelDag - This pass converts a legalized DAG into a
// PDP11-specific DAG, ready for instruction scheduling.
FunctionPass *createPDP11ISelDag(PDP11TargetMachine &TM);

// createPDP11DelaySlotFillerPass - This pass fills delay slots
// with useful instructions or nop's
FunctionPass *createPDP11DelaySlotFillerPass(const PDP11TargetMachine &TM);

// createPDP11MemAluCombinerPass - This pass combines loads/stores and
// arithmetic operations.
FunctionPass *createPDP11MemAluCombinerPass();

// createPDP11SetflagAluCombinerPass - This pass combines SET_FLAG and ALU
// operations.
FunctionPass *createPDP11SetflagAluCombinerPass();

void initializePDP11DAGToDAGISelLegacyPass(PassRegistry &);

} // namespace llvm

#endif // LLVM_LIB_TARGET_PDP11_PDP11_H
