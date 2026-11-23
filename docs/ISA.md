# XVM Instruction Set Architecture

## Architectural model

| Property                             | Definition                          |
| ------------------------------------ | ----------------------------------- |
| General-purpose registers            | 8 registers, `R0`-`R7`, each 8 bits |
| Arithmetic and memory data           | 8 bits                              |
| Program counter (PC)                 | 16 bits                             |
| Memory address operand               | 16 bits, unsigned                   |
| Instruction size                     | Fixed 4 bytes                       |
| Address operand byte order           | High byte first, then low byte      |
| General-purpose register reset value | Zero                                |
| Reset PC                             | `0x0000`                            |
Registers have no dedicated stack-pointer, link-register, or address-register
role. There is no stack, interrupt mechanism, or 16-bit arithmetic instruction
in the current ISA.

## Instruction encoding

| Byte offset | Meaning   |
| ----------- | --------- |
| `+0`        | Opcode    |
| `+1`        | Operand 1 |
| `+2`        | Operand 2 |
| `+3`        | Operand 3 |

|  Opcode  |    Mnemonic     | Operand encoding                              | Operation                                    |
| :------: | :-------------: | :--------------------------------------------- | :-------------------------------------------- |
|  `0x00`  |      `NOP`      | unused, unused, unused                        | No operation.                                |
|  `0x01`  |   `LOAD_IMM`    | register, immediate8, unused                  | `R[operand1] = operand2`.                    |
|  `0x02`  |   `LOAD_MEM`    | register, address-high, address-low           | Load one byte from memory into the register. |
|  `0x03`  |   `STORE_MEM`   | register, address-high, address-low           | Store one register byte to memory.           |
|  `0x04`  |   `COPY_REG`    | destination-register, source-register, unused | Copy one register to another.                |
|  `0x05`  |      `ADD`      | destination-register, source-register, unused | Add source to destination modulo 256.        |
|  `0x06`  |      `SUB`      | destination-register, source-register, unused | Subtract source from destination modulo 256. |
|  `0x07`  |      `CMP`      | register, register, unused                    | Set Z if the two register values are equal.  |
|  `0x08`  |     `JUMP`      | unused, address-high, address-low             | Set PC to the absolute target.               |
|  `0x09`  |   `JUMP_ZERO`   | unused, address-high, address-low             | Jump if Z is set.                            |
|  `0x0A`  | `JUMP_NOT_ZERO` | unused, address-high, address-low             | Jump if Z is clear.                          |
|  `0x0B`  |   `PORT_OUT`    | port8, source-register, unused                | Write the register byte to an I/O port.      |
| ``0x0C`` |   unassigned    | unassigned                                    | unassigned                                   |
|  `0x0D`  |     `HALT`      | unused, unused, unused                        | Halt execution.                              |
|  `0x0E`  |      `AND`      | destination-register, source-register, unused | Bitwise AND; result goes to destination.     |
|  `0x0F`  |      `OR`       | destination-register, source-register, unused | Bitwise OR; result goes to destination.      |
