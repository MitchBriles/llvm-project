//===-- PDP11TargetObjectFile.h - PDP11 Object Info -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_PDP11_PDP11TARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_PDP11_PDP11TARGETOBJECTFILE_H

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {
class PDP11TargetObjectFile : public TargetLoweringObjectFileELF {};
} // end namespace llvm

#endif // LLVM_LIB_TARGET_PDP11_PDP11TARGETOBJECTFILE_H
