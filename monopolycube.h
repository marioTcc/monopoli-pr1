/**
    |-------------------------------------------------------------------------------------------|
    | Autore: Mario Taccori, 65234                                                              |
    | Progetto: Monopolycube [progetto finale PR1 - 2015/2016]                                  |
    |                                                                                           |
    | Scopo: header principale del programma                                                    |
    |-------------------------------------------------------------------------------------------|
**/

#ifndef MONOPOLYCUBE_H_INCLUDED
#define MONOPOLYCUBE_H_INCLUDED


/**     DEFINES      **/
#define MOLTIPLICATORE_PEDAGGIO 1000 // Moltiplicatore per il quale ogni pedaggio viene moltiplicato (accellera le partite)
#define MAX_STRINGA 23+1 // Lunghezza massima di una stringa (es. nome giocatore, nome casella)
#define MAX_STRINGA_SFIGA 500+1 // Lunghezza massima di un messaggio di una carta sfiga

#define MAX_STRINGA_INPUT 10000 // Lunghezza massima di un input

#define NUM_SEGNAPOSTI 8 // Numero segnaposti selezionabili

#define N_FACCE_DADO_GIOCO 6 // Numero di facce dei dadi (usati in partita)

#define NUM_CASELLE 40 // Numero caselle del tabellone

#define COSTO_SCRIVANIA 50 // Costo di una scrivania
#define COSTO_TENDA 100 // Costo di una tenda
/**-----------------**/


/**     INCLUDES    **/
// Inclusione dele librerie standard
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
/**-----------------**/


/**     ENUMS       **/
/** Enumerazione che definisce le possibili tipologie di una casella del tabellone **/
typedef enum {AULA, TASSA, SFIGA, ALTRO} Tipo_casella;

/** Enumerazione che definisce i segnaposti assegnabili ai giocatori **/
typedef enum {MOUSE, TASTIERA, CHITARRA, GATTO, CANE, MELA, ROBOT, ALFIERE} Segnaposto;

/** Enumerazione che definisce i possibili tipi di sfiga **/
typedef enum {TASSA_SEMPLICE, TASSA_IMMOBILI, BATTUTA_MARTINA, GOTO} Tipo_sfiga;

/** Enumerazione che definisce i possibili stati del programma.
    ALLOCATION_FAILURE: uscita forzata dal programma casuata da un errore di allocazione (riallocazione) dinamica di memoria
    EXIT_FAIL: uscita forzata da un errore irreversibile
    EXIT: uscita regolare (es. partita finita, l'utente sceglie di uscire ecc.)
    RUN: l'esecuzione può proseguire, nessun problema
    RESTART: gioco in attesa di riavvio (es. partita finita e l'utente sceglie di rincominciare)
    FORCE_NEW: obbligo di nuova partita (es. errore caricamento dati: sarebbe inutile dare nuovamente la possibilità all'utente
                                             di tentare ancora il caricamento)
**/
typedef enum {ALLOCATION_FAILURE=-2, EXIT_FAIL=-1, EXIT, RUN, RESTART, FORCE_NEW} GameState;
/**-----------------**/


/**     STRUCTURES      **/
typedef struct casella /// Rappresenta una casella del tabellone di gioco
{
    int costo; // Costo della casella
    int pedaggio; // Pedaggio da pagare se si capita nella casella
    int pedaggio_scriv; // Pedaggio da pagare se nella casella in cui si finisce c'è una scrivania
    int pedaggio_tenda; // Pedaggio da pagare se nella casella in cui si finisce c'è una tenda
    char nome[MAX_STRINGA]; // Nome della casella
    Tipo_casella tipo; // Tipo della casella
    _Bool ha_scrivania; // Campo che indica la presenza di una scrivania nella casella
    _Bool ha_tenda; // Campo che indica la presenza di una tenda nella casella
    int proprietario; // Proprietario della casella (banco se -1, id giocatore altrimenti)
} Casella;

typedef struct nodo /// Rappresenta un nodo di una lista di id casella (usata anche come generica di interi)
{
    int id_casella; // Id della casella posseduta (corrisponde alla posizione della casella nel tabellone (VIA! = 0))
    struct nodo* next; // Puntatore al nodo successivo
} Nodo;

