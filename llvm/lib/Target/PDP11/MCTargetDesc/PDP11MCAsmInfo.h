//=====-- PDP11MCAsmInfo.h - PDP11 asm properties -----------*- C++ -*--====//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the PDP11MCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PDP11_MCTARGETDESC_PDP11MCASMINFO_H
#define LLVM_LIB_TARGET_PDP11_MCTARGETDESC_PDP11MCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class PDP11MCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit PDP11MCAsmInfo(const Triple &TheTriple,
                          const MCTargetOptions &Options);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_PDP11_MCTARGETDESC_PDP11MCASMINFO_H
