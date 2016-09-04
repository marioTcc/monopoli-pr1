/**
    |-------------------------------------------------------------------------------------------------------------------------------|
    | Autore: Mario Taccori, 65234                                                                                                  |
    | Progetto: Monopolycube [progetto finale PR1 - 2015/2016]                                                                      |
    |                                                                                                                               |
    | Scopo: file in cui sono definite le funzioni utilizzate per la                                                                |
    |        preparazione di una partita nuova o caricata                                                                           |
    |                                                                                                                               |
    | Funzioni definite:                                                                                                            |
    |       void menu_iniziale(Game* dati_gioco)                                                                                    |
    |       void menu_nuova_partita(Game* dati_gioco)                                                                               |
    |       Game nuova_partita(int num_giocatori, _Bool bonus_mode)                                                                 |
    |       Game carica_partita()                                                                                                   |
    |       Partita inizializza_partita(int num_giocatori)                                                                          |
    |       Giocatore* inizializza_giocatori(int num_giocatori, GameState* stato_gioco)                                             |
    |       void inizializza_budget(Casella* tabellone, Giocatore* array_giocatori, int num_giocatori)                              |
    |       Casella* inizializza_tabellone(GameState* stato_gioco)                                                                  |
    |       void inizializza_aule(Casella* tabellone, Giocatore* array_giocatori, int num_giocatori, GameState* stato)              |
    |       void inizializza_lista_aule_poss(Casella* tabellone, Giocatore* array_giocatori, int num_giocatori, GameState* stato)   |
    |       Carta_sfiga* inizializza_sfighe(Game* dg)                                                                               |
    |-------------------------------------------------------------------------------------------------------------------------------|
**/

#include "monopolycube.h" // Inclusione header principale del progetto

/**
    Procedura che stampa il menù iniziale del gioco e acquisisce le scelte iniziali dell'utente. Dunque chiama
    funzioni/procedure necessarie alla preparazione dei dati in base alle scelte effettuate.
    Se viene selezionato di uscire, setta stato_gioco a EXIT, in modo che il programma sappia che deve terminare (senza errore)
    Se viene selezionato nuova partita, viene chiamato il menù di nuova partita
    Se viene selezionato carica partita viene chiamata la funzione per il caricamento

    INPUT: puntatore alla struttura Game che conterrà i dati necessari alla partita
    OUTPUT: nessuno
**/
void menu_iniziale(Game* dati_gioco)
{
    char scelta[MAX_STRINGA_INPUT]={}; // Valore selezionato nel menù iniziale

    /** MENU INIZIALE --------------------------- **/
    printf("\nMenu': \n");
    printf("1: Nuova partita\n");
    printf("2: Carica partita\n\n");
    printf("exit: Esci\n\n");
    printf("-> ");
    /** ------------------------------------------**/

    do
    {
        scanf("%[^\n]s", scelta);
        getchar();

        if(strcmp(scelta, "exit")==0) // Se è stato selezionato di uscire
            dati_gioco->stato_gioco=EXIT; // Segnala che il gioco va terminato
        else if(strcmp(scelta, "1")==0) // Se è stato selezionato nuova partita
            menu_nuova_partita(dati_gioco); // Chiama il menù per la creazione di una nuova partita
        else if(strcmp(scelta, "2")==0) // Se è stato selezionato carica partita
            *dati_gioco=carica_partita(); // Chiama la funzione per il caricamento della partita

        if(strcmp(scelta, "1")!=0 && strcmp(scelta, "2")!=0 && strcmp(scelta, "exit")!=0) // Se non è stato inserito un input valido
            printf("Valore non valido, reinserire\n-> ");
    }
    while(strcmp(scelta, "1")!=0 && strcmp(scelta, "2")!=0 && strcmp(scelta, "exit")!=0); // Finchè non viene inserito un input valido
}


/**
    Procedura che visualizza il menù per la scelta del numero dei giocatori e della modalità della partita,
    dunque, acquisiti i dati necessari, chiama l'inizializzatore dei dati di gioco passandogli i dati acquisiti.
    Durante l'acquisizione verifica che l'input sia valido. Alla fine scrive i dati ottenuti/calcolati nella
    struttura passata come parametro

    INPUT: puntatore a struttura Game, destinataria dei dati
    OUTPUT: scrive nella struttura Game passata come parametro i dati ottenuti/calcolati
**/
void menu_nuova_partita(Game* dati_gioco)
{
    char _num_giocatori[MAX_STRINGA_INPUT]={}; // Valore di transizione per num_giocatori
    int num_giocatori=0; // Contiene il numero dei giocatori

    _Bool bonus_mode=false; // Specifica la modalità della partita
    char _bonus_mode[MAX_STRINGA_INPUT]={}; // Valore di transizione per bonus_mode (non si può acquisire _Bool con scanf)


    /** Acquisizione del numero di giocatori **/
    /// Se l'input non è valido chiede il reinserimento
    /// L'input è valido se formato da una sola cifra compresa tra 2 e 8 (estremi compresi)
    printf("\nInserire numero giocatori (min 2 - max 8): \n\n");
    printf("-> ");

    do
    {
        scanf("%[^\n]s", _num_giocatori);
        getchar();

        if((charToInt(_num_giocatori[0])<2 || charToInt(_num_giocatori[0])>8) || _num_giocatori[1]!='\0') // Se l'input inserito non è valido
            printf("Valore inserito non valido, reinserire\n-> ");
    }
    while(charToInt(_num_giocatori[0])<2 || charToInt(_num_giocatori[0])>8 || _num_giocatori[1]!='\0'); // Finchè non è inserito un input valido

    /** Conversione da char* a int **/
    num_giocatori=charToInt(_num_giocatori[0]); // Conversione numero da char a corrispondente int
    // FINE ACQ. NUM. GIOCATORI


    /** Acquisizione della modalità di partita desiderata **/
    printf("\nAvviare la partita in modalita' classica o bonus? \n");
    printf("\nc: Classica");
    printf("\nb: Bonus\n\n");
    printf("-> ");

    do
    {
        scanf("%[^\n]s", _bonus_mode);
        getchar();

        if(strcmp(_bonus_mode, "c")!=0 && strcmp(_bonus_mode, "b")!=0) // Se l'input non è valido
            printf("Valore non valido, reinserire\n-> ");
    }
    while(strcmp(_bonus_mode, "c")!=0 && strcmp(_bonus_mode, "b")!=0); // Finchè non viene inserito un input valido

    /** Conversione da char* a _Bool **/
    if(strcmp(_bonus_mode, "c")==0)
        bonus_mode=false;
    else bonus_mode=true;
    // FINE ACQ. MOD. PARTITA

    *dati_gioco=nuova_partita(num_giocatori, bonus_mode); // Scrive nella struttura passata come parametro i dati ottenuti/calcolati
                                                          // dall'inizializzatore di nuova partita
} // FINE MENU_NUOVA_PARTITA()


