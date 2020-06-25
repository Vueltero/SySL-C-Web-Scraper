//Sintaxis y semántica de los lenguajes
//Trabajo Práctico Nro 1

//C Web Scraper
//Vuelta Agustín
//Castelli Santiago

#include <stdio.h>

FILE *popen(const char *command, const char *node);
int pclose(FILE *stream);

void main()
{
	FILE *cmd = popen("wget -q -O - https://52.67.80.139/test/lideres-bcba_limpio.html --no-check-certificate", "r");
	char result[1048576];

	while (fgets(result, sizeof(result), cmd) != NULL)
		printf("%s", result);
	pclose(cmd);
}

//printf("Ing. num: "); scanf("%d", &num);
//printf("Num: %d", num);

//https://52.67.80.139/test/lideres-bcba_limpio.html