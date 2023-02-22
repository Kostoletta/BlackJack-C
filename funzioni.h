#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"

void pulisci()
{
    system("clear");
}

int check_si_no(char *input)
{
    // controllo sulla forma dell'input, se corretto avanti, altrimenti verrà chiesto di nuovo
    if (strcmp(input, "si") == 0 || strcmp(input, "no") == 0)
        return 0;
    else
    {
        printf("Errore, comando non riconosciuto\n");
        return 1;
    }
}

int do_continue()
{
    char input[15]; // stringa per l'input utente
    do
    {
        printf("Vuoi fare una partita a Blackjack? si/no ");
        fgets(input, (sizeof input), stdin); // prendo la scelta
        input[strcspn(input, "\n")] = '\0';  // tolgo \n e metto \0
        pulisci();
    } while (check_si_no(input));
    if (!strcmp(input, "si")) // se input equals si allora si
        return 1;             // può continuare a giocare, altrimenti
    else                      // fermerà l'esecuzione
        return 0;
}
