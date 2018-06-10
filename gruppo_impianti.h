/** @file gruppo_impianti.h
  * @brief Collezione di impianti con metodi di lettura, inserimento e rimozione
  *
  * @copyright (C)2018, NirGis Group
  * @version 1.0.0
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
  */

#ifndef GRUPPO_IMPIANTO_H
#define GRUPPO_IMPIANTO_H
#include "shapefil.h"
//#include "gruppo_antenne.h"
#include "gruppo_modelli.h"
#include "impianto.h"

/* costanti impianto:
* nomi dei campi del DB e dei tag del file xml corrispondenti alle variabili.
* Strutturata come matrice di singoli char: ogni parola occupa una singola riga e può 
* essere lunga 20 caratteri al massimo
*/
const int NUM_IMP_FLD = 29;
const char IMP_FLD[NUM_IMP_FLD][20] = {"IDSITO","NOME","CODSITO","INDIRIZZO","X_SITO",
                                        "Y_SITO","Z_SITO","RESPIMP","IDGEST","IDPROV",
                                        "IDCOMUNE","IDSTATO","NOTE1","NOTE2","IMMAGINE",
                                        "IDRESP","ID_PADRE","CREAZIONE","DATAMODIFY",
                                        "PARERE","COMUNICAZ","DISMISSION","IDPONTE",
                                        "IDPOST","COMDIR","ATTEDIF","RICDIRETTA","CHECKED","INFLUENTE"};

/* costanti antenne:
* nomi dei campi del DB e dei tag del file xml corrispondenti alle variabili.
* Strutturata come matrice di singoli char: ogni parola occupa una singola riga e può 
* essere lunga 20 caratteri al massimo
*/
const int NUM_ANT_FLD = 28;                                          
const char ANT_FLD[NUM_ANT_FLD][20]={"IDANT","X_ANT","Y_ANT","H","N_TX","POT_TX","TILT",
                                    "DIREZIONE","DATA_INI","DATA_FIN","ID_PADRE","ATTIVA",
                                    "IDSITO","IDMOD","IDSERV","NOTE1","IMMAGINE","FMIN",
                                    "FMAX","FREQUENZA","FASE","PPERC","IDSTATO","ALFA24",
                                    "ATT_PC","ATT_DTX","IDPOLAR","CHECKED"};
class gruppoImpianti {

    private:
        //mancano i metodi get/set di questi campi privati. Se necessario li aggiungo

        bool calcolaAlfa24;              /**< Flag sull'utilizzo del fattore alfa24 nel calcolo */
        gruppoModelli * grpModelli;      /**< Lista di modelli su cui fare ricerche utilizzata da associaModelli*/
        char strErr[100000];
    public:

        vector<impianto*> impianti;      /**< Vettore contenente gli impianti caricati*/
        vector<impianto*>::iterator itI; /**< Iteratore per lo scorrimento degli impianti all'interno del vettore */
        char md5FileImp[40];             /**< L'MD5 del file letto o salvato da leggiImpiantiAnatenneXML e scriviImpiantiAnatenneXML */ 

        /**
        * Costruttore 
        */
        gruppoImpianti();

        /**
        * Costruttore deep copy
        */
        gruppoImpianti(gruppoImpianti * gImp);
        /**
        * Distruttore che si occupa di cancellare gli oggetti di tipo impianto allocati nello heap 
        */
        ~gruppoImpianti();

        /**
        * Usato per debug. crea un  file che contiene gli errori (o i warning) contenuti nelle variabili statiche 
        * static string  debugAntenne; della classe antenne e
        * static string debugModelli; della classe modelli.
        */
        void creaFileErrori();

        /**
        * restituisce gli errori (o i warning) contenuti nelle variabili statiche 
        * static string  debugAntenne; della classe antenne e
        * static string debugModelli; della classe modelli.
        */
        char * getStrErrori();

        /**
        * azzera la stringa degli errori
        */
        void azzeraStrErrori();
        /**
        * Questo permette di svuotare il vettore impianti in maniera profonda: 
        * cancella, oltre al puntatore (per cui basta il metodo clean), anche l'oggetto puntato.
        * Utilizzato dal metodo gruppoImpianti::leggiImpiantiAnatenneXML
        */
        void deleteImpianti();

        /**
        * Permette di aggiungere un impianto al vettore e l'inserimento avviene in coda 
        *
        * @param newImp L'impianto di antenne da inserire
        *
        * @return Il numero aggiornato degli impianti contenuti nel vettore 
        */ 
        int addImpianto(impianto * newImp); 

