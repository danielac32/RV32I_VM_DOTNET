
public class Rv32Core
{
    public uint[] X { get; set; } = new uint[32]; // Registros x0-x31
    public uint Pc { get; set; } // Program Counter

    // Campos temporales
    public uint Instruction { get; private set; }
    public byte Opcode { get; private set; }
    public byte Rd, Rs1, Rs2;
    public byte Func3, Func7;
    public uint Immediate;
    public int JumpOffset;

    // Acciones (equivalente a punteros de función)
    public Action ExecuteAction;

    // Memoria externa (sin punteros)
    public IMemoryDevice Memory;

    public const int NR_RV32I_REGS = 32;
    public const int XREG_ZERO = 0;
    public const int XREG_RETURN_ADDRESS = 0;
    public const int XREG_STACK_POINTER = 2;

    public const uint STACK_POINTER_START_VAL = 0x0; //(4*NR_RAM_WORDS)
    public const uint BASE_RAM = 0x80000000;
    public const uint PROGRAM_COUNTER_START_VAL = BASE_RAM;

    /* R-Type Instructions */
    public const byte INSTR_ADD_SUB_SLL_SLT_SLTU_XOR_SRL_SRA_OR_AND = 0x33;
    public const byte FUNC3_INSTR_ADD_SUB = 0x0;
    public const byte FUNC7_INSTR_ADD = 0x00;
    public const byte FUNC7_INSTR_SUB = 0x20;
    public const byte FUNC3_INSTR_SLL = 0x1;
    public const byte FUNC7_INSTR_SLL = 0x00;
    public const byte FUNC3_INSTR_SLT = 0x2;
    public const byte FUNC7_INSTR_SLT = 0x00;
    public const byte FUNC3_INSTR_SLTU = 0x3;
    public const byte FUNC7_INSTR_SLTU = 0x00;
    public const byte FUNC3_INSTR_XOR = 0x4;
    public const byte FUNC7_INSTR_XOR = 0x00;
    public const byte FUNC3_INSTR_SRL_SRA = 0x5;
    public const byte FUNC7_INSTR_SRL = 0x00;
    public const byte FUNC7_INSTR_SRA = 0x20;
    public const byte FUNC3_INSTR_OR = 0x6;
    public const byte FUNC7_INSTR_OR = 0x00;
    public const byte FUNC3_INSTR_AND = 0x7;
    public const byte FUNC7_INSTR_AND = 0x00;

    /* I-Type Instructions */
    public const byte INSTR_JALR = 0x67;
    public const byte FUNC3_INSTR_JALR = 0x0;

    public const byte INSTR_ADDI_SLTI_SLTIU_XORI_ORI_ANDI_SLLI_SRLI_SRAI = 0x13;
    public const byte FUNC3_INSTR_ADDI = 0x0;
    public const byte FUNC3_INSTR_SLTI = 0x2;
    public const byte FUNC3_INSTR_SLTIU = 0x3;
    public const byte FUNC3_INSTR_XORI = 0x4;
    public const byte FUNC3_INSTR_ORI = 0x6;
    public const byte FUNC3_INSTR_ANDI = 0x7;
    public const byte FUNC3_INSTR_SLLI = 0x1;
    public const byte FUNC3_INSTR_SRLI_SRAI = 0x5;

    public const byte INSTR_LB_LH_LW_LBU_LHU = 0x03;
    public const byte FUNC3_INSTR_LB = 0x0;
    public const byte FUNC3_INSTR_LH = 0x1;
    public const byte FUNC3_INSTR_LW = 0x2;
    public const byte FUNC3_INSTR_LBU = 0x4;
    public const byte FUNC3_INSTR_LHU = 0x5;

    /* S-Type Instructions */
    public const byte INSTR_SB_SH_SW = 0x23;
    public const byte FUNC3_INSTR_SB = 0x0;
    public const byte FUNC3_INSTR_SH = 0x1;
    public const byte FUNC3_INSTR_SW = 0x2;

