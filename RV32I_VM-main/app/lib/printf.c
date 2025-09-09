/* printf.c - printf*/

 
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <kernel.h>
#include <syscall.h>

 
extern void _fdoprnt(char *, va_list, int (*)(int, int), int);
static int sprntf(int, int);
extern  void    _doprnt(char *, va_list, int (*)(int));

/*------------------------------------------------------------------------
 *  printf  -  standard C printf function
 *------------------------------------------------------------------------
 */

int kputc(int c) {
    // Usamos a0 para el carácter y a7 para el número de syscall
    __asm__ volatile (
        "li a7, 44\n"
        "mv a0, %0\n"
        "ecall\n"
        : 
        : "r"(c)
        : "a0", "a7"
    );
    return c;
}



#define reg_uart_data (*(volatile uint32_t*)0x300000)

int putchar(int c)
{
    if (c == '\n')
        putchar('\r');
    reg_uart_data = c;
}




int printf(
      const char        *fmt,
      ...
    )
{
    va_list ap;

    va_start(ap, fmt);
    //_fdoprnt((char *)fmt, ap, putsyscall, 0);
    _doprnt((char *)fmt, ap, (int (*)(int))kputc);
    va_end(ap);

    return 0;
}



/*------------------------------------------------------------------------
 *  sprintf  -  Format arguments and place output in a string.
 *------------------------------------------------------------------------
 */
int sprintf(
      char      *str,       /* output string        */
      char      *fmt,       /* format string        */
      ...
    )
{
    va_list ap;
    char *s;

    s = str;
    va_start(ap, fmt);
    _fdoprnt(fmt, ap, sprntf, (int)&s);
    va_end(ap);
    *s++ = '\0';

    return ((int)str);
}



/*------------------------------------------------------------------------
 *  sprntf  -  Routine called by _doprnt to handle each character.
 *------------------------------------------------------------------------
 */
static int  sprntf(
          int       acpp,
          int       ac
        )
{
    char **cpp = (char **)acpp;
    char c = (char)ac;

    return (*(*cpp)++ = c);
}



void hexDump2(unsigned int offset, void *addr, int len) 
{
    int i;
    unsigned char buff[17];
    unsigned char *pc = addr;

    
    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.
            printf ("  %04x ", offset+i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf ("  %s\n", buff);
}


 