typedef struct {   
  int valore;
  char seme[7];
  char valoreFig[2]; 
}carta;

typedef struct {   
  int punteggio;
  int size;
  carta* carte;
}mano;