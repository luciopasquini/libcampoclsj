/** @file gruppo_antenne.h
  * @brief Collezione di antenne con metodi di lettura, inserimento e rimozione
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

#ifndef GRUPPO_ANTENNE_H
#define GRUPPO_ANTENNE_H

#include <set>
#include <string>
#include <vector>
//#include "modello.h"
#include "antenna.h"



class gruppoAntenne {
    private:
    struct ordinaAntenne {
        inline bool operator() (antenna * primaAntenna,antenna * secondaAntenna){
            if (primaAntenna->getH() == secondaAntenna->getH()) {
                if (primaAntenna->getX() == secondaAntenna->getX())
                    return (primaAntenna->getY() < secondaAntenna->getY());
                else
                    return (primaAntenna->getX() < secondaAntenna->getX());
            }
            else
                return (primaAntenna->getH() < secondaAntenna->getH());
        }
    };

    public:
        void * grImp;     /** Gruppo impianti di appartenenza */
        vector<antenna*>::iterator itA; /**< Iteratore per lo scorrimento delle antenne all'interno del vettore */
        vector<antenna*> antenne;       /**< Vettore contenente le antenne caricate*/

        /**
        * Costruttore
        */
        gruppoAntenne();
        
        /**
        * Costruttore deep copy
        */
        gruppoAntenne(gruppoAntenne * gAnt);
        
        /**
        * Distruttore che si occupa di cancellare gli oggetti di tipo antenna allocati nello heap
        */
        ~gruppoAntenne();
        
        /**
        * Ritorna il minimo id delle antenne se ci sono id negativi altrimenti
        * ritorna zero. Utilizzato per inserire nuovi Modelli con id negativo
        * che non sono caricati nel DB
        */ 
        int getMinNegID();
        
        /**
        * Aggiunta di una antenna.
        * Per id positivi viene aggiunto l'antenna solo se non è presente
        * una antenna con lo stesso id nel gruppo antenne o eventualmente
        * al gruppoImpianti di apparteneza.
        * Se l'id dell'antenna che viene inserita è 0 allora viene inserita
        * con il primo id negativo.
        * Se l'id dell'antenna è negativo e già presente allora viene
        * cambiato con un nuovo id negativo (il più basso -1)
        *  
        * @param  antenna da aggiungere
        * 
        * @return Ritorna <code>0</code> se l'inserimento è andato a buon fine,
        * <code>1</code> se lo'antenna non è stata aggiunta
        * 
        */
        int somma(antenna * newAnt);

        /**
        * Unione di due gruppi di antenne: alla classe viengono aggiunte
        * le antenne della classe gruppoAntenne passata come parametro
        * 
        * @param un gruppoAntenne da aggiungere alla classe
        * 
        * @return Ritorna <code>0</code> se l'inserimento è andato a buon fine,
        * <code>n</code> con n il numero di antenne non aggiunte
        * 
        */
        int somma(gruppoAntenne * aGA);

        /**
        * Funzione che scorre tutte le antenne presenti nel vettore antenne 
        * e restituisce l'antenna il cui ID è uguale al valore in input
        *
        * @param id ID dell'antenna da ricercare tra quelle che compongono gli impianti presenti nel vettore
        *
        * @return L'antenna il cui ID corrisponde a quello ricercato, nel caso sia effettivamente
        * presente, altrimenti un valore nullo
        */
        antenna * getAntById(int id);
        
        /**
        * Cancella una antenna specificando il suo id
        * 
        * @param id ID dell'antenna da cancellare
        * 
        *  @return Ritorna <code>0</code> se è stata cancellata l'antenna,
        * <code>1</code> se non è stata cancellata nessuna antenna
        * 
        */
        int removeAntById(int id);
        
        
        /**
        * Questo permette di svuotare il vettore antenne in maniera profonda: 
        * cancella, oltre al puntatore (per cui basta il metodo clean), anche l'oggetto puntato.
        * Viene utilizzato dal metodo gruppoImpianti::leggiImpiantiAnatenneXML
        */
        void deleteAntenne();

        /**
        * Questo è il numero di antenne effettivamente contenute nel vettore 
        *
        * @return Il numero di antenna effettivamente contenute nel vettore
        */
        int size();

        /**
        * Permette di aggiungere un'antenna al vettore e l'inserimento avviene in coda 
        *
        * @param newMod L'antenna da inserire
        *
        * @return Il numero aggiornato delle antenne contenute nel vettore 
        */   
        int addAntenna(antenna * newAnt); 

        /**
        * Funzione utilizzata da python che posiziona l'iteratore itA sul primo elemento del vettore
        */
        void beginIter();

        /**
        * Funzione utilizzata da python per leggere il vettore delle antenne in forma di iteratore python.
        * Se l'iteratore itA non è posizionato sull'ultimo elemento del vettore la funzione restituisce 
        * l'elemento puntato da itA e sposta l'iteratore itA sull'elemento successivo
        *
        * @return La prossima antenna contenuta nel vettore o, nel caso non siano presenti 
        * altre antenne, un valore nullo
        */
        antenna * nextItem();  

        /**
        * Funzione che aggiunge un nuovo oggetto della classe antenna (allocato in heap) al vettore
        * antenne (tramite la funzione addAntenna) e restituisce il nuovo oggetto creato
        */
        antenna * newAntenna();
        
        /**
        * ordina il vettore antenne per h,x e y
        */
        void ordina();
};
#endif
