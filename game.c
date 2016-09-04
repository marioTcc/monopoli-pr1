/**
    |-------------------------------------------------------------------------------------------------------------|
    | Autore: Mario Taccori, 65234                                                                                |
    | Progetto: Monopolycube [progetto finale PR1 - 2015/2016]                                                    |
    |                                                                                                             |
    | Scopo: file in cui sono definite le funzioni per la gestione degli eventi di una partita                    |
    |        e la funzione che rappresenta la partita stessa.                                                     |
    |                                                                                                             |
    | Funzioni definite:                                                                                          |
    |       GameState gioca(Game* dg)                                                                             |
    |       void esegui_azione_destinazione(Game* dg, int* turno, Casella* destinazione)                          |
    |       int estrai_sfiga(int num_sfighe)                                                                      |
    |       void elimina_giocatore(Game* dg, int id_giocatore)                                                    |
    |       void costruisci_scrivania(Giocatore* array_giocatori, int* turno, Casella* aula)                      |
    |       void costruisci_tenda(Giocatore* array_giocatori, int* turno, Casella* aula)                          |
    |       void compra_aula(Giocatore* array_giocatori, int* turno, Casella* aula_acquistata, GameState* stato)  |
    |       int lancia_dado(int n_facce)                                                                          |
    |-------------------------------------------------------------------------------------------------------------|
**/


#include "monopolycube.h" // Inclusione header principale


