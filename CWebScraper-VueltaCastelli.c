//Sintaxis y semántica de los lenguajes
//Trabajo Práctico Nro 1

//Web Scraping Financiero
//Vuelta Agustín
//Castelli Santiago

//includes and defines
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define BOLSAR_HTML "wget -q -O - https://52.67.80.139/test/lideres-bcba_limpio.html --no-check-certificate"
#define OPEN_ERROR "Error al intentar abrir el archivo."
#define ARCH_CSV "BolsarData.csv"
#define ARCH_HTML "BolsarVariaciones.html"

//Usefull link:
//https://en.wikibooks.org/wiki/C_Programming/String_manipulation

//prototipos
bool Mostrar(char []);
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
		system("cls");
		switch(op)
		{
			case '1':
				if (!ListarVariacion(BOLSAR_HTML))
					printf(OPEN_ERROR);
				break;
			case '2':
				if (!ListarCompraVentaEnCSV(BOLSAR_HTML, ARCH_CSV))
					printf(OPEN_ERROR);
				break;
			case '3':
				if (!ListarVariacionHTML(BOLSAR_HTML, ARCH_HTML))
					printf(OPEN_ERROR);
				break;
		}
	} while (op != 27); //mientras que no presione Esc..
}

bool Mostrar(char ruta[])
{
	FILE *www;
	char buffer[129024];
	if (www = popen(ruta, "r"))
	{
		while (fgets(buffer, 129024, www))
			printf("%s", buffer);
		pclose(www);
		return true;
	}
	else
		return false;
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
				//antes que nada, sacarle todos los </td>, <tr>, </tr>
				removeSubstr(tabla, "</td>");
				removeSubstr(tabla, "<tr>");
				removeSubstr(tabla, "</tr>");
				char sub[] = "<td>", *p;
				int cont, cont1 = 0, i, j, k;
				char especie[10], variacion[10];
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
				return true;
			}
		}
		pclose(www);
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
					char especie[10], buy[10], sell[10], open[10], high[10], low[10];
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
							switch(cont1) //1,4,5,9,10,11
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
									break;
								case 5: //Venta
									while (tabla[i] != 60)
									{
										sell[k] = tabla[i];
										i++;
										k++;
									}
									k = 0;
									break;
								case 9: //Apertura
									while (tabla[i] != 60)
									{
										open[k] = tabla[i];
										i++;
										k++;
									}
									k = 0;
									break;
								case 10: //Maximo
									while (tabla[i] != 60)
									{
										high[k] = tabla[i];
										i++;
										k++;
									}
									k = 0;
									break;
								case 11: //Minimo
									while (tabla[i] != 60)
									{
										low[k] = tabla[i];
										i++;
										k++;
									}
									k = 0;
									for (p=low ; p=strchr(p, '.') ; ++p)
										*p = ',';
									break;
								case 16: //termino de leer la fila
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
								break;
							}
						}
						else
							i++;
					}
					printf("Archivo .csv creado");
					pclose(csv);
					return true;
				}
				else
					return false;
				return true;
			}
		}
		pclose(www);
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
					char especie[10], buy[10], sell[10], variacion[10], open[10];
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
										{
											fprintf(html, "\n\t\t<tr>\n\t\t\t<td>%s</td><td>%.2f</td>\n\t\t</tr>", especie, fVariacion);
										}
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
					return true;
				}
				else
					return false;
				return true;
			}
		}
		pclose(www);
		return true;
	}
	else
    	return false;
}

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
	printf("3 - Listar especies cuya variacion supera el 0.5% en archivo .HTML\n");
	printf("Esc - Salir\n\n");
}

//https://52.67.80.139/test/lideres-bcba_limpio.html

//1 = Especie
//2 = Vto.
//3 = Cant. Nominal
//4 = Precio Compra
//5 = Precio Venta
//6 = Cant. Nominal
//7 = Último
//8 = Variación %
//9 = Apertura
//10 = Máximo
//11 = Mínimo
//12 = Cierre Ant.
//13 = Vol. Nominal
//14 = Monto Operado ($)
//15 = Cant. Ope.
//16 = Hora Cotización
          
//TODO:
//- change from all bool functions, true to 1 and false to (-1) so theres no need for extra libraries