/**
    Procedura chiamata per iniziare una partita completamente nuova. Inizializza tutti i dati necessari allo svolgimento
    della partita e restituisce la struttura creata

    INPUT: numero giocatori partecipanti alla partita e indicatore modalità della partita (false: partita classica, true: modalità bonus)
    OUTPUT: struttura Game contenente tutti i dati di gioco necessari allo svolgimento della partita
**/
Game nuova_partita(int num_giocatori, _Bool bonus_mode)
{
    Game dati_gioco={}; // Variabile Game vuota(da restituire alla fine)
    dati_gioco.stato_gioco=RUN; // Iniz. campo stato_gioco a RUN

    /** Caricamento sfighe **/
    /// Alla fine del caricamento delle sfighe, stato_gioco varrà ancora RUN se andato a buon fine;
    /// EXIT_FAIL se non è andato a buon fine (è un errore irreversibile, impone terminazione)
    /// ALLOCATION_FAILURE in caso di errore nella creazione dell'array
    dati_gioco.array_sfighe=inizializza_sfighe(&dati_gioco); // Carica l'array con le sfighe
    if(dati_gioco.stato_gioco!=RUN) // Se si è verificato un errore
        return dati_gioco; // Termina la funzione (non ha senso proseguire)


    /** Inizializzazione dati di gioco **/
    dati_gioco.dettagli_partita=inizializza_partita(num_giocatori); // Inizializzazione della partita

    dati_gioco.tabellone=inizializza_tabellone(); // Inizializzazione del tabellone
    if(dati_gioco.stato_gioco!=RUN) // Se si è verificato un errore
        return dati_gioco; // Termina la funzione (non ha senso proseguire)

    dati_gioco.array_giocatori=inizializza_giocatori(num_giocatori, &dati_gioco.stato_gioco); // Inizializzazione dei giocatori
    if(dati_gioco.stato_gioco!=RUN) // Se si è verificato un errore
        return dati_gioco; // Termina la funzione (non ha senso proseguire)

    dati_gioco.bonus_mode=bonus_mode; // Salva indicatore modalità bonus

    if(!dati_gioco.bonus_mode) // Solo se NON in modalità bonus
    {
        inizializza_aule(dati_gioco.tabellone, dati_gioco.array_giocatori, dati_gioco.dettagli_partita.num_giocatori, &dati_gioco.stato_gioco); // Assegna casualmente le aule ai giocatori
        if(dati_gioco.stato_gioco!=RUN) // Se si è verificato un errore
            return dati_gioco; // Termina la funzione (non ha senso proseguire)
    }

    inizializza_lista_aule_poss(dati_gioco.tabellone, dati_gioco.array_giocatori, dati_gioco.dettagli_partita.num_giocatori, &dati_gioco.stato_gioco); // Genera le liste delle aule possedute da ciascun giocatore
    if(dati_gioco.stato_gioco!=RUN) // Se si è verificato un errore
        return dati_gioco; // Termina la funzione (non ha senso proseguire)

    inizializza_budget(dati_gioco.tabellone, dati_gioco.array_giocatori, dati_gioco.dettagli_partita.num_giocatori); // Calcola e assegna un budget iniziale ai giocatori

    return dati_gioco; // Restituzione della struttura creata
} // FINE NUOVA_PARTITA()


