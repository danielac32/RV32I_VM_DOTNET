#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
// =============================================
// Constantes RISC-V RV32I
// =============================================

#define NR_RV32I_REGS 32
#define XREG_ZERO 0
#define XREG_RETURN_ADDRESS 0
#define XREG_STACK_POINTER 2

#define STACK_POINTER_START_VAL 0x0
#define BASE_RAM 0x80000000U
#define PROGRAM_COUNTER_START_VAL BASE_RAM
#define UART_ADDRESS 0x300000U

// Instrucciones
#define INSTR_ADD_SUB_SLL_SLT_SLTU_XOR_SRL_SRA_OR_AND 0x33
#define INSTR_JALR 0x67
#define INSTR_ADDI_SLTI_SLTIU_XORI_ORI_ANDI_SLLI_SRLI_SRAI 0x13
#define INSTR_LB_LH_LW_LBU_LHU 0x03
#define INSTR_SB_SH_SW 0x23
#define INSTR_BEQ_BNE_BLT_BGE_BLTU_BGEU 0x63
#define INSTR_LUI 0x37
#define INSTR_AUIPC 0x17
#define INSTR_JAL 0x6F
#define INSTR_FENCE_FENCE_I 0x0F
#define INSTR_ECALL_EBREAK_CSRRW_CSRRS_CSRRC_CSRRWI_CSRRSI_CSRRCI 0x73

// Func3
#define FUNC3_INSTR_ADD_SUB 0x0
#define FUNC3_INSTR_SLL 0x1
#define FUNC3_INSTR_SLT 0x2
#define FUNC3_INSTR_SLTU 0x3
#define FUNC3_INSTR_XOR 0x4
#define FUNC3_INSTR_SRL_SRA 0x5
#define FUNC3_INSTR_OR 0x6
#define FUNC3_INSTR_AND 0x7

#define FUNC3_INSTR_JALR 0x0

#define FUNC3_INSTR_ADDI 0x0
#define FUNC3_INSTR_SLTI 0x2
#define FUNC3_INSTR_SLTIU 0x3
#define FUNC3_INSTR_XORI 0x4
#define FUNC3_INSTR_ORI 0x6
#define FUNC3_INSTR_ANDI 0x7
#define FUNC3_INSTR_SLLI 0x1
#define FUNC3_INSTR_SRLI_SRAI 0x5

#define FUNC3_INSTR_LB 0x0
#define FUNC3_INSTR_LH 0x1
#define FUNC3_INSTR_LW 0x2
#define FUNC3_INSTR_LBU 0x4
#define FUNC3_INSTR_LHU 0x5

#define FUNC3_INSTR_SB 0x0
#define FUNC3_INSTR_SH 0x1
#define FUNC3_INSTR_SW 0x2

#define FUNC3_INSTR_BEQ 0x0
#define FUNC3_INSTR_BNE 0x1
#define FUNC3_INSTR_BLT 0x4
#define FUNC3_INSTR_BGE 0x5
#define FUNC3_INSTR_BLTU 0x6
#define FUNC3_INSTR_BGEU 0x7

#define FUNC3_INSTR_FENCE 0x0
#define FUNC3_INSTR_FENCE_I 0x1

// Func7
#define FUNC7_INSTR_ADD 0x00
#define FUNC7_INSTR_SUB 0x20
#define FUNC7_INSTR_SLL 0x00
#define FUNC7_INSTR_SLT 0x00
#define FUNC7_INSTR_SLTU 0x00
#define FUNC7_INSTR_XOR 0x00
#define FUNC7_INSTR_SRL 0x00
#define FUNC7_INSTR_SRA 0x20
#define FUNC7_INSTR_OR 0x00
#define FUNC7_INSTR_AND 0x00

// =============================================
// Estructuras
// =============================================