typedef struct giocatore /// Rappresenta un giocatore
{
    char nome[MAX_STRINGA]; // Nome del giocatore
    Segnaposto segnaposto; // Segnaposto scelto dal giocatore
    int budget; // Capitale posseduto
    _Bool pulizia_bagni; // Indica se il giocatore sta lavando i bagni
    int turni_pulizia_bagni; // Turni rimanenti alla fine della pulizia dei bagni
    int n_aule; // Numero aule possedute
    int posizione; // Posizione del giocatore nel tabellone
    Nodo* lista_aule_poss; // Lista aule possedute
    int num_scriv; // Numero totale scrivanie possedute
    int num_tende; // Numero totale tende possedute
}Giocatore;

typedef struct giocatore_save /// Rappresenta un giocatore nel file di salvataggio (ha alcuni campi in meno di Giocatore)
{
    char nome[MAX_STRINGA]; // Nome del giocatore
    Segnaposto segnaposto; // Segnaposto scelto dal giocatore
    int budget; // Capitale posseduto
    int turni_pulizia_bagni; // Turni rimanenti alla fine della pulizia dei bagni
    int n_aule; // Numero aule possedute
    int posizione; // Posizione del giocatore nel tabellone
}Giocatore_save;

typedef struct partita /// Memorizza i dati generali di una partita
{
    int num_giocatori; // Numero giocatori
    int num_turni_giocati; //Numero totale dei turni giocati
    int turno_giocatore; // Id giocatore che deve giocare il turno
} Partita;

typedef struct carta_sfiga /// Definisce una "carta" sfiga generica
{
    Tipo_sfiga tipo_sfiga; // Tipo di sfiga
    char msg[MAX_STRINGA_SFIGA]; // Messaggio (o battuta se tipo BATTUTA_MARTINA) da visualizzare se si pesca la sfiga
    int moltiplicatore_n_aule; // Moltiplicatore del numero di aule possedute (solo tipo TASSA_IMMOBILI, non usato altrimenti)
    int moltiplicatore_n_scriv; // Moltiplicatore del numero di scrivanie possedute (solo tipo TASSA_IMMOBILI, non usato altrimenti)
    int moltiplicatore_n_tende; // Moltiplicatore del numero di tende possedute (solo tipo TASSA_IMMOBILI, non usato altrimenti)
    int casella_destinazione; // Casella dove si viene mandati (solo tipo GOTO, non usato altrimenti)
    int tassa; // Tassa da pagare (solo tipo TASSA_SEMPLICE, non usato altrimenti)
} Carta_sfiga;

typedef struct game /// Ingloba tutti i dati di gioco
{
    Partita dettagli_partita; // Contiene i dettagli generali della partita
    Giocatore* array_giocatori; // Array contenente le irmazioni di ciascun giocatore (giocatore1 = indice 0, ecc.)
    Casella* tabellone; // Array di caselle che rappresenta il tabellone
    Carta_sfiga* array_sfighe; // Array contenente le sfighe
    int num_sfighe; // Numero di sfighe nell'array delle sfighe

    _Bool bonus_mode; // Indica se la partita è stata avviata in modalità bonus o classica (true = modalità bonus)
    GameState stato_gioco; // Descrive lo stato del gioco
}Game;
/**---------------------**/


/**     PROTOTYPES       **/
//{ Funzioni di inizializzazione (definite in initFunctions.c): Preparano i dati necessari per una partita (nuova o caricata)
void menu_iniziale(Game* dati_gioco); /// Menù iniziale
void menu_nuova_partita(Game* dati_gioco); /// Menù per acquisire le preferenze per una nuova partita

Game nuova_partita(int num_giocatori, _Bool bonus_mode); /// Crea una nuova partita
Game carica_partita(); /// Carica una partita precedentemente salvata

    //{ NUOVA PARTITA: utilizzate solo per preparare una nuova partita
        Partita inizializza_partita(int num_giocatori); /// Crea la struttura con i dettagli della partita

        Giocatore* inizializza_giocatori(int num_giocatori, GameState* stato_gioco); /// Crea l'array dei giocatori
        void inizializza_budget(Casella* tabellone, Giocatore* array_giocatori, int num_giocatori); /// Calcola e assegna il budget iniziale ai giocatori

        Casella* inizializza_tabellone(); /// Inizializza il tabellone
        void inizializza_aule(Casella* tabellone, Giocatore* array_giocatori, int num_giocatori, GameState* stato); /// Assegna le aule disponibili ai giocatori
    //}

