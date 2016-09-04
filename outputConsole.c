/**
    |-------------------------------------------------------------------------------------------|
    | Autore: Mario Taccori, 65234                                                              |
    | Progetto: Monopolycube [progetto finale PR1 - 2015/2016]                                  |
    |                                                                                           |
    | Scopo: file in cui sono definite le procedure utilizzate per la                           |
    |        stampa a video di varie informazioni                                               |
    |                                                                                           |
    | Funzioni definite:                                                                        |
    |       void stampa_budget_tutti(Giocatore* array_giocatori, int num_giocatori)             |
    |       void stampa_giocatore_base(Giocatore giocatore, Casella* tabellone)                 |
    |       void stampa_lista_aule_poss(Nodo* lista, Casella* tabellone, Giocatore giocatore)   |
    |       void stampa_tabellone(Giocatore* array_giocatori, Casella* tabellone)               |
    |       void stampa_destinazione(Giocatore* array_giocatori, Casella* destinazione)         |
    |-------------------------------------------------------------------------------------------|
**/

#include "monopolycube.h" // Inclusione header principale

/**
    Procedura che stampa il budget di ciascun giocatore nell'array_giocatori

    INPUT: array contenente i giocatori, numero giocatori nell'array
    OUTPUT: stampa a video il budget di ciascun giocatore
**/
void stampa_budget_tutti(Giocatore* array_giocatori, int num_giocatori)
{
    int i=0; // Indice per cicli

    printf("\n\n");
    printf("|------------------|\n");
    printf("   ** Budgets ** \n\n");
    for(i=0; i<num_giocatori; i++) // Scorre tutti i giocatori
        printf("%s: %d\n", array_giocatori[i].nome, array_giocatori[i].budget); // Stampa budget giocatore i-esimo
    printf("|------------------|\n");
    printf("\n");
}


/**
    Procedura che stampa nome, segnaposto, budget rimasto e posizione di un giocatore

    INPUT: giocatore di cui stampare le informazioni, tabellone
    OUTPUT: stampa a schermo le informazioni del giocatore
**/
void stampa_giocatore_base(Giocatore giocatore, Casella* tabellone)
{
    printf("|------------------------------------|\n");
    printf("| Nome: %-*s      |\n", MAX_STRINGA-1, giocatore.nome);
    printf("| Segnaposto: %-*s|\n", MAX_STRINGA-1, nome_segnaposto(giocatore.segnaposto));
    printf("| Budget: %-6d                     |\n", giocatore.budget);
    printf("| Posizione: %-*s |\n", MAX_STRINGA-1, tabellone[giocatore.posizione].nome);
    printf("|------------------------------------|");
}


/**
    Procedura che stampa la lista delle aule possedute da un giocatore indicando,
    di ciascuna aula, nome, presenza scrivania (solo se presente), presenza tenda (solo se presente)
    Se la lista è vuota notifica all'utente che il giocatore associato a quella lista non possiede
    nessuna aula.

    INPUT: lista da stampare, tabellone, giocatore proprietario della lista
    OUTPUT: stampa le informazioni di ciascuna aula a schermo, altrimenti notifica che a lista è vuota
**/
void stampa_lista_aule_poss(Nodo* lista, Casella* tabellone, Giocatore giocatore)
{
    Nodo* pos=lista; // Puntatore usato per scorrere una lista

    printf("\n  *** Lista aule di %s (%s) ***\n", giocatore.nome, nome_segnaposto(giocatore.segnaposto)); // Notifica a quale giocatore è associata la lista
    if(lista!=NULL) // Se la lista non è vuota
    {
        while(pos!=NULL) // Scorre tutta la lista
        {
            printf("|----------------------------------------|\n");
            printf("| %-*s                |\n", MAX_STRINGA-1, tabellone[pos->id_casella].nome); // Stampa nome dell'aula

            if(tabellone[pos->id_casella].ha_scrivania) // Se è presente una scrivania
                printf("| E' presente una scrivania!             |\n");
            else if(tabellone[pos->id_casella].ha_tenda) // Se è presente una tenda
                printf("| E' presente una tenda!                 |\n");
            printf("|----------------------------------------|\n");

            pos=pos->next; // Sposta lo scorritore al nodo successivo
        }
    }
    else printf("Non possiedi nessuna aula!"); // Se la lista è vuota, notifica che non si possiedono aule
    printf("\n\n");
} // FINE STAMPA_LISTA_AULE_POSS()


