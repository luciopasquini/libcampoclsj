/** @file modello.h
  * @brief Classe contenente le caratteristiche tecniche dei modelli di antenna 
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

#ifndef MODELLO_H
#define MODELLO_H

#include <set>
#include <string>
#include <vector>

const int STR_FLD_MOD = 1024;
const int STR_INT_MOD = 20;
const int STR_DOUBLE_MOD = 30;
const int STR_BOOL_MOD = 10;

using namespace std;

class modello {

    private: 
    
    int idMod;                      /**< Codice univoco (ID) del modello di antenna */ 
    char strIdMod[STR_INT_MOD]; 
    char nomeMod[STR_FLD_MOD];      /**< Nome del modello di antenna */ 
    char marca[STR_FLD_MOD];        /**< Marca del modello di antenna */
    double frequenza;               /**< Frequenza del modello di antenna */
    char strFrequenza[STR_DOUBLE_MOD];
    double tilt;                    /**< Tilt elettrico del modello di antenna */
    char strTilt[STR_DOUBLE_MOD];
    double guadagno;                /**< Guadagno del modello di antenna */
    char strGuadagno[STR_DOUBLE_MOD];
    double lunghezza;               /**< Lunghezza antenna */
    char strLunghezza[STR_DOUBLE_MOD];
    char note1[STR_FLD_MOD];        /**< Note */
    char immagine[STR_FLD_MOD];     /**< Percorso immagine */
    char strVuota[2];

    vector<double> attO;   /**< Diagramma orizzontale */ 
    vector<double> grdO;   /**< Valori in gradi corrispondenti all'attenuazione orizzontale */
    vector<double> attV;   /**< Diagramma verticale */
    vector<double> grdV;   /**< Valori in gradi corrispondenti all'attenuazione verticale */

    /* I seguenti set stati messi privati perchè lanciano eccezioni che devono essere gestite
     * e perchè, comunque, per inserire i dati viene usata setDM.
     * Quando ritornanono 0 l'inserimento è andato a buon fine.
     */
    int setIdMod(char* valDato);
    int setNomeMod(char* valDato);
    int setMarca(char* valDato);
    int setFrequenza(char* valDato);
    int setTilt(char* valDato);
    int setGuadagno(char* valDato);
    int setLunghezza(char* valDato);
    int setNote1(char* valDato);
    int setImmagine(char* valDato);

    public:
    static string debugModelli;

    int numOgradi;          /**< Numero di valori inseriti per il diagramma orizzontale */
    int numVgradi;          /**< Numero di valori inseriti per il diagramma verticale */

    /**
    * Costruttore 
    */
    modello();

    /**
    * Costruttore deep copy
    */
    modello(modello* oMod);
    
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
    int setDM(char * dbFld, char * valDato);

    /**
    * Funzione di servizio per ricavare in formato testo il valore
    * delle variabili da inserire nel db o nel file xml
    *
    * @param dbFld Nome del tag o nome del campo nel database
    *
    * @return Il valore della variabile corrispondente al nome del tag o del campo preso in input.
    * Se viene inserito il nome di un tag o un campo inesistente ritorna NULL 
    */
    char * getDM(char * dbFld);

    int getIdModello();
    double getGuadagno();
    double getLunghezza();
    double getFrequenza();
    char * getNomeMod();
    char * getMarca();
    char * getNote1();
    char * getImmagine();

    int getSizeAttO();
    int getSizeGrdO();
    int getSizeAttV();
    int getSizeGrdV();

    double getAttO(unsigned int i);
    double getGrdO(unsigned int i);
    double getAttV(unsigned int i);
    double getGrdV(unsigned int i);

    void setIdMod(int id);
    void setAttO(unsigned int i, double valDato);
    void setGrdO(unsigned int i, double valDato);
    void setAttV(unsigned int i, double valDato);
    void setGrdV(unsigned int i, double valDato);

    /**
    * Aggiunge in coda al vettore attO il valore dell'attenuazione orizzontale
    *
    * @param valDato Il dato da inserire
    */
    void addAttO(double valDato);
    /**
    * Aggiunge in coda al vettore grdO il valore in gradi dell'attenuazione orizzontale
    *
    * @param valDato Il dato da inserire
    */
    void addGrdO(double valDato);
    /**
    * Aggiunge in coda al vettore attV il valore dell'attenuazione verticale
    *
    * @param valDato Il dato da inserire
    */
    void addAttV(double valDato);
    /**
    * Aggiunge in coda al vettore grdV il valore in gradi dell'attenuazione verticale
    *
    * @param valDato Il dato da inserire
    */
    void addGrdV(double valDato);
};
#endif