/**
    Funzione che rappresenta la partita vera e propria e restituisce un valore di tipo
    GameState quando la partita termina (a causa o meno di errori).

    Gestisce gli eventi della partita tra cui, oltre i vari controlli per la verifica che un giocatore possa
    leggittimamente giocare il suo turno, la verifica della presenza di un vincitore, il lancio dei dadi (compresa
    la gestione dei tiri bonus) e svariate notifiche. Le operazioni relative alle caselle di destinazione sono, invece,
    assegnate alla funzione esegui_azione_destinazione()

    Possibili stati di uscita:
        RESTART: la partita è finita regolarmente e si sceglie di iniziarne un altra
        EXIT: si sceglie di uscire attraverso gli appositi menù
        ALLOCATION_FAILURE: possibile stato dopo l'esecuzione di esegui_azione_destinazione(), quando si tenta
                            di comprare un aula ma la memoria non è sufficiente per l'aggiornamento della lista
                            aule possedute

    INPUT: struttura Game contenente i dati di gioco
    OUTPUT: stato del programma al termine dell'esecuzione (causata da errore o meno)
**/
GameState gioca(Game* dg)
{
    int i=0; // Indice per cicli
    int* turno=&dg->dettagli_partita.turno_giocatore; // Per leggibilità, punta all'intero che indica il turno nella struttura Partita
    char scelta[MAX_STRINGA_INPUT]={}; // Memorizza la scelta nel menù di gioco
    int dado1=0, dado2=0; // Variabili contenenti i valori dei dadi
    int cont_doppi=0; // Contatore tiri doppi consecutivi

    /** Stampa delle aule possedute da ciascun giocatore all'inizio della partita **/
    for(i=0;i<dg->dettagli_partita.num_giocatori;i++) // Scorre tutti i giocatori
        stampa_lista_aule_poss(dg->array_giocatori[i].lista_aule_poss, dg->tabellone, dg->array_giocatori[i]); // Stampa le aule possedute dal giocatore i-esimo

    /** INIZIO DELLA PARTITA **/
    while(dg->stato_gioco==RUN) // Ciclo di gioco
    {

        /** Se è il turno del giocatore 0, stampa i budget di tutti i giocatori **/
        /// il controllo di cont_doppi evita lo spam dei budget in caso di tiro doppio del giocatore 0
        if(*turno==0 && cont_doppi==0) // Se è il turno del giocatore con id 0, e non è un tiro bonus
        {
            stampa_budget_tutti(dg->array_giocatori, dg->dettagli_partita.num_giocatori); // Stampa il riepilogo dei budget
            pausa();
        }

        /** Notifica chi deve giocare il turno **/
        printf("\n\t** Turno di: **\n");
        stampa_giocatore_base(dg->array_giocatori[*turno], dg->tabellone); // Stampa info del giocatore di turno


        /// Se il giocatore di turno ha budget negativo, viene eliminato (tutti gli altri hanno già giocato il loro turno in questo punto)
        if(dg->array_giocatori[*turno].budget<0) // Se il giocatore di turno ha budget negativo
        {
            printf("\n\n%s e' stato eliminato (bancarotta)\n", dg->array_giocatori[*turno].nome); // Notifica l'eliminazione
            elimina_giocatore(dg, *turno); // Elimina effettivamente il giocatore
            if(dg->stato_gioco!=RUN) // Se si è verificato un errore nell'eliminazione del giocatore
                return dg->stato_gioco; // Termina la funzione, restituendo lo stato d'errore

            *turno=*turno%dg->dettagli_partita.num_giocatori; // Passa il turno al giocatore successivo

            /** Verifica di un eventuale vincitore **/
            /// Se è rimasto un solo giocatore in partita, vince in quanto ultimo superstite
            if(dg->dettagli_partita.num_giocatori==1) // Se è rimasto un solo giocatore
            {
                printf("\n\nLa partita e' finita! Il vincitore e':\n\n");
                printf(" ************************************\n");
                stampa_giocatore_base(dg->array_giocatori[*turno], dg->tabellone); // Stampa informazioni del vincitore
                printf("\n ************************************\n");
                printf("\nDopo %d turni giocati.\n", dg->dettagli_partita.num_turni_giocati);
                printf("Congratulazioni!");
                pausa();

                /** Richiesta di inizio nuova partita **/
                printf("\n\n\nIniziare una nuova partita?");
                if(menu_si_no()=='s') // Se l'utente sceglie "si"
                    return RESTART; // Esce dalla partita segnalando la neccessità di riavvio
                else return EXIT; // Segnala l'uscita dal programma
            }
        }
        /** Se il giocatore NON ha budget negativo **/
        else // Se il giocatore non ha budget negativo
        {
            if(dg->array_giocatori[*turno].turni_pulizia_bagni==0) // Se il numero di turni mancanti alla pulizia dei bagni è 0...
                dg->array_giocatori[*turno].pulizia_bagni=false; // ...il giocatore di turno può essere di nuovo considerato attivo nella partita

            if(dg->array_giocatori[*turno].pulizia_bagni==false) // Se il giocatore di turno non sta pulendo i bagni
            {

                /// Se arrivato in questo punto, il giocatore non ha budget negativo e non sta pulendo i bagni
                /// Dunque può regolarmente giocare il suo turno

                /** MENU DI GIOCO **/
                do
                {
                    printf("\n\n   t: tira i dadi   -   s: salva   -   c: stampa tabellone   -   exit: esci\n");
                    printf("   -> ");
                    scanf("%[^\n]s", scelta);
                    getchar();

                    if(strcmp(scelta,"t")!=0 && strcmp(scelta,"s")!=0 && strcmp(scelta,"exit")!=0 && strcmp(scelta,"c")!=0) // Se l'input non è valido
                        printf("\n\tOpzione selezionata non valida, reinserire\n"); // Notifica e richiede inserimento

                    /** Selezionato di salvare la partita **/
                    if(strcmp(scelta,"s")==0) // Se è stato scelto di salvare
                    {
                        salva_dati(*dg); // Salva la partita
                        strcpy(scelta, "\0"); // Azzera la scelta precedente
                        pausa();
                    }

                    /** Selezionato di stampare il tabellone **/
                    if(strcmp(scelta,"c")==0) // Se è stato selezionato di stampare il tabellone
                    {
                        stampa_tabellone(dg->array_giocatori, dg->tabellone); // Stampa riepilogo di tutto il tabellone
                        strcpy(scelta, "\0"); // Azzera la scelta precedente
                        pausa();
                    }

                    /** Selezionato di chiudere il gioco **/
                    if(strcmp(scelta,"exit")==0) // Se è stato scelto di uscire dal programma
                        return EXIT; // Segnala l'uscita dal programma
                }
                while(strcmp(scelta,"t")!=0); // Scorre finchè non viene scelto di tirare i dadi (l'unica opzione che fa avanzare la partita)

                /** Selezionato di lanciare i dadi **/
                if(strcmp(scelta,"t")==0) // Se è stato scelto di lancuiare i dadi
                {
                    dado1=lancia_dado(N_FACCE_DADO_GIOCO); // Lancio dado1
                    dado2=lancia_dado(N_FACCE_DADO_GIOCO); // Lancio dado2
                    printf("\nDado 1: %d\nDado 2: %d\n", dado1, dado2); // Notifica risultato del lancio
                }

                /** Valutazione eventuale passaggio dla Via! **/
                /// Se l'id della casella di destinazione è minore o uguale all'id della casella in cui il giocatore di turno
                /// si trovava prima del lancio, vuol dire che è passato dal Via!
                if((dg->array_giocatori[*turno].posizione+dado1+dado2)%NUM_CASELLE <= dg->array_giocatori[*turno].posizione) // Se l'id della destinazione è minore o uguale all'id della posizione precedente
                {
                    dg->array_giocatori[*turno].budget+=20; // Assegna i 20 euro per essere passato dal via
                    printf("\nSei passato dal Via!, eccoti 20 euro\n"); // Notifica il passaggio dal Via!
                }

                /** Sposta il giocatore **/
                dg->array_giocatori[*turno].posizione= (dg->array_giocatori[*turno].posizione+dado1+dado2)%NUM_CASELLE; // Calcolo nuova posizione
                stampa_destinazione(dg->array_giocatori, &dg->tabellone[dg->array_giocatori[*turno].posizione]); // Notifica casella di destinazione
                esegui_azione_destinazione(dg, turno, &dg->tabellone[dg->array_giocatori[*turno].posizione]); // Esecuzione conseguenze previste dall'arrivo in quella specifica casella
                if(dg->stato_gioco!=RUN) // Se si è verificato un errore
                    return dg->stato_gioco; // Restituisce lo stato d'errore
            }
            /** Se il giocatore sta pulendo i bagni **/
            else
            {
                dg->array_giocatori[*turno].turni_pulizia_bagni--; // Decrementa i turni di pulizia dei bagni del giocatore di turno
                if(dg->array_giocatori[*turno].turni_pulizia_bagni>0) // Se il giocatore di turno deve lavare i bagni per più di 0 turni
                    printf("\nDevi ancora pulire i bagni per altri %d turni oltre questo!\n", dg->array_giocatori[*turno].turni_pulizia_bagni); // Notifica turni di pulizia dei bagni rimanenti
                else printf("\nPotrai giocare il prossimo turno!\n"); // Notifica al giocatore che tornerà attivo al prossimo turno
            }

            /** Verifica eventuali tiro doppio **/
            /// Il controllo su dado1 e dado2 diversi da 0 è necessario per verificare che ci sia stato
            /// effettivamente un tiro di dadi nel turno attuale
            if(dado1==dado2 && dado1!=0 && dado2!=0) // Se i due dadi hanno dato lo stesso numero
                cont_doppi++; // Incrementa il contatore di tiri doppi
            else cont_doppi=0; // Se i due dadi non hanno dato lo stesso numero, azzera il contatore di tiri doppi

            if(cont_doppi==0) // Se non è stato fatto un tiro doppio...
                *turno=(*turno+1)%dg->dettagli_partita.num_giocatori; // ...passa il turno al giocatore successivo
            else // Se è stato fatto un tiro doppio
            {
                if(cont_doppi<3) // Se il tiro doppio non è il terzo consecutivo (quindi è il primo o secondo tiro doppio del giocatore di turno)
                {
                    if(dg->array_giocatori[*turno].budget>=0) // Se il giocatore di turno non ha budget negativo
                    {
                        if(dg->array_giocatori[*turno].pulizia_bagni==false) // Se il giocatore di turno non sta pulendo i bagni
                        {
                            /// Se si arriva in questo punto il giocatore non ha budget negativo e non sta pulendo i bagni
                            /// dunque può effettivamente eseguire un altro tiro bonus
                            printf("\nTiro doppio! Puoi tirare di nuovo i dadi! (Che culo comunque)\n\n"); // Notifica che si possono ritirare i dadi
                        }
                        else // Se il giocatore sta pulendo i bagni
                        {
                            printf("\nTiro doppio! Ma sei finito a lavare i bagni, non puoi tirare di nuovo!\n\n"); // Notifica che non si può tirare nuovamente nonostante il tiro doppio
                            *turno=(*turno+1)%dg->dettagli_partita.num_giocatori; // Passa il turno al giocatore successivo
                        }
                    }
                    else // Se il giocatore di turno ha budget negativo
                    {
                        printf("\nTiro doppio! Peccato pero' che stai per andare in bancarotta, non puoi ritirare!"); // Notifica che non si può tirare nuovamente nonostante il tiro doppio
                        *turno=(*turno+1)%dg->dettagli_partita.num_giocatori; // Passa il turno al giocatore successivo
                    }
                }
                else // Se il tiro doppio è il terzo consecutivo del giocatore di turno
                {
                    printf("\nUn altro tiro doppio? Stai cheatando sicuramente. Fila a lavare i bagni!\n\n"); // Notifica che si devono lavare i bagni per tre tiri doppi consecutivi
                    dg->array_giocatori[*turno].turni_pulizia_bagni=3; // Imposta i turni di pulizia dei bagni a 3
                    dg->array_giocatori[*turno].posizione=10; // Sposta il giocatore nella casella che rappresenta i bagni (senza passare dal Via!)
                    *turno=(*turno+1)%dg->dettagli_partita.num_giocatori; // Passa il turno al giocatore successivo
                }
            } // FINE CONTROLLI TIRO DOPPIO
        }// FINE GIOCATORE NON NEGATIVO

        /** Preparazione all'iterazione successiva **/
        dg->dettagli_partita.num_turni_giocati++; // Incrementa il numero dei tiri giocati
        dado1=0; // Azzera dado1
        dado2=0; // Azzera dado2
        strcpy(scelta, "\0"); // Azzera la scelta del menù
        pausa();
    } // FINE LOOP DI GIOCO

    return EXIT;
} // FINE GIOCA()



