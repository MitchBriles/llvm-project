add_llvm_component_library(LLVMPDP11Desc
  PDP11AsmBackend.cpp
  PDP11ELFObjectWriter.cpp
  PDP11InstPrinter.cpp
  PDP11MCAsmInfo.cpp
  # PDP11MCCodeEmitter.cpp
  PDP11MCTargetDesc.cpp
  PARTIAL_SOURCES_INTENDED

  LINK_COMPONENTS
  PDP11Info
  MC
  MCDisassembler
  Support
  TargetParser

  ADD_TO_COMPONENT
  PDP11
)
