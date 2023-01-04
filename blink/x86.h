#ifndef BLINK_X86_H_
#define BLINK_X86_H_
#include <stddef.h>

#include "blink/types.h"

#define XED_MODE_REAL   0
#define XED_MODE_LEGACY 1
#define XED_MODE_LONG   2

#define XED_ILD_MAP0    0  // 8086+  ...
#define XED_ILD_MAP1    1  // 286+   0x0F,...
#define XED_ILD_MAP2    2  // Core2+ 0x0F,0x38,...
#define XED_ILD_MAP3    3  // Core2+ 0x0F,0x3A,...
#define XED_ILD_MAP4    4
#define XED_ILD_MAP5    5
#define XED_ILD_MAP6    6
#define XED_ILD_BAD_MAP 7

#define XED_MAX_INSTRUCTION_BYTES 15

#define XED_ERROR_NONE              0
#define XED_ERROR_BUFFER_TOO_SHORT  1
#define XED_ERROR_GENERAL_ERROR     2
#define XED_ERROR_BAD_LEGACY_PREFIX 7
#define XED_ERROR_BAD_REX_PREFIX    8
#define XED_ERROR_BAD_MAP           10
#define XED_ERROR_INSTR_TOO_LONG    18
#define XED_ERROR_INVALID_MODE      19
#define XED_ERROR_LAST              22

struct XedOperands { /*
    ┌rexx
    │┌vexarg
    ││  ┌trips
    ││  │┌log₂𝑏 (-/16/32/64)
    ││  ││ ┌length
    ││  ││ │   ┌rep
    ││  ││ │   │ ┌map
    ││  ││ │   │ │  ┌opcode
    ││  ││ │   │ │  │       ┌sib scale
    ││  ││ │   │ │  │       │ ┌sib index
    ││  ││ │   │ │  │       │ │  ┌sib base
    ││  ││ │   │ │  │       │ │  │  ┌lock
    ││  ││ │   │ │  │       │ │  │  │┌ymm
    ││  ││ │   │ │  │       │ │  │  ││┌log₂𝑏 (8/16/32/64)
    ││  ││ │   │ │  │       │ │  │  │││ ┌mode
    ││  ││ │   │ │  │       │ │  │  │││ │ ┌eamode
    ││  ││ │   │ │  │       │ │  │  │││ │ │ ┌mod
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ┌asz
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ │┌sego
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  ┌rexx
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  │┌rex         REGISTER
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  ││┌rexb       DISPATCH
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  │││┌srm       ENCODING
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  ││││  ┌rex
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  ││││  │┌rexb
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  ││││  ││┌rm
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  ││││  │││  ┌rexw
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  ││││  │││  │┌osz
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  ││││  │││  ││┌rex
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  ││││  │││  │││┌rexr
    ││  ││ │   │ │  │       │ │  │  │││ │ │ │ ││  ││││  │││  ││││┌reg
    ││6 ││5│5  │5│4 │4      │3│3 │3 │││2│2│2│2││  ││││  │││  │││││
    ││0 ││7│3  │1│8 │0      │8│5 │2 │││8│6│4│2││18││││12│││ 7│││││ 0
    │├─┐│├┐├──┐├┐├─┐├──────┐├┐├─┐├─┐││├┐├┐├┐├┐│├─┐│││├─┐││├─┐││││├─┐
  0b0000000000000000000000000000000000000000000000000000000000000000*/
  u64 rde;
  u64 uimm0;  // $immediate mostly sign-extended
  i64 disp;   // displacement(%xxx) mostly sign-extended
  u8 has_modrm;
  u8 pos_opcode;
  u8 max_bytes;
};

struct XedDecodedInst {
  unsigned char length;
  u8 bytes[15];
  struct XedOperands op;
};

extern const char kXedCopyright[];
extern const char kXedErrorNames[];

int DecodeInstruction(struct XedDecodedInst *, const void *, size_t, u64);

#endif /* BLINK_X86_H_ */
