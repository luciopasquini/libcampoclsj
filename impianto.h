/** @file impianto.h
  * @brief Classe contenente le caratteristiche tecniche degli impianti di antenna 
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

#ifndef IMPIANTO_H
#define IMPIANTO_H

#include "gruppo_antenne.h"

class impianto : public gruppoAntenne {
    private:
        double x,y,z;               /**< Posizione dell'impianto x e y gauss boaga e z altezza slm del suolo/base del palo/traliccio (metri) */
        char strX[STR_DOUBLE],strY[STR_DOUBLE],strZ[STR_DOUBLE];
        int idImp;                  /**< ID impianto */
        char strIdImp[STR_INT];
        char nome[STR_FLD];            /**< Nome dell'impianto */
        char codImp[STR_FLD];          /**< Codice impianto assegnato dal gestore */
        char indirizzo[STR_FLD];       /**< Indirizzo del gestore */
        char respImp[STR_FLD];         /**< Responsabile impianto */
        int idGestore;              /**< ID del gestore */
        char strIdGestore[STR_INT];
        int idProv;                 /**< ID provincia (codice istat) */
        char srtIdProv[STR_INT];
        int idComune;               /**< ID comune (codice istat) */
        char strIdComune[STR_INT];
        int idStato;                /**< ID stato impianto */
        char strIdStato[STR_INT];
        char note1[STR_FLD];           /**< Note del gestore */
        char note2[STR_FLD];           /**< Note arpa */
        char immagine[STR_FLD];        /**< Percorso file immagine */
        int idResp;                 /**< ID responsabile */
        char strIdResp[STR_INT];
        int idPadre;                /**< ID impianto precedente */
        char strIdPadre[STR_INT];
        char creazione[STR_FLD];       /**< Data creazione */
        char dataModifica[STR_FLD];    /**< Data modifica */
        char parere[STR_FLD];          /**< Data del parere */
        char comunicazione[STR_FLD];   /**< Data comunicazione impianto */
        char dataDismissione[STR_FLD]; /**< Data dismissione impianto */
        int idPonte;                /**< ID ponte */
        char strIdPonte[STR_INT];
        int idPost;                 /**< ID postazione */
        char strIdPost[STR_INT];
        bool comDir;                /**< Comunicazione diretta */
        char strComDir[STR_BOOL];
        bool attEdif;               /**< Attenuazione edifici utilizzata dal gestore */
        char strAttEdif[STR_BOOL];
        bool ricDiretta;            /**< Riconfigurazione senza cambio di potenza */
        char strRicDiretta[STR_BOOL];
        bool calcoloImpianto;       /**<  Impianto selezionato per il calcolo del campo (corrisponde a CHECKED) */
        char strCalcoloImpianto[STR_BOOL];
        bool impiantoAttivo;        /**< Impianto in funzione (non dismesso). Al momento (16/04/2018) nel 
                                    database non esiste un valore booleano corrispondente ma si prevede di inserirlo. 
                                    Per questo, nei metodi setDI e getDI della classe impianto , si utilizza la variabile 
                                    fittizia ATTIVO come nome del tag o del campo nel database. */
        char strImpiantoAttivo[STR_BOOL];
        bool influente; /**<  Impianto influente nel calcolo: viene definito influente se all'interno dell'are di infuenza dalla funzione setInfluenti di gruppoImpianti*/
        char strInfluente[STR_BOOL];
        char strVuota[2];
        /* I seguenti set stati messi privati perchè lanciano eccezioni che devono essere gestite
         * e perchè, comunque, per inserire i dati viene usata setDI.
         * Quando i seguenti metodi ritornano 0 l'inserimento è andato a buon fine.
         */        

        int setIdImp(char* valDato);
        int setNome(char* valDato);
        int setCodImp(char* valDato);
        int setIndirizzo(char* valDato);
        int setX(char* valDato);
        int setY(char* valDato);
        int setZ(char* valDato);
        int setRespImp(char* valDato);
        int setIdGestore(char* valDato);
        int setIdProv(char* valDato);
        int setIdComune(char* valDato);
        int setIdStato(char* valDato);

        int setNote1(char* valDato);
        int setNote2(char* valDato);
        int setImmagine(char* valDato);
        int setIdResp(char* valDato);
        int setIdPadre(char* valDato);
        int setCreazione(char* valDato);

        int setDataModifica(char* valDato);
        int setParere(char* valDato);
        int setComunicazione(char* valDato);
        int setDataDismissione(char* valDato);
        int setIdPonte(char* valDato);
        int setIdPost(char* valDato);
        int setComDir(char* valDato);
        int setAttEdif(char* valDato);
        int setRicDiretta(char* valDato);
        int setCalcoloImpianto(char* valDato);
        int setImpiantoAttivo(char* valDato);
        int setInfluente(char* valDato);

    public:

        set<string> Modified;       /**< Per il controllo in caso di modifiche */

        /**
        * Costruttore 
        */
        impianto();

        /**
        * Costruttore deep copy
        */
        impianto(impianto * oImp);
        
        /**
        * Funzione di servizio per inserire i dati nelle variabili partendo
        * dai tag del file xml o dal nome dei campi del database 
        *
        * @param dbFld Nome del tag o nome del campo nel database
        * @param valDato Valore della variabile corrispondente al tag o al campo preso in input
        *
        * @return Ritorna <code>0</code> se l'inserimento è andato a buon fine,
        * <code>1</code> altrimenti
        */
        int setDI(char * dbFld, char * valDato);

        /**
        * Funzione di servizio per ricavare in formato testo il valore
        * delle variabili da inserire nel db o nel file xml
        *
        * @param dbFld Nome del tag o nome del campo nel database
        *
        * @return Il valore della variabile corrispondente al nome del tag o del campo preso in input.
        * Se viene inserito il nome di un tag o un campo inesistente ritorna NULL 
        */
        char * getDI(char * dbFld);
        
        /**
        * aggiunge all'insieme dei campi modificati il nome del campo
        * passato come parametro
        * 
        * @param dbFld Nome del tag o nome del campo nel database 
        */
        void setModified(char *dbFld);
        
        /**
        * rimuove dall'insieme dei campi modificati il nome del campo
        * passato come parametro
        * 
        * @param dbFld Nome campo nel database 
        */
        void noModified(char *dbFld);
        
        /**
        * Verifica la presenza del nome del campo nell'insieme dei  
        * campi modificati
        * 
        * @param dbFld Nome campo nel database 
        * 
        * @return True se il Nome del campo è presente nell'insieme
        */
        bool isModified(char *dbFld);


        void setIdImp(int id);
        void setInfluente(bool isInflu);
        bool getInfluente();
        bool getCalcoloImpianto();
        int getIdImp();
        double getX();
        double getY();
        double getZ();
        char * getNome();
        char * getCodImp();
        char * getIndirizzo();
        char * getRespImp();
        char * getNote1();
        char * getNote2();
        char * getImmagine();
        char * getCreazione();
        char * getDataModifica();
        char * getParere();
        char * getComunicazione();
        char * getDataDismissione();    

};
#endif
