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
// All integer args are passed by value on stack in 16-bit slots.
def CC_PDP11 : CallingConv<[
  CCIfType<[i8], CCPromoteToType<i16>>,
  CCIfType<[i16], CCAssignToStack<2, 2>>
]>;

// PDP11 C return-value convention.
def RetCC_PDP11 : CallingConv<[
  CCIfType<[i8], CCAssignToReg<[R0]>>,
  CCIfType<[i16], CCAssignToReg<[R0]>>,
  CCIfType<[i32], CCAssignToReg<[R0, R1]>>
]>;
