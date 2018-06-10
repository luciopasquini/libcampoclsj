/** @file gruppo_modelli.h
  * @brief Collezione di modelli di antenna con metodi di lettura, inserimento e rimozione
  *
  * @copyright (C)2018,  NirGis Group
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

#ifndef GRUPPO_MODELLI_H
#define GRUPPO_MODELLI_H

#include <set>
#include <string>
#include <vector>

#include "modello.h"

using namespace std;

/* costanti modello antenna:
 * nomi dei campi del DB e dei tag del file xml corrispondenti alle variabili
 */
const int NUM_MOD_FLD = 9;
const char MOD_FLD[NUM_MOD_FLD][20]={"IDMOD","MODELLO","MARCA","FREQUENZA","TILT_EL",
                                    "GUADAGNO","LUNGHEZZA","NOTE1","IMMAGINE"};  

class gruppoModelli {

public: 

    vector<modello*> modelli;       /**< Vettore contenente i modelli di antenna caricati*/ 
    vector<modello*>::iterator itM; /**< Iteratore per lo scorrimento dei modelli di antenna all'interno del vettore */ 
    char md5FileMod[40];            /**< L'MD5 del file ModelliDiagrammiXML che contiene l'elenco dei modelli di questa classe */ 

    /**
    * Costruttore 
    */
    gruppoModelli();
    
    /**
    * Costruttore deep copy
    */
    gruppoModelli(gruppoModelli * gMod);
    
    /**
    * Distruttore 
    */
    ~gruppoModelli();

    /**
    * Aggiunta di un modello:
    * Per id positivi viene aggiunto il modello solo se non è presente
    * una modello con lo stesso id.
    * Se l'id del Modello che viene inserito è 0 allora viene inserita
    * con id negativo .
    * Se l'id del modello è negativo e già presente allora viene
    * cambiato con un nuovo id negativo (il più basso -1)
    * L'id negativo significa che è un nuovo modello appena definito e
    * non ancora caricato nel DB 
    * 
    * @param un modello
    * 
    * @return Ritorna <code>0</code> se l'inserimento è andato a buon fine,
    * <code>1</code> se lo'antenna non è stata aggiunta
    * 
    */
    int somma(modello * newMod);

    
    /**
    * Ritorna il minimo id di un modello se ci sono id negativi altrimenti
    * ritorna zero. Utilizzato per inserire nuovi Modelli con id negativo
    * che non sono caricati nel DB
    */ 
    int getMinNegID();

    /**
    * Unione di due gruppi di modelli: alla classe vengono aggiunti
    * i modelli della classe gruppoModelli passata come parametro
    * 
    * @param un gruppoModelli da aggiungere alla classe
    * 
    * @return Ritorna <code>0</code> se l'inserimento è andato a buon fine,
    * <code>n</code> con n il numero di modelli non aggiunti
    * 
    */
    int somma(gruppoModelli * aGM);

    /**
    * Questo permette di svuotare il vettore modelli in maniera profonda: 
    * cancella, oltre al puntatore (per cui basta il metodo clean), anche l'oggetto puntato.
    * Utilizzato dal metodo gruppoModelli::leggiModelliDiagrammiXML
    */
    void deleteModelli();

    /**
    * Questo è il numero di modelli di antenna effettivamente contenuti nel vettore
    *
    * @return Il numero di modelli di antenna effettivamente contenuti nel vettore
    */
    int size(); 

    /**
    * Permette di aggiungere un modello di antenna al vettore e l'inserimento avviene in coda
    *
    * @param newMod Il modello da inserire
    *
    * @return Il numero aggiornato dei modelli contenuti nel vettore 
    */   
    int addModello(modello * newMod); 

    /**
    * Funzione che scorre tutto il vettore ricercando il modello il cui ID è uguale al 
    * valore preso in input
    *
    * @param id ID del modello da ricercare tra quelli memorizzati nel vettore
    *
    * @return Il modello il cui ID corrisponde a quello ricercato, nel caso sia effettivamente
    * presente nell'array, altrimenti un valore nullo
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
    * Funzione utilizzata da python che posiziona l'iteratore itM sul primo elemento del vettore
    */
    void beginIter();

    /**
    * Funzione utilizzata da python per leggere il vettore dei modelli di antenne in forma di iteratore python.
    * Se l'iteratore itM non è posizionato sull'ultimo elemento del vettore la funzione restituisce 
    * l'elemento puntato da itM e sposta l'iteratore itM sull'elemento successivo
    *
    * @return Il prossimo modello contenuto nel vettore o, nel caso non siano presenti 
    * altri modelli, un valore nullo
    */
    modello * nextItem();

    /**
    * Funzione che permette di caricare da un file xml i dati per popolare la classe
    * (utilizzando la funzione setDM per le singoli voci della classe modello) e 
    * aggiorna md5FileMod con l'MD5 del file xml letto (side-effect)
    *
    * @param nomeFile Nome del file contenente i dati sulle caratteristiche tecniche da caricare
    *
    * @return Ritorna <code>0</code> se l'operazione è andata a buon fine,
    * <code>1</code> se nel file da leggere ci sono alcuni dati che possono essere inesatti o mancanti 
    * e <code>2</code> se non è stato possibile aprire il file
    */ 
    int leggiModelliDiagrammiXML(char * nomeFile);

    /**
    * Funzione per memorizzare la lista di modelli di questa classe in un file xml
    * (utilizzando la funzione getDM per le singoli voci della classe modello) e 
    * aggiorna md5FileMod con l'MD5 del file xml dove si sono caricati i dati (side-effect)
    *
    * @param nomeFile Nome del file dove salvare la lista di modelli di questa classe
    *
    * @return Ritorna <code>0</code> se l'operazione è andata a buon fine,
    * <code>1</code> altrimenti
    */ 
    int scriviModelliDiagrammiXML(char * nomeFile);
};
#endif
