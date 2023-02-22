#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funzioni.h"
#include "printable.h"
#define nCarte 52

#define nMazzi 6

// valori di inizializzazione delle carte
const char *semi[4] = {"❤️", "♣️", "♦️", "♠️"};
const char *valoriFig[13] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
const int valori[13] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

int lunghezzaMazzo = nMazzi * nCarte; // lunghezza del mazzo completo

carta mazzo[nMazzi * nCarte]; // array di carta per contenere il mazzo

mano manoGiocatore;
mano manoCpu;

void resizeMazzo(int indice)
{
    carta temp;
    temp = mazzo[indice];
    mazzo[indice] = mazzo[lunghezzaMazzo - 1];
    mazzo[lunghezzaMazzo - 1] = temp;
    lunghezzaMazzo--;
}

mano chiamaCarta(mano mano)
{
    // ritona una mano uguale a quella passata con una carta in più
    mano.size++;                                                          // aumentato il contatore delle carte
    mano.carte = (carta *)realloc(mano.carte, mano.size * sizeof(carta)); // allocato lo spazio per una carta in più
    int indice = rand() % lunghezzaMazzo;                                 // viene scelto un indice casuale per pescare la carta dal mazzo
                                                                          // N.B. il mazzo non è mescolato
    carta cartaPescata = mazzo[indice];                                   // viene pescata la carta
    mano.carte[mano.size - 1] = mazzo[indice];                            // la carta viene aggiunta alla mano

    resizeMazzo(indice); // viene modificato il mazzo globale per impedire che la stessa carta venga ripescata
   
    // calcolo punteggio
    int n_assi = 0; // contatore assi
    int punteggio = 0;

    for (int i = 0; i < mano.size; i++)
    {
        int valore_carta = mano.carte[i].valore;
        if (valore_carta == 11)
        {
            n_assi++;
        }
        punteggio += valore_carta;
    }
    // sottratto il valore degli assi che farebbe superare il 21
    while (n_assi > 0 && punteggio > 21)
    {
        punteggio -= 10;
        n_assi--;
    }

    mano.punteggio = punteggio; // assegnato il putneggio

    return mano;
}

mano inizializza_mano()
{
    // ritorna una mano inizializzata con valori 0 e con array di carte vuoto
    mano mano;
    mano.size = 0;
    mano.punteggio = 0;
    mano.carte = (carta *)calloc(mano.size, sizeof(carta));
    return mano;
}

void genera_mazzo()
{
    lunghezzaMazzo = nMazzi * nCarte; // reset della variabile lunghezzaMazzo
    // inizializzazione del mazzo
    // viene inizializzato carta per carta, mazzo per mazzo
    for (int k = 0; k < nMazzi; k++) // cilco mazzi
    {
        for (int i = 0; i < 4; i++) // cilco semi
        {
            // ciclo che itera l'array mazzo di 13 posizioni in volta
            // andando a prendere i dati dagli array di dati
            // manipolando gli indici con l'operatore modulo
            //
            // per più informazioni guardare doc.txt
            for (int j = 13 * i * k; j < 13 * ((i + 1) * (k + 1)); j++)
            {
                strcpy(mazzo[j].seme, semi[i]);
                strcpy(mazzo[j].valoreFig, valoriFig[j % 13]);
                mazzo[j].valore = valori[j % 13];
            }
        }
    }
}