typedef struct {
    uint32_t X[NR_RV32I_REGS]; // Registros x0-x31
    uint32_t Pc;               // Program Counter
    uint32_t Instruction;      // Instrucción actual
    uint8_t Opcode;
    uint8_t Rd, Rs1, Rs2;
    uint8_t Func3, Func7;
    uint32_t Immediate;
    int32_t JumpOffset;
    
    uint32_t mstatus;
    uint32_t cyclel;
    uint32_t cycleh;

    uint32_t timerl;
    uint32_t timerh;
    uint32_t timermatchl;
    uint32_t timermatchh;

    uint32_t mscratch;
    uint32_t mtvec;
    uint32_t mie;
    uint32_t mip;

    uint32_t mepc;
    uint32_t mtval;
    uint32_t mcause;

    // Note: only a few bits are used.  (Machine = 3, User = 0)
    // Bits 0..1 = privilege.
    // Bit 2 = WFI (Wait for interrupt)
    // Bit 3+ = Load/Store reservation LSBs.
    uint32_t extraflags;
} Rv32Core;

typedef struct {
    uint8_t *ram;
    uint32_t base_addr;
    size_t size_bytes;
} RamDevice;

// =============================================
// Funciones auxiliares
// =============================================

// Lee un entero de 32 bits en little-endian desde la memoria
static uint32_t read_word(const uint8_t *mem, uint32_t offset) {
    return (uint32_t)(mem[offset + 0]) |
           ((uint32_t)(mem[offset + 1]) << 8) |
           ((uint32_t)(mem[offset + 2]) << 16) |
           ((uint32_t)(mem[offset + 3]) << 24);
}

// Escribe un entero de 32 bits en little-endian en la memoria
static void write_word(uint8_t *mem, uint32_t offset, uint32_t value) {
    mem[offset + 0] = (uint8_t)(value >> 0);
    mem[offset + 1] = (uint8_t)(value >> 8);
    mem[offset + 2] = (uint8_t)(value >> 16);
    mem[offset + 3] = (uint8_t)(value >> 24);
}

// Lectura de memoria (simula acceso de 32 bits)
static uint32_t ram_read(RamDevice *ram, uint32_t address) {
    if (address == UART_ADDRESS) {
        return 0;
    }
    if (address >= ram->base_addr && address < ram->base_addr + ram->size_bytes) {
        uint32_t offset = address - ram->base_addr;
        return read_word(ram->ram, offset);
    }
    fprintf(stderr, "Error: Lectura en dirección inválida 0x%08X\n", address);
    return 0;
}

// Escritura en memoria (con ancho: 1, 2, 4 bytes)
static void ram_write(RamDevice *ram, uint32_t address, uint32_t value, int width) {
    if (address == UART_ADDRESS) {
        putchar(value & 0xFF);
        return;
    }

    if (address < ram->base_addr || address + width > ram->base_addr + ram->size_bytes) {
        fprintf(stderr, "Error: Escritura inválida 0x%08X\n", address);
        return;
    }

    uint32_t offset = address - ram->base_addr;

    switch (width) {
        case 4:
            write_word(ram->ram, offset, value);
            break;
        case 2:
            ram->ram[offset + 0] = (uint8_t)(value >> 0);
            ram->ram[offset + 1] = (uint8_t)(value >> 8);
            break;
        case 1:
            ram->ram[offset + 0] = (uint8_t)(value >> 0);
            break;
        default:
            fprintf(stderr, "Ancho no soportado: %d\n", width);
            break;
    }
}

// Carga programa binario desde archivo
static int load_program(RamDevice *ram, const char *filepath) {
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        fprintf(stderr, "Error al abrir el archivo: %s\n", filepath);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    rewind(f);

    if (file_size > (long)ram->size_bytes) {
        fprintf(stderr, "El programa es demasiado grande para la RAM.\n");
        fclose(f);
        return -1;
    }

    size_t read_bytes = fread(ram->ram, 1, file_size, f);
    fclose(f);

    if (read_bytes != (size_t)file_size) {
        fprintf(stderr, "Error al leer el programa.\n");
        return -1;
    }

    printf("Programa cargado: %ld bytes desde %s\n", file_size, filepath);
    return 0;
}

// =============================================
// Funciones de ejecución del core
// =============================================

static void fetch(Rv32Core *core, RamDevice *ram) {
    core->Instruction = ram_read(ram, core->Pc);
    core->Pc += 4;
}