    /* B-Type Instructions */
    public const byte INSTR_BEQ_BNE_BLT_BGE_BLTU_BGEU = 0x63;
    public const byte FUNC3_INSTR_BEQ = 0x0;
    public const byte FUNC3_INSTR_BNE = 0x1;
    public const byte FUNC3_INSTR_BLT = 0x4;
    public const byte FUNC3_INSTR_BGE = 0x5;
    public const byte FUNC3_INSTR_BLTU = 0x6;
    public const byte FUNC3_INSTR_BGEU = 0x7;

    /* U-Type Instructions */
    public const byte INSTR_LUI = 0x37;   /* LOAD UPPER IMMEDIATE INTO DESTINATION REGISTER */
    public const byte INSTR_AUIPC = 0x17; /* ADD UPPER IMMEDIATE TO PROGRAM COUNTER */

    /* J-Type Instructions */
    public const byte INSTR_JAL = 0x6F;   /* JUMP and Link */

    /* System level instructions */
    public const byte INSTR_FENCE_FENCE_I = 0x0F;
    public const byte FUNC3_INSTR_FENCE = 0x0;
    public const byte FUNC3_INSTR_FENCE_I = 0x1;
    public const byte INSTR_ECALL_EBREAK_CSRRW_CSRRS_CSRRC_CSRRWI_CSRRSI_CSRRCI = 0x73;



private void DebugInstruction(string stage)
{
    string instrName = GetInstructionName();
    Console.WriteLine($"[{stage}] PC=0x{Pc - 4:X8} | Instr=0x{Instruction:X8} | {instrName}");
    Console.WriteLine($"    Opcode=0x{Opcode:X2} | Rd=x{Rd} | Rs1=x{Rs1} | Rs2=x{Rs2} | Func3=0x{Func3:X1} | Func7=0x{Func7:X2}");
    Console.WriteLine($"    Imm=0x{Immediate:X} | JumpOffset={JumpOffset} | sp=0x{X[2]:X8}");
    
    // Mostrar valores de registros involucrados
    if (Rs1 < 32) Console.WriteLine($"    x{Rs1} (rs1) = 0x{X[Rs1]:X8}");
    if (Rs2 < 32) Console.WriteLine($"    x{Rs2} (rs2) = 0x{X[Rs2]:X8}");
    if (Rd < 32 && stage == "AFTER") Console.WriteLine($"    x{Rd} (rd)  = 0x{X[Rd]:X8}");
    Console.WriteLine();
}

private string GetInstructionName()
{
    switch (Opcode)
    {
        case INSTR_LUI: return "LUI";
        case INSTR_AUIPC: return "AUIPC";
        case INSTR_JAL: return "JAL";
        case INSTR_JALR: return "JALR";
        case INSTR_BEQ_BNE_BLT_BGE_BLTU_BGEU:
            return Func3 switch
            {
                FUNC3_INSTR_BEQ => "BEQ",
                FUNC3_INSTR_BNE => "BNE",
                FUNC3_INSTR_BLT => "BLT",
                FUNC3_INSTR_BGE => "BGE",
                FUNC3_INSTR_BLTU => "BLTU",
                FUNC3_INSTR_BGEU => "BGEU",
                _ => "B-???"
            };
        case INSTR_LB_LH_LW_LBU_LHU:
            return Func3 switch
            {
                FUNC3_INSTR_LB => "LB",
                FUNC3_INSTR_LH => "LH",
                FUNC3_INSTR_LW => "LW",
                FUNC3_INSTR_LBU => "LBU",
                FUNC3_INSTR_LHU => "LHU",
                _ => "L-???"
            };
        case INSTR_SB_SH_SW:
            return Func3 switch
            {
                FUNC3_INSTR_SB => "SB",
                FUNC3_INSTR_SH => "SH",
                FUNC3_INSTR_SW => "SW",
                _ => "S-???"
            };
        case INSTR_ADDI_SLTI_SLTIU_XORI_ORI_ANDI_SLLI_SRLI_SRAI:
            return Func3 switch
            {
                FUNC3_INSTR_ADDI => "ADDI",
                FUNC3_INSTR_SLTI => "SLTI",
                FUNC3_INSTR_SLTIU => "SLTIU",
                FUNC3_INSTR_XORI => "XORI",
                FUNC3_INSTR_ORI => "ORI",
                FUNC3_INSTR_ANDI => "ANDI",
                FUNC3_INSTR_SLLI => "SLLI",
                FUNC3_INSTR_SRLI_SRAI => Func7 == FUNC7_INSTR_SRA ? "SRAI" : "SRLI",
                _ => "I-???"
            };
        case INSTR_ADD_SUB_SLL_SLT_SLTU_XOR_SRL_SRA_OR_AND:
            return Func3 switch
            {
                FUNC3_INSTR_ADD_SUB => Func7 == FUNC7_INSTR_SUB ? "SUB" : "ADD",
                FUNC3_INSTR_SLL => "SLL",
                FUNC3_INSTR_SLT => "SLT",
                FUNC3_INSTR_SLTU => "SLTU",
                FUNC3_INSTR_XOR => "XOR",
                FUNC3_INSTR_SRL_SRA => Func7 == FUNC7_INSTR_SRA ? "SRA" : "SRL",
                FUNC3_INSTR_OR => "OR",
                FUNC3_INSTR_AND => "AND",
                _ => "R-???"
            };
        case INSTR_ECALL_EBREAK_CSRRW_CSRRS_CSRRC_CSRRWI_CSRRSI_CSRRCI:
            return "ECALL";
        case INSTR_FENCE_FENCE_I:
            return "FENCE";
        default:
            return "UNKNOWN";
    }
}

