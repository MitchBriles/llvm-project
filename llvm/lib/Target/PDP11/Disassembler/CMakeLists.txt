add_llvm_component_library(LLVMPDP11Disassembler
  PDP11Disassembler.cpp

  LINK_COMPONENTS
  CodeGenTypes
  PDP11Desc
  PDP11Info
  MC
  MCDisassembler
  Support

  ADD_TO_COMPONENT
  PDP11
  )
