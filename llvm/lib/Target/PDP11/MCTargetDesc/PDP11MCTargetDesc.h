//===-- PDP11MCTargetDesc.h - PDP11 Target Descriptions ---------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_PDP11_MCTARGETDESC_PDP11MCTARGETDESC_H
#define LLVM_LIB_TARGET_PDP11_MCTARGETDESC_PDP11MCTARGETDESC_H

#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCSubtargetInfo;
class Target;

extern Target ThePDP11Target;

MCAsmBackend *createPDP11AsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter> createPDP11ELFObjectWriter(uint8_t OSABI);
} // namespace llvm

// Defines symbolic names for PDP11 registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "PDP11GenRegisterInfo.inc"

// Defines symbolic names for the PDP11 instructions.
#define GET_INSTRINFO_ENUM
#include "PDP11GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "PDP11GenSubtargetInfo.inc"

#endif // LLVM_LIB_TARGET_PDP11_MCTARGETDESC_PDP11MCTARGETDESC_H
