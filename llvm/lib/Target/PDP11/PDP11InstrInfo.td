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

def PDP11RTSNode : SDNode<"PDP11ISD::RTS", SDTBrind,
                          [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

let isReturn = 1, isTerminator = 1, isBarrier = 1 in {
  def RET : PDP11Inst<(outs), (ins GPROperand:$rs), "rts\t$rs",
                      [(PDP11RTSNode GPROperand:$rs)]>;
}

let isAsCheapAsAMove = 1 in {
  def MOVrr : PDP11Inst<(outs GPR:$dst), (ins GPR:$src),
                        "mov\t$src, $dst">;
}

def MOVri : PDP11Inst<(outs GPR:$dst), (ins i16imm:$imm),
                      "mov\t$$$imm, $dst",
                      [(set GPR:$dst, imm:$imm)]>;

let Uses = [R6], Defs = [R6], mayStore = 1 in {
  def PUSHr : PDP11Inst<(outs), (ins GPR:$src), "mov\t$src, -(sp)">;
}

let Uses = [R6], Defs = [R6], mayLoad = 1 in {
  def POPr : PDP11Inst<(outs GPR:$dst), (ins), "mov\t(sp)+, $dst">;
}

let mayLoad = 1, canFoldAsLoad = 1 in {
  def MOVrm : PDP11Inst<(outs GPR:$dst), (ins i16imm:$off, GPR:$base),
                        "mov\t$off($base), $dst",
                        [(set GPR:$dst, (load (add GPR:$base, imm:$off)))]>;
}

let mayStore = 1 in {
  def MOVmr : PDP11Inst<(outs), (ins i16imm:$off, GPR:$base, GPR:$src),
                        "mov\t$src, $off($base)",
                        [(store GPR:$src, (add GPR:$base, imm:$off))]>;
}

let Constraints = "$src2 = $dst", isCommutable = 1 in {
  def ADDrr : PDP11Inst<(outs GPR:$dst), (ins GPR:$src2, GPR:$src1),
                        "add\t$src1, $dst",
                        [(set GPR:$dst, (add GPR:$src2, GPR:$src1))]>;
}

let Constraints = "$src = $dst" in {
  def ADDri : PDP11Inst<(outs GPR:$dst), (ins GPR:$src, i16imm:$imm),
                        "add\t$$$imm, $dst",
                        [(set GPR:$dst, (add GPR:$src, imm:$imm))]>;
}
