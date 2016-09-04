/**
    |-------------------------------------------------------------------------------------------|
    | Autore: Mario Taccori, 65234                                                              |
    | Progetto: Monopolycube [progetto finale PR1 - 2015/2016]                                  |
    |                                                                                           |
    | Scopo: file in cui sono definite le procedure utilizzate per stampare informazioni        |
    |        utili per il debug                                                                 |
    | NOTA: ogni procedura va chiamata nella posizione del programma opportuna (non sono        |
    |       presenti nel codice normalmente)                                                    |
    |                                                                                           |
    | Funzioni definite:                                                                        |
    |       void DEBUG_stampa_array_sfighe(Carta_sfiga* array_sfighe, int start, int num_sfighe)|
    |       void DEBUG_stampa_lista(Nodo* lista)                                                |
    |       void DEBUG_stampa_array_giocatori(Giocatore* array_giocatori, int num_giocatori)    |
    |       void DEBUG_stampa_giocatore(Giocatore giocatore)                                    |
    |-------------------------------------------------------------------------------------------|

**/

#include "monopolycube.h" // Inclusione header principale

/**
    Procedura che stampa le informazioni di ciascuna sfiga nell'array delle sfighe
    NOTA: è possibile stampare solo una parte dell'array passando start e num_sfighe opportuni

    INPUT: array contenente le sfighe, posizione da cui iniziare a stampare, numero sfighe nell'array (o posizione finale da stampare)
    OUTPUT: stampa a schermo le informazioni di ciascuna sfiga
**/
void DEBUG_stampa_array_sfighe(Carta_sfiga* array_sfighe, int start, int num_sfighe)
{
    int i=0; // Indice per cicli
    for(i=start; i<num_sfighe; i++) // Scorre da start a numsfighe-1
    {
        printf("\n\n");
        printf("Tipo sfiga: %d", array_sfighe[i].tipo_sfiga);
        printf("\nMessaggio: %s", array_sfighe[i].msg);
        printf("\nMolt aule: %d", array_sfighe[i].moltiplicatore_n_aule);
        printf("\nMolt scriv: %d", array_sfighe[i].moltiplicatore_n_scriv);
        printf("\nMolt tende: %d", array_sfighe[i].moltiplicatore_n_tende);
        printf("\nTassa: %d", array_sfighe[i].tassa);
        printf("\nDestinazione: %d", array_sfighe[i].casella_destinazione);
    }
    printf("\n\n");
}


/**
    Procedura che stampa una lista

    INPUT: lista da stampare
    OUTPUT: stampa a schermo tutti gli elementi della lista
**/
void DEBUG_stampa_lista(Nodo* lista)
{
    if(lista!=NULL) // Se la lista non è vuota
    {
        Nodo* pos=lista; // Scorritore lista posizionato all'inizio della lista stessa
        printf("\n\n");
        while(pos!=NULL) // Scorre tutta la lista
        {
            printf("%d\n", pos->id_casella); // Stampa l'intero contenuto nel nodo attuale
            pos=pos->next; // Passa al nodo successivo
        }
    }
    else printf("Lista vuota"); // Se la lista è vuota
}


/**
    Procedura che stampa le informazioni di ogni giocatore nell'array giocatori
    NOTA: utilizza DEBUG_stampa_giocatore()

    INPUT: array giocatori, numero giocatori nell'array
    OUTPUT: stampa le informazioni di ciascun giocatore a video (utilizza DEBUG_stampa_giocatore())
**/
void DEBUG_stampa_array_giocatori(Giocatore* array_giocatori, int num_giocatori)
{
    int i=0; // Indice per cicli
    for(i=0; i<num_giocatori; i++) // Scorre tutti i giocatori
        DEBUG_stampa_giocatore(array_giocatori[i]);
}


/**
    Procedura che stampa tutte le informazioni di un giocatore

    INPUT: giocatore di cui stampare le informazioni
    OUTPUT: informazioni giocatore a video (nome, segnaposto, budget, n aule,
                                            n scrivanie, n tende)
**/
void DEBUG_stampa_giocatore(Giocatore giocatore)
{
    printf("\n");
    printf("Nome: %s\n", giocatore.nome);
    printf("Segnaposto: %s\n", nome_segnaposto(giocatore.segnaposto));
    printf("Budget: %d\n", giocatore.budget);
    printf("N aule: %d\n", giocatore.n_aule);
    printf("N scriv: %d\n", giocatore.num_scriv);
    printf("N tende: %d\n", giocatore.num_tende);
    printf("\n");
}