static void decode(Rv32Core *core) {
    core->Opcode = (uint8_t)(core->Instruction & 0x7F);
    core->Rd = (uint8_t)((core->Instruction >> 7) & 0x1F);
    core->Func3 = (uint8_t)((core->Instruction >> 12) & 0x7);
    core->Rs1 = (uint8_t)((core->Instruction >> 15) & 0x1F);
    core->Rs2 = (uint8_t)((core->Instruction >> 20) & 0x1F);
    core->Func7 = (uint8_t)((core->Instruction >> 25) & 0x7F);
}

static void execute_rtype(Rv32Core *core) {
    switch (core->Func3) {
        case FUNC3_INSTR_ADD_SUB:
            if (core->Func7 == FUNC7_INSTR_SUB)
                core->X[core->Rd] = core->X[core->Rs1] - core->X[core->Rs2];
            else
                core->X[core->Rd] = core->X[core->Rs1] + core->X[core->Rs2];
            break;

        case FUNC3_INSTR_SLL:
            core->X[core->Rd] = core->X[core->Rs1] << (core->X[core->Rs2] & 0x1F);
            break;

        case FUNC3_INSTR_SLT:
            core->X[core->Rd] = ((int32_t)core->X[core->Rs1] < (int32_t)core->X[core->Rs2]) ? 1u : 0u;
            break;

        case FUNC3_INSTR_SLTU:
            core->X[core->Rd] = (core->X[core->Rs1] < core->X[core->Rs2]) ? 1u : 0u;
            break;

        case FUNC3_INSTR_XOR:
            core->X[core->Rd] = core->X[core->Rs1] ^ core->X[core->Rs2];
            break;

        case FUNC3_INSTR_SRL_SRA:
            if (core->Func7 == FUNC7_INSTR_SRA)
                core->X[core->Rd] = (uint32_t)((int32_t)core->X[core->Rs1] >> (core->X[core->Rs2] & 0x1F));
            else
                core->X[core->Rd] = core->X[core->Rs1] >> (core->X[core->Rs2] & 0x1F);
            break;

        case FUNC3_INSTR_OR:
            core->X[core->Rd] = core->X[core->Rs1] | core->X[core->Rs2];
            break;

        case FUNC3_INSTR_AND:
            core->X[core->Rd] = core->X[core->Rs1] & core->X[core->Rs2];
            break;

        default:
            fprintf(stderr, "Unknown R-type func3: 0x%02X\n", core->Func3);
            exit(1);
    }
}

static void execute_itype_arith(Rv32Core *core) {
    int32_t imm = (int32_t)((core->Instruction >> 20) & 0xFFF);
    if (imm & 0x800) imm |= 0xFFFFF000; // sign extend

    switch (core->Func3) {
        case FUNC3_INSTR_ADDI:
            core->X[core->Rd] = core->X[core->Rs1] + (uint32_t)imm;
            break;

        case FUNC3_INSTR_SLTI:
            core->X[core->Rd] = ((int32_t)core->X[core->Rs1] < imm) ? 1u : 0u;
            break;

        case FUNC3_INSTR_SLTIU:
            core->X[core->Rd] = (core->X[core->Rs1] < (uint32_t)imm) ? 1u : 0u;
            break;

        case FUNC3_INSTR_XORI:
            core->X[core->Rd] = core->X[core->Rs1] ^ (uint32_t)imm;
            break;

        case FUNC3_INSTR_ORI:
            core->X[core->Rd] = core->X[core->Rs1] | (uint32_t)imm;
            break;

        case FUNC3_INSTR_ANDI:
            core->X[core->Rd] = core->X[core->Rs1] & (uint32_t)imm;
            break;

        case FUNC3_INSTR_SLLI:
            core->X[core->Rd] = core->X[core->Rs1] << (imm & 0x1F);
            break;

        case FUNC3_INSTR_SRLI_SRAI:
            if (core->Func7 == FUNC7_INSTR_SRA)
                core->X[core->Rd] = (uint32_t)((int32_t)core->X[core->Rs1] >> imm);
            else
                core->X[core->Rd] = core->X[core->Rs1] >> imm;
            break;

        default:
            fprintf(stderr, "Unknown I-type func3: 0x%02X\n", core->Func3);
            exit(1);
    }
}

