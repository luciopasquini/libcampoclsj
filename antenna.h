/** @file antenna.h
  * @brief Classe contenente le caratteristiche tecniche delle antenne 
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

#ifndef ANTENNA_H
#define ANTENNA_H

#include <set>
#include <string>
#include <vector>
#include "modello.h"

const int STR_FLD = 1024;
const int STR_INT = 20;
const int STR_DOUBLE = 30;
const int STR_BOOL = 10;


class antenna {
    private:
        int idAnt;            /**< Codice univoco (ID) antenna */
        char strIdAnt[STR_INT]; 
        int idPadre;          /**< ID antenna precedente */
        char strIdPadre[STR_INT];
        double x,y,h;         /**< Posizione antenna coordinate x e y gauss boaga e h altezza dal suolo (metri) */
        char strX[STR_DOUBLE],strY[STR_DOUBLE],strH[STR_DOUBLE];
        int numTx;            /**< Numero di trasmettitori */
        char strNumTx[STR_INT];
        double potTx;         /**< Potenza per trasmettitore in watt */
        char strPotTx[STR_INT];
        double tilt;          /**< Tilt meccanico */
        char strTilt[STR_DOUBLE];
        double direzione;     /**< Direzione di puntamento - azimut */
        char strDirezione[STR_DOUBLE];
        bool attiva;          /**< Antenna attiva */
        char strAttiva[STR_BOOL];
        bool antCalc;         /**< Antenna attivata per il calcolo */
        char strAntCalc[STR_BOOL];
        int idImpianto;       /**< ID dell'impianto dove è installata l'antenna */
        char strIdImpianto[STR_INT];
        int idModello;        /**< ID del modello dell'antenna (marca modello diagramma comprensivo di tilt elettrico) */
        char strIdModello[STR_INT];
        int idServizio;       /**< ID servizio radio,tv,srb... ecc */
        char strIdServizio[STR_INT];
        int idStato;          /**< ID stato antenna (dismessa,attiva ecc..) */
        char strIdStato[STR_INT];
        int idPolar;          /**< ID polarizzazione antenna 0 = V, 1 = H, 2 = C */
        char strIdPolar[STR_INT];
        double freqMin;       /**< Frequenza minima */
        char strFreqMin[STR_DOUBLE];
        double freqMax;       /**< Frequenza massima */
        char strFreqMax[STR_DOUBLE];
        double frequenza;     /**< Fequenza utilizzo MHz */
        char strFrequenza[STR_DOUBLE];
        double fase;          /**< Fase (impianti radio - TV) */
        char strFase[STR_DOUBLE];
        double alfa24;        /**< Alfa24 */
        char strAlfa24[STR_DOUBLE];
        double attPC;         /**< Attenuazione PC */
        char strAttPC[STR_DOUBLE];
        double attDTX;        /**< Attenuazione DTX */
        char strAttDTX[STR_DOUBLE];
        double attTDD;        /**< Attenuazione per LTE TDD. Il valore del tag o campo dati del database che la contiene è ATT_TDD (non ancora presente)*/
        char strAttTDD[STR_DOUBLE];
        double potenzaPerc;   /**< Potenza percentuale */
        char strPotenzaPerc[STR_DOUBLE];
        char dataIniz[STR_FLD];  /**< Data iniziale */
        char dataFin[STR_FLD];   /**< Data finale */
        char note1[STR_FLD];     /**< Note */
        char immagine[STR_FLD];  /**< Percorso immagine */
        char outBuf[STR_FLD];
        char strVuota[2];
        modello * aMod;       /**< Modello di antenna utilizzato */ 
        
        
        /**
        * Variabili di servizio per il calcolo del campo
        */

        double pot;   /**<  potenza totale all'antenna calcolata in base alle attenuazioni Alpha24 pperc */

        /* I seguenti set stati messi privati perchè lanciano eccezioni che devono essere gestite
         * e perchè, comunque, per inserire i dati viene usata setDA.
         * Quando ritornanono 0 l'inserimento è andato a buon fine.
         */


        int setIdAntenna(char* valDato);
        int setIdPadre(char* valDato);
        int setX(char* valDato);
        int setY(char* valDato);
        int setH(char* valDato);
        int setNumTx(char* valDato);
        int setPotenzaTx(char* valDato);
        int setTilt(char* valDato);
        int setDirezione(char* valDato);
        int setAttiva(char* valDato);
        int setAntCalc(char* valDato);
        int setIdImpianto(char* valDato);
        int setIdModello(char* valDato);
        int setIdServizio(char* valDato);
        int setIdPolar(char* valDato);
        int setIdStato(char* valDato);
        int setFreqMin(char* valDato);
        int setFreqMax(char* valDato);
        int setFrequenza(char* valDato);
        int setFase(char* valDato); 
        int setAlfa24(char* valDato);
        int setAttPC(char* valDato);
        int setAttDTX(char* valDato);
        int setAttTDD(char* valDato);
        int setPotenzaPerc(char* valDato);
        int setDataIn(char* valDato);
        int setDataFin(char* valDato);
        int setNote1(char* valDato);
        int setImmagine(char* valDato);


    public:

        static string  debugAntenne; /**< Contiene eventuali messaggi d'errore causati dai metodi delle classi antenna e impianti */ 
        set<string> Modified;        /**< Per il controllo in caso di modifiche */ 

        /**
        * Costruttore 
        */
        antenna();

        /**
        * Costruttore deep copy
        */
        antenna(antenna * oAnt);

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
        int setDA(char * dbFld, char * valDato);

        /**
        * Funzione di servizio per ricavare in formato testo il valore
        * delle variabili da inserire nel db o nel file xml
        *
        * @param dbFld Nome del tag o nome del campo nel database
        *
        * @return Il valore della variabile corrispondente al nome del tag o del campo preso in input.
        * Se viene inserito il nome di un tag o un campo inesistente ritorna NULL 
        */
        char * getDA(char * dbFld);

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

        // Per ogni campo dati ho creato un metodo pubblico get. Nel caso non vengano utilizzati 
        // si può valutare se tenerli o meno


        int getIdAntenna();
        int getIdPadre();
        double getX();
        double getY();
        double getH();
        int getNumTx();
        double getPotenzaTx();
        double getTilt();
        double getDirezione();
        bool getAttiva();
        bool getAntCalc();
        int getIdImpianto();
        int getIdModello();
        int getIdServizio();
        int getIdPolar();
        int getIdStato();
        double getFreqMin();
        double getFreqMax();
        double getFrequenza();
        double getFase(); 
        double getAlfa24();
        double getAttPC();
        double getAttDTX();
        double getAttTDD();
        double getPotenzaPerc();
        char * getDataIn();
        char * getDataFin();
        char * getNote1();
        char * getImmagine();

        modello * getModello();
        void setModello(modello * mod);

        /**
        * Metodi get e set per le variabili di servizio per il calcolo del campo
        */
        void setIdAnt(int id);
        void setPot(double pW);
        
        double getPot();
};
#endif
