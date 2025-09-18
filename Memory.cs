public class RamDevice : IMemoryDevice
{
    private readonly byte[] _ram;
    private readonly uint _baseRam;
    private const uint UartAddress = 0x300000;

    public RamDevice(uint baseAddr, int sizeBytes = 60000)
    {
        _ram = new byte[sizeBytes];
        _baseRam = baseAddr;
    }

    public uint Read(uint address)
    {
        if (address == UartAddress)
        {
            return 0;
        }

        if (address >= _baseRam && address < _baseRam + _ram.Length)
        {
            uint offset = address - _baseRam;
            
            // FORMA CORRECTA: Little-endian (LSB first)
            return (uint)_ram[offset] |
                   ((uint)_ram[offset + 1] << 8) |
                   ((uint)_ram[offset + 2] << 16) |
                   ((uint)_ram[offset + 3] << 24);
        }

        Console.WriteLine($"Error: Lectura en dirección inválida 0x{address:X}");
        return 0;
    }

    public void Write(uint address, uint value, int width)
    {
        if (address == UartAddress)
        {
            Console.Write((char)(value & 0xFF));
            return;
        }

        if (address < _baseRam || address + width > _baseRam + _ram.Length)
        {
            Console.WriteLine($"Error: Escritura inválida 0x{address:X}");
            return;
        }

        uint offset = address - _baseRam;

        switch (width)
        {
            case 4:
                // FORMA CORRECTA: Little-endian (LSB first)
                _ram[offset] = (byte)(value);
                _ram[offset + 1] = (byte)(value >> 8);
                _ram[offset + 2] = (byte)(value >> 16);
                _ram[offset + 3] = (byte)(value >> 24);
                break;

            case 2:
                _ram[offset] = (byte)(value);
                _ram[offset + 1] = (byte)(value >> 8);
                break;

            case 1:
                _ram[offset] = (byte)(value);
                break;

            default:
                Console.WriteLine($"Ancho no soportado: {width}");
                break;
        }
    }

    public void LoadProgram(string filePath)
    {
        byte[] program = File.ReadAllBytes(filePath);
        if (program.Length > _ram.Length)
            throw new InvalidOperationException("El programa es demasiado grande para la RAM.");

        Array.Copy(program, _ram, program.Length);
        Console.WriteLine($"Programa cargado: {program.Length} bytes desde {filePath}");
    }
}