/**
    Procedura che si occupa di eseguire le azioni che la casella in cui il giocatore è capitato prevede.
    Se la casella è di tipo TASSA calcola la tassa, e la paga aggiornando il budget
    Se la casella è di tipo ALTRO, interessa solo se questa è la batcaverna; in tal caso spedisce il giocatore ai bagni per tre turni

    Se la casella è di tipo AULA:
            Se il giocatore è proprietario gli si propone la costruzione di immobili (dopo aver verificato le condizioni da regolamento per la costruzione)
            Se il proprietario è un altor giocatore, la funzione calcola e paga il pedaggio al giocatore proprietario (solo se questo non sta lavando i bagni, vedi regolamento)

            Se il proprietario è il banco:
                Se si è in modalità bonus, si può acquistare l'aula (se si dispone di denaro sufficiente) altrimenti si paga pedaggio al banco
                NOTA: per questo utilizza compra_aula() che potrebbe generare uno stato ALLOCATION_FAILURE, gestito poi in gioca()

    Se la casella è di tipo sfiga:
        Se la sfiga è di tipo TASSA_SEMPLICE o TASSA_IMMOBILI, la funzione calcola e paga la tassa
        Se la sfiga è di tipo BATTUTA_MARTINA, stampa a schermo la battuta estratta
        Se la sfiga è di tipo GOTO, questa funzione manda il giocatore nella casella indicata dalla sfiga e si chiama ricorsivamente sulla nuova destinazione

    INPUT: dati di gioco, id del giocatore di turno, casella di destinazione
    OUTPUT: esegue l'azione prevista dalla casella destinazione
**/
void esegui_azione_destinazione(Game* dg, int* turno, Casella* destinazione)
{
    int sfiga_estratta; // Contiene l'indice della sfiga estratta
    int pedaggio_totale=0; // Variabile di supporto per il calcolo del pedaggio

    switch(destinazione->tipo) // Valuta il tipo della casella di destinazione
    {
        case TASSA: // Se la destinazione è di tipo TASSA
            printf("\n\nDevi pagare %d euro di tasse! \n", destinazione->pedaggio); // Notifica l'importo da pagare
            dg->array_giocatori[*turno].budget -= destinazione->pedaggio; // Paga il pedaggio e aggiorna il budget
            break;

        case SFIGA: // Se la destinazione è di tipo SFIGA
            sfiga_estratta=estrai_sfiga(dg->num_sfighe); // Estrae una sfiga

            switch(dg->array_sfighe[sfiga_estratta].tipo_sfiga) // Valuta il tipo della sfiga estratta
            {
                case TASSA_SEMPLICE: // Se la sfiga è di tipo TASSA_SEMPLICE
                    printf("\n\n%s", dg->array_sfighe[sfiga_estratta].msg); // Stampa il messaggio della sfiga
                    printf("\nCi hai rimesso %d euro\n", dg->array_sfighe[sfiga_estratta].tassa); // Notifica l'ammontare della tassa
                    dg->array_giocatori[*turno].budget-=dg->array_sfighe[sfiga_estratta].tassa; // Paga la tassa e aggiorna il budget
                    break;

                case TASSA_IMMOBILI: // Se la sfiga è di tipo TASSA_IMMOBILI
                    printf("\n\n%s", dg->array_sfighe[sfiga_estratta].msg); // Stampa il messaggio della sfiga
                    pedaggio_totale=0; // Azzera il totale da pagare (per sicurezza)

                    /// Formula del pedaggio: sommatoria di ciascun immobile posseduto * relativo moltiplicatore
                    pedaggio_totale=dg->array_giocatori[*turno].n_aule    * dg->array_sfighe[sfiga_estratta].moltiplicatore_n_aule  +
                                    dg->array_giocatori[*turno].num_scriv * dg->array_sfighe[sfiga_estratta].moltiplicatore_n_scriv +
                                    dg->array_giocatori[*turno].num_tende * dg->array_sfighe[sfiga_estratta].moltiplicatore_n_tende ;

                    printf("\nTassa totale: %d\n", pedaggio_totale); // Notifica l'ammontare della tassa
                    dg->array_giocatori[*turno].budget-=pedaggio_totale; // Paga la tassa e aggiorna il budget
                    break;

                case BATTUTA_MARTINA: // Se la sfiga è di tipo BATTUTA_MARTINA
                    printf("          _.===.                    \n");
                    printf("       _,/_ \\\\\\ `-.                 \n");
                    printf("      /`/| /      _\\'               \n");
                    printf("     // ` /////////\\'               \n");
                    printf("    | /`|/=*===*=| (                \n");
                    printf("   /`/` /|<o>)(o>(  )               \n");
                    printf("   \\  \\/\\|    /  /\\/   <<==(Martina)\n");
                    printf("   \\`/` \\/\\ `=' /                   \n");
                    printf("   |/\\/` \\/`._.\\\\                   \n");
                    printf("   /` |/`/      \\\\                 \n");
                    printf("   \\//| /        \\\\                 \n");
                    printf("    ' '/          \\\\                \n");
                    printf("  _________________\\\\____________________________________________\n");
                    printf(" /\n");
                    printf("| %s\n", dg->array_sfighe[sfiga_estratta].msg); // Stampa il messaggio della sfiga
                    break;

                case GOTO:
                    printf("\n\n%s\n", dg->array_sfighe[sfiga_estratta].msg); // Stampa il messaggio della sfiga
                    printf("\n(vai alla casella %d, %s)\n", dg->array_sfighe[sfiga_estratta].casella_destinazione, dg->tabellone[dg->array_sfighe[sfiga_estratta].casella_destinazione].nome); // Notifica in quale casella si viene mandati dalla sfiga estratta

                    /// Se la posizione indicata dalla sfiga GOTO è minore o uguale alla posizione attuale, allora si è passati dal Via!
                    if(dg->array_sfighe[sfiga_estratta].casella_destinazione<=dg->array_giocatori[*turno].posizione) // Valuta il passaggio dal Via!
                    {
                        printf("\n+20 euro per essere passato dal Via!\n"); // Notifica il passaggio dla via
                        dg->array_giocatori[*turno].budget+=20; // Aggiorna il budget
                    }
                    dg->array_giocatori[*turno].posizione=dg->array_sfighe[sfiga_estratta].casella_destinazione; // Aggiorna la posizione del giocatore
                    esegui_azione_destinazione(dg, turno, &dg->tabellone[dg->array_giocatori[*turno].posizione]); // Chiamata ricorsiva per la nuova destinazione
                    break;
            }
            break;// FINE CASO SFIGA

        case AULA: // Se la casella di destinazione è di tipo AULA

            if(destinazione->proprietario!=*turno) // Se il giocatore non è proprietario della destinazione
            {
                if(destinazione->proprietario==-1) // Se il banco è proprietario della destinazione
                {
                    if(dg->bonus_mode) // Se si sta giocando la modalità bonus
                    {
                        /// Essendo in modalità bonus, se il giocatore possiede abbastanza denaro, può comprare l'aula
                        if(dg->array_giocatori[*turno].budget>=destinazione->costo) // Se si possiede abbastanza denaro
                        {
                            printf("\nVuoi comprare l'aula? (costa %d)", destinazione->costo); // Proposta di comprare l'aula

                            if(menu_si_no()=='s')
                            {
                                compra_aula(dg->array_giocatori, turno, destinazione, &dg->stato_gioco); // Effettua l'acquisto dell'aula
                                /// Se dovesse esserci errore nell'allocazione del nodo della lista per registrare l'aula comprata,
                                /// la gestione dell'errore avverrà in gioca()
                                dg->array_giocatori[*turno].n_aule++; // Aggiorna il contatore di aule possedute
                            }
                        }
                        else printf("\nSei finito in un aula acquistabile ma non hai i soldi per comprarla.\n"); // Se il giocatore non ha abbastanza soldi per comprare l'aula, notifica l'evento
                    }// FINE AULA BONUS MODE
                    /// Se non si sta giocando la modalità bonus, non è possibile comprare l'aula e si paga pedaggio al banco
                    else // Se NON si sta giocando la modalità bonus
                    {
                        printf("\nDevi pagare il pedaggio al banco. Ammontare: %d\n", MOLTIPLICATORE_PEDAGGIO*destinazione->pedaggio); // Notifica l'ammontare del pedaggio
                        dg->array_giocatori[*turno].budget -= MOLTIPLICATORE_PEDAGGIO*destinazione->pedaggio; // Paga il pedaggio e aggiorn ail budget
                    }

                } // FINE PROPRIETARIO -1

                else // Se il proprietario è un giocatore diverso dal quello di turno
                {
                    pedaggio_totale=0; // Azzeramento totale da pagare (per sicurezza)
                    if(destinazione->ha_tenda==true) // Se nell'aula è presente una tenda
                        pedaggio_totale=MOLTIPLICATORE_PEDAGGIO* destinazione->pedaggio_tenda;
                    else if(destinazione->ha_scrivania) // Se nell'aula è presente una tenda
                        pedaggio_totale=MOLTIPLICATORE_PEDAGGIO*destinazione->pedaggio_scriv;
                    else pedaggio_totale=MOLTIPLICATORE_PEDAGGIO*destinazione->pedaggio; // Se nell'aula non ci sono tende o scrivanie

                    printf("\nDevi pagare il pedaggio a %s. Ammontare: %d\n", dg->array_giocatori[destinazione->proprietario].nome, pedaggio_totale); // Notific al'ammontare da pagare

                    /// Se il proprietario sta pulendo i bagni, non gli si paga il pedaggio
                    if(dg->array_giocatori[destinazione->proprietario].pulizia_bagni==true) // Se il giocatore proprietario dell'aula sta lavando i bagni
                        printf("\nMa %s e' distratto (sta pulendo i bagni) e non puo' vederti. Che fortuna!\n", dg->array_giocatori[destinazione->proprietario].nome); // Notifica l'evento
                    /// Se il proprietario non sta pulendo i bagni, si paga regolarmente pedaggio
                    else dg->array_giocatori[*turno].budget -= pedaggio_totale; // Paga il pedaggio e aggiorn ail budget
                }
            }// FINE PROP DIVERSO DA GIOCATORE

            else // Se il proprietario dell'aula è il giocatore di turno
            {
                /// Mentre non è consentito comprare un aula se ciò fa si che il budget diventi negativo,
                /// è invece consentito comprare scrivanie/tende, per consentire qualche piccola
                /// strategia di gioco in più

                /// Non ci sono nè tende nè scrivanie, costruire una scrivania?
                if(destinazione->ha_scrivania==false && destinazione->ha_tenda==false) // Se nell'aula non ci sono nè tende nè scrivanie
                {
                    printf("\nVuoi costruire una scrivania? (costa %d)", COSTO_SCRIVANIA); // Notifica costo scrivania e chiede si/no

                    if(menu_si_no()=='s') // Se l'utente decide si
                        costruisci_scrivania(dg->array_giocatori, turno, destinazione); // Effettua la costruzione della scrivania
                } // FINE NO SCRIVANIA, NO TENDA

                /// C'è scrivania ma non tenda, rimuovere la scrivania e mettere una tenda al suo posto?
                else if(destinazione->ha_scrivania==true && destinazione->ha_tenda==false) // Se nell'aula non ci sono tende ma c'è una scrivania
                {
                    printf("\nVuoi costruire una tenda? (costa %d)", COSTO_TENDA); // Notifica costo tenda e chiede si/no

                    if(menu_si_no()=='s') // Se l'utente decide si
                        costruisci_tenda(dg->array_giocatori, turno, destinazione); // Effettua la costruzione della tenda (rimuove la scrivania)
                } // FINE HA GIà SCRIVANIA
            }
            break; // FINE CASO AULA

        case ALTRO: // Se la casella di destinazione è di tipo ALTRO
            if(strcmp(destinazione->nome,"BATCAVERNA!")==0) // Se la destinazione è la batcaverna
            {
                printf("\nTi hanno beccato in Batcaverna. Vai a lavare i bagni! (3 turni)\n"); // Notifica la posizione in batcaverna

                /// Come da regolamento, il giocatore finisce a lavare i bagni per 3 turni
                dg->array_giocatori[*turno].turni_pulizia_bagni=3;
                dg->array_giocatori[*turno].pulizia_bagni=true;
                dg->array_giocatori[*turno].posizione=10; // Sposta il giocatore nei bagni
            }
            break;
    }
} // FINE ESEGUI_AZIONE_DESTINAZIONE()


