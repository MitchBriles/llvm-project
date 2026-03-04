//===-- PDP11TargetInfo.cpp - PDP11 Target Implementation -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/PDP11TargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"

using namespace llvm;

Target &llvm::getThePDP11Target() {
  static Target ThePDP11Target;
  return ThePDP11Target;
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializePDP11TargetInfo() {
  RegisterTarget<Triple::pdp11> X(getThePDP11Target(), "pdp11", "PDP11",
                                  "PDP11");
}

// Step-1 bootstrap: wire no-op parser/disassembler init so
// InitializeAllAsmParsers/InitializeAllDisassemblers can link even before
// those components are implemented.
extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializePDP11AsmParser() {}
extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializePDP11Disassembler() {}
