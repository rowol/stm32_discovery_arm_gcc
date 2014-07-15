
#include <stdio.h>


int main(void)
{
   // Disable STDOUT buffering. Otherwise nothing will be printed
   // before a newline character or when the buffer is flushed.
   // This MUST be done before any writes to STDOUT to have any effect...
   setbuf(stdout, NULL);

   printf("Hello world\r\n");
   printf("Press return to continue\r\n");

   //Wait for a return key press in the OpenOCD/GDB server window
   getchar();
   
   printf("Goodbye world\r\n");

   
   for (;;) 
      ;
   
   return 0;
}