/**
    Funzione che tiene memoria dell'ultima sfiga estratta e restituisce l'indice della successiva

    Nota: se si estrae l'ultima sfiga del "mazzo" (l'array), si rincomincia dalla prima

    INPUT: numero sfighe nell'array
    OUTPUT: indice della sfiga successiva all'ultima estratta
**/
int estrai_sfiga(int num_sfighe)
{
    static int indice_sfiga=0; // Si inizia dalla sfiga nella posizione 0
    indice_sfiga=(indice_sfiga+1)%num_sfighe; // Estrae la sfiga successiva all'ultima estratta
    return indice_sfiga;
}


/**
    Funzione che gestisce l'eliminazione di un giocatore dalla partita.

    Nota: restituisce ALLOCATION_FAILURE se avviene un errore nella riallocazione dell'array giocatori

    INPUT: dati di gioco, id del giocatore da eliminare
    OUTPUT: elimina le liste del giocatore, elimina il giocatore dall'array, aggiorna gli id degli
            altri giocatori e aggiorna il tabellone

**/
void elimina_giocatore(Game* dg, int id_giocatore)
{
    int i=0; // Indice per cicli

    /** Eliminazione della lista aule possedute dal giocatore **/
    elimina_lista(dg->array_giocatori[id_giocatore].lista_aule_poss);

    /** Restituzione delle aule possedute dal giocatore al banco e modifica id altri giocatori sul tabellone **/
    /// Se l'aula è del giocatore da eliminare, va restituita al banco senza scrivanie o tende eventuali.
    /// Se l'aula NON è del giocatore da eliminare, ma di uno dei giocatori con id successivo al suo, l'id del proprietario
    /// dell'aula va decrementato di 1 in quanto, una volta che il giocatore sarà eliminato, gli id degli altri giocatori (con id superiore
    /// a quello appena eliminato) saranno aggiornati (decrementati di 1).
    /// Quest'ultima modifica preserva la consistenza dei dati sul tabellone.
    for(i=0; i< NUM_CASELLE; i++) // Scorre tutto il tabellone
    {
        if(dg->tabellone[i].proprietario==id_giocatore) // Se il proprietario della cella i-esima è il giocatore da eliminare
        {
            dg->tabellone[i].proprietario=-1; // Restituisce l'aula al banco
            dg->tabellone[i].ha_scrivania=false; // Elimina eventuali scrivanie
            dg->tabellone[i].ha_tenda=false; // Elimina eventuali tende
        }
        if(dg->tabellone[i].proprietario>id_giocatore) // Se la casella i-esima appartiene a un giocatore
            dg->tabellone[i].proprietario--; // Decrementa l'id del proprietario segnato sul tabellone
    }

    /** Eliminazione del giocatore dall'array **/
    /// Vengono copiati tutti i giocatori con id superiore a quello da eliminare nella cella
    /// precedente rispetto a quella in cui si trovano nell'array giocatori. Dunque, l'array,
    /// viene ridimensionato con una cella in meno e il giocatore da eliminare risulterà effettivamente eliminato
    for(i=id_giocatore+1; i<dg->dettagli_partita.num_giocatori; i++) // Scorre tutti i giocatori successivi a quello da eliminare
        dg->array_giocatori[i-1]=dg->array_giocatori[i]; // Sposta il giocatore i-esimo nella cella precedente

    dg->dettagli_partita.num_giocatori-=1; // Decrementa il numero di giocatori in partita
    dg->array_giocatori=(Giocatore*)realloc(dg->array_giocatori, (dg->dettagli_partita.num_giocatori)* sizeof(Giocatore)); // Ridimensiona l'array giocatori togliendo una cella
    dg->stato_gioco=isPtrValid(dg->array_giocatori); // Verifica che l'allocamento sia andato a buon fine

    /// Alla fine di questa funzione dg->stato_gioco vale RUN se tutto è andato a buon fine
    /// ALLOCATION_FAILURE altrimenti
}