static void execute_load(Rv32Core *core, RamDevice *ram) {
    int32_t imm = (int32_t)((core->Instruction >> 20) & 0xFFF);
    if (imm & 0x800) imm |= 0xFFFFF000;

    uint32_t addr = core->X[core->Rs1] + (uint32_t)imm;

    switch (core->Func3) {
        case FUNC3_INSTR_LB:
            {
                uint8_t b = (uint8_t)(ram_read(ram, addr) & 0xFF);
                core->X[core->Rd] = (uint32_t)(int8_t)b; // sign extend
            }
            break;

        case FUNC3_INSTR_LH:
            {
                uint16_t h = (uint16_t)(ram_read(ram, addr) & 0xFFFF);
                core->X[core->Rd] = (uint32_t)(int16_t)h; // sign extend
            }
            break;

        case FUNC3_INSTR_LW:
            core->X[core->Rd] = ram_read(ram, addr);
            break;

        case FUNC3_INSTR_LBU:
            core->X[core->Rd] = ram_read(ram, addr) & 0xFF;
            break;

        case FUNC3_INSTR_LHU:
            core->X[core->Rd] = ram_read(ram, addr) & 0xFFFF;
            break;

        default:
            fprintf(stderr, "Unknown load func3: 0x%02X\n", core->Func3);
            exit(1);
    }
}

static void execute_store(Rv32Core *core, RamDevice *ram) {
    // S-type: imm[11:5] + imm[4:0]
    int32_t imm = ((int32_t)((core->Instruction >> 25) & 0x7F) << 5) | ((int32_t)(core->Instruction >> 7) & 0x1F);
    if (imm & 0x800) imm |= 0xFFFFF000;

    uint32_t addr = core->X[core->Rs1] + (uint32_t)imm;
    uint32_t value = core->X[core->Rs2];

    switch (core->Func3) {
        case FUNC3_INSTR_SB:
            ram_write(ram, addr, value & 0xFF, 1);
            break;
        case FUNC3_INSTR_SH:
            ram_write(ram, addr, value & 0xFFFF, 2);
            break;
        case FUNC3_INSTR_SW:
            ram_write(ram, addr, value, 4);
            break;
        default:
            fprintf(stderr, "Unknown store func3: 0x%02X\n", core->Func3);
            exit(1);
    }
}

static void execute_branch(Rv32Core *core) {
    // B-type: inmediato de 12 bits
    int32_t jump_offset =
        (((int32_t)((core->Instruction >> 8) & 0xF)) << 1) |
        (((int32_t)((core->Instruction >> 25) & 0x3F)) << 5) |
        (((int32_t)((core->Instruction >> 7) & 0x1)) << 11) |
        (((int32_t)((core->Instruction >> 31) & 0x1)) << 12);

    if (jump_offset & 0x1000) jump_offset |= 0xFFFFE000; // sign extend

    bool taken = false;
    switch (core->Func3) {
        case FUNC3_INSTR_BEQ: taken = (core->X[core->Rs1] == core->X[core->Rs2]); break;
        case FUNC3_INSTR_BNE: taken = (core->X[core->Rs1] != core->X[core->Rs2]); break;
        case FUNC3_INSTR_BLT: taken = ((int32_t)core->X[core->Rs1] < (int32_t)core->X[core->Rs2]); break;
        case FUNC3_INSTR_BGE: taken = ((int32_t)core->X[core->Rs1] >= (int32_t)core->X[core->Rs2]); break;
        case FUNC3_INSTR_BLTU: taken = (core->X[core->Rs1] < core->X[core->Rs2]); break;
        case FUNC3_INSTR_BGEU: taken = (core->X[core->Rs1] >= core->X[core->Rs2]); break;
        default:
            fprintf(stderr, "Unknown branch func3: 0x%02X\n", core->Func3);
            exit(1);
    }

    if (taken) {
        core->Pc = (core->Pc - 4) + (uint32_t)jump_offset;
    }
}

static void execute_jal(Rv32Core *core) {
    // J-type: inmediato de 20 bits
    int32_t jump_offset =
        (((int32_t)((core->Instruction >> 21) & 0x3FF)) << 1) |
        (((int32_t)((core->Instruction >> 20) & 0x1)) << 11) |
        (((int32_t)((core->Instruction >> 12) & 0xFF)) << 12) |
        (((int32_t)((core->Instruction >> 31) & 0x1)) << 20);

    if (jump_offset & 0x100000) jump_offset |= 0xFFE00000; // sign extend

    core->X[core->Rd] = core->Pc; // link
    core->Pc = (core->Pc - 4) + (uint32_t)jump_offset;
}

