
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
	return 0;
}