//Sintaxis y semántica de los lenguajes
//Trabajo Práctico Nro 1

//C Web Scraper
//Vuelta Agustín
//Castelli Santiago

//includes and defines
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define BOLSAR_HTML "wget -q -O - https://52.67.80.139/test/lideres-bcba_limpio.html --no-check-certificate"
#define OPEN_ERROR "Error al intentar abrir el archivo."


//Usefull link:
//https://en.wikibooks.org/wiki/C_Programming/String_manipulation


//prototipos
bool Mostrar(char []);
void Menu();
bool Variacion(char []);
bool DevolverTabla(char []);

//main function
void main()
{
	char op;
  do
  {
    Menu();
    do
    {
      op = getch();
    } while ((op != 27) && (op < 49 || op > 57)); //27 = Esc ; 49 = 1 ; 57 = 9
    system("cls");
    switch(op)
    {
      case '1':
        if (!Mostrar(BOLSAR_HTML))
          printf(OPEN_ERROR);
      	break;    
			case '2':
        if (!Variacion(BOLSAR_HTML))
          printf(OPEN_ERROR);
      	break;  
			case '3':
        if (!DevolverTabla(BOLSAR_HTML))
          printf(OPEN_ERROR);
      	break;
    }
  } while (op != 27); //mientras que no presione Esc..
  return 0;
}

bool Mostrar(char ruta[])
{
  FILE *popen(const char *command, const char *node);
	int pclose(FILE *stream);
  char buffer[129024];
  FILE *www;
  if (www = popen(ruta, "r"))
  {
    while (fgets(buffer, sizeof(buffer), www) != NULL)
			printf("%s", buffer);
    pclose(www);
    return true;
  }
  else
    return false;
}

//estoy probando algunas funciones para ver como se comportan
//strcmp compara dos strings
//strtok pones un limitadot y va borra y separa los strings
bool Variacion(char ruta[])
{
  FILE *popen(const char *command, const char *node);
	int pclose(FILE *stream);
  char buffer[256];
  FILE *www;
  char *str;
  if (www = popen(ruta, "r"))
  {
    while (fgets(buffer, sizeof(buffer), www) != NULL)
    {
			str = (strstr(buffer,"Variac")) ; // busca la palabra dentro del texto, al resto la pasa a null
			printf("%s", str);		
		}
    pclose(www);
    return true;
  }
  else
    return false;
}

//estoy tratrando de recortar todo el html y que guardemos en un string solo la tabla (https://puu.sh/G1EQG/439029db62.png)
bool DevolverTabla(char ruta[])
{
  FILE *popen(const char *command, const char *node);
	int pclose(FILE *stream);
  char buffer[129024];
  char str[15];
  FILE *www;
  if (www = popen(ruta, "r"))
  {
    fgets(buffer, sizeof(buffer), www);
    strcpy(str, buffer);
    while (fgets(buffer, sizeof(buffer), www) != NULL)
    	strcat(str, buffer);
    pclose(www);
    char *tok = strtok(NULL, "<tbody><tr><td>Especie</td><td>Vto.</td><td>Cant. Nominal</td><td>Precio Compra</td><td>Precio Venta</td><td>Cant. Nominal</td><td>Último</td><td>Variación %</td><td>Apertura</td><td>Máximo</td><td>Mínimo</td><td>Cierre Ant.</td><td>Vol. Nominal</td><td>Monto Operado ($)</td><td>Cant. Ope.</td><td>Hora Cotización</td></tr><tr");  	
		printf("\ntok: %s", tok);
    return true;
  }
  else
    return false;
}
/*
char *s = "asdf,1234,qwer";
char str[15];
strcpy(str, s);
printf("\nstr: %s", str);
char *tok = strtok(str, ",");
printf("\ntok: %s", tok);
tok = strtok(NULL, ",");
printf("\ntok: %s", tok);
tok = strtok(NULL, ",");
printf("\ntok: %s", tok);

str: asdf,1234,qwer
tok: asdf
tok: 1234
tok: qwer*/

/*<tbody><tr>
<td>Especie</td><td>Vto.</td><td>Cant. Nominal</td><td>Precio Compra</td><td>Precio Venta</td><td>Cant. Nominal</td><td>Último</td><td>Variación %</td><td>Apertura</td><td>Máximo</td><td>Mínimo</td><td>Cierre Ant.</td><td>Vol. Nominal</td><td>Monto Operado ($)</td><td>Cant. Ope.</td><td>Hora Cotización</td>
</tr>
<tr>*/

void Menu()
{
  printf("\n\n****** Elija una opcion ******\n");
  printf("1 - Mostrar .html Bolsar.com\n");
  printf("2 - Variacion .html Bolsar.com\n");
  printf("3 - Devolver solo la tabla para analizar los datos\n");
  printf("Esc - Salir\n\n");
}

//printf("Ing. num: "); scanf("%d", &num);
//printf("Num: %d", num);

//https://52.67.80.139/test/lideres-bcba_limpio.html
//#define INICIO_TABLA "<td>Especie</td><td>Vto.</td><td>Cant. Nominal</td><td>Precio Compra</td><td>Precio Venta</td><td>Cant. Nominal</td><td>+Ültimo</td><td>Variaci+¦n %</td><td>Apertura</td><td>M+íximo</td><td>M+¡nimo</td><td>Cierre Ant.</td><td>Vol. Nominal</td><td>Monto Operado ($)</td><td>Cant. Ope.</td><td>Hora Cotizaci+¦n</td>"

//TODO:
//- change from all bool functions, true to 1 and false to (-1) so theres no need for extra libraries


