# PDP-11 Target Machine Reference for LLVM Backend Development

This document describes the PDP-11 architecture as needed to implement
`llvm/lib/Target/PDP11`. It covers the general (baseline) PDP-11 ISA — the
intersection of features present on all models from the PDP-11/20 through the
LSI-11 — unless otherwise noted.

---

## 1. Architecture Overview

| Property | Value |
|---|---|
| Word size | 16 bits |
| Byte size | 8 bits (byte-addressable) |
| Endianness | Little-endian (LSB at lower address) |
| Address space | 16-bit logical (64 KB); 18-bit or 22-bit physical on later models |
| Instruction style | CISC, variable-length (1–3 words per instruction) |
| Execution | In-order, microcoded, scalar |
| I/O model | Memory-mapped (no dedicated I/O instructions) |

The PDP-11 is a **two-address CISC** machine. Unlike load-store architectures,
most arithmetic and logic instructions can read or write directly to or from
memory. Any operand in the instruction set can use any addressing mode with any
register — the design is largely orthogonal.

---

## 2. Registers

### 2.1 General-Purpose Registers

The PDP-11 has **eight 16-bit general-purpose registers**, R0–R7. All are
accessible in the instruction encoding via a 3-bit register field.

| Register | Alias | Role |
|---|---|---|
| R0 | — | General purpose; return value (low word) |
| R1 | — | General purpose; return value (high word for 32-bit) |
| R2 | — | General purpose |
| R3 | — | General purpose |
| R4 | — | General purpose |
| R5 | FP | Frame pointer (by software convention) |
| R6 | SP | Stack pointer (hardware-enforced for interrupts/traps) |
| R7 | PC | Program counter |

**Key properties:**
- R7 (PC) is a true general-purpose register. Addressing modes applied to R7
  produce immediate, absolute, and PC-relative operands.
- R6 (SP) is automatically used by hardware during interrupts and traps. The
  stack grows **downward** (toward lower addresses). Auto-decrement/increment
  on SP always operates by 2 (word size).
- Bytes can be held in the **low byte** of R0–R5 only. R6 and R7 cannot hold
  byte operands.
- Some models (PDP-11/45 and later) provide **dual register sets** for R0–R5
  to accelerate context switching; only one set is active at a time.

### 2.2 Processor Status Word (PSW)

The PSW is a 16-bit register, memory-mapped at address `0177776` on most
models. It is not a general-purpose register but must be modeled in the backend.

| Bits | Field | Description |
|---|---|---|
| 15–14 | CM | Current mode (00=Kernel, 01=Supervisor, 11=User) |
| 13–12 | PM | Previous mode |
| 11 | — | Reserved |
| 10–8 | Priority | Interrupt priority level (0–7) |
| 7 | T | Trace trap enable |
| 3 | N | Negative condition code |
| 2 | Z | Zero condition code |
| 1 | V | Overflow condition code |
| 0 | C | Carry condition code |

The four condition codes **N, Z, V, C** are set by virtually all arithmetic,
logic, and data-movement instructions. Conditional branches test combinations
of these codes.

---

## 3. Data Types and Memory Layout

| Type | Size | Alignment |
|---|---|---|
| Byte | 8 bits | Any address |
| Word | 16 bits | Even address (odd-address word access causes a trap) |
| Long/Double | 32 bits | Even address; stored as two words, high word at higher address |

Words are stored **little-endian**: the least-significant byte is at the lower
(even) address, and the most-significant byte is at the next (odd) address.

32-bit values (from optional EIS, FIS, or FPP extensions) use a format
sometimes called "PDP-endian": the two 16-bit words are stored in
**middle-endian** order — the high word at the lower pair of bytes — which
differs from straightforward little-endian 32-bit layout.

For the baseline backend, only 8-bit and 16-bit types need to be handled
natively. 32-bit integers and floating-point are optional extensions.

---

## 4. Memory Map

| Address Range (octal) | Description |
|---|---|
| `000000`–`000777` | Interrupt and trap vectors (2-word PC+PSW pairs) |
| `001000`–`157777` | User program space (typical) |
| `160000`–`177577` | I/O device registers (memory-mapped) |
| `177600`–`177677` | General-purpose registers (memory-mapped, most models) |
| `177776` | Processor Status Word |
| `177777` | (top of 16-bit address space) |

