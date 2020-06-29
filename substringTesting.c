#include <stdio.h>
#include <string.h>
 
void removeSubstr (char *string, char *sub)
{
    char *match;
    int len = strlen(sub);
    while ((match = strstr(string, sub)))
    {
        *match = '\0';
        strcat(string, match+len);
    }
}
 
int main()
{
    char test[] = "<td>ALUA</td><td>P. E.</td><td>100</td><td>17,000</td><td>17,900</td><td>289</td><td>17,150</td><td>-1,72 </td><td>17,200</td><td>17,650</td><td>17,100</td><td>17,450</td><td>370.531</td><td>6.386.376</td><td>244</td><td>17:15:0</td>";
    puts(test);
    printf("\n");
    removeSubstr(test, "<td>");
    puts(test);
    printf("\n");
    removeSubstr(test, "</td>");
    puts(test); 
    printf("\n");   
    return 0;
}

/*
<td>ALUA</td><td>P. E.</td><td>100</td><td>17,000</td><td>17,900</td>
<td>289</td><td>17,150</td><td>-1,72 </td><td>17,200</td><td>17,650</td>
<td>17,100</td><td>17,450</td><td>370.531</td><td>6.386.376</td><td>244</td>
<td>17:15:0</td>
*/