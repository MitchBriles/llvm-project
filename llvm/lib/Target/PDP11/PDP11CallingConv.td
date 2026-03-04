//===- PDP11CallingConv.td - Calling Conventions PDP11 -------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the PDP11 architectures.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Return Value Calling Conventions
//===----------------------------------------------------------------------===//

// PDP11 C Calling convention.
def CC_PDP11 : CallingConv<[]>;
// PDP11 C return-value convention.
def RetCC_PDP11 : CallingConv<[]>;