The top 4K words (`160000`–`177777`) are reserved for I/O and system
registers and are **never populated with RAM**.

---

## 5. Instruction Encoding

All instructions are exactly **one 16-bit word**, potentially followed by one
or two additional 16-bit **extension words** when an operand requires an
immediate value, absolute address, or PC-relative offset.

### 5.1 Double-Operand Instructions

```
 15  14 12  11  9   8  6   5  3   2  0
+---+-----+-----+-----+-----+-----+-----+
| B | OPC |  Mode  Src |  Mode  Dst     |
+---+-----+-----+-----+-----+-----+-----+
```

Bit 15 selects byte (1) vs. word (0) mode. Bits 14–12 are the opcode. Bits
11–6 encode the source operand (3-bit mode + 3-bit register), and bits 5–0
encode the destination operand similarly.

| Opcode (octal) | Mnemonic | Effect |
|---|---|---|
| `01` | MOV / MOVB | `dst ← src` |
| `02` | CMP / CMPB | Set flags on `src − dst`, discard result |
| `03` | BIT / BITB | Set flags on `src AND dst`, discard result |
| `04` | BIC / BICB | `dst ← dst AND NOT src` |
| `05` | BIS / BISB | `dst ← dst OR src` |
| `06` | ADD | `dst ← dst + src` (word only) |
| `16` | SUB | `dst ← dst − src` (word only) |

### 5.2 Single-Operand Instructions

```
 15  14   6    5   3   2  0
+---+--------+-----+-----+
| B |  OPC   | Mode  Dst |
+---+--------+-----+-----+
```

Bit 15 selects byte/word mode. Bits 14–6 are the 9-bit opcode. Bits 5–0
encode the single operand.

| Opcode (octal) | Mnemonic | Effect |
|---|---|---|
| `0050` | CLR / CLRB | `dst ← 0` |
| `0051` | COM / COMB | `dst ← NOT dst` |
| `0052` | INC / INCB | `dst ← dst + 1` |
| `0053` | DEC / DECB | `dst ← dst − 1` |
| `0054` | NEG / NEGB | `dst ← 0 − dst` |
| `0055` | ADC / ADCB | `dst ← dst + C` |
| `0056` | SBC / SBCB | `dst ← dst − C` |
| `0057` | TST / TSTB | Set flags on `dst`, no write |
| `0060` | ROR / RORB | Rotate right through carry |
| `0061` | ROL / ROLB | Rotate left through carry |
| `0062` | ASR / ASRB | Arithmetic shift right |
| `0063` | ASL / ASLB | Arithmetic shift left |
| `0067` | SXT | Sign extend N flag to word |
| `0003` | SWAB | Swap high and low bytes of word |

### 5.3 "One-and-a-Half" Operand Instructions (EIS)

These require the Extended Instruction Set option (standard on most models
after the 11/40).

```
 15     12  11  9   8   6   5  3   2  0
+--------+------+------+-----+-----+
|  OPC   |  Reg | Mode  Src/Dst    |
+--------+------+------+-----+-----+
```

| Opcode (octal) | Mnemonic | Effect |
|---|---|---|
| `070` | MUL | `(Reg,Reg+1) ← Reg × src` |
| `071` | DIV | `Reg ← quotient; Reg+1 ← remainder` |
| `072` | ASH | Arithmetic shift register by count |
| `073` | ASHC | Arithmetic shift register pair |
| `074` | XOR | `dst ← dst XOR Reg` (word only) |
| `077` | SOB | Decrement Reg; branch back if non-zero |

For MUL, DIV, and ASHC, the register must be **even** to use the full 32-bit
pair; if odd, only the lower 16-bit result is stored.

### 5.4 Branch Instructions

```
 15       8   7           0
+----------+---------------+
|  Opcode  | Signed offset |
+----------+---------------+
```

The 8-bit signed offset is added to PC (in words, i.e., multiplied by 2),
giving a range of **−128 to +127 words** from the instruction following the
branch. For larger jumps, use JMP or JSR.

