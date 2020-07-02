void main()
{
	/*
	float f;
	sscanf("4.08", "%f", &f);
	printf("%.2f", f);
	*/

	/*
	char sub[] = "<td>";
	printf("%d", strlen(sub));
	*/

	char variacion[] = "-1,65", *p;
	float fVariacion;
    for (p=variacion ; p=strchr(p, ',') ; ++p)
      *p = '.';
  	printf("%s\n", variacion);
  	sscanf(variacion, "%f", &fVariacion);
  	printf("%.2f", fVariacion);  	
}
