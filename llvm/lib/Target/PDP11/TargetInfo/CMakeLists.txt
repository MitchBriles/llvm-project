add_llvm_component_library(LLVMPDP11Info
  PDP11TargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  PDP11
  )
