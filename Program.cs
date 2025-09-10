using System;
using System.Collections.Generic;
using System.IO;

namespace RiscVEmulator;








public class Rv32Emulator
{

    
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
            Pc = Rv32Core.PROGRAM_COUNTER_START_VAL,           // Dirección de inicio
            Memory = ram               // Asignar el dispositivo de memoria
        };
        
        rv32_core.X[Rv32Core.XREG_STACK_POINTER] = Rv32Core.STACK_POINTER_START_VAL;

        /* byte data = (byte)rv32_core.Memory.Read(0x80000000);
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

 */



        Console.WriteLine("Emulador RISC-V iniciado.");
        while (true)
        {
            rv32_core.Step();
        }
    }
}