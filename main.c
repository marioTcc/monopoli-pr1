/**
    |-------------------------------------------------------------------------------------------------------|
    | Autore: Mario Taccori, 65234                                                                          |
    | Progetto: Monopolycube [progetto finale PR1 - 2015/2016]                                              |
    | Descrizione: programma che implementa il gioco del monopoli ambientato al palazzo delle               |
    |              scienze. Da 2 a 8 giocatori.                                                             |
    |                                                                                                       |
    | Scopo (questo file): punto di ingresso del programma dove si chiamano le funzioni per la              |
    |                      preparazione dei dati, e dunque si avvia la partita. In questo file              |
    |                      si trova anche la gestione di tutte le casistiche di stato del                   |
    |                      programma stesso (compresi errori che causano terminazione o meno)               |
    |                                                                                                       |
    |                      In particolare:                                                                  |
    |                           menu_iniziale(): EXIT, EXIT_FAIL, ALLOCATION_FAILURE, FORCE_NEW             |
    |                           menu_nuova_partita() (causata da FORCE_NEW): EXIT_FAIL, ALLOCATION_FAILURE  |
    |                           gioca(): RESTART, ALLOCATION_FAILURE, EXIT                                  |
    |-------------------------------------------------------------------------------------------------------|
**/

#include "monopolycube.h" // Inclusione header principale

int main()
{
    srand(time(NULL)); // Inizializzazione seed della funzione random

    Game dati_gioco={}; // Contenitore dati di gioco (inizializzato vuoto)
    elimina_dati_gioco(&dati_gioco); // Pulisce la struttura Game

    /** Messaggio di benvenuto **/
    printf("\t\t** Benvenuti su **\n");
    printf("    _   _   _   _   _   _   _   _   _   _   _   _  \n");
    printf("*  / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\ / \\  *\n");
    printf("* ( M ( o ( n ( o ( p ( o ( l ( y ( C ( u ( b ( e ) *\n");
    printf("*  \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/ \\_/  *\n\n");

    do
    {
        menu_iniziale(&dati_gioco); // Avvia la sequenza di inizializzazione

        /** Gestione dei possibili stati all'uscita dell'inizializzazione **/
        switch(dati_gioco.stato_gioco)
        {
            case EXIT: // Uscita senza errore
                elimina_dati_gioco(&dati_gioco); // Elimina i dati di gioco
                return EXIT; // Restituisce codice uscita senza errore (0)

            case EXIT_FAIL: // Uscita causata da errore irreversibile
                elimina_dati_gioco(&dati_gioco); // Elimina i dati di gioco
                printf("\n\nSi è verificato un errore irreversibile, impossibile proseguire. Il programma verra' terminato.\n\n");
                pausa();
                return EXIT_FAIL; // Restituisce codice uscita con errore (-1)

            case ALLOCATION_FAILURE: // Uscita causata da un errore di allocamento in memoria
                elimina_dati_gioco(&dati_gioco); // Elimina i dati di gioco
                printf("\n\nErrore nell'allocazione in memoria delle risorse necessarie all'esecuzione, impossibile proseguire.\n");
                printf("Il programma verra' terminato.\n\n");
                pausa();
                return ALLOCATION_FAILURE; // Restituisce codice allocazione fallita (-2)

            case FORCE_NEW: // Obbliga a nuova partita (esclude possibilità di caricamento)
                elimina_dati_gioco(&dati_gioco); // Elimina i dati di gioco
                menu_nuova_partita(&dati_gioco); // Chiama il menù di nuova partita (saltando il menù iniziale)

                /** Gestione possibili stati all'uscita da menu_nuova_partita() (FORCE_NEW) **/
                switch(dati_gioco.stato_gioco)
                {
                    case EXIT_FAIL: // Uscita causata da errore irreversibile
                        elimina_dati_gioco(&dati_gioco); // Elimina i dati di gioco
                        return EXIT_FAIL; // Restituisce codice uscita con errore (-1)

                    case ALLOCATION_FAILURE: // Uscita causata da un errore di allocamento in memoria
                        elimina_dati_gioco(&dati_gioco); // Elimina i dati di gioco
                        printf("\n\nErrore nell'allocazione delle risorse necessarie in memoria, impossibile proseguire.\n");
                        printf("Il programma verra' terminato.\n\n");
                        pausa();
                        return ALLOCATION_FAILURE; // Restituisce codice allocazione fallita (-2)
                }
                break; // FINE CASO FORCE_NEW
        }

        /** Avvio della partita **/
        dati_gioco.stato_gioco=gioca(&dati_gioco); // Avvia la partita

        /** Gestione dei possibili stati all'uscita dalla partita **/
        switch(dati_gioco.stato_gioco)
        {
            case RESTART: // Riavvia il programma dalla sequenza di inizializzazione
                elimina_dati_gioco(&dati_gioco); // Elimina i dati di gioco
                break;

            case ALLOCATION_FAILURE: // Uscita causata da un errore di allocamento in memoria
                elimina_dati_gioco(&dati_gioco); // Elimina i dati di gioco
                printf("\n\nErrore nell'allocazione delle risorse necessarie in memoria, impossibile proseguire.\n");
                printf("Il programma verra' terminato.\n\n");
                pausa();
                return ALLOCATION_FAILURE; // Restituisce codice allocazione fallita (-2)

            case EXIT: // Uscita senza errore
                elimina_dati_gioco(&dati_gioco); // Elimina i dati di gioco
                return EXIT; // Restituisce codice uscita senza errore (0)
        }
    }
    while(dati_gioco.stato_gioco==RESTART); // Torna all'inizio se è stato richiesto un riavvio

    return EXIT; // Uscita senza errore
}
