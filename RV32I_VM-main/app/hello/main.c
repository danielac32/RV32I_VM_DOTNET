
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>   
#include <stdio.h>

extern int kputc(int c);



typedef uint32_t uint32;
typedef int32_t int32;

void test_basic_operations() {
    printf("=== Test: Operaciones Basicas ===\n");
    
    uint32 a = 15;
    uint32 b = 7;
    
    printf("a = %u, b = %u\n", a, b);
    printf("ADD: a + b = %u\n", a + b);
    printf("SUB: a - b = %u\n", a - b);
    printf("AND: a & b = 0x%08x\n", a & b);
    printf("OR:  a | b = 0x%08x\n", a | b);
    printf("XOR: a ^ b = 0x%08x\n", a ^ b);
    printf("\n");
}

void test_shift_operations() {
    printf("=== Test: Operaciones de Desplazamiento ===\n");
    
    uint32 value = 0x12345678;
    int32 signed_value = (int32)value;
    
    printf("Valor original: 0x%08x\n", value);
    printf("SLL << 4: 0x%08x\n", value << 4);
    printf("SRL >> 4: 0x%08x\n", value >> 4);
    printf("SRA >> 4 (con signo): 0x%08x\n", signed_value >> 4);
    printf("\n");
}

void test_comparison_operations() {
    printf("=== Test: Operaciones de Comparacion ===\n");
    
    uint32 a = 10;
    uint32 b = 20;
    
    printf("a = %u, b = %u\n", a, b);
    printf("a == b: %s\n", (a == b) ? "true" : "false");
    printf("a != b: %s\n", (a != b) ? "true" : "false");
    printf("a < b:  %s\n", (a < b) ? "true" : "false");
    printf("a >= b: %s\n", (a >= b) ? "true" : "false");
    printf("a <= b: %s\n", (a <= b) ? "true" : "false");
    printf("a > b:  %s\n", (a > b) ? "true" : "false");
    printf("\n");
}

void test_memory_operations() {
    printf("=== Test: Operaciones de Memoria ===\n");
    
    uint32 stack_array[8];
    uint32 sum = 0;
    
    printf("Escribiendo en array...\n");
    for (uint32 i = 0; i < 8; i++) {
        stack_array[i] = i * 10;
        printf("array[%u] = %u\n", i, stack_array[i]);
    }
    
    printf("Leyendo y verificando...\n");
    for (uint32 i = 0; i < 8; i++) {
        if (stack_array[i] == i * 10) {
            sum += stack_array[i];
            printf("array[%u] OK: %u\n", i, stack_array[i]);
        } else {
            printf("ERROR en array[%u]\n", i);
        }
    }
    
    printf("Suma total: %u\n", sum);
    printf("\n");
}

void test_control_flow() {
    printf("=== Test: Flujo de Control ===\n");
    
    uint32 result = 0;
    
    printf("Probando bucles...\n");
    for (uint32 i = 0; i < 5; i++) {
        result += i;
        printf("i=%u, result=%u\n", i, result);
    }
    
    printf("Probando condicionales anidados...\n");
    for (uint32 i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            if (i % 4 == 0) {
                result += i * 2;
                printf("i=%u (multiplo de 4): +%u\n", i, i * 2);
            } else {
                result += i;
                printf("i=%u (par): +%u\n", i, i);
            }
        } else {
            result -= i;
            printf("i=%u (impar): -%u\n", i, i);
        }
    }
    
    printf("Resultado final: %u\n", result);
    printf("\n");
}

uint32 recursive_sum(uint32 n) {
    if (n == 0) {
        printf("recursive_sum(0) = 0\n");
        return 0;
    }
    uint32 result = n + recursive_sum(n - 1);
    printf("recursive_sum(%u) = %u\n", n, result);
    return result;
}

void test_function_calls() {
    printf("=== Test: Llamadas a Funciones ===\n");
    
    printf("Probando recursion...\n");
    uint32 result = recursive_sum(5);  // 5 + 4 + 3 + 2 + 1 = 15
    printf("Resultado recursive_sum(5): %u\n", result);
    printf("\n");
}

void test_immediate_operations() {
    printf("=== Test: Operaciones con Inmediatos ===\n");
    
    printf("ADDI: 0x1000 + 0x2000 = 0x%08x\n", 0x1000 + 0x2000);
    printf("ANDI: 0x1000 & 0x00FF = 0x%08x\n", 0x1000 & 0x00FF);
    printf("ORI:  0x1000 | 0x00FF = 0x%08x\n", 0x1000 | 0x00FF);
    printf("XORI: 0x1000 ^ 0x00FF = 0x%08x\n", 0x1000 ^ 0x00FF);
    printf("\n");
}

void test_pointer_operations() {
    printf("=== Test: Operaciones con Punteros ===\n");
    
    uint32 data = 0xABCD1234;
    uint32* ptr = &data;
    
    printf("Direccion de data: 0x%08x\n", (uint32)ptr);
    printf("Valor de data: 0x%08x\n", data);
    printf("Valor via puntero: 0x%08x\n", *ptr);
    
    // Modificar via puntero
    *ptr = 0xDEADBEEF;
    printf("Nuevo valor de data: 0x%08x\n", data);
    printf("\n");
}




int count=77;
	char buf[20];
int main(int argc, char const *argv[])
{



    


    uitoa(458,buf,sizeof buf,10);

    printf("%s\n",buf );
    sprintf(buf,"numero: %d\n",count);
    printf("%s\n",buf );
	strcpy(buf,"hola como estas");
    

    kputc('A');  // Debería imprimir 'A'
    kputc('\n');
    
    // Prueba números
    printf("Testing numbers:\n");
    for(int i = 0; i < 10; i++) {
        printf("%d\n", i);
    }
    printf("\n");
    
    // Prueba strings
    printf("Testing strings:\n");
    printf("Simple string\n");
    printf("Number: %d\n", 1234);



	printf("holaaa: %d\n",44);

	printf("%s\n", buf);

    // Prueba 1: kputc
    kputc('H');
    kputc('e');
    kputc('l');
    kputc('l');
    kputc('o');
    kputc('!');
    kputc('\n');


    const int height = 8;  // Altura de la pirámide

    printf("Piramide de asteriscos (altura = %d):\n\n", height);

    for (int i = 1; i <= height; i++)
    {
        // Espacios iniciales
        for (int j = 0; j < height - i; j++)
            printf(" ");

        // Asteriscos
        for (int k = 0; k < 2 * i - 1; k++)
            printf("*");

        // Salto de línea
        printf("\n");
    }

    printf("\nPiramide completada!\n");

printf("========================================\n");
    printf("  TEST RISC-V 32I \n");
    printf("========================================\n\n");
    
    test_basic_operations();
    test_shift_operations();
    test_comparison_operations();
    test_memory_operations();
    test_control_flow();
    test_function_calls();
    test_immediate_operations();
    test_pointer_operations();
    
    printf("========================================\n");
    printf("  TODAS LAS PRUEBAS COMPLETADAS\n");
    printf("========================================\n");


	return 0;
}