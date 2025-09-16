using System;
using System.Collections.Generic;
using System.IO;

namespace RiscVEmulator;








public class Rv32Emulator
{

    
    public static void Main(string[] args)
    {

        var ram = new RamDevice(Rv32Core.BASE_RAM,62000);

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
            Pc = Rv32Core.PROGRAM_COUNTER_START_VAL,           // Dirección de inicio
            Memory = ram               // Asignar el dispositivo de memoria
        };
 

        Console.WriteLine("Emulador RISC-V iniciado.");
        while (true)
        {
            rv32_core.Step();
        }
    }
}