        /**
        * A ciascuna antenna di ogni impianto presente nel vettore impianti
        * viene associato il modello di antenna indicato nel campo dati idModello della classe antenna. 
        * Se non è presente il modello ricercato, all'antenna non viene associato alcun modello.
        *
        * @param gMod L'elenco di modelli dove cercare i modelli richiesti
        *
        * @return Il numero di antenne alle quali non è stato associato il modello di antenna
        */
        int associaModelli();

        /**
        * Funzione utilizzata da python che posiziona l'iteratore itI sul primo elemento del vettore impianti
        */
        void beginIter();

        /**
        * Funzione utilizzata da python per leggere il vettore impianti in forma di iteratore python.
        * Se l'iteratore itI non è posizionato sull'ultimo elemento del vettore la funzione restituisce 
        * l'elemento puntato da itI e sposta l'iteratore itI sull'elemento successivo
        *
        * @return Il prossimo impianto contenuto nel vettore o, nel caso non siano presenti 
        * altri impianti, un valore nullo
        */
        impianto * nextItem();

        /**
        * Funzione che scorre tutto il vettore ricercando l'impianto il cui ID è uguale al 
        * valore preso in input
        *
        * @param id ID dell'impianto da ricercare tra quelli memorizzati nel vettore
        *
        * @return L'impianto il cui ID corrisponde a quello ricercato, nel caso sia effettivamente
        * presente nell'array, altrimenti un valore nullo
        */ 
        impianto * getImpById(int id);

        /**
        * rimuove l'impianto specificando il suo ID 
        * l'antenna il cui ID è uguale al valore preso in input 
        * 
        * @param id ID dell'impianto da rimuovere
        * 
        *  @return Ritorna <code>0</code> se è stata cancellato l'impianto,
        * <code>1</code> se non è stato cancellato nessun impianto
        * 
        */
        int removeImpById(int id);


        /**
        * Funzione che scorre tutte le antenne di tutti gli impianti presenti nel vettore ricercando 
        * l'antenna il cui ID è uguale al valore preso in input
        *
        * @param id ID dell'antenna da ricercare tra quelle che compongono gli impianti presenti nel vettore
        *
        * @return L'antenna il cui ID corrisponde a quello ricercato, nel caso sia effettivamente
        * presente, altrimenti un valore nullo
        */
        antenna * getAntById(int id);  

        /**
        * Funzione che scorre tutte le antenne di tutti gli impianti presenti nel vettore ricercando 
        * l'antenna il cui ID è uguale al valore preso in input 
        * 
        * @param id ID dell'antenna da cancellare
        * 
        *  @return Ritorna <code>0</code> se è stata cancellata l'antenna,
        * <code>1</code> se non è stata cancellata nessuna antenna
        * 
        */
        int removeAntById(int id);


        /**
        * Funzione che permette di caricare da un file xml i dati per popolare la classe
        * (utilizzando la funzione setDI per le singoli voci della classe impianto e setDA per la classe antenna) e 
        * aggiorna md5FileMod con l'MD5 del file xml letto (side-effect)
        *
        * @param nomeFile Nome del file contenente i dati sull'impianto di antenne
        *
        * @return Ritorna <code>0</code> se l'operazione è andata a buon fine,
        * <code>1</code> se nel file da leggere ci sono alcuni dati che possono essere inesatti o mancanti 
        * e <code>2</code> se non è stato possibile aprire il file
        */ 
        int leggiImpiantiAntenneXML(char * nomeFile);

        /**
        * Funzione per memorizzare la lista di impianti di antenne di questa classe in un file xml
        * (utilizzando la funzione getDI per le singoli voci della classe impianto e getDA per la classe antenna) e 
        * aggiorna md5FileMod con l'MD5 del file xml dove si sono caricati i dati (side-effect)
        *
        * @param nomeFile Nome del file dove salvare la lista di impianti di questa classe
        *
        * @return Ritorna <code>0</code> se l'operazione è andata a buon fine,
        * <code>1</code> altrimenti
        */ 
        int scriviImpiantiAntenneXML(char * nomeFile);
        
        /**
        * Funzione che permette di caricare da un file xml i dati dei modelli
        * nella classe gruppoModelli istanziata nel costruttore: grpModelli
        *
        * @param nomeFile Nome del file contenente i dati dei modelli
        *
        * @return Ritorna <code>0</code> se l'operazione è andata a buon fine,
        * <code>1</code> se nel file da leggere ci sono alcuni dati che possono essere inesatti o mancanti 
        * e <code>2</code> se non è stato possibile aprire il file
        */
        
