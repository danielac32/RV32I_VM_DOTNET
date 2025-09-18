
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>   
#include <stdio.h>
#include <memory.h>
#include <syscall.h>
#include <math.h>
struct  memblk  memlist;    /* List of free memory blocks       */


extern int kputc(int c);
extern  void meminit(void);


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


uint32 get_host_clock(void) {
    uint32 result;
    __asm__ volatile (
        "li a7, %2\n"
        "ecall\n"
        "mv %0, a0\n"
        "mv %1, a1\n"
        : "=r" (result)           // salida: resultado en a0
        : "r" (0), "i" (45)  // entrada: syscall número
        : "a0", "a1", "a7"
    );
    return result;
}






#define ITERATIONS 100000
#define CLOCKS_PER_SEC 1000000

void prime_calculation() {
    printf("Calculando números primos...\n");
    uint32 start =  get_host_clock();
    
    long count = 0;
    for (long i = 2; i < ITERATIONS; i++) {
        int is_prime = 1;
        for (long j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            count++;
        }
    }
    
    uint32 end =  get_host_clock();
    float time_taken = ((float)(end - start));// / CLOCKS_PER_SEC;
    printf("Primos encontrados: %d\n", count);
    printf("Tiempo: %f segundos\n\n", time_taken);
}


void taylor_series() {
    printf("Calculando serie de Taylor (sin(x))...\n");
    
    uint32_t start = get_host_clock();  // ← ¡Microsegundos!
    
    float sum = 0.0f;
    for (int i = 0; i < ITERATIONS; i++) {
        float x = (float)i / 1000.0f;
        float term = x;
        float result = term;
        int sign = -1;
        
        for (int j = 3; j < 20; j += 2) {
            term *= (x * x) / ((float)((j - 1) * j));
            result += sign * term;
            sign *= -1;
        }
        sum += result;
    }
    
    uint32_t end = get_host_clock();
    uint32_t elapsed_us = end - start;

    // Si hubo desbordamiento (start > end), lo manejamos
    if (elapsed_us > end) {  // Esto nunca pasa si el tiempo es < 1.2h
        elapsed_us = 0;
    }

    float elapsed_seconds = (float)elapsed_us / 1000000.0f;

    printf("Resultado suma: %f\n", sum);
    printf("Tiempo: %f segundos\n\n", elapsed_seconds);
}

void monte_carlo_pi() {
    printf("Calculando  pi usando metodo Monte Carlo...\n");
    uint32 start = get_host_clock();
    
    long inside_circle = 0;
    for (long i = 0; i < ITERATIONS; i++) {
        float x = (float)rand() / RAND_MAX;
        float y = (float)rand() / RAND_MAX;
        
        if (x * x + y * y <= 1.0) {
            inside_circle++;
        }
    }
    
    float pi_estimate = 4.0 * inside_circle / ITERATIONS;
    uint32 end = get_host_clock();
    float time_taken = ((float)(end - start)) / CLOCKS_PER_SEC;
    
    printf("pi estimado: %f\n", pi_estimate);
    printf("Tiempo: %f segundos\n\n", time_taken);
}
/************************************************************************/



/***********************************************************************/

extern int rv32i( );

