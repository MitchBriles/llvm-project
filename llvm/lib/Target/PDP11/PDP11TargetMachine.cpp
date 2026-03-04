//===-- PDP11TargetMachine.cpp - Define TargetMachine for PDP11 ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implements the info about PDP11 target spec.
//
//===----------------------------------------------------------------------===//

#include "PDP11TargetMachine.h"

#include "PDP11.h"
#include "PDP11MachineFunctionInfo.h"
#include "PDP11TargetTransformInfo.h"
#include "TargetInfo/PDP11TargetInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"
#include <optional>

using namespace llvm;

static StringRef getCPU(StringRef CPU) {
  if (CPU.empty())
    return "generic";
  return CPU;
}

static std::string computeDataLayout(const Triple &TT) {
  // PDP-11 is a 16-bit little-endian architecture.
  return "e-m:e-p:16:16-i8:8:8-i16:16:16-n8:16-S16";
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::Static);
}

extern "C" LLVM_ABI LLVM_EXTERNAL_VISIBILITY void LLVMInitializePDP11Target() {
  RegisterTargetMachine<PDP11TargetMachine> X(getThePDP11Target());

  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializePDP11DAGToDAGISelLegacyPass(PR);
}

PDP11TargetMachine::PDP11TargetMachine(const Target &T, const Triple &TT,
                                       StringRef CPU, StringRef FS,
                                       const TargetOptions &Options,
                                       std::optional<Reloc::Model> RM,
                                       std::optional<CodeModel::Model> CM,
                                       CodeGenOptLevel OL, bool JIT)
    : CodeGenTargetMachineImpl(T, computeDataLayout(TT), TT, getCPU(CPU), FS,
                               Options, getEffectiveRelocModel(RM),
                               getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(getCPU(CPU)), std::string(FS), *this, Options,
                getCodeModel(), OL) {
  initAsmInfo();
}

TargetTransformInfo
PDP11TargetMachine::getTargetTransformInfo(const Function &F) const {
  return TargetTransformInfo(std::make_unique<PDP11TTIImpl>(this, F));
}

MachineFunctionInfo *PDP11TargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return PDP11MachineFunctionInfo::create<PDP11MachineFunctionInfo>(Allocator,
                                                                    F, STI);
}

namespace {
class PDP11PassConfig : public TargetPassConfig {
public:
  PDP11PassConfig(PDP11TargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  PDP11TargetMachine &getPDP11TargetMachine() const {
    return getTM<PDP11TargetMachine>();
  }

  bool addInstSelector() override;
};
} // namespace

TargetPassConfig *PDP11TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new PDP11PassConfig(*this, PM);
}

bool PDP11PassConfig::addInstSelector() {
  addPass(createPDP11ISelDag(getPDP11TargetMachine()));
  return false;
}