| Opcode (octal) | Mnemonic | Condition |
|---|---|---|
| `0004xx` | BR | Always |
| `0010xx` | BNE | Z=0 |
| `0014xx` | BEQ | Z=1 |
| `0020xx` | BGE | N XOR V = 0 |
| `0024xx` | BLT | N XOR V = 1 |
| `0030xx` | BGT | Z=0 AND (N XOR V)=0 |
| `0034xx` | BLE | Z=1 OR (N XOR V)=1 |
| `1000xx` | BPL | N=0 |
| `1004xx` | BMI | N=1 |
| `1010xx` | BHI | C=0 AND Z=0 |
| `1014xx` | BLOS | C=1 OR Z=1 |
| `1020xx` | BVC | V=0 |
| `1024xx` | BVS | V=1 |
| `1030xx` | BCC / BHIS | C=0 |
| `1034xx` | BCS / BLO | C=1 |

### 5.5 Jump and Subroutine Instructions

| Mnemonic | Format | Effect |
|---|---|---|
| `JMP dst` | 1 operand | PC ← dst (any mode except register-direct) |
| `JSR Rn, dst` | reg + operand | Push Rn to stack, Rn ← PC, PC ← dst |
| `RTS Rn` | register | PC ← Rn, Rn ← pop from stack |
| `RTI` | zero operand | Pop PC and PSW from kernel stack |
| `RTT` | zero operand | Same as RTI, but T-bit suppressed until next instruction |

Standard C-style calls use `JSR PC, dst` (pushing the return address onto the
stack), equivalent to a CALL instruction on other architectures.

### 5.6 Miscellaneous and Zero-Operand Instructions

| Mnemonic | Opcode | Effect |
|---|---|---|
| `HALT` | `000000` | Stop CPU |
| `NOP` | `000240` | No operation (CCC with no bits) |
| `RESET` | `000005` | Reset all external devices |
| `WAIT` | `000001` | Suspend CPU until interrupt |
| `BPT` | `000003` | Breakpoint trap |
| `IOT` | `000004` | I/O trap |
| `EMT n` | `104000` | Emulator trap |
| `TRAP n` | `104400` | Software trap |
| `SPL n` | `000230` | Set processor priority level (11/45+) |

### 5.7 Condition Code Operations

```
 15              4   3   2   1   0
+------------------+---+---+---+---+
|  000024 / 000026 | N | Z | V | C |
+------------------+---+---+---+---+
```

Instructions starting with `000240` clear selected condition codes; `000260`
sets them. Setting no bits (`000240`) is the canonical NOP.

---

## 6. Addressing Modes

Every operand is encoded in 6 bits: 3 for mode, 3 for register. This scheme
applies to both source and destination operands in double-operand instructions.

| Mode | Syntax | Description |
|---|---|---|
| 0 — Register | `Rn` | Operand is the register contents |
| 1 — Register Deferred | `@Rn` or `(Rn)` | Register holds the address of the operand |
| 2 — Autoincrement | `(Rn)+` | Register is address; then Rn += 2 (or 1 for byte) |
| 3 — Autoincrement Deferred | `@(Rn)+` | Register points to address of operand; then Rn += 2 |
| 4 — Autodecrement | `-(Rn)` | Rn -= 2 (or 1 for byte) first; then used as address |
| 5 — Autodecrement Deferred | `@-(Rn)` | Rn -= 2 first; then used as pointer to address |
| 6 — Index | `X(Rn)` | Address = Rn + extension word X |
| 7 — Index Deferred | `@X(Rn)` | Address = *(Rn + X) |

When these modes are applied to **R7 (PC)**, they yield special forms:

| PC Mode | Syntax | Description |
|---|---|---|
| Mode 2 on PC | `#N` | Immediate: operand is next instruction word |
| Mode 3 on PC | `@#N` | Absolute: address is the next instruction word |
| Mode 6 on PC | `A` | Relative: address = PC + offset word |
| Mode 7 on PC | `@A` | Relative deferred: address = *(PC + offset word) |

The stack push idiom is `-(SP)` (mode 4 on R6) and pop is `(SP)+` (mode 2 on R6).

---

## 7. Calling Convention (Unix/C ABI)

