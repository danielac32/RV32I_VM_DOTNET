
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>   
#include <stdio.h>

extern int kputc(int c);


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
	return 0;
}