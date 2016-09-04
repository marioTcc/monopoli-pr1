/**
    |-------------------------------------------------------------------|
    | Autore: Mario Taccori, 65234                                      |
    | Progetto: Monopolycube [progetto finale PR1 - 2015/2016]          |
    |                                                                   |
    | Scopo: file in cui sono definite le funzioni utilizzate per la    |
    |        lettura/scrittura di file e gestione degli errori          |
    |        relativi.                                                  |
    |                                                                   |
    | Funzioni definite:                                                |
    |       void salva_dati(Game dati_gioco)                            |
    |       Game carica_dati()                                          |
    |       Game errore_caricamento()                                   |
    |       Carta_sfiga* carica_sfighe(Game* dg)                        |
    |-------------------------------------------------------------------|
**/

#include "monopolycube.h" // Inclusione header principale del progetto

/**
    |-------------------------------------------------------------------------|
    | Specifiche file di salvataggio:                                         |
    | Nome file: saveGame.save                                                |
    | Posizione file: stesso path dell'eseguibile                                |
    | Tipo file: binario                                                      |
    |                                                                         |
    | Struttura file:                                                         |
    |     -Partita (struttura)                                                |
    |     -Array giocatori (n strutture Giocatore_save, n definito in Partita)|
    |     -Dettagli caselle (40 strutture Casella)                            |
    |     -Indicatore modalità (intero)                                       |
    |-------------------------------------------------------------------------|
**/
/**
    Procedura che salva lo stato della partita nel momento in cui viene richiesto su un file binario. Se esiste già un
    file di salvataggio, verrà sovrascritto.
    NOTA: l'array giocatori usato in partita contiene strutture di tipo Giocatore, mentre, per il salvataggio,
          servono strutture di tipo Giocatore_save: la conversione avviene in questa funzione stessa.

    INPUT: struttura di tipo Game contenente i dati di gioco
    OUTPUT: memorizza i dati di gioco in un file binario (saveGame.save) e notifica a terminale l'avvenuto salvataggio
**/
void salva_dati(Game dati_gioco)
{
    Giocatore_save array_giocatori_save[dati_gioco.dettagli_partita.num_giocatori]; // Contiene l'array giocatori convertito nel
                                                                                    // tipo necessario per il salvataggio
    int i=0; // Indice per cicli

    FILE* fp=fopen("saveGame.save", "wb");
    if(fp!=NULL) // Se il file è stato aperto\creato correttamente
    {
        /** Conversione array_giocatori da Giocatore* a Giocatore_save* **/
        /// Riempe i campi di array_giocatori_save con i campi corrispondenti in array_giocatori
        for(i=0; i<dati_gioco.dettagli_partita.num_giocatori; i++) // Scorre tutti i giocatori
        {
            strcpy(array_giocatori_save[i].nome, dati_gioco.array_giocatori[i].nome);
            array_giocatori_save[i].segnaposto= dati_gioco.array_giocatori[i].segnaposto;
            array_giocatori_save[i].budget=dati_gioco.array_giocatori[i].budget;
            array_giocatori_save[i].turni_pulizia_bagni=dati_gioco.array_giocatori[i].turni_pulizia_bagni;
            array_giocatori_save[i].n_aule=dati_gioco.array_giocatori[i].n_aule;
            array_giocatori_save[i].posizione=dati_gioco.array_giocatori[i].posizione;
        } // FINE CONVERSIONE


        /** Salva dettagli partita **/
        fwrite(&dati_gioco.dettagli_partita, sizeof(Partita), 1, fp);

        /** Salva array_giocatori (già convertito) **/
        fwrite(array_giocatori_save, sizeof(Giocatore_save), dati_gioco.dettagli_partita.num_giocatori, fp);

        /** Salva stato del tabellone **/
        fwrite(dati_gioco.tabellone, sizeof(Casella), NUM_CASELLE, fp);

        /** Salva indicatore modalità bonus **/
        fwrite(&dati_gioco.bonus_mode, sizeof(_Bool), 1, fp);

        fclose(fp); // Salvataggio terminato, chiude lo stream

        printf("\n\nPartita salvata con successo\n\n"); // Notifica che il salvataggio è stato completato
    }
    else printf("Errore. Impossibile salvare la partita"); // Se il file non è stato aperto corretamente notifica un errore
} // FINE SALVA_DATI()


