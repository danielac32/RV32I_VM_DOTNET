#include <stdint.h>
#include <stddef.h>


extern int __syscall(int ,...);

 

#define reg_uart_data (*(volatile uint32_t*)0x300000)

void putchar(char c)
{
	if (c == '\n')
		putchar('\r');
	reg_uart_data = c;
}

void print(const char *p)
{
	while (*p)
		putchar(*(p++));
}



void *memset(void *dst, int c, int n)
{
  if (n != 0) {
    unsigned char *d = dst;

    do
      *d++ = (unsigned char)c;
    while (--n != 0);
  }
  return (dst);
}



char buffer[100];
int main(void)
{
  int i = 0;
 char test[66];
 memset(test,0,sizeof test);

 memset(buffer,99,100);
 

  __syscall(44,9);
	print("Hello World from a simple RV32I ISA emulator!\n");

  while(1)
  {
    i++;
    print("test\n");
    if(i==10) break;
  }

	print("Bye!\n");
  __syscall(123,9);
  return 0;
}