/**
    Procedura chiamata per caricare una partita salvata precedentemente in un file binario. Carica: partita, array giocatori
    tabellone e indicatore di modalità dal file. Dunque, genera le liste con le aule possedute da ciascun giocatore, conta le tende
    e scrivanie che possiedono, e carica l'array contenente le sfighe.

    INPUT: nessuno
    OUTPUT: struttura Game con tutti i dati necessari allo svolgimento della partita
**/
Game carica_partita()
{
    int i=0; // Indice per cicli
    Nodo* scorritore=NULL; // Puntatore per scorrere una lista
    Game dati_gioco={}; // Variabile Game vuota(da restituire alla fine)
    dati_gioco.stato_gioco=RUN; // Inizializzazione stato_gioco

    /** Caricamento dati di gioco **/
    /// Alla fine del caricamento stato_gioco varrà RUN se il caricamento è andato a buon fine;
    /// EXIT o FORCE_NEW se errore nell'apertura del file di salvataggio
    /// ALLOCATION_FAILURE se errore nell'allocamento delle risorse necessarie
    dati_gioco=carica_dati(); // Carica partita, array giocatori, tabellone, indicatore modalità
    if(dati_gioco.stato_gioco!=RUN) // Se si è verificato un errore
        return dati_gioco; // Termina la funzione (non ha senso proseguire)

    /** Caricamento sfighe **/
    /// Alla fine del caricamento delle sfighe, stato_gioco varrà ancora RUN se andato a buon fine;
    /// EXIT_FAIL se non è andato a buon fine (è un errore irreversibile, impone terminazione)
    /// ALLOCATION_FAILURE in caso di errore nella creazione dell'array
    dati_gioco.array_sfighe=inizializza_sfighe(&dati_gioco); // Carica l'array con le sfighe
    if(dati_gioco.stato_gioco!=RUN) // Se si è verificato un errore
        return dati_gioco; // Termina la funzione (non ha senso proseguire)

    /** Inizializzazione lista aule possedute e conteggio immobili **/
    /// Le inizializzazioni seguenti vengono effettuate solo se i caricamenti precedenti sono andati a buon fine

    /** Iniz. lista aule possedute **/
    inizializza_lista_aule_poss(dati_gioco.tabellone, dati_gioco.array_giocatori, dati_gioco.dettagli_partita.num_giocatori, &dati_gioco.stato_gioco); // Genera le liste delle aule possedute da ciascun giocatore
    if(dati_gioco.stato_gioco!=RUN) // Se si è verificato un errore
        return dati_gioco; // Termina la funzione (non ha senso proseguire)

    /** Conta tende e scrivanie **/
    /// Scorre la lista di aule possedute da ciascun giocatore e per ciascuna aula valuta la presenza
    /// di scrivania/tenda; dunque, eventualmente,  incrementa il contatore relativo.
    for(i=0; i<dati_gioco.dettagli_partita.num_giocatori; i++)
    {
        scorritore=dati_gioco.array_giocatori[i].lista_aule_poss; // Posiziona lo scorritore all'inizio della lista del giocatore i-esimo
        while(scorritore!=NULL) // Scorre tutta la lista
        {
            if(dati_gioco.tabellone[scorritore->id_casella].ha_scrivania==true) // Se nella casella è presente una scrivania
                dati_gioco.array_giocatori[i].num_scriv++; // Incrementa il numero di scrivanie possedute dal giocatore i-esimo

            if(dati_gioco.tabellone[scorritore->id_casella].ha_tenda==true) // Se nella casella è presente una tenda
                dati_gioco.array_giocatori[i].num_tende++; // Incrementa il numero di tende possedute dla giocatore i-esimo

            scorritore=scorritore->next; // Sposta lo scorritore al nodo successivo
        }
    } // FINE CONTEGGI SCRIV/TENDE

    return dati_gioco; // Restituzione della struttura creata
} // FINE CARICA_PARTITA()


/**
    Funzione che crea, inizializza e restituisce la struttura contenente i dettagli generali di una partita.

    INPUT: numero giocatori in partita
    OUTPUT: struttura di tipo Partita
**/
Partita inizializza_partita(int num_giocatori)
{
    /** Creazione struttura **/
    Partita partita;

    /** Inizializzazione dei campi **/
    partita.num_giocatori=num_giocatori;
    partita.num_turni_giocati=0;
    partita.turno_giocatore=0;

    return partita; // Restituisce la struttura creata
}


