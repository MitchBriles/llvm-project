//===-- PDP11InstrInfo.td - Target Description for PDP11 Target -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the PDP11 instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "PDP11InstrFormats.td"

// Keep the initial instruction set intentionally minimal.
// Full pattern-based ISel comes later; this stage only bootstraps the target.
let isCodeGenOnly = 1, hasSideEffects = 1, Uses = [R6], Defs = [R6] in {
  def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i16imm:$amt1, i16imm:$amt2), []>;
  def ADJCALLSTACKUP   : Pseudo<(outs), (ins i16imm:$amt1, i16imm:$amt2), []>;
}

def PDP11RetNode : SDNode<"PDP11ISD::RET", SDTBrind,
                          [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

let isCodeGenOnly = 1, isReturn = 1, isTerminator = 1, isBarrier = 1 in {
  def RET : Pseudo<(outs), (ins GPROperand:$rs),
                   [(PDP11RetNode GPROperand:$rs)]>;
}