The following reflects the convention used by the original UNIX C compiler for
the PDP-11, which is the de facto ABI for C code on this platform.

### 7.1 Argument Passing

All arguments are passed **on the stack**, pushed right-to-left (last argument
pushed first, so the first argument is at the top of the stack on entry). All
arguments are passed by value. Arguments are padded to word (16-bit) size.

### 7.2 Return Values

| Return type | Location |
|---|---|
| 16-bit integer | R0 |
| 32-bit integer / pointer | R0 (low word), R1 (high word) |
| Struct / large value | Via caller-allocated buffer; pointer passed as hidden first argument |

### 7.3 Register Conventions

| Register | Callee-saved? | Role |
|---|---|---|
| R0 | No (caller-saved) | Return value; scratch |
| R1 | No (caller-saved) | Return value high; scratch |
| R2 | No (caller-saved) | Scratch |
| R3 | No (caller-saved) | Scratch |
| R4 | No (caller-saved) | Scratch |
| R5 | Yes (callee-saved) | Frame pointer |
| R6 / SP | Yes (managed by ABI) | Stack pointer |
| R7 / PC | N/A | Program counter |

R5 is used as the **frame pointer (FP)**. The callee's prologue saves the
previous FP and sets up a new frame. The epilogue restores FP and SP before
returning.

### 7.4 Stack Frame Layout

Stack grows downward. On entry to a function, after the prologue:

```
  (high addresses)
  arg N          <- pushed last by caller
  ...
  arg 1          <- pushed first by caller (top of args)
  return address <- pushed by JSR PC,dest
  saved R5 (FP) <- pushed by callee prologue    <-- FP points here
  local vars     <- allocated by callee
  saved regs     <- callee-saved registers
                    <-- SP points here
  (low addresses)
```

### 7.5 Function Prologue / Epilogue

Typical prologue (generated by the `csv` helper or inline):
```asm
JSR  R5, csv   ; save R5, set new FP, save R2-R4
```

Typical epilogue:
```asm
JMP  cret      ; restore R2-R4, FP, return
```

For simple backends, the prologue can be expanded inline:
```asm
MOV  R5, -(SP) ; save frame pointer
MOV  SP, R5   ; establish new frame pointer
SUB  #N, SP   ; allocate N bytes of local space
```

---

## 8. Instruction Timing

The PDP-11 has **no pipeline**, no branch prediction, and no out-of-order
execution. Every instruction is microcoded and executes sequentially.

Timing has three additive components:

1. **Fetch/execute time** — base cost of the instruction opcode.
2. **Source operand access time** — depends on addressing mode.
3. **Destination operand access time** — depends on addressing mode.

Representative timings on the PDP-11/70 (from cache):

| Case | Time |
|---|---|
| Register-to-register `ADD Rm, Rn` | ~0.3 µs (≈2 cycles) |
| Indexed load `ADD x(Rm), y(Rn)` | ~2.55 µs (≈12+ cycles) |
| Cache miss penalty per access | +1.02 µs |
| Single-precision FP add (FPP) | 2.4–5.5 µs + operand fetch |

For LLVM scheduling purposes: the architecture is **strictly in-order**; all
`MicroOpBufferSize` and `LoopMicroOpBufferSize` values should be **0**, and
`IssueWidth` should be **1**.

---

## 9. Optional Instruction Set Extensions

These are not part of the baseline ISA but should be modeled as subtarget
features.

| Extension | Feature Name | Instructions Added |
|---|---|---|
| EIS (Extended Instruction Set) | `HasEIS` | MUL, DIV, ASH, ASHC, XOR, SOB |
| FIS (Floating Instruction Set) | `HasFIS` | FADD, FSUB, FMUL, FDIV (stack-based, single-precision) |
| FPP (Floating Point Processor) | `HasFPP` | Full float/double ALU; adds 6 dedicated 64-bit FP registers (AC0–AC5) |
| CIS (Commercial Instruction Set) | `HasCIS` | BCD string and decimal arithmetic |

EIS is **standard** on all models after the PDP-11/40 and should be enabled
by default in a general-purpose backend target.

---

## 10. Interrupts and Traps

Interrupt vectors occupy low memory starting at address `0`. Each vector is
two words: the new PC value followed by the new PSW value for the handler.