/**
    Procedura che effettua le modifiche necessarie all'acquisto di una scrivania

    INPUT: array con i giocatori, id giocatore di turno (che effettua l'acquisto), aula in cui
           costruire la scrivania
    OUTPUT: budget giocatore aggiornato, flag presenza scrivania aggiornato, contatore scrivanie possedute
            dla giocatore aggiornato
**/
void costruisci_scrivania(Giocatore* array_giocatori, int* turno, Casella* aula)
{
    array_giocatori[*turno].budget -= COSTO_SCRIVANIA; // Sottrae dal budget del giocatore il costo della scrivania
    aula->ha_scrivania=true; // Ora nell'aula specificata è presente una scrivania
    array_giocatori[*turno].num_scriv++; // Incrementa il contatore di scrivanie del giocatore che effettua l'acquisto
}


/**
    Procedura che effettua le modifiche per l'acquisto di una tenda

    INPUT: array con i giocatori, id giocatore di turno, aula dove costruire la tenda
    OUTPUT: budge aggiornato, flag di presenza scrivania e tenda aggiornati, contatori scrivanie e tende del giocatore
            aggiornati
**/
void costruisci_tenda(Giocatore* array_giocatori, int* turno, Casella* aula)
{
    array_giocatori[*turno].budget -= COSTO_TENDA; // Sottrae il costo della tenda dal budget del giocatore che effettua l'acquisto
    aula->ha_scrivania=false; // Se si compra una tenda, la scrivania va eliminata dall'aula specificata
    aula->ha_tenda=true; // Ora è presente una tenda nell'aula specificata
    array_giocatori[*turno].num_scriv--; // Decrementa il numero di scrivanie del giocatore
    array_giocatori[*turno].num_tende++; // Incrementa il numero di tende possedute dal giocatore
}


