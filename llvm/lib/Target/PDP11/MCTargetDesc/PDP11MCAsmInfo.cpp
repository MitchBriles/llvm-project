//===-- PDP11MCAsmInfo.cpp - PDP11 asm properties -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the PDP11MCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "PDP11MCAsmInfo.h"

#include "llvm/TargetParser/Triple.h"

using namespace llvm;

void PDP11MCAsmInfo::anchor() {}

PDP11MCAsmInfo::PDP11MCAsmInfo(const Triple & /*TheTriple*/,
                               const MCTargetOptions &Options) {}
