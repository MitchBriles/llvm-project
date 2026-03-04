//===-- PDP11MCTargetDesc.cpp - PDP11 Target Descriptions -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides PDP11 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "PDP11MCTargetDesc.h"
#include "PDP11InstPrinter.h"
#include "PDP11MCAsmInfo.h"
#include "TargetInfo/PDP11TargetInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/TargetParser/Triple.h"
#include <cstdint>
#include <string>

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "PDP11GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "PDP11GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "PDP11GenRegisterInfo.inc"

using namespace llvm;

static MCInstPrinter *createPDP11MCInstPrinter(const Triple & /*T*/,
                                               unsigned SyntaxVariant,
                                               const MCAsmInfo &MAI,
                                               const MCInstrInfo &MII,
                                               const MCRegisterInfo &MRI) {
  if (SyntaxVariant != 0)
    return nullptr;
  return new PDP11InstPrinter(MAI, MII, MRI);
}

static MCSubtargetInfo *
createPDP11MCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  std::string CPUName = std::string(CPU);
  if (CPUName.empty())
    CPUName = "generic";

  return createPDP11MCSubtargetInfoImpl(TT, CPUName, /*TuneCPU*/ CPUName, FS);
}

static MCInstrInfo *createPDP11MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitPDP11MCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createPDP11MCRegisterInfo(const Triple & /*TT*/) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitPDP11MCRegisterInfo(X, PDP11::R7);
  return X;
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void
LLVMInitializePDP11TargetMC() {
  Target &Targ = getThePDP11Target();

  // Register the MC asm info.
  RegisterMCAsmInfo<PDP11MCAsmInfo> X(Targ);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(Targ, createPDP11MCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(Targ, createPDP11MCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(Targ, createPDP11MCSubtargetInfo);

  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(Targ, createPDP11MCInstPrinter);

  // Register the asm backend.
  TargetRegistry::RegisterMCAsmBackend(Targ, createPDP11AsmBackend);
}