/**
    Funzione che crea l'array contenente i giocatori. Per ogni giocatore viene richiesto all'utente di inserire
    il nome del giocatore e di scegliere un segnaposto. Dunque vengono inizializzati tutti gli altri campi.

    NOTA: in caso di errore di creazione dell'array la funzione termina scrivendo l'errore ALLOCATION_FAILURE nel campo relativo
    NOTA2: se viene inserito un nome giocatore più lungo della dimensione consentita, il nome viene troncato alla dimensione prevista

    INPUT: numero giocatori in partita, puntatore alla variabile che indica lo stato del programma
    OUTPUT: array di strutture Giocatore
**/
Giocatore* inizializza_giocatori(int num_giocatori, GameState* stato_gioco)
{
    int i=0, j=0; // Indici per cicli
    int counter=1; // Indica a terminale quale giocatore si sta creando
    int num_segnaposti_disponibili=NUM_SEGNAPOSTI; // Contiene il numero di segnaposti ancora disponibili
    int segnaposto_scelto=0; // Usato per acquisire il segnaposto
    char _segnaposto_scelto[MAX_STRINGA_INPUT]={}; // Variabile transitoria per segnaposto_scelto (per validazione)
    char _nome_giocatore[MAX_STRINGA_INPUT]={}; // Variabile transitoria per nome giocatore (per eventuale troncamento)

    Nodo* lista_segnaposti_disp=NULL; // Lista che contiene i segnaposti ancora disponibili
    Nodo* scorritore=NULL; // Puntatore utilizzato per scorrere una lista
    Nodo* nodo_precedente=NULL; // Punta al nodo precedente di quello a cui punta scorritore

    /** Creazione dell'array giocatori **/
    Giocatore* array_giocatori=(Giocatore*) malloc(sizeof(Giocatore)*num_giocatori);
    *stato_gioco=isPtrValid(array_giocatori); // Verifica che l'allocamento sia andato a buon fine
    if(*stato_gioco!=RUN) // Se l'allocamento non è andato a buon fine
        return NULL; // Termina l'esecuzione della funzione

    /** Riempimento della lista dei segnaposti disponibili **/
    for(i=NUM_SEGNAPOSTI; i>=0; i--) // Scorre tutti i segnaposti...
    {
        lista_segnaposti_disp=inserisci_in_testa(lista_segnaposti_disp, i, stato_gioco); // ...e li inserisce nella lista
        if(*stato_gioco!=RUN) // Se si è verificato un errore
            return NULL; // Termina la funzione (non ha senso proseguire)
    }

    scorritore=lista_segnaposti_disp; // Sposta lo scorritore all'inizio della lista

    /** Creazione giocatori **/
    for(i=0; i<num_giocatori; i++) // Scorre tutti i giocatori
    {
        /** Richiesta inserimento nome giocatore i-esimo **/
        /// Se non è inserito un input valido chiede che venga reinserito.
        /// Qualsiasi input è valido purchè non vuoto. Se eccede i 23 caratteri viene troncato a 23 caratteri
        strcpy(array_giocatori[i].nome,""); // Inizializzazione nome del giocatore i-esimo a stringa vuota
        printf("\nNome giocatore %d: ", counter);

        do
        {
            //scanf("%[^\n]23s %*s", array_giocatori[i].nome); // Memorizza i primi 23 caratteri e scarta tutti gli eventuali altri (previene overflow)

            /** Acquisizione nome giocatore **/
            scanf("%[^\n]s", _nome_giocatore);
            getchar();

            strncpy(array_giocatori[i].nome, _nome_giocatore, MAX_STRINGA); // Copia dalla stringa temporanea massimo MAX_STRINGA caratteri
            array_giocatori[i].nome[MAX_STRINGA]='\0'; // Aggiunge il carattere di fine stringa nell'ultima cella dell'array che
                                                       // rappresenta la stringa (per sicurezza)

            if(array_giocatori[i].nome[0]=='\0')
                printf("\nInserire un nome!\n-> ");
        }
        while(array_giocatori[i].nome[0]=='\0');

        /** Scelta segnaposto **/
        /// Modus operandi: ottenuta precedentemente la lista dei segnaposti disponibili (si utilizza
        ///                 la stessa lista delle aule, considerandola come generica lista di interi),
        ///                 questa viene stampata a terminale con i segnaposti numerati da 0 a n.
        ///                 Una volta che l'utente avrà scelto un numero (valido) si scorrerà la lista
        ///                 fino al nodo con quel numero (ad esempio il nodo numero 2, quindi il terzo, e NON il secondo)
        ///                 e si assegnerà quel segnaposto al giocatore i-esimo. Il segnaposto scelto verrà dunque
        ///                 rimosso dalla lista di quelli disponibili, e il numero di segnaposti ancora assegnabili
        ///                 decrementato. Se viene inserito un input non valido, quindi caratteri casuali o numeri
        ///                 esterni al range 0-n-segnaposti, viene assegnato un segnaposto casuale.
        ///                 Se dovessero avanzare segnaposti alla fine della creazione di tutti i giocatori, la lista
        ///                 verrà comunque eliminata.

        printf("\n%s, scegli il tuo segnaposto: ", array_giocatori[i].nome);

        /** Stampa segnaposti ancora disponibili **/
        scorritore=lista_segnaposti_disp; // Sposta lo scorritore allìinizio della lista
        for(j=0; j<num_segnaposti_disponibili; j++) // Scorre tutti i segnaposti disponibili
        {
            printf("\n%d - %s", j, nome_segnaposto(scorritore->id_casella)); // Stampa il segnaposto
            scorritore=scorritore->next; // Sposta lo scorritore al nodo successivo
        }
        printf("\n -> ");

        /** Assegnamento segnaposto scelto al giocatore i-esimo **/
        scanf("%[^\n]s", _segnaposto_scelto); // Memorizza i primi due caratteri, scarta tutto il resto
        getchar();

        /** Validazione input **/
        /// Un input è vaido se formato da una cifra tra 2 e 8 (estremi compresi) e il simbolo di fine stringa subito dopo
        if( charToInt(_segnaposto_scelto[0]) < 0 ||
            charToInt(_segnaposto_scelto[0]) > num_segnaposti_disponibili-1 ||
            _segnaposto_scelto[1]!='\0') // Se NON è stato selezionato un valore di menù valido
                                         // o all'interno del range di segnaposti disponibili
        {
            printf("\nTe ne sara' assegnato uno a caso!\n\n");
            segnaposto_scelto=rand()%num_segnaposti_disponibili; // Sceglie un segnaposto a caso tra quelli ancora disponibili
        }
        else segnaposto_scelto=charToInt(_segnaposto_scelto[0]); // Se è stato selezionato un valore valido


        if(segnaposto_scelto!=0) // Se il segnaposto scelto non è quello in posizione 0
        {
            scorritore=lista_segnaposti_disp; // Riporta lo scorritore all'inizio della lista

            for(j=1; j<=segnaposto_scelto; j++) // Scorre la lista fino al numero di nodo estratto (al nodo con "indice" selettore random)
            {
                nodo_precedente=scorritore; // Memorizza il nodo attuale...
                scorritore=scorritore->next; // ...e sposta lo scorritore al successivo
            }
            //NOTA: alla fine del ciclo scorritore punta al nodo col segnaposto selezionato

            array_giocatori[i].segnaposto=scorritore->id_casella; // Assegna il segnaposto scelto al giocatore i-esimo
            nodo_precedente->next=cancella_nodo_in_testa(nodo_precedente->next); // Elimina il nodo corrispondente al segnaposto scelto
        }
        else
        {
            array_giocatori[i].segnaposto=lista_segnaposti_disp->id_casella; // Assegna il segnaposto scelto al giocatore i-esimo
            lista_segnaposti_disp=cancella_nodo_in_testa(lista_segnaposti_disp); // Altrimenti elimina dalla lista il segnaposto in
                                                                                 // posizione 0
        }

        num_segnaposti_disponibili--; // Un segnaposto è stato assegnato, ce n'è uno in meno
        // FINE SCELTA SEGNAPOSTO


        /** Inizializzazione altri valori della struttura **/
        array_giocatori[i].budget=0;
        array_giocatori[i].n_aule=0;
        array_giocatori[i].posizione=0;
        array_giocatori[i].pulizia_bagni=false;
        array_giocatori[i].turni_pulizia_bagni=0;
        array_giocatori[i].num_scriv=0;
        array_giocatori[i].num_tende=0;
        array_giocatori[i].lista_aule_poss=NULL;

        /** Preparazione all'iterazione successiva **/
        counter++; // Identifica il giocatore successivo
    } // FINE FOR CREAZIONE GIOCATORI

    /** Eliminazione lista dei segnaposti rimasti **/
    elimina_lista(lista_segnaposti_disp); // Elimina la lista coi segnaposti avanzati (non serve più)

    return array_giocatori;
} // FINE INIZIALIZZA_GIOCATORI()


