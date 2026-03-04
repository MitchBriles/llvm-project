add_llvm_component_group(PDP11)

set(LLVM_TARGET_DEFINITIONS PDP11.td)

tablegen(LLVM PDP11GenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM PDP11GenCallingConv.inc -gen-callingconv)
tablegen(LLVM PDP11GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM PDP11GenInstrInfo.inc -gen-instr-info)
# tablegen(LLVM PDP11GenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM PDP11GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM PDP11GenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(PDP11CommonTableGen)

add_llvm_target(PDP11CodeGen
  PDP11AsmPrinter.cpp
  PDP11FrameLowering.cpp
  PDP11InstrInfo.cpp
  PDP11ISelDAGToDAG.cpp
  PDP11ISelLowering.cpp
  PDP11MachineFunctionInfo.cpp
  PDP11MCInstLower.cpp
  PDP11RegisterInfo.cpp
  PDP11SelectionDAGInfo.cpp
  PDP11Subtarget.cpp
  PDP11TargetMachine.cpp
  PDP11TargetObjectFile.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  PDP11Desc
  PDP11Info
  MC
  SelectionDAG
  Support
  Target
  TargetParser
  TransformUtils

  ADD_TO_COMPONENT
  PDP11
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