    public void Step()
    {
        Fetch();
        Decode();
        Execute();
    }
    public void Fetch()
    {
        Instruction = Memory.Read(Pc);
       // Console.WriteLine($"pc: 0x{Pc:X8} Op: 0x{Instruction:X8}");
        Pc += 4;
    }

    public void Decode()
    {
        Opcode = (byte)(Instruction & 0x7F);
        Rd = (byte)((Instruction >> 7) & 0x1F);
        Func3 = (byte)((Instruction >> 12) & 0x7);
        Rs1 = (byte)((Instruction >> 15) & 0x1F);
        Rs2 = (byte)((Instruction >> 20) & 0x1F);
        Func7 = (byte)((Instruction >> 25) & 0x7F);
    }
    public void Execute()
    {
        // x0 siempre debe ser 0
        X[0] = 0;
       // DebugInstruction("BEFORE");
        switch (Opcode)
        {
            // ========== R-Type ==========
            case INSTR_ADD_SUB_SLL_SLT_SLTU_XOR_SRL_SRA_OR_AND:
                ExecuteRType();
                break;

            // ========== I-Type ==========
            case INSTR_JALR:
                ExecuteJALR();
                break;

            case INSTR_ADDI_SLTI_SLTIU_XORI_ORI_ANDI_SLLI_SRLI_SRAI:
                ExecuteITypeArith();
                break;

            case INSTR_LB_LH_LW_LBU_LHU:
                ExecuteLoad();
                break;

            // ========== S-Type ==========
            case INSTR_SB_SH_SW:
                ExecuteStore();
                break;

            // ========== B-Type ==========
            case INSTR_BEQ_BNE_BLT_BGE_BLTU_BGEU:
                ExecuteBranch();
                break;

            // ========== U-Type ==========
            case INSTR_LUI:
                uint imm = (Instruction >> 12) & 0xFFFFF;
                X[Rd] = imm << 12;
                break;

            case INSTR_AUIPC:
                imm = (Instruction >> 12) & 0xFFFFF;
                X[Rd] = (Pc - 4) + (imm << 12);
                break;

            // ========== J-Type ==========
            case INSTR_JAL:
                ExecuteJAL();
                break;

            // ========== System ==========
            case INSTR_ECALL_EBREAK_CSRRW_CSRRS_CSRRC_CSRRWI_CSRRSI_CSRRCI:
                ExecuteECALL();
                break;

            case INSTR_FENCE_FENCE_I:
                // Ignorar por ahora
                break;

            default:
                throw new InvalidOperationException($"Unknown opcode: 0x{Opcode:X2} (instrucción: 0x{Instruction:X8})");
        }
        // Asegurar que x0 siempre sea 0 después de cualquier instrucción
        X[0] = 0;
       /// DebugInstruction("AFTER");
    }