/**
    Procedura che calcola e assegna il budget iniziale a ciascun giocatore secondo la formula [(12000/numero giocatori)-(valore aule possedute)]
    Il risultato è scritto direttamente nella struttura Giocatore di ogni giocatore presente nell'array.

    NOTA: in caso di errore di creazione dell'array la funzione termina scrivendo l'errore ALLOCATION_FAILURE nel campo relativo

    INPUT: tabellone, array dei giocatori, numero giocatori
    OUTPUT: scrive nell'array giocatori, per ciascun giocatore, il budget che gli è stato assegnato
**/
void inizializza_budget(Casella* tabellone, Giocatore* array_giocatori, int num_giocatori)
{
    int i=0; // Variabile per ciclo
    Nodo* scorritore_lista = NULL; // Puntatore a nodo utilizzato per scorrere una lista
    int totale=0; // Variabile utilizzata per contare il valore totale delle aule di un giocatore


    for(i=0;i<num_giocatori;i++) // Scorre tutti i giocatori
    {
        scorritore_lista=array_giocatori[i].lista_aule_poss; // Porta lo scorritore all'inizio della lista del giocatore i-esimo
        totale=0; // Azzera il totale del conteggio precedente

        /** Calcolo valore totale aule possedute dal giocatore i-esimo **/
        while(scorritore_lista!=NULL) // Finchè non si avrà scorso tutta la lista del giocatore i-esimo
        {
            totale+=tabellone[scorritore_lista->id_casella].costo; // Aggiunge al totale il valore dell'aula nel nodo attuale della lista
            scorritore_lista=scorritore_lista->next; // Sposta lo scorritore al nodo successivo
        }

        /** Calcolo e assegnamento budget al giocatore i-esimo **/
        array_giocatori[i].budget=((12000/num_giocatori)-(totale)); // Assegna al giocatore i-esimo il budget calcolato
    }
} // FINE INIZIALIZZA_BUDGET()