        int leggiModelliDiagrammiXML(char * nomeFile);
        /**
        * Funzione che permette di scrivere i dati dei modelli
        * nella classe gruppoModelli istanziata nel costruttore: grpModelli
        *
        * @param nomeFile Nome del file contenente i dati dei modelli
        *
        * @return Ritorna <code>0</code> se l'operazione è andata a buon fine,
        * <code>1</code> se nel file da leggere ci sono alcuni dati che possono essere inesatti o mancanti 
        * e <code>2</code> se non è stato possibile aprire il file
        */
        int scriviModelliDiagrammiXML(char * nomeFile);

        /**
        * Funzione che aggiunge un nuovo oggetto della classe impianto (allocato in heap) al vettore
        * impianti (tramite la funzione addImpianto) e restituisce il nuovo oggetto creato
        */
        impianto * newImpianto();
        
        /**
        * Funzione che aggiunge un nuovo modello alla classe gruppoModelli (allocata in grpModelli)
        *  e restituisce il nuovo oggetto
        * @return Ritorna il nuovo oggetto modello
        */
        modello * newModello();
        
        /**
        * Funzione che restituisce il gruppo modelli utilizzato dalla classe (grpModelli)
        * @return Ritorna il gruppo (grpModelli)
        */        
        gruppoModelli * getGruppoModelli();
        
        /**
        * Funzione che restituisce il modello cercandolo con il suo id  dalla classe (grpModelli)
        * 
        * @param id Identificativo del modello (ID_MOD) da ricercare 
        * 
        * @return Ritorna il modello trovato o NULL
        */ 
        modello * getModById(int id);

        /**
        * Cancella una modello specificando il suo id
        * 
        * @param id ID del modello da cancellare
        * 
        *  @return Ritorna <code>0</code> se è stato cancellato il modello,
        * <code>1</code> se non è stata cancellato nessun modello
        * 
        */
        int removeModById(int id);

        /**
        * Ritorna il minimo id delle antenne presenti nel gruppo impianti
        * se ci sono id negativi altrimenti ritorna zero. Utilizzato per
        * inserire nuovi Modelli con id negativoche non sono caricati nel DB
        */ 
        int getMinNegAntID();
        
        /**
        * Ritorna il minimo id degli impianti presenti nel gruppo impianti
        * se ci sono id negativi altrimenti ritorna zero. Utilizzato per
        * inserire nuovi Modelli con id negativoche non sono caricati nel DB
        */         
        int getMinNegImpID();
        
        /**
        * Scrive MD5 del file corrispondente, utilizzato nella deep copy
        */
        int setMd5(char* valDato);
        
        char * getMd5();
        
        /**
        * Aggiunta di un impianto.
        * Per id positivi viene aggiunto l'impianto solo se non è presente
        * un impianto o una antenna con lo stesso id 
        * Se l'id dell'impianto che viene inserita è 0 allora viene inserita
        * con il primo id negativo.
        * Se l'id dell'impianto o di una antenna è negativo e già presente
        * allora viene
        * cambiato con un nuovo id negativo (il più basso -1)
        *  
        * @param  impianto da aggiungere
        * 
        * @return Ritorna <code>0</code> se l'inserimento è andato a buon fine,
        * <code>1</code> se l'impianto non è stato aggiunto
        * 
        */
        int somma(impianto * newImp);
        
        
        
         /**
        * Unione di due gruppi di impianti: alla classe vengono aggiunti
        * gli impianti della classe gruppoImpianti passata come parametro
        * 
        * @param un gruppoImpianti da aggiungere alla classe
        * 
        * @return Ritorna <code>0</code> se l'inserimento è andato a buon fine,
        * <code>n</code> con n il numero di impianti non aggiunti
        */       
        int somma(gruppoImpianti * newGrImp);
        

        void inizializzaCalcolo();
        
        /**
        * imposta l'influenza dei siti nel calcolo pasando il nome dello
        * shape che contiene come seconda geometria l'area di unfluenza
        */
        int setInfluenti(char * nomeFileShape);
        void setInfluenti(SHPObject * aInf);
        
        /** PNPOLY - Point Inclusion in Polygon Test W. Randolph Franklin (WRF)
        *  verifica se un punto si trova all'interno di un poligono
        */
        int pnpoly(int nvert, double *vertx, double *verty, double testx, double testy);
        
        /**
        * ordina tutte le antenne all'interno degli impianti
        */
        void ordinaSorgenti();
        
        /**
        * restituisce un nuovo gruppoImpianti contenente solo gli impianti
        * checked e influenti contenennti solo le antenne checked
        * le antenne saranno ordinate con ordinaSorgenti
        */
        gruppoImpianti * getActiveImp();
        
        /**
        * restituisce il numero degli impianti contenuti nel gruppoImpianti
        * @return Ritorna gruppoImpianti
        */
        int size();
};
#endif
