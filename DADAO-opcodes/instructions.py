import functools

# Verilog-like integer slicing
def _slice(i, x, y):
    return (i & ((1 << (y + 1)) - 1)) >> x

class BitPat:
    all_opcodes = set()
    def __init__(self, pat):
        self.mask  = int(pat[1:].replace('0', '1').replace('?', '0'), 2)
        self.match = int(pat[1:].replace('?', '0'), 2)
        self.all_opcodes.add(self)

    @functools.lru_cache(maxsize=4096)
    def matches(self, inst):
        return (inst & self.mask) == self.match

    @classmethod
    @functools.lru_cache(maxsize=4096)
    def decode(self, inst):
        for opcode in self.all_opcodes:
            if opcode.matches(inst):
                return opcode
        return None

class Instruction:
    def __init__(self, inst):
        self._opcode = None
        self.inst = inst

    @property
    def opcode(self):
        if not self._opcode:
            self._opcode = BitPat.decode(self.inst)
        return self._opcode

    def __getitem__(self, i):
        if isinstance(i, slice):
            assert(i.step is None)
            start = i.start
            stop = i.stop
            assert(start >= 0 and start < 32)
            assert(stop >= 0 and stop < 32 and stop > start)
            return (self.inst & ((1 << (stop + 1)) - 1)) >> start
        else:
            assert(i >= 0 and i < 32)
            return (self.inst >> i) & 1


#instructions's bitpat, this part is generated by parse-opcodes.py (don't delete this line)





#insn-type (don't delete this line)
BR_OPCODES = {BRN, BRNN, BRZ, BRNZ, BRP, BRNP, BREQ, BRNE}
JMP_OPCODES = {JUMPr, JUMPi, CALLr, CALLi, RET}
BRJMP_OPCODES = BR_OPCODES | JMP_OPCODES
LD_OPCODES = {LDBS, LDBU, LDWS, LDWU, LDTS, LDTU, LDMBS, LDMBU, LDMWS, LDMWU, LDMTS, LDMTU, LDMO, LDRB, LDMRB, LDFT, LDFO, LDMFT, LDMFO, LDMRA}
ST_OPCODES = {STB, STW, STT, STO, STMB, STMW, STMT, STMO, STRB, STMRB, STFT, STFO, STMFT, STMFO, STMRA}
LDST_OPCODES = LD_OPCODES | ST_OPCODES
SET_REG_OPCODES = {RD2RD, RD2RB, RB2RD, RB2RB, RF2RF, RF2RD, RD2RF, RA2RD, RD2RA, CSN, CSZ, CSP, CSEQ, CSNE}
SET_IMM_OPCODES = {SETOW, SETZWrd, SETZWrb, ORWrd, ORWrb, ANDNWrd, ANDNWrb, SETW}
SET_OPCODES = SET_REG_OPCODES | SET_IMM_OPCODES
ARITH_OPCODES = {ADDrd, ADDrb, SUBrd, SUBrb, MULS, MULU, DIVS, DIVU, ADRP, ADDIrd, ADDIrb, CMPSr, CMPSi, CMPUr, CMPUi, CMP, AND, ORR, XOR, XNOR, SHLUi, SHLUr, SHRSi, SHRSr, SHRUi, SHRUr, EXTSi, EXTSr, EXTZi, EXTZr}