/**
    Funzione che carica i dati di gioco di una partita salvata precedentemente da un file binario.
    Gestisce eventuali errori di caricamento chiamando la procedura errore_caricamento().
    NOTA: attualmente gestisce solo l'errore dovuto al file di salvataggio inesistente.
    NOTA: l'array giocatori caricato è di tipo Giocatore_save*, in partita serve di tipo Giocatore*:
          la conversione avviene in questa funzione stessa.

    INPUT: nessuno
    OUTPUT: struttura game con i campi dettagli_partita, array_giocatori, tabellone e indicatore modalità
            caricati da file; stato di gioco impostato su RUN (ALLOCATION_FAILURE se qualcosa non ha funzionato)
**/
Game carica_dati()
{
    Game game_tmp={}; // Variabile temporanea da restituire
    game_tmp.stato_gioco=RUN; // Imposta lo stato di gioco su RUN (nessun errore al momento)
    int i=0; // Indice per cicli

    FILE* fp=fopen("saveGame.save", "rb");
    if(fp!=NULL) // Se il file è stato aperto correttamente
    {
        /** Carica dettagli partita **/
        fread(&game_tmp.dettagli_partita, sizeof(Partita), 1, fp);

        /** Carica array giocatori **/
        Giocatore_save gs_tmp[game_tmp.dettagli_partita.num_giocatori]; // Array di appoggio per contenere i dati caricati
        game_tmp.array_giocatori=(Giocatore*) malloc(sizeof(Giocatore)*game_tmp.dettagli_partita.num_giocatori); // Array giocatori (dopo conversione)
        game_tmp.stato_gioco=isPtrValid(game_tmp.array_giocatori); // Verifica che l'allocamento sia andato a buon fine
        if(game_tmp.stato_gioco!=RUN) // Se l'allocamento non è andato a buon fine
            return game_tmp; // Ferma l'esecuzione della funzione con errore

        fread(gs_tmp, sizeof(Giocatore_save), game_tmp.dettagli_partita.num_giocatori, fp);

        /** Conversione array da Giocatore_save* a Giocatore* **/
        /// Riempe i campi di array_giocatori con i campi corrispondenti in array_giocatori_save
        /// e inizializza a 0 gli altri campi per ciascuna cella
        for(i=0; i<game_tmp.dettagli_partita.num_giocatori; i++) // Scorre tutti i giocatori
        {
            strcpy(game_tmp.array_giocatori[i].nome, gs_tmp[i].nome);
            game_tmp.array_giocatori[i].segnaposto= gs_tmp[i].segnaposto;
            game_tmp.array_giocatori[i].budget=gs_tmp[i].budget;
            game_tmp.array_giocatori[i].turni_pulizia_bagni=gs_tmp[i].turni_pulizia_bagni;
            game_tmp.array_giocatori[i].n_aule=gs_tmp[i].n_aule;
            game_tmp.array_giocatori[i].posizione=gs_tmp[i].posizione;

            /** Inizializzazione altri campi Giocatore **/
            game_tmp.array_giocatori[i].lista_aule_poss=NULL;
            game_tmp.array_giocatori[i].num_scriv=0;
            game_tmp.array_giocatori[i].num_tende=0;

            if(game_tmp.array_giocatori[i].turni_pulizia_bagni>0)
                game_tmp.array_giocatori[i].pulizia_bagni=true;
            else game_tmp.array_giocatori[i].pulizia_bagni=false;
        } // FINE CONVERSIONE

        /** Carica tabellone **/
        game_tmp.tabellone=(Casella*) malloc(sizeof(Casella)*NUM_CASELLE);
        game_tmp.stato_gioco=isPtrValid(game_tmp.tabellone); // Verifica che l'allocamento sia andato a buon fine
        if(game_tmp.stato_gioco!=RUN) // Se l'allocamento non è andato a buon fine
            return game_tmp; // Ferma l'esecuzione della funzione con errore

        fread(game_tmp.tabellone, sizeof(Casella), NUM_CASELLE, fp);

        /** Carica indicatore modalità bonus **/
        fread(&game_tmp.bonus_mode, sizeof(_Bool), 1, fp);

        fclose(fp); // Caricamento completato, chiude lo stream

        /// Se non ci sono errori game_tmp.stato_gioco vale RUN
        return game_tmp;

    } // FINE CONTROLLO PUNTATORE FILE
    else return errore_caricamento(); // Se l'apertura del file è fallita
} // FINE CARICA_DATI()


