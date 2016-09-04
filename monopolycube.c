/**
    |-------------------------------------------------------------------------------------------|
    | Autore: Mario Taccori, 65234                                                              |
    | Progetto: Monopolycube [progetto finale PR1 - 2015/2016]                                  |
    |                                                                                           |
    | Scopo: file in cui sono definite le funzioni/procedure per la gestione del programma,     |
    |        la gestione delle strutture dati (es. le liste), e altre funzioni generiche        |
    |        necessarie al corretto funzionamento del programma                                 |
    |                                                                                           |
    | Funzioni definite:                                                                        |
    |       void elimina_dati_gioco(Game* dg)                                                   |
    |       char* nome_segnaposto(Segnaposto segnaposto)                                        |
    |       Nodo* inserisci_in_testa(Nodo* lista, int id_casella, GameState* stato)             |
    |       Nodo* cancella_nodo_in_testa(Nodo* lista)                                           |
    |       void elimina_lista(Nodo* lista)                                                     |
    |       char menu_si_no()                                                                   |
    |       void pausa()                                                                        |
    |       GameState isPtrValid(void* ptr)                                                     |
    |       int charToInt(char c)                                                               |
    |-------------------------------------------------------------------------------------------|
**/

#include "monopolycube.h" // Inclusione header principale

/**
    Procedura che elimina i dati di gioco dalla struttura di cui si passa un puntatore:
    elimina la lista aule possedute di ciascun giocatore, elimina l'array giocatori,
    elimina il tabellone, elimina l'array delle sfighe e setta i quattro puntatori di questi ultimi a NULL.
    Dunque, azzera tutti gli altri campi della struttura.

    NOTA: solo il campo stato_gioco viene mantenuto

    INPUT: puntatore a struttura di tipo Game da svuotare
    OUTPUT: modifica direttamente la struttura Game, azzerando tutti i campi (tranne stato_gioco) e settando a NULL i puntatori
**/
void elimina_dati_gioco(Game* dg)
{
    int i=0; // Indice per cicli
    GameState stato_gioco_tmp=dg->stato_gioco; // Salva temporaneamente il valore di stato_gioco

    if(dg->array_giocatori!=NULL) // Se esiste un array_giocatori nella struttura
    {
        for(i=0;i<dg->dettagli_partita.num_giocatori;i++) // Scorre tutti i giocatori
            elimina_lista(dg->array_giocatori[i].lista_aule_poss); // Elimina la lista aule possedute di ciascun giocatore

        free(dg->array_giocatori); // Elimina l'array giocatori
    }

    if(dg->tabellone!=NULL) // Se esiste un tabellone nella struttura
        free(dg->tabellone); // Elimina il tabellone

    if(dg->array_sfighe!=NULL) // Se esiste un array_sfighe nella struttura
        free(dg->array_sfighe); // Elimina l'array_sfighe

    *dg=(Game){}; // Reinizializza la struttura a struttura Game vuota

    /** Inizializzazione dei puntatori **/
    dg->array_giocatori=NULL;
    dg->tabellone=NULL;
    dg->array_sfighe=NULL;

    dg->stato_gioco=stato_gioco_tmp; // Ripristino del valore di stato_gioco salvato precedentemente
}


/**
    Funzione che converte un segnaposto di tipo Segnaposto (enum) nella stringa che
    lo rappresenta

    INPUT: segnaposto di tipo Segnaposto (enum) da convertire
    OUTPUT: stringa che rappresenta il segnaposto passato
**/
char* nome_segnaposto(Segnaposto segnaposto)
{
    switch(segnaposto)
    {
        case 0:
            return "Mouse";
            break;
        case 1:
            return "Tastiera";
            break;
        case 2:
            return "Chitarra";
            break;
        case 3:
            return "Gatto";
            break;
        case 4:
            return "Cane";
            break;
        case 5:
            return "Mela";
            break;
        case 6:
            return "Robot";
            break;
        case 7:
            return "Alfiere";
            break;
    }
    return "???"; // Se è stato passato un segnaposto non presente nell'enumerazione Segnaposto
}


/**
    Funzione che inserisce un nuovo nodo con l'informazione specificata in id_casella in testa alla lista
    specificata nel parametro lista.

    NOTA: se si dovesse verificare un errore nell'allocamento del nuovo nodo, lo stato verrà modificato in
          ALLOCATION_FAILURE e la funzione terminata immediatamente

    INPUT: lista in cui inserire, valore contenuto nel nuovo nodo, puntatore alla variabile che indica lo stato del
           programma (per scrivere eventuali errori)
    OUTPUT: puntatore al nuovo nodo (il primo della lista, alla fine della funzione)
**/
Nodo* inserisci_in_testa(Nodo* lista, int id_casella, GameState* stato)
{
    Nodo* tmp=(Nodo*)malloc(sizeof(Nodo)); // Nuovo nodo
    *stato=isPtrValid(tmp);
    if(*stato==ALLOCATION_FAILURE) // Se l'allocamento non è andato a buon fine
        return NULL; // Interrompe prematuramente la funzione

    tmp->id_casella=id_casella; // Assegna al nuovo nodo il valore che deve contenere

    if(lista==NULL) // Se la lista è vuota
    {
        tmp->next=NULL; // tmp è il primo nodo della lista quindi il suo next punta a null
        return tmp; // Restituisce il puntatore al nuovo nodo (ora il primo e unico della lista)
    }
    else // Se la lista NON è vuota
    {
        tmp->next=lista; // Il nuovo nodo punta al primo nodo della lista
        return tmp; // Restituisce il puntatore al nuovo nodo (ora il primo della lista)
    }
}


