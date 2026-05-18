#include <stdio.h>
#include <string.h>
int main (void)
{
    char* msg = "hello world";
    FILE* fp;
    fp = fopen ("/dev/JTAG_UART", "w");
    if (fp)
    {
        fprintf(fp, "%s",msg);
        fclose (fp);
    }
    return 0;
}