/**
    Questa funzione alloca dinamicamente un array di 40 caselle (il tabellone) e carica tutti i dati di tutte
    le caselle del tabellone. In lista di inizializzazione di ciascuna casella ci sono i valori base di quella casella.
    Un ciclo, poi, calcola e scrive sul tabellone i pedaggi nel caso siano o meno presenti scrivania/tenda, imposta a
    false la presenza di scrivania e tenda (inizialmente non ce ne sono in nessuna casella) e assegna tutte le caselle
    al banco.

    NOTA: in caso di errore, "stato" varrà ALLOCATION_FAILURE e la funzione non verrà completata

    INPUT: puntatore alla variabile che indica lo stato del programma
    OUTPUT: Puntatore alla prima cella dell'array creato (il tabellone)
**/
Casella* inizializza_tabellone(GameState* stato_gioco)
{
    int i=0; // Indice per cicli

    /** Creazione tabellone **/
    Casella* tabellone=(Casella*) malloc(sizeof(Casella)*NUM_CASELLE);
    *stato_gioco=isPtrValid(tabellone); // Verifica che l'allocamento sia andato a buon fine
    if(*stato_gioco!=RUN) // Se l'allocamento non è andato a buon fine
        return NULL; // Termina l'esecuzione della funzione

    /** Inizializzazione tabellone **/
                         // costo - ped - ped_scriv - ped_tenda -    nome      -         tipo
    tabellone[0]=(Casella)  {  0,    0,     0,           0,       "VIA!",               ALTRO  };
    tabellone[1]=(Casella)  { 60,    0,     0,           0,       "AULA G",             AULA   };
    tabellone[2]=(Casella)  {  0,    0,     0,           0,       "SFIGA!",             SFIGA  };
    tabellone[3]=(Casella)  { 60,    0,     0,           0,       "AULA C",             AULA   };
    tabellone[4]=(Casella)  { 80,    0,     0,           0,       "AULA F",             AULA   };
    tabellone[5]=(Casella)  {  0,  100,     0,           0,       "TASSE!",             TASSA  };
    tabellone[6]=(Casella)  {100,    0,     0,           0,       "AULA STUDIO",        AULA   };
    tabellone[7]=(Casella)  {  0,    0,     0,           0,       "SFIGA!",             SFIGA  };
    tabellone[8]=(Casella)  {100,    0,     0,           0,       "SIMAZ",              AULA   };
    tabellone[9]=(Casella)  {120,    0,     0,           0,       "LAB T",              AULA   };

                         // costo - ped - ped_scriv - ped_tenda -    nome      -         tipo
    tabellone[10]=(Casella) {  0,    0,     0,           0,       "BAGNI",              ALTRO  };
    tabellone[11]=(Casella) {140,    0,     0,           0,       "VAX",                AULA   };
    tabellone[12]=(Casella) {  0,    0,     0,           0,       "SFIGA!",             SFIGA  };
    tabellone[13]=(Casella) {140,    0,     0,           0,       "G.A.R.",             AULA   };
    tabellone[14]=(Casella) {160,    0,     0,           0,       "BIBLIOTECA",         AULA   };
    tabellone[15]=(Casella) {  0,  150,     0,           0,       "TASSE!",             TASSA  };
    tabellone[16]=(Casella) {180,    0,     0,           0,       "AULA B",             AULA   };
    tabellone[17]=(Casella) {  0,    0,     0,           0,       "SFIGA!",             SFIGA  };
    tabellone[18]=(Casella) {180,    0,     0,           0,       "SEGRETERIA",         AULA   };
    tabellone[19]=(Casella) {200,    0,     0,           0,       "AULA A",             AULA   };

                         // costo - ped - ped_scriv - ped_tenda -    nome      -         tipo
    tabellone[20]=(Casella) {  0,    0,     0,           0,       "CORRIDOIO",          ALTRO  };
    tabellone[21]=(Casella) {220,    0,     0,           0,       "UFFICIO SCATENI",    AULA   };
    tabellone[22]=(Casella) {  0,    0,     0,           0,       "SFIGA!",             SFIGA  };
    tabellone[23]=(Casella) {220,    0,     0,           0,       "BAGNO PROFESSORI",   AULA   };
    tabellone[24]=(Casella) {240,    0,     0,           0,       "AULA D",             AULA   };
    tabellone[25]=(Casella) {  0,  250,     0,           0,       "TASSE!",             TASSA  };
    tabellone[26]=(Casella) {260,    0,     0,           0,       "LAB 3",              AULA   };
    tabellone[27]=(Casella) {  0,    0,     0,           0,       "SFIGA!",             SFIGA  };
    tabellone[28]=(Casella) {260,    0,     0,           0,       "LAB 4",              AULA   };
    tabellone[29]=(Casella) {280,    0,     0,           0,       "LAB M",              AULA   };

                         // costo - ped - ped_scriv - ped_tenda -    nome      -         tipo
    tabellone[30]=(Casella) {  0,    0,     0,           0,       "BATCAVERNA!",        ALTRO  };
    tabellone[31]=(Casella) {300,    0,     0,           0,       "AULA M FISICA",      AULA   };
    tabellone[32]=(Casella) {  0,    0,     0,           0,       "SFIGA!",             SFIGA  };
    tabellone[33]=(Casella) {300,    0,     0,           0,       "AULA M CHIMICA",     AULA   };
    tabellone[34]=(Casella) {320,    0,     0,           0,       "AULA M MATEMATICA",  AULA   };
    tabellone[35]=(Casella) {  0,  350,     0,           0,       "TASSE!",             TASSA  };
    tabellone[36]=(Casella) {360,    0,     0,           0,       "BAR ROTONDO",        AULA   };
    tabellone[37]=(Casella) {  0,    0,     0,           0,       "SFIGA!",             SFIGA  };
    tabellone[38]=(Casella) {360,    0,     0,           0,       "AULA ANATOMICA",     AULA   };
    tabellone[39]=(Casella) {400,    0,     0,           0,       "AULA COSTA",         AULA   };

    /** Calcolo pedaggi e inizializzazione ultimi campi **/
    for(i=0; i<NUM_CASELLE; i++) // Scorre tutto il tabellone
    {
        if(tabellone[i].tipo==AULA) // Se la casella i-esima è di tipo aula
        {
            tabellone[i].pedaggio=tabellone[i].costo/20; // Pedaggio senza tenda nè scrivania (formula: costo_aula/20)
            tabellone[i].pedaggio_scriv=tabellone[i].costo/10; // Pedaggio con scrivania (formula: costo_aula/10)
            tabellone[i].pedaggio_tenda=tabellone[i].costo/5; // Pedaggio con tenda (formula: costo_aula/5)
        }

        tabellone[i].ha_scrivania=false; // Inizialmente non ci sono scrivanie
        tabellone[i].ha_tenda=false; // Inizialmente non ci sono tende
        tabellone[i].proprietario=-1; // Inizialmente il tabellone appartiene tutto al banco
    }

    return tabellone; // Restituisce il puntatore al tabellone
} // FINE INIZIALIZZA_TABELLONE()