int gioca()
{
    genera_mazzo(); // viene inizializzato il mazzo globale

    char risultato_partita[45]; // stringa che conterrà il messaggio dell'esisto della partita

    int continuo_partita = 1; // flag continuo partita, utile solo al rinnovo del mazzo
    while (continuo_partita)
    {
        print_image(); // stampata una ascii art

        manoCpu = inizializza_mano();       // inizializzata la mano del croupier
        manoGiocatore = inizializza_mano(); // inizializzata la mano del gicoatore

        char input[15];                             // stringa che contiene gli input dell'utente
        manoGiocatore = chiamaCarta(manoGiocatore); // viene data una carta
        manoCpu = chiamaCarta(manoCpu);             // viene data una carta
        manoCpu = chiamaCarta(manoCpu);             // viene data una carta
        int game_over = 0;                          // variabile che indica se la partita in corso deve termianare
        do
        {
            pulisci();                                  // pulisce schermo
            print_image();                              // stampa ascii art
            manoGiocatore = chiamaCarta(manoGiocatore); // viene data una carta

            // stampa della mano e del punteggio del gicatore
            printf("La tua mano: [");
            for (int i = 0; i < manoGiocatore.size; i++)
            {
                printf("%s%s", manoGiocatore.carte[i].valoreFig, manoGiocatore.carte[i].seme);
                if (i != manoGiocatore.size - 1)
                    printf(", ");
            }
            printf("]\n");
            printf("\tPunteggio: %d", manoGiocatore.punteggio);
            // stampa la mano della cpu tenendo nascolta la seconda carta, nemmeno il punteggio viene comunicato
            printf("\n\nLa mano del croupier: [");
            printf("%s%s", manoCpu.carte[0].valoreFig, manoCpu.carte[0].seme);
            printf(", *");
            printf("]\n\n");

            // condizioni di uscita
            if (manoGiocatore.punteggio > 21)
            {
                strcpy(risultato_partita, "Hai perso - superato il 21");
                game_over = 1;
            }
            else if (manoCpu.punteggio == 21)
            {
                strcpy(risultato_partita, "Hai perso - il banco ha fatto Blackjack!!");
                game_over = 1;
            }
            else if (manoGiocatore.punteggio == 21)
            {
                strcpy(risultato_partita, "Hai vinto - hai fatto Blackjack!!");
                game_over = 1;
            }
            else
            {
                // se nessuna delle precedenti condizioni si verifica, viene chieto
                // se si vuole pescare un'altra carta, nel caso negativo il giocatore
                // termina di giocare ed inizia la cpu
                do
                {
                    printf("Vuoi pescare un altra carta? ");
                    fgets(input, (sizeof input), stdin);
                    input[strcspn(input, "\n")] = '\0';
                } while (check_si_no(input));
            }

        } while (strcmp(input, "si") == 0 && game_over == 0);
        // qui il giocatore smette e comincia la CPU
        while (manoCpu.punteggio < 17 && game_over != 1) // se la cpu non ha raggiunto il putneggio di 17 deve pescare
        {
            manoCpu = chiamaCarta(manoCpu);
        }
        // annuncio vincitore
        pulisci();
        print_image();
        printf("La tua mano: [");
        for (int i = 0; i < manoGiocatore.size; i++)
        {
            printf("%s%s", manoGiocatore.carte[i].valoreFig, manoGiocatore.carte[i].seme);
            if (i != manoGiocatore.size - 1)
                printf(", ");
        }
        printf("]\n");
        printf("\tPunteggio: %d", manoGiocatore.punteggio);
        printf("\n\nLa mano del croupier: [");
        for (int i = 0; i < manoCpu.size; i++)
        {
            printf("%s%s", manoCpu.carte[i].valoreFig, manoCpu.carte[i].seme);
            if (i != manoCpu.size - 1)
                printf(", ");
        }
        printf("]\n");
        printf("\tPunteggio: %d\n", manoCpu.punteggio);

        if (game_over == 1) // valutato l'exit code
            printf("%s\n", risultato_partita);
        else
        {
            if (manoCpu.punteggio == 21)
                printf("Hai perso - il banco ha fatto Blackjack!!");
            else if (manoGiocatore.punteggio == manoCpu.punteggio)
                printf("Hai perso - hai fatto lo stesso punteggio del banco");
            else if (manoCpu.punteggio > 21)
                printf("Hai vinto - il banco è andato oltre 21");
            else if (manoCpu.punteggio > manoGiocatore.punteggio)
                printf("Hai perso - il banco si avvicina di più a 21");
            else
                printf("Hai vinto - ti avvicini di più a 21");
        }
        printf("\n\n");

        if (lunghezzaMazzo < 52) // a fine di ogni partita viene controllato se il mazzo ha abbastanza carte
        {
            printf("\nIl mazzo ha troppe poche carte...\n");
            printf("Un rinnovo del mazzo è stato eseguito\n\n");

            continuo_partita = 0; // la partita con il seguente mazzo termina
        }
        else // se il mazzo va bene viene chiesto se si vuole fare un altra partita
        {
            if (!do_continue())
            {
                return 0; // se non si vuole proseguire viene ritornato 0 che terminerà il programma
            }
        }
        free(manoCpu.carte);
        free(manoGiocatore.carte);
    }
    return 1; // se non si continua con la corrente partita, ma comunque giocare di nuovo, viene ritornato un valore non 0
}

int main(int argc, char *argv[])
{
    pulisci(); // pulisco schermo
    printf("Benvenuto in Blackjack\n");
    srand(time(NULL));                    // inizializzazione random
    int terminazione = 1;                 // flag che indica se il programma deve terminare
    while (do_continue() && terminazione) // ciclo che gestire ogni nuova partita
    {
        terminazione = gioca();
        if (terminazione == 0) // se il flag è 0 allora il programma ritorna
            return 0;
    }
}