/**
    Procedura chiamata in caso di errore nel caricamento di una partita salvata.
    Notifica a terminale l'errore nel caricamento e chiede all'utente se avviare una nuova partita
    o uscire dal programma.

    INPUT: nessuno
    OUTPUT: notifica a schermo l'errore; se l'utente sceglie di avviare una nuova partita restituisce
            una struttura Game vuota con stato_gioco settato a RESTART_FORCE_NEW, altrimenti
            restituisce una struttura Game vuota con stato_gioco settato a EXIT
**/
Game errore_caricamento()
{
    Game game_tmp={}; // Variabile temporanea da restituire

    /** Notifica dell'errore **/
    printf("\n**ERRORE DI CARICAMENTO**\n");
    printf("File di salvataggio mancante o corrotto\n");

    /** Richiesta avvio nuova partita**/
    printf("\nAvviare una nuova partita? \n");
    if(menu_si_no()=='s')
        game_tmp.stato_gioco=FORCE_NEW; // è stato scelto di iniziare una nuova partita
    else
        game_tmp.stato_gioco=EXIT; // è stato scelto di uscire

    return game_tmp;
}


/**
    Funzione che crea e inizializza l'array contenente le sfighe caricando le informazioni da file testuale.
    NOTA: attualmente gestisce l'errore relativo al file di sfighe mancante chiamando la
    procedura errore_caricamento_sfighe(), se l'allocamento dell'array fallisce imposta lo stato di gioco su ALLOCATION_FAILURE.

    INPUT: puntatore al campo della struttura Game dove memorizzare il numero di sfighe
    OUTPUT: puntatore all'array di sfighe, numero di sfighe nell'array (scritto nel campo passato come parametro);
            puntatore a NULL in caso di errore nel caricamento; imposta stato_gioco a ALLOCATION_FAILURE in caso di
            errore di allocamento dell'array

    |------------------------------------------------------------------------------------------------------|
    | Specifiche file testuale contenente le sfighe:                                                       |
    | Nome file: sfighe.txt                                                                                |
    | Posizione file: stessa cartella dell'eseguibile                                                      |
    | Tipo file: testuale                                                                                  |
    |                                                                                                      |
    | Struttura file:                                                                                      |
    |                                                                                                      |
    | <inizio file>n_tipo1 n_tipo2 n_tipo3 n_tipo4 \n (numero sfighe per ciascun tipo)                     |
    | int messaggio                   \n (tassa da pagare, messaggio da visualizzare 500 caratteri max)    |
    | ...                                (n_tipo1 sfighe di tipo 1 (TASSA_SEMPLICE))                       |
    | int int int messaggio           \n (molt. n aule, molt. n scriv., molt. n tende, messaggio (500 max) |
    | ...                                (n_tipo2 sfighe di tipo 2 (TASSA_IMMOBILI))                       |
    | messaggio                       \n (messaggio (500 max)                                              |
    | ...                                (n_tipo3 sfighe di tipo 3 (BATTUTA_MARTINA))                      |
    | int messaggio                   \n (casella destinazione (da 0 a 39), messaggio (500 max)            |
    | ...                                (n_tipo4 sfighe di tipo 4 (GOTO)                                  |
    | <fine file>                                                                                          |
    |------------------------------------------------------------------------------------------------------|
**/
Carta_sfiga* carica_sfighe(Game* dg)
{
    int n_tipo1=0, n_tipo2=0, n_tipo3=0, n_tipo4=0, n_totale=0; // Quantità sfighe per ciascun tipo e totale
    int i=0; // Indice per cicli

    FILE* fp=fopen("sfighe.txt", "r");
    if(fp==NULL) // Se c'è stato un errore nell'apertura del file
    {
        printf("\n\n*** Errore irreversibile nel caricamento file delle sfighe ***\n");
        printf("\nVerificare che nella cartella dell'eseguibile sia presente il file delle sfighe\ne di avere i permessi necessari per accedervi.");
        printf("\nUna reinstallazione del programma potrebbe risolvere il problema");
        printf("\nIl processo verra' terminato.");
        dg->stato_gioco=EXIT_FAIL; // Segnala l'errore
        return NULL;
    }

    /** Acquisizione informazioni sul numero di sfighe nel file **/
    fscanf(fp, "%d", &n_tipo1); // Acquisizione numero sfighe primo tipo
    fscanf(fp, "%d", &n_tipo2); // Acquisizione numero sfighe secondo tipo
    fscanf(fp, "%d", &n_tipo3); // Acquisizione numero sfighe terzo tipo
    fscanf(fp, "%d", &n_tipo4); // Acquisizione numero sfighe quarto tipo

    fscanf(fp, "\n");
    n_totale=n_tipo1+n_tipo2+n_tipo3+n_tipo4; // Calcolo del numero totale delle sfighe

    dg->num_sfighe=n_totale; // Scrive il numero totale delle sfighe nella struttura contenente i dati di gioco

    /** Creazione dell'array delle sfighe **/
    Carta_sfiga* array_sfighe=(Carta_sfiga*) malloc(sizeof(Carta_sfiga)* n_totale);
    dg->stato_gioco=isPtrValid(array_sfighe); // Verifica che l'allocamento sia andato a buon fine
    if(dg->stato_gioco!=RUN) // Se l'allocamento non è andato a buon fine
        return NULL; // Termina la funzione (non ha senso continuare)

    if(dg->stato_gioco==RUN)
    {
        /** Riempimento array sfighe **/
        for(i=0; i<n_totale; i++) // Scorre tutte le sfighe
        {
            /** Sfighe di primo tipo (TASSA_SEMPLICE) **/
            if(i<n_tipo1)
            {
                /** Acquisizione campi esclusivi del primo tipo **/
                array_sfighe[i].tipo_sfiga=TASSA_SEMPLICE;
                fscanf(fp, "%d", &array_sfighe[i].tassa);
                fscanf(fp, " %[^\n]s", &array_sfighe[i].msg);
                fscanf(fp, "\n");

                /** Inizializzazione altri campi (inutilizzati in primo tipo) **/
                array_sfighe[i].moltiplicatore_n_aule=0;
                array_sfighe[i].moltiplicatore_n_scriv=0;
                array_sfighe[i].moltiplicatore_n_tende=0;
                array_sfighe[i].casella_destinazione=0;
            } // FINE TIPO 1

            /** Sfighe di secondo tipo (TASSA_IMMOBILI) **/
            if(i>=n_tipo1 && i<n_tipo2+n_tipo1)
            {
                /** Acquisizione campi esclusivi del secondo tipo **/
                array_sfighe[i].tipo_sfiga=TASSA_IMMOBILI;
                fscanf(fp, "%d", &array_sfighe[i].moltiplicatore_n_aule);
                fscanf(fp, "%d", &array_sfighe[i].moltiplicatore_n_scriv);
                fscanf(fp, "%d", &array_sfighe[i].moltiplicatore_n_tende);
                fscanf(fp, " %[^\n]s", &array_sfighe[i].msg);
                fscanf(fp, "\n");

                /** Inizializzazione altri campi (inutilizzati in secondo tipo) **/
                array_sfighe[i].tassa=0;
                array_sfighe[i].casella_destinazione=0;
            } // FINE TIPO 2

            /** Sfighe di terzo tipo (BATTUTA_MARTINA) **/
            if(i>=n_tipo1+n_tipo2 && i<n_tipo3+n_tipo1+n_tipo2)
            {
                /** Acquisizione campi esclusivi del terzo tipo **/
                array_sfighe[i].tipo_sfiga=BATTUTA_MARTINA;
                fscanf(fp, " %[^\n]s", &array_sfighe[i].msg);
                fscanf(fp, "\n");

                /** Inizializzazione altri campi (inutilizzati in terzo tipo) **/
                array_sfighe[i].moltiplicatore_n_aule=0;
                array_sfighe[i].moltiplicatore_n_scriv=0;
                array_sfighe[i].moltiplicatore_n_tende=0;
                array_sfighe[i].tassa=0;
                array_sfighe[i].casella_destinazione=0;
            } // FINE TIPO 3

            /** Sfighe di quarto tipo (GOTO) **/
            if(i>=n_tipo3+n_tipo1+n_tipo2 && i<n_totale)
            {
                /** Acquisizione campi esclusivi del quarto tipo **/
                array_sfighe[i].tipo_sfiga=GOTO;
                fscanf(fp, "%d", &array_sfighe[i].casella_destinazione);
                fscanf(fp, " %[^\n]s", &array_sfighe[i].msg);
                fscanf(fp, "\n");

                /** Inizializzazione altri campi (inutilizzati in quarto tipo) **/
                array_sfighe[i].moltiplicatore_n_aule=0;
                array_sfighe[i].moltiplicatore_n_scriv=0;
                array_sfighe[i].moltiplicatore_n_tende=0;
                array_sfighe[i].tassa=0;
            } // FINE TIPO 4

        }// FINE FOR
    } // FINE CONTROLLO ALLOCAMENTO ARRAY SFIGHE

    fclose(fp); // Lettura completata, chiude lo stream

    /// Se l'allocamento dell'array è fallito, dg->stato_gioco varrà ALLOCATION_FAILURE, RUN altrimenti
    return array_sfighe;
} // FINE CARICA_SFIGHE()
