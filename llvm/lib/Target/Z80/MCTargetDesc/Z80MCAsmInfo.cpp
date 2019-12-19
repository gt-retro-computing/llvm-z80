//
// Created by codetector on 12/14/19.
//

#include "Z80MCAsmInfo.h"

llvm::Z80MCAsmInfo::Z80MCAsmInfo(const llvm::Triple &TT) {

  // MARK: Assembly Format

  Data8bitsDirective = "\tDB\t";
  CommentString = ";";
//	AscizDirective = 0;
//	HiddenVisibilityAttr = MCSA_Invalid;
//	HiddenDeclarationVisibilityAttr = MCSA_Invalid;
//	ProtectedVisibilityAttr = MCSA_Invalid;

  PrivateGlobalPrefix = "_lpg";
  PrivateLabelPrefix = "_lp";
  LinkerPrivateGlobalPrefix = "_llp";
  IsLittleEndian = true;

  UsesNonexecutableStackSection = false;
  HasDotTypeDotSizeDirective = false;
  HasSingleParameterDotFile = false;
  NeedsLocalForSize = false;
  HasIdentDirective = false;
}
