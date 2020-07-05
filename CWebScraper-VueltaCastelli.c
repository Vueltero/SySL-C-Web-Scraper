//Sintaxis y semántica de los lenguajes
//2020 - K2054
//Trabajo Práctico Nro 1

//Web Scraping Financiero
//Vuelta Agustín
//Castelli Santiago

//librerias
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//redefiniciones
#define BOLSAR_HTML "wget -q -O - https://52.67.80.139/test/lideres-bcba_limpio.html --no-check-certificate"
#define OPEN_ERROR "Error al intentar abrir el archivo."
#define ARCH_CSV "BolsarData.csv"
#define ARCH_HTML "BolsarVariaciones.html"

//prototipos
void Menu();
bool ListarVariacion(char []);
void removeSubstr(char *, char *);
bool ListarCompraVentaEnCSV(char [], char []);
bool ListarVariacionHTML(char [], char []);

//driver function
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
		system("cls"); //limpia la pantalla luego de ejecutar una opcion
		switch(op)
		{
			case '1':
				//ejecuta la funcion dentro del if y si devuelve false escribe un mensaje de error
				if (!ListarVariacion(BOLSAR_HTML))
					printf(OPEN_ERROR);
				break;
			case '2':
				if (!ListarCompraVentaEnCSV(BOLSAR_HTML, ARCH_CSV))
					printf(OPEN_ERROR); //alguna de las aperturas de los archivos devolvio false
				break;
			case '3':
				if (!ListarVariacionHTML(BOLSAR_HTML, ARCH_HTML))
					printf(OPEN_ERROR);
				break;
		}
	} while (op != 27); //mientras que no presione Esc..
}

//va leyendo el string, va contando "<td>", si es el 1, guarda la especie
//en una variable, sigue leyendo, si es el 8, guarda la variacion % en una
//variable, si la variacion es > 0.5, imprime la especie y su variacion,
//sino, sigue leyendo hasta leer el <td> 16, resetea el contador, repeat
bool ListarVariacion(char ruta[])
{
	FILE *www;
	char buffer[129024], tabla[129024], *ptr;
	char tableStart[] = "<td>ALUA</td><td>P. E.</td>";
	char tableEnd[] = "<td>YPFD</td><td>P. E.</td>";
	bool foundTabla = false;
	if (www = popen(ruta, "r"))
	{
		while (fgets(buffer, 129024, www) != NULL) //mientras que lo que leyo y guardo en el buffer sea distinto de vacio
		{
			if (ptr = strstr(buffer, tableStart))
				foundTabla = true;
			if (foundTabla == true)
				strcat(tabla, buffer);
			if (ptr = strstr(buffer, tableEnd))
			{
				//aca ya guardó la tabla en el string tabla
				pclose(www);
				//antes que nada, sacarle todos los </td>, <tr>, </tr>
				removeSubstr(tabla, "</td>");
				removeSubstr(tabla, "<tr>");
				removeSubstr(tabla, "</tr>");
				char sub[] = "<td>", *p;
				int cont, cont1 = 0, i, j, k; //contadores e indices
				char especie[10] = {'\0'}, variacion[10] = {'\0'};
				float fVariacion;
				for (i=0 ; i<strlen(tabla) ;)
				{
					j = 0;
					cont = 0;
					while ((tabla[i] == sub[j]))
					{
						cont++;
						i++;
						j++;
					}
					if (cont == strlen(sub)) //encontro un <td>
					{
						cont1++;
						switch(cont1)
						{
							case 1: //guarda la especie
								while (tabla[i] != 60) //60 = '<'
								{
									especie[k] = tabla[i];
									i++;
									k++;
								}
								k = 0;
								break;
							case 8: //guarda la variacion
								while (tabla[i] != 32) //32 = ' '
								{
									variacion[k] = tabla[i];
									i++;
									k++;
								}
								i++;
								k = 0;
								//remplaza la coma por un punto
								for (p=variacion ; p=strchr(p, ',') ; ++p)
									*p = '.';
								//convierte el string variacion a float
								sscanf(variacion, "%f", &fVariacion);
								if (fVariacion > 0.5)
									printf("%s: %.2f\n", especie, fVariacion);
								memset(especie, '\0', sizeof especie);
								memset(variacion, '\0', sizeof variacion);
								break;
							case 16: //termino de leer la fila
								//resetea el contador de <td>
								cont1 = 0;
								break;
						}
					}
					else
						i++;
				}
				memset(tabla, '\0', sizeof tabla);
				return true;
			}
		}
		pclose(www);
		memset(tabla, '\0', sizeof tabla);
		return true;
	}
	else
		return false;
}

