//===- PDP11.td - Describe the PDP11 Target Machine --------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "PDP11Schedule.td"
include "PDP11RegisterInfo.td"
include "PDP11CallingConv.td"
include "PDP11InstrInfo.td"

defm : RemapAllTargetPseudoPointerOperands<GPR>;

def PDP11InstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// PDP11 processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic", PDP11SchedModel, []>;

def PDP11InstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def PDP11 : Target {
  // Pull in Instruction Info:
  let InstructionSet = PDP11InstrInfo;
  let AssemblyWriters = [PDP11InstPrinter];
}