/**
    Procedura che effettua le modifiche necessarie all'acquisto di un aula

    NOTA: dovendo aggiornare la lista delle aule possedute, questa funzione potrebbe generare un errore di tipo
          ALLOCATION_FAILURE che verrà scritto nello stato e gestito nelle funzioni chiamanti

    INPUT: array dei giocatori, id giocatore di turno, casella acquistata, puntatore alla variabile che indica lo stato del programma
    OUTPUT: sottrae il costo dell'aula dal budget del giocatore, aggiunge la nuova aula alla sua lista e segna
            il nuovo proprietario sul tabellone
**/
void compra_aula(Giocatore* array_giocatori, int* turno, Casella* aula_acquistata, GameState* stato)
{
    array_giocatori[*turno].budget -= aula_acquistata->costo; // Sottrae dal budget del giocatore che effettua l'acquisto, il costo dell'aula
    inserisci_in_testa(array_giocatori[*turno].lista_aule_poss, array_giocatori[*turno].posizione, stato); // Inserisce nella lista delle aule del giocatore che effettua l'acquisto la nuova aula
    /// Se dovesse esserci errore nell'allocazione del nodo della lista, la gestioen dell'errore
    /// avverrà in gioca()
    aula_acquistata->proprietario=*turno; // Segna sul tabellone il nuovo proprietario dell'aula acquistata
}


/**
    Funzione che simula il lancio di un dado
    INPUT: Numero di facce del dado
    OUTPUT: Numero intero casuale tra 1 e n_facce (estremi compresi)
**/
int lancia_dado(int n_facce) { return 1+rand()%(n_facce); }