bool ListarCompraVentaEnCSV(char ruta[], char rutaCSV[])
{
	FILE *www, *csv;
	char buffer[129024], tabla[129024], *ptr;
	char tableStart[] = "<td>ALUA</td><td>P. E.</td>";
	char tableEnd[] = "<td>YPFD</td><td>P. E.</td>";
	bool foundTabla = false;
	if (www = popen(ruta, "r"))
	{
		while (fgets(buffer, 129024, www) != NULL)
		{
			if (ptr = strstr(buffer, tableStart))
				foundTabla = true;
			if (foundTabla == true)
				strcat(tabla, buffer);
			if (ptr = strstr(buffer, tableEnd))
			{
				pclose(www);  
				//aca ya guardó la tabla en el string tabla
				if (csv = fopen(rutaCSV, "w"))
				{
					fprintf(csv, "Especie;Compra;Venta;Apertura;Maximo;Minimo\n");
					//antes que nada, sacarle todos los </td>, <tr>, </tr>
					removeSubstr(tabla, "</td>");
					removeSubstr(tabla, "<tr>");
					removeSubstr(tabla, "</tr>");
					char sub[] = "<td>", *p;
					int cont, cont1 = 0, i, j, k;
					char especie[10] = {'\0'}, buy[10] = {'\0'}, sell[10] = {'\0'}, open[10] = {'\0'}, high[10] = {'\0'}, low[10] = {'\0'};
					for (i=0 ; i<strlen(tabla) ;)
					{
						j = 0;
						cont = 0;
						while ((tabla[i] == sub[j]))
						{
							cont++;
							i++;
							j++;
						}
						if (cont == strlen(sub)) //encontro un <td>
						{
							cont1++;
							while (tabla[i] != 60)
							{
								if (cont1 == 1)	especie[k] = tabla[i];
								if (cont1 == 4)	buy[k] = tabla[i];
								if (cont1 == 5)	sell[k] = tabla[i];
								if (cont1 == 9)	open[k] = tabla[i];
								if (cont1 == 10) high[k] = tabla[i];
								if (cont1 == 11) low[k] = tabla[i];
								i++;
								k++;
							}
							k = 0;
							if (cont1 == 11) for (p=low ; p=strchr(p, '.') ; ++p) *p = ',';
							if (cont1 == 16) //termino de leer la fila
							{
								//escribe variables guardadas en .csv
								fprintf(csv, "%s;%s;%s;%s;%s;%s\n", especie, buy, sell, open, high, low);
								//resetea variables
								memset(especie, '\0', sizeof especie);
								memset(buy, '\0', sizeof buy);
								memset(sell, '\0', sizeof sell);
								memset(open, '\0', sizeof open);
								memset(high, '\0', sizeof high);
								memset(low, '\0', sizeof low);
								//resetea el contador de <td>
								cont1 = 0;
							}
						}
						else
							i++;
					}
					printf("Archivo .csv creado");
					pclose(csv);
					memset(tabla, '\0', sizeof tabla);
					return true;
				}
				else
				{
					memset(tabla, '\0', sizeof tabla);
					return false;
				}
				memset(tabla, '\0', sizeof tabla);
				return true;
			}
		}
		pclose(www);
		memset(tabla, '\0', sizeof tabla);
		return true;
	}
	else
    	return false;
}