/**
    Procedura che stampa le informazioni di tutte le caselle del tabellone

    INPUT: array giocatori (per nome proprietario e segnaposto), tabellone
    OUTPUT: informazioni di ciascuna casella del tabellone stampate a schermo
**/
void stampa_tabellone(Giocatore* array_giocatori, Casella* tabellone)
{
    int i=0; // Indice per cicli
    for(i=0;i<NUM_CASELLE;i++) // Scorre tutto il tabellone
    {
        printf("|----------------------------------------|\n");
        printf("| %-*s                |\n", MAX_STRINGA-1, tabellone[i].nome); // Stampa il nome della casella

        if(tabellone[i].proprietario!=-1) // Se il proprietario NON è il banco
        {
            printf("| Proprietario: %-*s  |\n", MAX_STRINGA-1, array_giocatori[tabellone[i].proprietario].nome); // Stampa nome proprietario
            printf("| Segnaposto: %-*s    |\n", MAX_STRINGA-1, nome_segnaposto(array_giocatori[tabellone[i].proprietario].segnaposto)); // Stampa segnaposto del proprietario
        }
        else // Se il proprietario è il banco
            printf("| Proprietario: Banco                    |\n");

        if(tabellone[i].ha_scrivania) // Se è presente una scrivania
            printf("| E' presente una scrivania!             |\n");
        else if(tabellone[i].ha_tenda) // Se è presente una tenda
            printf("| E' presente una tenda!                 |\n");
    }
    printf("|----------------------------------------|\n");
}


/**
    Procedura che notifica all'utente su quale casella è finito dopo il lancio dei dadi, stampando
    un informazione diversa in base alla casella su cui si finisce.

    INPUT: array giocatori, puntatore alla casella in cui si è finiti
    OUTPUT: stampa informazioni della casella a schermo
**/
void stampa_destinazione(Giocatore* array_giocatori, Casella* destinazione)
{
    printf("\n\t**** Sei finito in: ****\n");
    printf("|----------------------------------------|\n");
    printf("| %-*s                |\n", MAX_STRINGA-1, destinazione->nome); // Stampa nome della casella
    switch(destinazione->tipo) // In base al tipo della casella...
    {
        case TASSA: // Se la casella è di tipo TASSA
            printf("| Tassa: %-32d|\n", destinazione->pedaggio); // Notifica a schermo l'ammontare del pedaggio da pagare
            printf("|----------------------------------------|\n");
            break;

        case AULA: // Se la casella è di tipo AULA

            if(destinazione->proprietario!=-1) // Se proprietario è diverso da banco
            {
                printf("| Proprietario: %-*s  |\n", MAX_STRINGA-1, array_giocatori[destinazione->proprietario].nome); // Stampa nome giocatore proprietario
                printf("| Segnaposto: %-*s    |\n", MAX_STRINGA-1, nome_segnaposto(array_giocatori[destinazione->proprietario].segnaposto)); // Stampa segnaposto giocatore proprietario
            }
            else
                printf("| Proprietario: Banco                    |\n"); // Se il propetario è il banco, stampa a schermo che il proprietario è il banco

            if(destinazione->ha_scrivania) // Se nella casella è presente una scrivania
                printf("| E' presente una scrivania!             |\n");
            else if(destinazione->ha_tenda) // Se nella casella è presente una tenda
                printf("| E' presente una tenda!                 |\n");

            printf("|----------------------------------------|\n");
            break;

        case SFIGA: // Se la casella è di tipo SFIGA
            printf("|               ,; ,                     |\n");
            printf("|                ;;;.                    |\n");
            printf("|              .;;;;;.                   |\n");
            printf("|             .;;';;';.         (Mother!)|\n");
            printf("|             ;;'     ;       /          |\n");
            printf("|            ,;;  Oo ;;.                 |\n");
            printf("|           .'   ---`-   '_.__           |\n");
            printf("|         ;  .'   ....-```  . `.         |\n");
            printf("|         . '..         ..;`---          |\n");
            printf("|          ``.' ````````'-.'             |\n");
            printf("|             ````````````               |\n");
            printf("|----------------------------------------|\n");
            pausa();
            break;

        case ALTRO: // Se la casella è di tipo ALTRO
            printf("|----------------------------------------|\n");
            break;
    }
}
