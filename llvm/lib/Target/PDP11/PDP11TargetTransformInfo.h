//===-- PDP11TargetTransformInfo.h - PDP11 specific TTI ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file a TargetTransformInfo::Concept conforming object specific to the
// PDP11 target machine. It uses the target's detailed information to
// provide more precise answers to certain TTI queries, while letting the
// target independent and default TTI implementations handle the rest.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PDP11_PDP11TARGETTRANSFORMINFO_H
#define LLVM_LIB_TARGET_PDP11_PDP11TARGETTRANSFORMINFO_H

#include "PDP11.h"
#include "PDP11Subtarget.h"
#include "PDP11TargetMachine.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/BasicTTIImpl.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/Support/MathExtras.h"

namespace llvm {
class PDP11TTIImpl : public BasicTTIImplBase<PDP11TTIImpl> {
  typedef BasicTTIImplBase<PDP11TTIImpl> BaseT;
  typedef TargetTransformInfo TTI;
  friend BaseT;

  const PDP11Subtarget *ST;
  const PDP11TargetLowering *TLI;

  const PDP11Subtarget *getST() const { return ST; }
  const PDP11TargetLowering *getTLI() const { return TLI; }

public:
  explicit PDP11TTIImpl(const PDP11TargetMachine *TM, const Function &F)
      : BaseT(TM, F.getParent()->getDataLayout()), ST(TM->getSubtargetImpl(F)),
        TLI(ST->getTargetLowering()) {}
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_PDP11_PDP11TARGETTRANSFORMINFO_H