//indicar en rojo las filas cuyas especies tienen compra y venta < apertura
bool ListarVariacionHTML(char ruta[], char rutaHTML[])
{
	FILE *www, *html;
	char buffer[129024], tabla[129024], *ptr;
	char tableStart[] = "<td>ALUA</td><td>P. E.</td>";
	char tableEnd[] = "<td>YPFD</td><td>P. E.</td>";
	bool foundTabla = false;
	if (www = popen(ruta, "r"))
	{
		while (fgets(buffer, 129024, www) != NULL)
		{
			if (ptr = strstr(buffer, tableStart))
				foundTabla = true;
			if (foundTabla == true)
				strcat(tabla, buffer);
			if (ptr = strstr(buffer, tableEnd))
			{
				pclose(www);  
				//aca ya guardó la tabla en el string tabla
				if (html = fopen(rutaHTML, "w"))
				{					
					//abre la tabla
					fprintf(html, "<table border=\"1\">\n\t<tbody>");
					//escribe los nombres de las columnas
					fprintf(html, "\n\t\t<tr>\n\t\t\t<td>Especie</td><td>Variación %%</td>\n\t\t</tr>");
					//antes que nada, sacarle todos los </td>, <tr>, </tr> al string tabla
					removeSubstr(tabla, "</td>");
					removeSubstr(tabla, "<tr>");
					removeSubstr(tabla, "</tr>");
					char sub[] = "<td>", *p;
					int cont, cont1 = 0, i, j, k;
					char especie[10] = {'\0'}, buy[10] = {'\0'}, sell[10] = {'\0'}, variacion[10] = {'\0'}, open[10] = {'\0'};
					float fBuy, fSell, fVariacion, fOpen;
					for (i=0 ; i<strlen(tabla) ;)
					{
						j = 0;
						cont = 0;
						while ((tabla[i] == sub[j]))
						{
							cont++;
							i++;
							j++;
						}
						if (cont == strlen(sub)) //encontro un <td>
                        {
                            cont1++;
                            switch(cont1)
                            {
 								case 1: //Especie
									while (tabla[i] != 60) //60 = '<'
									{
										especie[k] = tabla[i];
										i++;
										k++;
									}
									k = 0;
									break;
								case 4: //Compra
									while (tabla[i] != 60)
									{
										buy[k] = tabla[i];
										i++;
										k++;
									}
									k = 0;									
									for (p=buy ; p=strchr(p, ',') ; ++p)
										*p = '.';
									sscanf(buy, "%f", &fBuy);
									break;
								case 5: //Venta
									while (tabla[i] != 60)
									{
										sell[k] = tabla[i];
										i++;
										k++;
									}
									k = 0;									
									for (p=sell ; p=strchr(p, ',') ; ++p)
										*p = '.';
									sscanf(sell, "%f", &fSell);
									break;
								case 8: //Variacion
									while (tabla[i] != 32) //32 = ' '
									{
										variacion[k] = tabla[i];
										i++;
										k++;
									}
									i++;
									k = 0;
									//remplaza la coma por un punto
									for (p=variacion ; p=strchr(p, ',') ; ++p)
										*p = '.';
									//convierte el string variacion a float
									sscanf(variacion, "%f", &fVariacion);
									break;
								case 9: //Apertura
									while (tabla[i] != 60)
									{
										open[k] = tabla[i];
										i++;
										k++;
									}
									k = 0;
									for (p=open ; p=strchr(p, ',') ; ++p)
										*p = '.';
									sscanf(open, "%f", &fOpen);
									break;
								case 16: //termino de leer la fila
									if (fVariacion > 0.5)
									{
										if ((fBuy < fOpen) && (fSell < fOpen))
										{
											fprintf(html, "\n\t\t<tr bgcolor=\"#ff6666\">");
											fprintf(html, "\n\t\t\t<td>%s</td><td>%.2f</td>\n\t\t</tr>", especie, fVariacion);
										}
										else
											fprintf(html, "\n\t\t<tr>\n\t\t\t<td>%s</td><td>%.2f</td>\n\t\t</tr>", especie, fVariacion);
									}	
									//resetea variables
									memset(especie, '\0', sizeof especie);
									memset(buy, '\0', sizeof buy);
									memset(sell, '\0', sizeof sell);
									memset(variacion, '\0', sizeof variacion);
									memset(open, '\0', sizeof open);
									//resetea el contador de <td>
									cont1 = 0;
									break;
                            }
                        }
						else
							i++;
					}
					//termino, este vacia la tabla o no, la cierra
					fprintf(html, "\n\t</tbody>\n</table>");
					printf("Archivo .html creado");
					pclose(html);
					memset(tabla, '\0', sizeof tabla);
					return true;
				}
				else
				{
					memset(tabla, '\0', sizeof tabla);
					return false;
				}
				memset(tabla, '\0', sizeof tabla);
				return true;
			}
		}
		pclose(www);
		memset(tabla, '\0', sizeof tabla);
		return true;
	}
	else
    	return false;
}

//remueve todas las ocurrencias de un string dentro de otro
void removeSubstr(char *string, char *sub)
{
	char *match;
	int len = strlen(sub);
	while ((match = strstr(string, sub)))
	{
		*match = '\0';
		strcat(string, match+len);
	}
}

void Menu()
{
	printf("\n\n****** Elija una opcion ******\n");
	printf("1 - Listar especies cuya variacion supera el 0.5%%\n");
	printf("2 - Listar cotizaciones de compra y venta en archivo .CSV\n");
	printf("3 - Listar especies cuya variacion supera el 0.5%% en archivo .HTML\n");
	printf("Esc - Salir\n\n");
}