/**
    Procedura chiamata solo in caso di nuova partita in modalità classica. Assegna casualmente tutte le aule disponibili
    ai giocatori. C'è una probabilità su tre che l'aula venga assegnata al giocatore con una scrivania gratuita già costruita.

    NOTA: in caso di errore "stato" varrà ALLOCATION_FAILURE e la procedura non verrà completata

    INPUT: tabellone, array giocatori, numero giocatori, puntatore alla variabile che indica lo stato del programma
    OUTPUT: scrive il proprietario (il giocatore a cui è stata assegnata) di un'aula nel tabellone, scrive il numero di aule
            possedute (ed eventualmente incrementa il numero di scrivanie possedute) nelle strutture Giocatore all'interno dell'array giocatori
**/
void inizializza_aule(Casella* tabellone, Giocatore* array_giocatori, int num_giocatori, GameState* stato)
{
    Nodo* lista_aule=NULL; // Lista che contiene le aule vendibili del tabellone
    Nodo* scorritore_lista=NULL; // Puntatore a nodo usato per scorrere una lista

    Nodo* nodo_precedente=NULL; // Puntatore a nodo usato per conservare l'indirizzo
                                // del nodo precedente a un nodo da eliminare

    int num_aule_disponibili=0; // Numero aule assegnabili rimaste
    int num_aule_per_giocatore=0; // Numero aule da assegnare a ciascun giocatore

    int selettore_random=0; // Seleziona (casualmente) l'aula da assegnare al giocatore corrente

    int k=0, i=0, j=0; // Indici per cicli


    /** Estrae dal tabellone la lista delle aule assegnabili ai giocatori e il loro numero **/
    for(i=0;i<NUM_CASELLE && *stato==RUN ;i++) // Scorre tutte le caselle del tabellone (si ferma se avviene un errore)
    {
        if(tabellone[i].tipo==AULA && tabellone[i].proprietario==-1) // La casella corrente, è di tipo aula con proprietario banco?
                                                                     // (e quindi assegnabile)
        {
            lista_aule=inserisci_in_testa(lista_aule, i, stato); // Lista aule assegnabili
            num_aule_disponibili++; // Contatore aule disponibili all'assegnamento
        }
    }

    if(*stato==RUN) // Se non ci sono stati errori
    {
        /**         ASSEGNAMENTO AULE           **/
        /// Modus operandi: avendo già generato una lista delle aule assegnabili e ottenuto il loro numero, la porzione di codice seguente,
        ///                 scorrendo tutti i giocatori con un ciclo, assegna <num_aule_per_giocatore> a ciascun giocatore utilizzando
        ///                 un altro ciclo più interno.
        ///                 Ad ogni iterazione del ciclo più interno, un'aula viene scelta casualmente e assegnata al giocatore
        ///                 in quel momento "selezionato" dal ciclo più esterno. L'aula selezionata viene quindi rimossa dalla lista
        ///                 di quelle disponibili e il contatore delle aule disponibili decrementato.
        ///                 Se, alla fine dell'assegnamento iniziale, sono avanzate aule nella lista delle aule disponibili,
        ///                 anche queste verranno assegnate: avendo cura di assegnarne massimo una per giocatore, finchè non sono state assegnate tutte,
        ///                 viene selezionato un giocatore casualmente, e dopo aver verificato che non sia già stato estratto in precedenza,
        ///                 gli viene assegnata la prima aula della lista. Nel codice, si tiene anche conto del numero totale di aule possedute
        ///                 da ciascun giocatore incrementando il corrispondente contatore ogni volta che un'aula gli viene assegnata, e del contatore
        ///                 di scrivanie possedute, che viene incrementato nel caso un aula venga assegnata con una scrivania già costruita (1 probabilità su 3).

        scorritore_lista=lista_aule; // Sposta lo scorritore della lista sul primo nodo
        num_aule_per_giocatore=(int)(num_aule_disponibili/num_giocatori); // Calcola quante aule devono essere assegnate a ogni giocatore

        /** Estrazione casuale e assegnamento aule **/
        for(k=0; k<num_giocatori; k++) // Scorre tutti i giocatori
        {
            for(array_giocatori[k].n_aule=0; array_giocatori[k].n_aule<num_aule_per_giocatore; array_giocatori[k].n_aule++) // Scorre finchè al giocatore corrente è stato assegnato
                                                                                                                            // il numero di aule che gli spettano (ad ogni iterazione
                                                                                                                            // incrementa anche il conteggio delle aule che possiede)
            {
                selettore_random=rand()%(num_aule_disponibili); // Estrae un numero a caso tra 0 e num_aule_disponibili-1
                for(j=1; j<=selettore_random; j++) // Scorre la lista fino al numero di nodo estratto (al nodo con "indice" selettore random)
                {
                        nodo_precedente=scorritore_lista; // Memorizza il nodo attuale...
                        scorritore_lista=scorritore_lista->next; // ...e sposta lo scorritore al successivo
                }
                // Alla fine del ciclo, scorritore punta al nodo estratto, e nodo_precedente al nodo precedente a quello estratto
                // NOTA: Se selettore_random è uguale a 0, lo scorritore non viene incrementato e punta al primo nodo (il nodo 0 appunto)

                tabellone[scorritore_lista->id_casella].proprietario=k; // Segna sul tabellone che la casella estratta
                                                                        // è stata assegnata al giocatore attuale (k-esimo)

                /** Eventuale assegnamento di scrivania gratuita **/
                if(lancia_dado(3)==3) // Una probabilità su 3 che l'aula venga assegnata con una scrivania
                {
                    tabellone[scorritore_lista->id_casella].ha_scrivania=true; // Assegnamento scrivnaia
                    array_giocatori[k].num_scriv++;// Incremento del numero di scrivanie possedute dal giocatore k-esimo
                }


                /** Rimozione dell'aula dalla lista delle aule disponibili **/
                if(selettore_random!=0) // Se non è stato estratto il primo nodo della lista (il nodo 0)
                    nodo_precedente->next=cancella_nodo_in_testa(nodo_precedente->next); // Elimina il nodo con la casella appena assegnata dalla lista di quelle
                                                                                         // ancora disponibili tramite il puntatore al nodo precedente

                else lista_aule=cancella_nodo_in_testa(lista_aule); // Se il nodo estratto è il nodo 0, elimina il nodo in testa alla lista


                /** Preparazione all'iterazione successiva **/
                num_aule_disponibili--; // Decrementa il numero di aule ancora disponibili
                scorritore_lista=lista_aule; // Riporta lo scorritore della lista all'inizio della lista stessa
            }
        }


        /** Assegnamento eventuali aule avanzate **/
        while(lista_aule!=NULL) // Scorre finchè non ci sono più aule da assegnare nella lista
        {
            selettore_random=rand()%num_giocatori; // Sceglie un giocatore a caso

            if(array_giocatori[selettore_random].n_aule<num_aule_per_giocatore+1) // Se il numero di aule del giocatore estratto è minore di num_aule_per_giocatore+1
                                                                                  // (ovvero se non gli è stata assegnata un aula tra quelle avanzate)
            {
                tabellone[lista_aule->id_casella].proprietario=selettore_random; // Assegna la prima aula nella lista al giocatore i-esimo
                array_giocatori[selettore_random].n_aule++; // Incremento numero aule possedute dal giocatore i-esimo

                if(lancia_dado(3)==3) // Una probabilità su 3 che l'aula venga assegnata con una scrivania
                {
                    tabellone[lista_aule->id_casella].ha_scrivania=true; // Assegnamento della scrivania
                    array_giocatori[selettore_random].num_scriv++; // Incremento del numero di scrivanie possedute dal giocatore selezionato
                }

                lista_aule=cancella_nodo_in_testa(lista_aule); // Cancella l'aula appena assegnata dalla lista
            }
        }
    } // FINE CONTROLLO STATO

} //FINE INIZIALIZZA_AULE()


