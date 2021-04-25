/*
 * sim3.c
 *
 *  Created on: Jul 14, 2019
 *      Author: Omar R. Gebril
 */

#include "sim3.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

void extract_instructionFields(WORD instruction, InstructionFields *fieldsOut) {
  unsigned int tempInst = instruction;
  fieldsOut->opcode = (tempInst >> 26) & 0x3f;
  tempInst = tempInst << 6;
  fieldsOut->rs = (tempInst >> 27) & 0x3f;
  tempInst = instruction;
  tempInst = tempInst << 11;
  fieldsOut->rt = (tempInst >> 27) & 0x3f;
  tempInst = instruction;
  tempInst = tempInst << 16;
  fieldsOut->rd = (tempInst >> 27) & 0x3f;
  tempInst = instruction;
  tempInst = tempInst << 21;
  fieldsOut->shamt = (tempInst >> 27) & 0x3f;
  tempInst = instruction;
  tempInst = tempInst << 26;
  fieldsOut->funct = (tempInst >> 26) & 0x3f;
  tempInst = instruction;
  tempInst = tempInst << 16;
  fieldsOut->imm16 = (tempInst >> 16);
  fieldsOut->imm32 = signExtend16to32(fieldsOut->imm16);
  tempInst = instruction;
  tempInst = tempInst << 6;
  fieldsOut->address = (tempInst >> 6);
}