/**
    Funzione che cancella il primo nodo dalla lista passata come paramentro e restituisce
    il puntatore al secondo nodo (il nuovo primo nodo dopo l'eliminazione)

    INPUT: lista da cui eliminare
    OUTPUT: puntatore al secondo nodo (il nuovo primo nodo)
**/
Nodo* cancella_nodo_in_testa(Nodo* lista)
{
    if(lista!=NULL) //Se la lista NON è vuota
    {
        Nodo* tmp=lista->next; // Salva il puntatore al nodo successivo al primo della lista
                               // in un nodo temporaneo
        free(lista); // Elimina il primo nodo della lista
        return tmp; // Restituisce il puntatore al nodo temporaneo (ora il primo della lista)
    }
    else return lista; // Se la lista è vuota restituisce lista (che vale comunque NULL)
}


/**
    Procedura ricorsiva che elimina una lista.

    INPUT: lista da eliminare
    OUTPUT: tutti i nodi della lista vengono deallocati
**/
void elimina_lista(Nodo* lista)
{
    if(lista!=NULL) // Se la lista non è vuota
    {
        elimina_lista(lista->next); // Chiamata ricorsiva sulla sotto-lista successiva
        free(lista); // Elimina il nodo di questa chiamata
    }
}


/**
    Funzione che chiede all'utente di rispondere a una domanda che implica una
    risposta si/no. Continua a richiedere l'inserimento finchè non viene inserito un
    input valido

    INPUT: nessuno
    OUTPUT: carattere 's' per si, carattere 'n' per no
**/
char menu_si_no()
{
    char scelta[MAX_STRINGA_INPUT]={}; // Utilizzata per memorizzare l'input
    printf("\ns: si\nn: no\n");
    do
    {
        printf("-> ");

        /** Acquisizione scelta **/
        scanf("%[^\n]s", scelta);
        getchar();

        if(strcmp(scelta, "s")!=0 && strcmp(scelta, "n")!=0) // Se l'input inserito non è valido
            printf("\nInput non valido, reinserire\n"); // Chiede il reinserimento
    }
    while(strcmp(scelta, "s")!=0 && strcmp(scelta, "n")!=0); // Finchè non viene inserito un input valido

    return scelta[0]; // Restituisce il carattere ('s' o 'n')
}


/**
    Procedura che chiede all'utente di premere invio per proseguire con l'esecuzione,
    utilizzata per mettere in pausa e permettere di leggere i dati a schermo prima di proseguire

    NOTA: se l'utente inserisce dei caratteri prima di premere invio, questi vengono eliminati

    INPUT: nessuno
    OUTPUT: richiesta di pressione invio
**/
void pausa()
{
    char input; // Utilizzato per verificare che non siano rimasti
                // caratteri nel buffer dopo l'invio

    printf("\n\n");
    printf("Premi invio per continuare..."); // Richiesta di premere invio

    input=getchar(); // Prende il primo carattere nel buffer (e lo rimuove dal buffer)

    if(input!='\n') // Se il primo carattere del buffer NON è \n significa
                    // che potrebbero esserci caratteri indesiderati nel buffer
    {
        while ( getchar() != '\n' ); // Svuota il buffer usando getchar() ripetutamente
                                     // finchè non si incontra \n (viene scartato anche \n)
    }

    printf("\n\n");
}


/**
    Procedura chiamata dopo l'allocamento (o riallocamento) dinamico. Verifica che l'allocamento sia andato a buon fine,
    Restituisce uno stato di errore altrimenti (ALLOCATION_FAILURE)

    INPUT: puntatore all'area allocata
    OUTPUT: stato di errore se il puntatore è NULL (errore -2, ALLOCATION_FAILURE), stato RUN altrimenti
**/
GameState isPtrValid(void* ptr)
{
    if(ptr==NULL) // Se il puntatore non è valido
        return ALLOCATION_FAILURE; // Restituisce un errore di allocazione
    else return RUN; // Se il puntatore è valido, si può proseguire
}


/**
    Funzione che converte un carattere compreso tra '0' e '9' nell'intero che il
    carattere rappresenta. Restituisce -1 se il carattere inserito non è una cifra.

    INPUT: carattere da convertire
    OUTPUT: valore intero rappresentato dal carattere (-1 se il carattere inserito non è una cifra)
**/
int charToInt(char c)
{
    if(c>=48 && c<=57) // Se il carattere è compreso tra '0' e '9'
        return c-48; // restituisce il numero che il carattere rappresenta (-48 è lo scarto sulla tabella ASCII)
    else return -1; // Se il carattere passato come parametro non è una cifra restituisce -1
}
