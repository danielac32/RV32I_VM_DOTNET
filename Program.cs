using System;
using System.Collections.Generic;
using System.IO;

namespace RiscVEmulator;








public class Rv32Emulator
{
    public const int NR_RV32I_REGS = 32;
    public const int XREG_ZERO = 0;
    public const int XREG_RETURN_ADDRESS = 0;
    public const int XREG_STACK_POINTER = 2;

    public const int NR_RAM_WORDS = 4096;
    public const int NR_ROM_WORDS = 4096;

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




    public static void Main(string[] args)
    {

        var ram = new RamDevice();

        // 2. Cargar el programa (por ejemplo, un binario RISC-V)
        try
        {
            ram.LoadProgram(args[0]);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error al cargar el programa: {ex.Message}");
            return;
        }

        // 3. Crear el núcleo y asignar la memoria
        var rv32_core = new Rv32Core
        {
            Pc = 0x80000000,           // Dirección de inicio
            Memory = ram               // Asignar el dispositivo de memoria
        };

        byte data = (byte)rv32_core.Memory.Read(0x80000000);
        Console.WriteLine("0x{0:X}",data);
        // 4. Ejecutar el ciclo principal


        Console.WriteLine("=== Prueba de memoria ===");

        // 3. Escribir valor en RAM (dirección 0x80000000)
        uint testValue = 0x12345678;
        uint address = 0x80000000;
        rv32_core.Memory.Write(address, testValue, 4); // write word

        // 4. Leerlo de vuelta
        uint readValue = rv32_core.Memory.Read(address);

        // 5. Verificar
        Console.WriteLine($"Escrito: 0x{testValue:X8}");
        Console.WriteLine($"Leído:    0x{readValue:X8}");
        Console.WriteLine($"¿Coinciden? {testValue == readValue}");

        // 6. Probar UART (escribir un carácter)
        Console.WriteLine("\n=== Prueba de UART ===");
        rv32_core.Memory.Write(0x300000, 'H', 1); // H
        rv32_core.Memory.Write(0x300000, 'o', 1); // o
        rv32_core.Memory.Write(0x300000, 'l', 1); // l
        rv32_core.Memory.Write(0x300000, 'a', 1); // a
        Console.WriteLine(); // salto de línea

        // 7. Intentar leer de UART (debería ignorar)
        uint uartRead = rv32_core.Memory.Read(0x300000);
        Console.WriteLine($"Lectura de UART: 0x{uartRead:X} (debe ser 0)");


        Console.WriteLine("Emulador RISC-V iniciado.");
    }
}