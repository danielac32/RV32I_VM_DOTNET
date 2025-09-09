
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
}

