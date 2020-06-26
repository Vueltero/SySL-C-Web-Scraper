//Sintaxis y semántica de los lenguajes
//Trabajo Práctico Nro 1

//C Web Scraper
//Vuelta Agustín
//Castelli Santiago

//includes and defines
#include <stdio.h>
#include <stdbool.h>
#define BOLSAR_HTML "wget -q -O - https://52.67.80.139/test/lideres-bcba_limpio.html --no-check-certificate"
#define OPEN_ERROR "Error al intentar abrir el archivo."

//structs


//prototipos
bool Mostrar(char []);
void Menu();

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
    switch(op)
    {
      case '1':
        if (!Mostrar(BOLSAR_HTML))
          printf(OPEN_ERROR);
      	break;
      //case '2':
        //..
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

void Menu()
{
  printf("\n****** Elija una opcion ******\n");
  printf("1 - Mostrar .html Bolsar.con\n");
  //printf("2 - ..\n");
  printf("Esc - Salir\n\n");
}

//printf("Ing. num: "); scanf("%d", &num);
//printf("Num: %d", num);

//https://52.67.80.139/test/lideres-bcba_limpio.html