//===- PDP11RegisterInfo.td - PDP11 Register defs ------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//  Declarations that describe the PDP11 register file
//===----------------------------------------------------------------------===//

// Registers are identified with 3-bit ID numbers.
class PDP11Reg<bits<3> Num, string Name, list<string> Alt = []>
    : Register<Name> {
  let Namespace = "PDP11";
  let HWEncoding{15-3} = 0;
  let HWEncoding{2-0} = Num;
  let AltNames = Alt;
}

// The baseline PDP-11 has exactly eight 16-bit integer registers.
def R0 : PDP11Reg<0, "r0">, DwarfRegNum<[0]>;
def R1 : PDP11Reg<1, "r1">, DwarfRegNum<[1]>;
def R2 : PDP11Reg<2, "r2">, DwarfRegNum<[2]>;
def R3 : PDP11Reg<3, "r3">, DwarfRegNum<[3]>;
def R4 : PDP11Reg<4, "r4">, DwarfRegNum<[4]>;
def R5 : PDP11Reg<5, "r5", ["fp"]>, DwarfRegNum<[5]>;
def R6 : PDP11Reg<6, "r6", ["sp"]>, DwarfRegNum<[6]>;
def R7 : PDP11Reg<7, "r7", ["pc"]>, DwarfRegNum<[7]>;

class PDP11RegisterClass<list<ValueType> Types, int S, int Alignment,
                         dag RegList, int CC = 1>
    : RegisterClass<"PDP11", Types, Alignment, RegList> {
  let Size = S;
  let CopyCost = CC;
}

def GPR : PDP11RegisterClass<[i16], 16, 16, (add (sequence "R%u", 0, 7))>;

def GPROperand : RegisterOperand<GPR>;