| Vector address | Event |
|---|---|
| `000004` | Bus error (odd address, timeout) |
| `000010` | Reserved / illegal instruction |
| `000014` | BPT trace trap |
| `000020` | IOT trap |
| `000024` | Power fail |
| `000030` | EMT emulator trap |
| `000034` | TRAP instruction |

Hardware interrupts are prioritized at levels 4–7. A device's interrupt
request is only granted if its level exceeds the current CPU priority stored
in PSW bits 7–5.

The interrupt sequence pushes the current PSW and PC onto the kernel stack,
then loads the vector. RTI (or RTT) restores both.

---

## 11. LLVM Backend Implementation Notes

### Register Classes

- `GR16` — all 8 registers (R0–R7), 16-bit.
- `GR8` — pseudo class for byte sub-registers of R0–R5 (low byte only).
- Special registers PC (R7) and SP (R6) should be marked reserved; FP (R5)
  reserved if a frame pointer is used.

### Instruction Formats

Define at minimum:
- `DoubleOp` — 16-bit opcode word with src and dst 6-bit operand fields.
- `SingleOp` — 16-bit opcode word with one 6-bit operand field.
- `Branch` — 16-bit opcode word with 8-bit signed offset.
- `OneAndHalf` — 16-bit opcode with 3-bit register and 6-bit operand fields.
- `NoOp` — 16-bit zero-operand instruction.

### Addressing Mode Lowering

Since LLVM's SelectionDAG does not natively model auto-increment or
auto-decrement memory access, these modes are best used only for stack push
and pop operations (`-(SP)` and `(SP)+`), which map naturally to
`ISD::CALLSEQ_START` / `ISD::CALLSEQ_END` and prologue/epilogue code.

General memory access should be lowered to Register Deferred (mode 1) and
Index (mode 6) modes, which map cleanly to `load`/`store` with base + offset.

### SelectionDAG Legalization

- Native types: `i8`, `i16`. All operations on these are legal.
- `i32` is not natively legal. Expand to pairs of `i16` operations unless EIS
  is available (which provides MUL/DIV returning 32-bit results in register
  pairs).
- No native `i1`; expand to `i8` or `i16`.

### Condition Codes

PDP-11 condition codes are set as a side effect of most instructions (unlike
ARM or RISC-V where comparison is explicit). Model this using a virtual
`PSW` register as an implicit def on all ALU operations, and as an implicit use
in branch instructions. Use `ISD::BR_CC` and `ISD::SELECT_CC` nodes that lower
to a compare instruction followed by a conditional branch.

### Stack and Frame Management

- Stack pointer: R6 (SP).
- Frame pointer: R5 (FP), used when `hasFP()` is true.
- Stack grows downward; use `-(SP)` for push, `(SP)+` for pop.
- Words are always pushed/popped as 16-bit quantities; SP is always even.
- Return address is on the stack (not in a link register).

### Scheduler Model

```tablegen
def PDP11SchedModel : SchedMachineModel {
  let MicroOpBufferSize    = 0;  // In-order; no OOO buffer
  let LoopMicroOpBufferSize = 0; // No loop buffer
  let IssueWidth           = 1;  // Scalar
  let LoadLatency          = 2;  // Cached load
  let MispredictPenalty    = 5;  // Approximate pipeline flush
  let CompleteModel        = 0;
  let PostRAScheduler      = 0;
  let Itineraries          = PDP11Itinerary;
}
```

---

## 12. Key Reference Documents

- *PDP-11/40 Processor Handbook*, Digital Equipment Corporation, 1972.
- *PDP-11/70 Processor Handbook*, Digital Equipment Corporation, 1975.
- *LSI-11 PDP-11/03 Processor Handbook*, Digital Equipment Corporation, 1975.
- PDP-11 Architecture, Wikipedia: https://en.wikipedia.org/wiki/PDP-11_architecture
- PDP-11 C Stack Operation (Noel Chiappa, MIT): https://gunkies.org/wiki/PDP-11_C_stack_operation
- PDP-11 Instruction Reference: http://ftp.dbit.com/pub/pdp11/faq/faq.pages/PDPinst.html