int fill_CPUControl(InstructionFields *fields, CPUControl *controlOut) {
  if (fields->opcode == 0x00) {
	  // add || addu
    if (fields->funct == 0x20 || fields->funct == 0x21) {


      controlOut->ALUsrc = 0;
      controlOut->ALU.op = 2;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 1;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;

      return 1;
    }
    // SUB || SUBU
    if (fields->funct == 0x22 || fields->funct == 0x23) {

      controlOut->ALUsrc = 0;
      controlOut->ALU.op = 2;
      controlOut->ALU.bNegate = 1;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 1;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // and
    if (fields->funct == 0x24) {

      controlOut->ALUsrc = 0;
      controlOut->ALU.op = 0;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 1;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // or
    if (fields->funct == 0x25) {

      controlOut->ALUsrc = 0;
      controlOut->ALU.op = 1;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 1;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // xor
    if (fields->funct == 0x26) {

      controlOut->ALUsrc = 0;
      controlOut->ALU.op = 4;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 1;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // slt
    if (fields->funct == 0x2a) {

      controlOut->ALUsrc = 0;
      controlOut->ALU.op = 3;
      controlOut->ALU.bNegate = 1;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 1;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // mult

    if (fields->funct == 0x18) {
    	controlOut->ALUsrc = 0;
        controlOut->ALU.op = 2;
        controlOut->ALU.bNegate = 0;
        controlOut->memRead = 0;
        controlOut->memWrite = 0;
        controlOut->memToReg = 0;
        controlOut->regDst = 1;
        controlOut->regWrite = 1;
        controlOut->branch = 0;
        controlOut->jump = 0;
        controlOut->extra1 = fields->imm32;
        return 1;
        }
  } else {
	  // addi
    if (fields->opcode == 0x08) {

      controlOut->ALUsrc = 1;
      controlOut->ALU.op = 2;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 0;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // addiu
    if (fields->opcode == 0x09) {

      controlOut->ALUsrc = 1;
      controlOut->ALU.op = 2;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 0;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // andi
    if (fields->opcode == 0x0c) {

      controlOut->ALUsrc = 1;
      controlOut->ALU.op = 0;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 0;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // lb
    if (fields->opcode == 0x20) {

      controlOut->ALUsrc = 1;
      controlOut->ALU.op = 2;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 1;
      controlOut->memWrite = 0;
      controlOut->memToReg = 1;
      controlOut->regDst = 0;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // slti
    if (fields->opcode == 0x0a) {

      controlOut->ALUsrc = 1;
      controlOut->ALU.op = 3;
      controlOut->ALU.bNegate = 1;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 0;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // lw
    if (fields->opcode == 0x23) {

      controlOut->ALUsrc = 1;
      controlOut->ALU.op = 2;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 1;
      controlOut->memWrite = 0;
      controlOut->memToReg = 1;
      controlOut->regDst = 0;
      controlOut->regWrite = 1;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // sw
    if (fields->opcode == 0x2b) {

      controlOut->ALUsrc = 1;
      controlOut->ALU.op = 2;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 0;
      controlOut->memWrite = 1;
      controlOut->memToReg = 0;
      controlOut->regDst = 0;
      controlOut->regWrite = 0;
      controlOut->branch = 0;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // beq
    if (fields->opcode == 0x04) {

      controlOut->ALUsrc = 0;
      controlOut->ALU.op = 2;
      controlOut->ALU.bNegate = 1;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 0;
      controlOut->regWrite = 0;
      controlOut->branch = 1;
      controlOut->jump = 0;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
    // j
    if (fields->opcode == 0x02) {

      controlOut->ALUsrc = 0;
      controlOut->ALU.op = 0;
      controlOut->ALU.bNegate = 0;
      controlOut->memRead = 0;
      controlOut->memWrite = 0;
      controlOut->memToReg = 0;
      controlOut->regDst = 0;
      controlOut->regWrite = 0;
      controlOut->branch = 0;
      controlOut->jump = 1;
      controlOut->extra1 = fields->imm32;
      return 1;
    }
  }
  return 0;
}

WORD getInstruction(WORD curPC, WORD *instructionMemory) {
	WORD instr = 0;

  if (instructionMemory[curPC / 4]) {
    instr = instructionMemory[curPC / 4];
  }
  return instr;
}

WORD getALUinput1(CPUControl *control, InstructionFields *fields, WORD rsVal, WORD rtVal, WORD reg32, WORD reg33, WORD oldPC) {

  return rsVal;
}

WORD getALUinput2(CPUControl *control, InstructionFields *fields, WORD rsVal, WORD rtVal, WORD reg32, WORD reg33, WORD oldPC) {

  if (control->ALUsrc == 0) {
    return rtVal;

  } else {
    return fields->imm32;
  }
}

void execute_ALU(CPUControl *control, WORD input1, WORD input2, ALUResult *aluResult) {
  aluResult->zero = 0;
  if (control->ALU.op == 0) {
    aluResult->result = input1 & input2;
  }
  if (control->ALU.op == 1) {
    aluResult->result = input1 | input2;
  }
  if (control->ALU.op == 2) {
    if (control->ALU.bNegate) {
      aluResult->result = input1 - input2;

    } else {
      aluResult->result = input1 + input2;
    }
  }
  if (control->ALU.op == 3) {
    aluResult->result = input1 < input2;
    if (!aluResult->result) {
      aluResult->zero = 1;
    }
  }
  if (control->ALU.op == 4) {
    aluResult->result = input1 ^ input2;
  }
  if (!aluResult->result)
    aluResult->zero = 1;
}

void execute_MEM(CPUControl *control, ALUResult *aluResult, WORD rsVal, WORD rtVal, WORD *memory, MemResult *resultFin) {

  WORD result = 0xffff;

  if (control->memRead && control->memToReg) {
    result = (result << 16) | aluResult->result;
    resultFin->readVal = result;
  }
  if (control->memWrite) {
    //   aluResultIn->result[memory]

    memory[aluResult->result / 4] = rtVal;
  }
}

WORD getNextPC(InstructionFields *fields, CPUControl *control, int aluZero, WORD rsVal, WORD rtVal, WORD oldPC) {
  if ((fields->opcode == 0x04 && aluZero)) {
    return oldPC + 4 + fields->imm32 * 4;
  }
  if (fields->opcode == 0x02) {

    unsigned int addr;
    addr = oldPC >> 28;
    addr = addr << 28;
    addr += fields->address << 2;
    return addr;
  } else {
    return oldPC + 4;
  }
}

void execute_updateRegs(InstructionFields *fields, CPUControl *control, ALUResult *aluResult, MemResult *memResult, WORD *regs) {
  if (control->regWrite) {

    if (control->regDst) {
      if (control->memRead && control->memToReg) {
        regs[fields->rd] = memResult->readVal;
      } else {
        regs[fields->rd] = aluResult->result;
      }
    } else {
      if (control->memRead && control->memToReg) {
        regs[fields->rt] = memResult->readVal;
      } else {
        regs[fields->rt] = aluResult->result;
      }
    }
  }
}