        private void ExecuteRType()
    {
        switch (Func3)
        {
            case FUNC3_INSTR_ADD_SUB:
                if (Func7 == FUNC7_INSTR_SUB)
                    X[Rd] = X[Rs1] - X[Rs2];
                else // ADD
                    X[Rd] = X[Rs1] + X[Rs2];
                break;

            case FUNC3_INSTR_SLL:
                X[Rd] = X[Rs1] << (int)(X[Rs2] & 0x1F);
                break;

            case FUNC3_INSTR_SLT:
                X[Rd] = (int)X[Rs1] < (int)X[Rs2] ? 1u : 0u;
                break;

            case FUNC3_INSTR_SLTU:
                X[Rd] = X[Rs1] < X[Rs2] ? 1u : 0u;
                break;

            case FUNC3_INSTR_XOR:
                X[Rd] = X[Rs1] ^ X[Rs2];
                break;

            case FUNC3_INSTR_SRL_SRA:
                if (Func7 == FUNC7_INSTR_SRA)
                    X[Rd] = (uint)((int)X[Rs1] >> (int)(X[Rs2] & 0x1F));
                else // SRL
                    X[Rd] = X[Rs1] >> (int)(X[Rs2] & 0x1F);
                break;

            case FUNC3_INSTR_OR:
                X[Rd] = X[Rs1] | X[Rs2];
                break;

            case FUNC3_INSTR_AND:
                X[Rd] = X[Rs1] & X[Rs2];
                break;

            default:
                throw new InvalidOperationException($"Unknown R-type func3: 0x{Func3:X2}");
        }
    }

    private void ExecuteITypeArith()
    {
        // Sign-extend inmediato de 12 bits
        int imm = (int)((Instruction >> 20) & 0xFFF);
        if ((imm & 0x800) != 0) imm |= unchecked((int)0xFFFFF000);

        switch (Func3)
        {
            case FUNC3_INSTR_ADDI:
                X[Rd] = X[Rs1] + (uint)imm;
                break;

            case FUNC3_INSTR_SLTI:
                X[Rd] = (int)X[Rs1] < imm ? 1u : 0u;
                break;

            case FUNC3_INSTR_SLTIU:
                X[Rd] = X[Rs1] < (uint)imm ? 1u : 0u;
                break;

            case FUNC3_INSTR_XORI:
                X[Rd] = X[Rs1] ^ (uint)imm;
                break;

            case FUNC3_INSTR_ORI:
                X[Rd] = X[Rs1] | (uint)imm;
                break;

            case FUNC3_INSTR_ANDI:
                X[Rd] = X[Rs1] & (uint)imm;
                break;

            case FUNC3_INSTR_SLLI:
                X[Rd] = X[Rs1] << (imm & 0x1F);
                break;

            case FUNC3_INSTR_SRLI_SRAI:
                if (Func7 == FUNC7_INSTR_SRA)
                    X[Rd] = (uint)((int)X[Rs1] >> imm);
                else // SRLI
                    X[Rd] = X[Rs1] >> imm;
                break;

            default:
                throw new InvalidOperationException($"Unknown I-type func3: 0x{Func3:X2}");
        }
    }

    private void ExecuteLoad()
    {
        int imm = (int)((Instruction >> 20) & 0xFFF);
        if ((imm & 0x800) != 0) imm |= unchecked((int)0xFFFFF000);

        uint addr = X[Rs1] + (uint)imm;

        switch (Func3)
        {
            case FUNC3_INSTR_LB:
                sbyte b = (sbyte)(Memory.Read(addr) & 0xFF);
                X[Rd] = (uint)b;
                break;

            case FUNC3_INSTR_LH:
                short h = (short)(Memory.Read(addr) & 0xFFFF);
                X[Rd] = (uint)h;
                break;

            case FUNC3_INSTR_LW:
                X[Rd] = Memory.Read(addr);
                break;

            case FUNC3_INSTR_LBU:
                X[Rd] = Memory.Read(addr) & 0xFF;
                break;

            case FUNC3_INSTR_LHU:
                X[Rd] = Memory.Read(addr) & 0xFFFF;
                break;

            default:
                throw new InvalidOperationException($"Unknown load func3: 0x{Func3:X2}");
        }
    }

