#include <stdio.h>
#include <string.h>
 
int main()
{
    char str[100] = "one => two";
    char *temp;
    temp = strchr(str,'=');
    *temp = '\0';   
    return 0;
}