//===-- PDP11ELFObjectWriter.cpp - PDP11 ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {

class PDP11ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  explicit PDP11ELFObjectWriter(uint8_t OSABI);

  ~PDP11ELFObjectWriter() override = default;

protected:
  unsigned getRelocType(const MCFixup &Fixup, const MCValue &Target,
                        bool IsPCRel) const override;
  bool needsRelocateWithSymbol(const MCValue &Val,
                               unsigned Type) const override;
};

} // end anonymous namespace

PDP11ELFObjectWriter::PDP11ELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit_=*/false, OSABI, ELF::EM_PDP11,
                              /*HasRelocationAddend_=*/true) {}

unsigned PDP11ELFObjectWriter::getRelocType(const MCFixup &Fixup,
                                            const MCValue &Target,
                                            bool IsPCRel) const {
  return 0;
}

bool PDP11ELFObjectWriter::needsRelocateWithSymbol(const MCValue &Val,
                                                   unsigned Type) const {
  return false;
}

namespace llvm {
std::unique_ptr<MCObjectTargetWriter>
createPDP11ELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<PDP11ELFObjectWriter>(OSABI);
}
} // namespace llvm