    private void ExecuteStore()
    {
        // S-type: inmediato = imm[11:5] + imm[4:0]
        int imm = (int)(((Instruction >> 25) << 5) | ((Instruction >> 7) & 0x1F));
        if ((imm & 0x800) != 0) imm |= unchecked((int)0xFFFFF000);

        uint addr = X[Rs1] + (uint)imm;
        uint value = X[Rs2];

        switch (Func3)
        {
            case FUNC3_INSTR_SB:
                Memory.Write(addr, value & 0xFF, 1);
                break;

            case FUNC3_INSTR_SH:
                Memory.Write(addr, value & 0xFFFF, 2);
                break;

            case FUNC3_INSTR_SW:
                Memory.Write(addr, value, 4);
                break;

            default:
                throw new InvalidOperationException($"Unknown store func3: 0x{Func3:X2}");
        }
    }

    private void ExecuteBranch()
    {

        
        // B-type: inmediato de 12 bits (codificado en varios campos)
        JumpOffset = ((int)((Instruction >> 8) & 0xF) << 1) |    // bits 4:1
                     ((int)((Instruction >> 25) & 0x3F) << 5) |  // bits 10:5
                     ((int)((Instruction >> 7) & 0x1) << 11) |   // bit 11
                     ((int)((Instruction >> 31) & 0x1) << 12);   // bit 12 (signo)

        if ((JumpOffset & 0x1000) != 0) // sign extend
            JumpOffset |= unchecked((int)0xFFFFE000);

        bool taken = false;
        switch (Func3)
        {
            case FUNC3_INSTR_BEQ:
                taken = X[Rs1] == X[Rs2];
                break;
            case FUNC3_INSTR_BNE:
                taken = X[Rs1] != X[Rs2];
                break;
            case FUNC3_INSTR_BLT:
                taken = (int)X[Rs1] < (int)X[Rs2];
                break;
            case FUNC3_INSTR_BGE:
                taken = (int)X[Rs1] >= (int)X[Rs2];
                break;
            case FUNC3_INSTR_BLTU:
                taken = X[Rs1] < X[Rs2];
                break;
            case FUNC3_INSTR_BGEU:
                taken = X[Rs1] >= X[Rs2];
                break;
            default:
                throw new InvalidOperationException($"Unknown branch func3: 0x{Func3:X2}");
        }

        if (taken)
            Pc = (Pc - 4) + (uint)JumpOffset; // -4 porque ya avanzamos en Fetch
    }

    private void ExecuteJAL()
    {
        // J-type: inmediato de 20 bits
        JumpOffset = ((int)((Instruction >> 21) & 0x3FF) << 1) |
                     ((int)((Instruction >> 20) & 0x1) << 11) |
                     ((int)((Instruction >> 12) & 0xFF) << 12) |
                     ((int)((Instruction >> 31) & 0x1) << 20);

        if ((JumpOffset & 0x100000) != 0) // sign extend
            JumpOffset |= unchecked((int)0xFFE00000);

        X[Rd] = Pc; // link
        Pc = (Pc - 4) + (uint)JumpOffset;
    }

    private void ExecuteJALR()
    {
        int imm = (int)((Instruction >> 20) & 0xFFF);
        if ((imm & 0x800) != 0) imm |= unchecked((int)0xFFFFF000);

        X[Rd] = Pc; // link
        Pc = (X[Rs1] + (uint)imm) & 0xFFFFFFFEu; // alineado a par
    }

    private void ExecuteECALL()
    {
        uint syscall_num = X[17]; // a7

        switch (syscall_num)
        {
            case 44: // putchar (asumiendo tu syscall 44)
                char c = (char)(X[10] & 0xFF); // a0
                Console.Write(c);
                break;

            default:
                Console.WriteLine($"Unhandled ECALL: {syscall_num}");
                break;
        }
    }


}