void inizializza_lista_aule_poss(Casella* tabellone, Giocatore* array_giocatori, int num_giocatori, GameState* stato); /// Genera le liste di aule possedute da ciascun giocatore
Carta_sfiga* inizializza_sfighe(Game* dg); /// Carica l'array di sfighe e mischia le sfighe
//}


//{ Funzioni per la lettura/scrittura file (definite in fileFunctions.c): Leggono/scrivono dati da/su file e gestiscono eventuali errori
void salva_dati(Game dati_gioco); /// Salva la partita su file binario

Game carica_dati(); /// Carica una partita salvata in file binario
Game errore_caricamento(); /// Eseguita in caso di errore nel caricamento

Carta_sfiga* carica_sfighe(Game* dg); /// Carica le sfighe da file testuale
//}


//{ Funzioni di gioco (definite in game.c): funzioni per la gestione degli eventi di una partita
GameState gioca(Game* dati_gioco); /// Loop di gioco
int lancia_dado(int n_facce); /// Lancia un dado
void elimina_giocatore(Game* dg, int id_giocatore); /// Elimina un giocatore dalla partita
void compra_aula(Giocatore* array_giocatori, int* turno, Casella* aula_acquistata, GameState* stato); /// Effettua le modifiche necessarie a comprare un aula
void esegui_azione_destinazione(Game* dg, int* turno, Casella* destinazione); /// Esegue l'azione prevista per la casella in cui il giocatore va a finire
int estrai_sfiga(int num_sfighe); /// Estrae una sfig adal mazzo
void costruisci_scrivania(Giocatore* array_giocatori, int* turno, Casella* aula); /// Effettua le modifiche necessarie all'acquisto di una scrivania
void costruisci_tenda(Giocatore* array_giocatori, int* turno, Casella* aula); /// Effettua le modifiche necessarie all'acquisto di una tenda
//}


//{ Funzioni per la stampa di informazioni a video (definite in outputConsole.c): si occupato solamente di stampare informazioni a video
    void stampa_budget_tutti(Giocatore* array_giocatori, int num_giocatori); /// Stampa budget di tutti i giocatori
    void stampa_giocatore_base(Giocatore giocatore, Casella* tabellone); /// Stampa le informazioni basilari di un giocatore
    void stampa_lista_aule_poss(Nodo* lista, Casella* tabellone, Giocatore giocatore); /// Stampa la lista di aule possedute da un giocatore
    void stampa_tabellone(Giocatore* array_giocatori, Casella* tabellone); /// Stampa tutto il tabellone
    void stampa_destinazione(Giocatore* array_giocatori, Casella* destinazione); /// Stampa le informazioni relative alla casella in cui il giocatore è andato a finire
//}


//{ Funzioni di gestione programma, strutture dati, altro (definite in monopolycube.c): funzioni non associabili alle altre sezioni
void elimina_dati_gioco(Game* dg); /// Azzera i campi di una struttura Game
char* nome_segnaposto(Segnaposto segnaposto); /// Converte un segnaposto dal suo valore di enumerazione a stringa

    //{ GESTIONE LISTE
        Nodo* inserisci_in_testa(Nodo* lista, int id_casella, GameState* stato); /// Inserisce un nodo all'inizio di una lista
        Nodo* cancella_nodo_in_testa(Nodo* lista); /// Cancella il primo nodo di una lista
        void elimina_lista(Nodo* lista); /// Elimina una lista
    //}

    //{ MENU DI SUPPORTO
        char menu_si_no(); /// Chiede all'utente di rispondere si/no
        void pausa(); /// Semplice pausa, "premere invio per continuare"
    //}

GameState isPtrValid(void* ptr); /// Verifica che un'allocazione sia andata a buon fine
int charToInt(char c); /// Converte un carattere che rappresenta una cifra nell'intero a cui la cifra corrisponde
//}


//{ Funzioni di debug (definite in debugFunctions.c)
void DEBUG_stampa_array_sfighe(Carta_sfiga* array_sfighe, int start, int num_sfighe); /// Stampa tutte le sfighe
void DEBUG_stampa_lista(Nodo* lista); /// Stampa una lista
void DEBUG_stampa_array_giocatori(Giocatore* array_giocatori, int num_giocatori); /// Stampa le informazioni di tutti i giocatori
void DEBUG_stampa_giocatore(Giocatore giocatore); /// Stampa le informazioni di un singolo giocatore
//}


#endif // MONOPOLYCUBE_H_INCLUDED
