//===-- PDP11SelectionDAGInfo.cpp - PDP11 SelectionDAG Info -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the PDP11SelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#include "PDP11SelectionDAGInfo.h"

#include "PDP11TargetMachine.h"

#define DEBUG_TYPE "pdp11-selectiondag-info"

namespace llvm {

SDValue PDP11SelectionDAGInfo::EmitTargetCodeForMemcpy(
    SelectionDAG & /*DAG*/, const SDLoc & /*dl*/, SDValue /*Chain*/,
    SDValue /*Dst*/, SDValue /*Src*/, SDValue Size, Align /*Alignment*/,
    bool /*isVolatile*/, bool /*AlwaysInline*/,
    MachinePointerInfo /*DstPtrInfo*/,
    MachinePointerInfo /*SrcPtrInfo*/) const {
  return SDValue();
}

} // namespace llvm
