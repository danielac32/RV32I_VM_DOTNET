

public class RamDevice : IMemoryDevice
{
    private readonly byte[] _ram;
    private uint BaseRam;// = 0x80000000;
    private const uint UartAddress = 0x300000;

    public RamDevice(uint baseAddr,int sizeBytes = 60000)
    {
        _ram = new byte[sizeBytes];
        BaseRam = baseAddr;
    }


    
    public uint Read(uint address)
    {
        if (address == UartAddress)
        {
            return 0;
        }

        if (address >= BaseRam && address < BaseRam + _ram.Length)
        {
            uint offset = (address - BaseRam);
            /*return ((uint)_ram[offset+3] <<24) | 
                   ((uint)_ram[offset+2] << 16) |
                   ((uint)_ram[offset+1] << 8)|
                   (_ram[offset+0]);*/
                   return ( _ram[offset+ 0]) | 
                   ((uint)_ram[offset+1] << 8) |
                   ((uint)_ram[offset+2] << 16)|
                   ((uint)_ram[offset+3] << 24);
             
        }

        Console.WriteLine($"Error: Lectura en dirección inválida 0x{address:X}");
        return 0;
    }
  
 

 
public void Write(uint address, uint value, int width)
{
    if (address == 0x300000) // UART
    {
        Console.Write((char)(value & 0xFF));
        return;
    }

    if (address < BaseRam || address + width > BaseRam + _ram.Length)
    {
        Console.WriteLine($"Error: Escritura inválida 0x{address:X}");
        return;
    }

    uint offset =  (address - BaseRam);

    switch (width)
    {
        case 4:
            _ram[offset + 3] = (byte)(value >> 24);
            _ram[offset + 2] = (byte)(value >> 16);
            _ram[offset + 1] = (byte)(value >> 8);
            _ram[offset + 0] = (byte)value;
            break;

        case 2:
            _ram[offset + 1] = (byte)(value >> 8);
            _ram[offset + 0] = (byte)value;
            break;

        case 1:
            _ram[offset + 0] = (byte)value;
            break;

        default:
            Console.WriteLine($"Ancho no soportado: {width}");
            break;
    }
}
 /*
    public void Write(uint address, uint value, int width)
    {
        if (address == UartAddress)
        {
            Console.Write((char)(value & 0xFF)); // Salida UART
            return;
        }

        if (address >= BaseRam && address + width <= BaseRam + _ram.Length)
        {
            int offset = (int)(address - BaseRam);
            for (int i = 0; i < width; i++)
            {
                _ram[offset + i] = (byte)((value >> (8 * i)) & 0xFF);
            }
            return;
        }

        Console.WriteLine($"Error: Escritura en dirección inválida 0x{address:X}");
    }
*/
    // Método para cargar el programa
    public void LoadProgram(string filePath)
    {
        byte[] program = File.ReadAllBytes(filePath);
        if (program.Length > _ram.Length)
            throw new InvalidOperationException("El programa es demasiado grande para la RAM.");

        Array.Copy(program, _ram, program.Length);
        Console.WriteLine($"Programa cargado: {program.Length} bytes desde {filePath}");
    }
}