static void execute_jalr(Rv32Core *core) {
    int32_t imm = (int32_t)((core->Instruction >> 20) & 0xFFF);
    if (imm & 0x800) imm |= 0xFFFFF000;

    core->X[core->Rd] = core->Pc; // link
    core->Pc = (core->X[core->Rs1] + (uint32_t)imm) & 0xFFFFFFFEu; // aligned to even
}

static void execute_lui(Rv32Core *core) {
    uint32_t imm = (core->Instruction >> 12) & 0xFFFFF;
    core->X[core->Rd] = imm << 12;
}

static void execute_auipc(Rv32Core *core) {
    uint32_t imm = (core->Instruction >> 12) & 0xFFFFF;
    core->X[core->Rd] = (core->Pc - 4) + (imm << 12);
}

static void execute_ecall(Rv32Core *core) {
    uint32_t syscall_num = core->X[17]; // a7
    switch (syscall_num) {
        case 44: // putchar
            putchar(core->X[10] & 0xFF); // a0
            break;
        case 45: // get microsecond (fake)
            core->X[10] = 0; // Placeholder
            break;
        default:
            fprintf(stderr, "Unhandled ECALL: %u\n", syscall_num);
            core->X[10] = 1;
            break;
    }
}

static void execute(Rv32Core *core, RamDevice *ram) {
    // x0 always zero
    core->X[XREG_ZERO] = 0;

    switch (core->Opcode) {
        case INSTR_ADD_SUB_SLL_SLT_SLTU_XOR_SRL_SRA_OR_AND:
            execute_rtype(core);
            break;

        case INSTR_JALR:
            execute_jalr(core);
            break;

        case INSTR_ADDI_SLTI_SLTIU_XORI_ORI_ANDI_SLLI_SRLI_SRAI:
            execute_itype_arith(core);
            break;

        case INSTR_LB_LH_LW_LBU_LHU:
            execute_load(core, ram);
            break;

        case INSTR_SB_SH_SW:
            execute_store(core, ram);
            break;

        case INSTR_BEQ_BNE_BLT_BGE_BLTU_BGEU:
            execute_branch(core);
            break;

        case INSTR_LUI:
            execute_lui(core);
            break;

        case INSTR_AUIPC:
            execute_auipc(core);
            break;

        case INSTR_JAL:
            execute_jal(core);
            break;

        case INSTR_ECALL_EBREAK_CSRRW_CSRRS_CSRRC_CSRRWI_CSRRSI_CSRRCI:
            execute_ecall(core);
            break;

        case INSTR_FENCE_FENCE_I:
            // Ignorar
            break;

        default:
            fprintf(stderr, "Unknown opcode: 0x%02X (instr: 0x%08X)\n", core->Opcode, core->Instruction);
            exit(1);
    }

    // Asegurar que x0 sea 0
    core->X[XREG_ZERO] = 0;
}

static void step(Rv32Core *core, RamDevice *ram) {
    fetch(core, ram);
    decode(core);
    execute(core, ram);
}

// =============================================
// Main
// =============================================

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <programa.bin>\n", argv[0]);
        return 1;
    }

    // Inicializar RAM
    const size_t RAM_SIZE = 62000;
    uint8_t *ram_data = malloc(RAM_SIZE);
    if (!ram_data) {
        fprintf(stderr, "No se pudo asignar memoria RAM.\n");
        return 1;
    }
    memset(ram_data, 0, RAM_SIZE);

    RamDevice ram = {
        .ram = ram_data,
        .base_addr = BASE_RAM,
        .size_bytes = RAM_SIZE
    };

    // Cargar programa
    if (load_program(&ram, argv[1]) != 0) {
        free(ram_data);
        return 1;
    }

    // Inicializar core
    Rv32Core core = {
        .Pc = PROGRAM_COUNTER_START_VAL,
        .X = {0}
    };

    printf("Emulador RISC-V iniciado.\n");

    // Ciclo principal
    while (1) {
        step(&core, &ram);
    }

    free(ram_data);
    return 0;
}