/**
    Procedura che genera le liste delle aule possedute da ciascun giocatore leggendo i dati necessari
    a stabilire chi sia il proprietario di ogni aula dal tabellone.
    La lista di ciascun giocatore viene direttamente scritta nel campo lista_aule_poss della struttura
    Giocatore.

    NOTA: in caso di errore, "stato" varrà ALLOCATION_FAILURE e la procedura non verrà completata

    INPUT: tabellone, array dei giocatori, numero giocatori, puntatore alla variabile che indica lo stato del programma
    OUTPUT: lista delle aule possedute da un giocatore (scritta direttamente nella struttura di ogni giocatore)
**/
void inizializza_lista_aule_poss(Casella* tabellone, Giocatore* array_giocatori, int num_giocatori, GameState* stato)
{
    int i=0; // Indice per cicli

    /** Inizializzazione lista di ciascun giocatore **/
    for(i=0; i<num_giocatori; i++) // Scorre tutti i giocatori
        array_giocatori[i].lista_aule_poss=NULL; // Inizializza la lista del giocatore i-esimo a lista vuota

    /** Riempimento delle liste **/
    for(i=0;i<NUM_CASELLE && *stato==RUN;i++) // Scorre tutte le caselle del tabellone (si ferma se avviene un errore)
        if(tabellone[i].tipo==AULA && tabellone[i].proprietario!=-1) // Se la casella attuale è un aula e non appartiene al banco
            array_giocatori[tabellone[i].proprietario].lista_aule_poss=inserisci_in_testa(array_giocatori[tabellone[i].proprietario].lista_aule_poss,i, stato); // Inserisce l'aula nella lista del giocatore a cui l'aula appartiene
}


/**
    Funzione che invoca la funzione carica_sfighe() per caricare le sfighe da file e
    restituisce il puntatore all'array dopo aver mischiato le sfighe.
    NOTA: se carica_sfighe() fallisce non viene eseguita nessuna operazione

    INPUT: puntatore al campo della struttura che contiene i dati di gioco per memorizzare
           il numero totale di sfighe nell'array
    OUTPUT: puntatore all'array delle sfighe (ora mischiate)
**/
Carta_sfiga* inizializza_sfighe(Game* dg)
{
    Carta_sfiga* array_sfighe=carica_sfighe(dg); // Carica l'array delle sfighe
    int i=0; // Indice per cicli

    int selettore_random=0; // Variabile usata per selezionare un indice casuale dell'array
    Carta_sfiga tmp; // Variabile di supporto

    if(dg->stato_gioco!=EXIT_FAIL && dg->stato_gioco!=ALLOCATION_FAILURE) // Se non ci sono stati errore nel caricamento delle sfighe da file
    {
        /** Mescolamento delle carte sfiga **/
        /// Modus operandi: scorre tutto l'array e scambia la cella i-esima con una scelta casualmente
        for (i=0; i<(dg->num_sfighe); i++) // Scorre tutto l'array delle sfighe
        {
            tmp=array_sfighe[i]; // Copia l'elementi i-esimo nella variabile di supporto
            selettore_random=rand()%(dg->num_sfighe); // Estrae un numero casuale tra 0 e *num_sfighe-1

            // Scambia la cella i-esima dell'array con quella di indice casuale
            array_sfighe[i] = array_sfighe[selettore_random];
            array_sfighe[selettore_random]=tmp;
        }
    }

    return array_sfighe; // Restituisce il puntatore all'array (adesso mischiato)
} // FINE INIZ SFIGHE
