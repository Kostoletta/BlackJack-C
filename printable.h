#include <stdio.h>
#define MAX_LEN 128

void print_image()
{

    char *filename = "logo.txt";
    FILE *fptr = NULL;
 
    if((fptr = fopen(filename,"r")) == NULL)
    {
        fprintf(stderr,"Errore nell'apertura di %s\nVerificare l'integrità dei file\n\n",filename);
        return;
    }
    char read_string[MAX_LEN];
 
    while(fgets(read_string,sizeof(read_string),fptr) != NULL)
        printf("%s",read_string);
    fclose(fptr);
}