int main(int argc, char const *argv[])
{
    struct  memblk  *memptr;    /* Ptr to memory block      */
    uint32  free_mem;       /* Total amount of free memory  */
    meminit();
    free_mem = 0;
    for (memptr = memlist.mnext; memptr != NULL;
                        memptr = memptr->mnext) {
        free_mem += memptr->mlength;
    }
    printf ("Build date: %s %s\n\n", __DATE__, __TIME__);
    printf("%10d bytes of free memory.  Free list:\n", free_mem);
    for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
        printf("           [0x%08x to 0x%08x]\n",
        (uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
    }

    printf("%10d bytes of Xinu code.\n",
        (uint32)&_etext - (uint32)&_text);
    printf("           [0x%08x to 0x%08x]\n",
        (uint32)&_text, (uint32)&_etext - 1);
    printf("%10d bytes of data.\n",
        (uint32)&_ebss - (uint32)&_sdata);
    printf("           [0x%08x to 0x%08x]\n\n",
        (uint32)&_sdata, (uint32)&_ebss - 1);



   
     char *ptr1, *ptr2, *ptr3;
    uint32 free_before, free_after;

    
    // Muestra estado inicial del heap
    free_before = heap_free();
    printf("=== HEAP DEBUG: INITIAL STATE ===\n");
    printf("Total free memory: %d bytes\n", free_before);
    printf("minheap: 0x%08x\n", minheap);
    printf("maxheap: 0x%08x\n", maxheap);
    printf("\n");

    // 🔍 PRUEBA 1: Asignar un bloque pequeño
    printf("=== PRUEBA 1: malloc(64) ===\n");
    ptr1 = malloc(64);
    if (ptr1 == NULL) {
        printf("ERROR: malloc(64) failed!\n");
    } else {
        printf(" esta es una prueba\n");
        printf("malloc(64) OK → ptr1 = 0x%08X\n", (uint32)ptr1);
        // Escribimos un valor para asegurar que la memoria es accesible
        *(uint32*)ptr1 = 0xDEADBEEF;
        printf("Wrote 0xDEADBEEF at 0x%08x\n", (uint32)ptr1);
    }

    free_after = heap_free();
    printf("Free memory after malloc(64): %d bytes (decreased by %d)\n",
           free_after, free_before - free_after);
    printf("\n");

    // 🔍 PRUEBA 2: Asignar otro bloque grande
    printf("=== PRUEBA 2: malloc(2000) ===\n");
    ptr2 = malloc(2000);
    if (ptr2 == NULL) {
        printf("ERROR: malloc(2000) failed!\n");
    } else {
        printf("malloc(2000) OK → ptr2 = 0x%08x\n", (uint32)ptr2);
        // Escribe algunos bytes
        for (int i = 0; i < 16; i++) {
            ptr2[i] = 'A' + (i % 26);
        }
        ptr2[15] = '\0';
        printf("Wrote: \"%s\" at 0x%08x\n", ptr2, (uint32)ptr2);
    }

    free_after = heap_free();
    printf("Free memory after malloc(2000): %d bytes (decreased by %d)\n",
           free_after, free_before - free_after);
    printf("\n");

    // 🔍 PRUEBA 3: Intentar asignar más de lo disponible
    printf("=== PRUEBA 3: malloc(heap_free()) ===\n");
    uint32 remaining = heap_free();
    printf("Remaining free memory: %d bytes\n", remaining);
    ptr3 = malloc(remaining);
    if (ptr3 == NULL) {
        printf("CORRECTO: malloc(%d) FAILED — heap fragmented or too small\n", remaining);
    } else {
        printf(" ERROR: malloc(%d) SUCCEEDED — this should NOT happen!\n", remaining);
        free(ptr3); // Si por error funciona, libera
    }
    printf("\n");

    // 🔍 PRUEBA 4: Liberar el primer bloque
    printf("=== PRUEBA 4: free(ptr1) ===\n");
    if (ptr1 != NULL) {
        free(ptr1);
        printf("free(ptr1) called.\n");
    }

    free_after = heap_free();
    printf("Free memory after free(ptr1): %d bytes (increased by %d)\n",
           free_after, free_after - (free_before - 64 - 2000));
    printf("\n");

    // 🔍 PRUEBA 5: Liberar el segundo bloque
    printf("=== PRUEBA 5: free(ptr2) ===\n");
    if (ptr2 != NULL) {
        free(ptr2);
        printf("free(ptr2) called.\n");
    }

    free_after = heap_free();
    printf("Free memory after free(ptr2): %d bytes (back to initial? %s)\n",
           free_after,
           (free_after == free_before) ? "YES" : " NO (fragmentation?)");
    printf("\n");

    // 🔍 PRUEBA 6: Verifica que free(NULL) no rompe
    printf("=== PRUEBA 6: free(NULL) ===\n");
    free(NULL);
    printf("free(NULL) called — no crash!\n");
    printf("\n");

    // 🔍 PRUEBA 7: Último chequeo de heap
    printf("=== FINAL HEAP STATUS ===\n");
    printf("Final free memory: %d bytes\n", heap_free());
    printf("Expected: ~%d bytes (initial)\n", free_before);

    printf("\nok\n");


   /*char *buff=(char *)calloc(free_mem-100, sizeof(char));

   if(buff==NULL){
    printf("error malloc\n");
    return 0;
   }*/
   

  
   printf("\nok\n");
 
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
   float value= 7.898;
    printf("%f\n",value );
    
printf("========================================\n");
   rv32i();
    printf("========================================\n");
    
    prime_calculation();
    taylor_series();
    monte_carlo_pi();
  


    return 0;
}