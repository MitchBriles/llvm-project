include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_component_library(LLVMPDP11AsmParser
  PDP11AsmParser.cpp

  LINK_COMPONENTS
  CodeGenTypes
  PDP11Desc
  PDP11Info
  MC
  MCParser
  Support

  ADD_TO_COMPONENT
  PDP11
  )

add_dependencies( LLVMPDP11AsmParser PDP11CommonTableGen )
