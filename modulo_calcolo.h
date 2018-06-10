/** @file modulo_calcolo.h
  * @brief Classe contenente i metodi per il calcolo del campo orizzontale e verticale
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

#ifndef MODULO_CALCOLO_H
#define MODULO_CALCOLO_H

#include "gruppo_impianti.h"


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define sz_TERMINATOR 1
#define MAX_CHARS 30
#define PIGRECO 3.141592654
#define ZCAR 376.73031346177
#define LAMBDA 0.3
#define GRADI 360
#define N_ANTENNE_MAX 50000
#define TIPI_ANT_MAX 50000 
// al 11/06/2014 le antenne nel database sono poco più di 10000
#define LEN_WHERE_IDMOD 60000   
/* lunghezza della clausola where per caricamento dati modelli di antenne 
 * where idmod in (cod_ant, cod_ant, ...) circa 
 * 5 cifre di cod_ant + virgola * num_tipi_antenne
*/
#define SIZE_CONNSTRINGOUT 200
#define SIZE_STR_LOG 100000
#define NODATA -9999
#define RADQ2 1.4142135623730950488


class moduloCalcolo {

    private:
        gruppoImpianti * gruppoImp;  /**< gruppo impianti su cui si esegue ii calcolo */
        double potTotPunto;          /**< campo elettrico totale in V/m */
        double potTotPuntoH;         /**< campo magnetico totale in A/m */
        double potTotPuntoS;         /**< densità di potenza totale in W/mq */
        int indexString;             /**< */
        long handleWinProgrMain;     /**< */
        const double SENO[360] = {0.00000,0.01745,0.03490,0.05234,0.06976,0.08716,0.10453,0.12187,0.13917,0.15643,0.17365,0.19081,0.20791,0.22495,0.24192,0.25882,0.27564,
            0.29237,0.30902,0.32557,0.34202,0.35837,0.37461,0.39073,0.40674,0.42262,0.43837,0.45399,0.46947,0.48481,0.50000,0.51504,0.52992,0.54464,0.55919,
            0.57358,0.58779,0.60182,0.61566,0.62932,0.64279,0.65606,0.66913,0.68200,0.69466,0.70711,0.71934,0.73135,0.74314,0.75471,0.76604,0.77715,0.78801,
            0.79864,0.80902,0.81915,0.82904,0.83867,0.84805,0.85717,0.86603,0.87462,0.88295,0.89101,0.89879,0.90631,0.91355,0.92050,0.92718,0.93358,0.93969,
            0.94552,0.95106,0.95630,0.96126,0.96593,0.97030,0.97437,0.97815,0.98163,0.98481,0.98769,0.99027,0.99255,0.99452,0.99619,0.99756,0.99863,0.99939,
            0.99985,1.00000,0.99985,0.99939,0.99863,0.99756,0.99619,0.99452,0.99255,0.99027,0.98769,0.98481,0.98163,0.97815,0.97437,0.97030,0.96593,0.96126,
            0.95630,0.95106,0.94552,0.93969,0.93358,0.92718,0.92050,0.91355,0.90631,0.89879,0.89101,0.88295,0.87462,0.86603,0.85717,0.84805,0.83867,0.82904,
            0.81915,0.80902,0.79864,0.78801,0.77715,0.76604,0.75471,0.74314,0.73135,0.71934,0.70711,0.69466,0.68200,0.66913,0.65606,0.64279,0.62932,0.61566,
            0.60182,0.58779,0.57358,0.55919,0.54464,0.52992,0.51504,0.50000,0.48481,0.46947,0.45399,0.43837,0.42262,0.40674,0.39073,0.37461,0.35837,0.34202,
            0.32557,0.30902,0.29237,0.27564,0.25882,0.24192,0.22495,0.20791,0.19081,0.17365,0.15643,0.13917,0.12187,0.10453,0.08716,0.06976,0.05234,0.03490,
            0.01745,0.00000,-0.01745,-0.03490,-0.05234,-0.06976,-0.08716,-0.10453,-0.12187,-0.13917,-0.15643,-0.17365,-0.19081,-0.20791,-0.22495,-0.24192,
            -0.25882,-0.27564,-0.29237,-0.30902,-0.32557,-0.34202,-0.35837,-0.37461,-0.39073,-0.40674,-0.42262,-0.43837,-0.45399,-0.46947,-0.48481,-0.50000,
            -0.51504,-0.52992,-0.54464,-0.55919,-0.57358,-0.58779,-0.60182,-0.61566,-0.62932,-0.64279,-0.65606,-0.66913,-0.68200,-0.69466,-0.70711,-0.71934,
            -0.73135,-0.74314,-0.75471,-0.76604,-0.77715,-0.78801,-0.79864,-0.80902,-0.81915,-0.82904,-0.83867,-0.84805,-0.85717,-0.86603,-0.87462,-0.88295,
            -0.89101,-0.89879,-0.90631,-0.91355,-0.92050,-0.92718,-0.93358,-0.93969,-0.94552,-0.95106,-0.95630,-0.96126,-0.96593,-0.97030,-0.97437,-0.97815,
            -0.98163,-0.98481,-0.98769,-0.99027,-0.99255,-0.99452,-0.99619,-0.99756,-0.99863,-0.99939,-0.99985,-1.00000,-0.99985,-0.99939,-0.99863,-0.99756,
            -0.99619,-0.99452,-0.99255,-0.99027,-0.98769,-0.98481,-0.98163,-0.97815,-0.97437,-0.97030,-0.96593,-0.96126,-0.95630,-0.95106,-0.94552,-0.93969,
            -0.93358,-0.92718,-0.92050,-0.91355,-0.90631,-0.89879,-0.89101,-0.88295,-0.87462,-0.86603,-0.85717,-0.84805,-0.83867,-0.82904,-0.81915,-0.80902,
            -0.79864,-0.78801,-0.77715,-0.76604,-0.75471,-0.74314,-0.73135,-0.71934,-0.70711,-0.69466,-0.68200,-0.66913,-0.65606,-0.64279,-0.62932,-0.61566,
            -0.60182,-0.58779,-0.57358,-0.55919,-0.54464,-0.52992,-0.51504,-0.50000,-0.48481,-0.46947,-0.45399,-0.43837,-0.42262,-0.40674,-0.39073,-0.37461,
            -0.35837,-0.34202,-0.32557,-0.30902,-0.29237,-0.27564,-0.25882,-0.24192,-0.22495,-0.20791,-0.19081,-0.17365,-0.15643,-0.13917,-0.12187,-0.10453,
            -0.08716,-0.06976,-0.05234,-0.03490,-0.01745}; /**< vettore costante per il seno precalcolato di angoli interi 0 - 359 gradi */
        const double COSENO[360] ={1.00000,0.99985,0.99939,0.99863,0.99756,0.99619,0.99452,0.99255,0.99027,0.98769,0.98481,0.98163,0.97815,0.97437,0.97030,0.96593,0.96126,
            0.95630,0.95106,0.94552,0.93969,0.93358,0.92718,0.92050,0.91355,0.90631,0.89879,0.89101,0.88295,0.87462,0.86603,0.85717,0.84805,0.83867,0.82904,
            0.81915,0.80902,0.79864,0.78801,0.77715,0.76604,0.75471,0.74314,0.73135,0.71934,0.70711,0.69466,0.68200,0.66913,0.65606,0.64279,0.62932,0.61566,
            0.60182,0.58779,0.57358,0.55919,0.54464,0.52992,0.51504,0.50000,0.48481,0.46947,0.45399,0.43837,0.42262,0.40674,0.39073,0.37461,0.35837,0.34202,
            0.32557,0.30902,0.29237,0.27564,0.25882,0.24192,0.22495,0.20791,0.19081,0.17365,0.15643,0.13917,0.12187,0.10453,0.08716,0.06976,0.05234,0.03490,
            0.01745,0.00000,-0.01745,-0.03490,-0.05234,-0.06976,-0.08716,-0.10453,-0.12187,-0.13917,-0.15643,-0.17365,-0.19081,-0.20791,-0.22495,-0.24192,
            -0.25882,-0.27564,-0.29237,-0.30902,-0.32557,-0.34202,-0.35837,-0.37461,-0.39073,-0.40674,-0.42262,-0.43837,-0.45399,-0.46947,-0.48481,-0.50000,
            -0.51504,-0.52992,-0.54464,-0.55919,-0.57358,-0.58779,-0.60182,-0.61566,-0.62932,-0.64279,-0.65606,-0.66913,-0.68200,-0.69466,-0.70711,-0.71934,
            -0.73135,-0.74314,-0.75471,-0.76604,-0.77715,-0.78801,-0.79864,-0.80902,-0.81915,-0.82904,-0.83867,-0.84805,-0.85717,-0.86603,-0.87462,-0.88295,
            -0.89101,-0.89879,-0.90631,-0.91355,-0.92050,-0.92718,-0.93358,-0.93969,-0.94552,-0.95106,-0.95630,-0.96126,-0.96593,-0.97030,-0.97437,-0.97815,
            -0.98163,-0.98481,-0.98769,-0.99027,-0.99255,-0.99452,-0.99619,-0.99756,-0.99863,-0.99939,-0.99985,-1.00000,-0.99985,-0.99939,-0.99863,-0.99756,
            -0.99619,-0.99452,-0.99255,-0.99027,-0.98769,-0.98481,-0.98163,-0.97815,-0.97437,-0.97030,-0.96593,-0.96126,-0.95630,-0.95106,-0.94552,-0.93969,
            -0.93358,-0.92718,-0.92050,-0.91355,-0.90631,-0.89879,-0.89101,-0.88295,-0.87462,-0.86603,-0.85717,-0.84805,-0.83867,-0.82904,-0.81915,-0.80902,
            -0.79864,-0.78801,-0.77715,-0.76604,-0.75471,-0.74314,-0.73135,-0.71934,-0.70711,-0.69466,-0.68200,-0.66913,-0.65606,-0.64279,-0.62932,-0.61566,
            -0.60182,-0.58779,-0.57358,-0.55919,-0.54464,-0.52992,-0.51504,-0.50000,-0.48481,-0.46947,-0.45399,-0.43837,-0.42262,-0.40674,-0.39073,-0.37461,
            -0.35837,-0.34202,-0.32557,-0.30902,-0.29237,-0.27564,-0.25882,-0.24192,-0.22495,-0.20791,-0.19081,-0.17365,-0.15643,-0.13917,-0.12187,-0.10453,
            -0.08716,-0.06976,-0.05234,-0.03490,-0.01745,0.00000,0.01745,0.03490,0.05234,0.06976,0.08716,0.10453,0.12187,0.13917,0.15643,0.17365,0.19081,
            0.20791,0.22495,0.24192,0.25882,0.27564,0.29237,0.30902,0.32557,0.34202,0.35837,0.37461,0.39073,0.40674,0.42262,0.43837,0.45399,0.46947,0.48481,
            0.50000,0.51504,0.52992,0.54464,0.55919,0.57358,0.58779,0.60182,0.61566,0.62932,0.64279,0.65606,0.66913,0.68200,0.69466,0.70711,0.71934,0.73135,
            0.74314,0.75471,0.76604,0.77715,0.78801,0.79864,0.80902,0.81915,0.82904,0.83867,0.84805,0.85717,0.86603,0.87462,0.88295,0.89101,0.89879,0.90631,
            0.91355,0.92050,0.92718,0.93358,0.93969,0.94552,0.95106,0.95630,0.96126,0.96593,0.97030,0.97437,0.97815,0.98163,0.98481,0.98769,0.99027,0.99255,
            0.99452,0.99619,0.99756,0.99863,0.99939,0.99985}; /**< vettore costante per il coseno precalcolato di angoli interi 0 - 359 gradi */
        /**
        * Calcola il campo elettrico, il campo magnetico e/o entrambi più la densità
        * di potenza in un punto.
        * Versione modificata per calcolo vettoriale se idServizio
        * (della prima antenna dell'impianto) corrisponde a una radio o TV
        * 
        * @param xPunto Coordinata x del punto di cui si vuole calcolare il campo
        * @param yPunto Coordinata y del punto di cui si vuole calcolare il campo
        * @param zPunto Coordinata z del punto di cui si vuole calcolare il campo
        * @param strD2Lambda Opzione (ignorata) per eseguire il calcolo anche con le antenne per le quali il punto è in campo vicino
        * @param scalaVM Valore numerico per l'unità di misura (se 1 unità standard altrimenti multipli o sottomultipli)
        * @param tipoCampo Per selezionare il tipo di calcolo: 0 solo campo elettrico, 1 solo campo magnetico, 2 campo elettrico, magnetico e densità di potenza
        * 
        * @return Ritorna <code>0</code> se l'operazione è andata a buon fine,
        * <><code>0</code> altrimenti  
        */
        int calculateField(double xPunto,double yPunto, double zPunto, char* strD2Lambda, int scalaVM, int tipoCampo);

        /**
        * Calcola il valore efficace al quadrato del campo elettrico
        * generato da un'antenna in un punto.
        * 
        * @param pAnt Puntatore all'antenna corrente
        * @param pMod Puntatore al modello dell'antenna
        * @param zImp Altezza della base dell'impianto (dove si trova l'antenna) in metri sul livello del mare
        * @param xyz_punto[] Coordinate del punto in Gauss Boaga Fuso Ovest
        * @param xyzUno[] Coordinate del punto nel sistema di riferimento dell'antenna (eventualemente ruotata) con asse y in direzione di puntamento
        * @param xyzZero[] Coordinate del punto nel sistema di riferimento con origine nel centro elettrico dell'antenna
        * @param r2 Distanza al quadrato del centro elettrico dell'atenna dal punto
        * @param grad[] Vettore dove sono salvati figrad e tetagrad: rispettivamente azimut e abbassamento del punto nel sistema di riferimento di xyzUno[]
        * @param dirant[] Vettore dove sono salvati seno e coseno della direzione di puntamento e del tilt dell'antenna
        * @param rot Rotazione dell'antenna sul prooprio asse (direzione di puntamento)
        * 
        * @return Restituisce il valore del campo elettrico efficace al quadrato
        */
        double calcoloModulo(antenna* pAnt, modello* pMod, double zImp, double xyz_punto[], double xyzUno[], double xyzZero[], double r2, double grad[],double dirant[], int rot);
        /**
        * Calcola (aggiorna) i versori per il calcolo vettoriale
        * 
        * @param u[] Versori
        * @param xyzZero[] Coordinate del punto nel sistema di riferimento con origine nel centro elettrico dell'antenna
        * @param dirant[] Vettore dove sono salvati seno e coseno della direzione di puntamento e del tilt dell'antenna
        * @param r2 Distanza al quadrato del centro elettrico dell'atenna dal punto
        * @param rot Rotazione dell'antenna sul prooprio asse (direzione di puntamento)
        * 
        */
        void calcoloVersori(double u[], double xyzZero[],double dirant[], double r2, int rot);
        /**
        * Calcola gli addendi (XSsin, XSCos...) dovuti ad un'antenna da usare per il calcolo vettoriale del campo
        * generato da un impianto radiotelevisivo in un punto.
        * 
        * @param pAnt Puntatore all'antenna corrente
        * @param efficace2 Valore efficace del campo elettrico al quadrato
        * @param freqz Frequenza dell'antenna (prima antenna dell'impianto)
        * @param ck[] Vettore per memorizzare gli addendi (per ottenere i valori efficaci delle componenti x,y,z della risultante del campo dopo la somma vettoriale)
        * @param u[] Versori
        * @param rot Rotazione dell'antenna sul prooprio asse (direzione di puntamento)
        * @param tipo Tipo di campo cui applicare il calcolo vettoriale: 0 campo elettrico, 1 campo magnetico
        * 
        */
        void calcoloVett(antenna* pAnt, double r2, double efficace2, double freqz, double ck[], double u[], int rot, int tipo);
    public:
        
        /**
        * Costruttore 
        */
        moduloCalcolo();

        /**
        * Metodi get e set per i campi privati di moduloCalcolo.
        * Se questi metodi non vengono usati al di fuori 
        */

        gruppoImpianti * getGruppoImpianti();
        double getPotTotPunto();
        double getPotTotPuntoH();
        int getIndexString();
        long getHandleWinProgrMain();

        void setGruppoImpianti(gruppoImpianti * gruppoImpianto);
        void setPotTotPunto(double valDato);
        void setPotTotPuntoH(double valDato);
        void setIndexString(int valDato);
        void setHandleWinProgrMain(long valDato);
    
        /**
        * Crea il raster per il campo elettrico o magnetico orizzontale o per entrambi più la densità di potenza
        *
        * @param xGB Coordinata x grid del punto iniziale a sinistra
        * @param yGB Coordinata y grid del punto iniziale in basso
        * @param dx Base del rettangolo di misura
        * @param dy Altezza del rettangolo di misura
        * @param cellSize PassoGrid
        * @param altitudine Coordinata z (quota in metri sul livello del mare)
        * @param strFileOutput  File output per campo elettrico
        * @param strFileOutputH File output per campo magnetico
        * @param strFileOutputS File output per densità di potenza
        * @param tipoCampo Tipo di sezione calcolata (0 campo elettrico, 1 campo magnetico, 2 campo elettrico, magnetico e densità di potenza)
        * @param tipoRaster Tipo Raster ( 0 Arc/Info ASCII Grid, 1 GeoTiff, 2 entrambi)
        *
        * @return Restituisce <code>0</code> se l'operazione è andata a buon fine,
        * <><code>0</code> altrimenti    
        */
        int calcoloOrizz(double xGB,double yGB, double dx,double dy,double cellSize,
                            double altitudine, char* strFileOutput, char* strFileOutputH,char* strFileOutputS,int tipoCampo,int tipoRaster);
        
        /**
        * Crea il raster per il campo elettrico o magnetico verticale o per entrambi più la densità di potenza
        *
        * @param xGB0 Coordinata x del punto iniziale 
        * @param yGB0 Coordinata y del punto iniziale 
        * @param xGB1 Coordinata x del punto finale
        * @param yGB1 Coordinata y del punto finale
        * @param cellSize Dimensione della cella di calcolo
        * @param altitudine Altitudine iniziale della sezione
        * @param dh Altezza della sezione
        * @param strFileOutput  File output per campo elettrico 
        * @param strFileOutputH File output per campo magnetico 
        * @param strFileOutputS File output per densità di potenza
        * @param tipoCampo Tipo campo da calcolare (0 elettrico, 1 magnetico, 2 entrambi più densità di potenza)
        * @param tipoRaster Tipo Raster ( 0 Arc/Info ASCII Grid, 1 GeoTiff, 2 entrambi)
        *
        * @return Restituisce <code>0</code> se l'operazione è andata a buon fine,
        * <><code>0</code> altrimenti 
        */
        int calcoloVert(double xGB0,double yGB0, double xGB1,double yGB1, double cellSize,
                            double altitudine,double dh,char* strFileOutputE,char* strFileOutputH,char* strFileOutputS, int tipoCampo,int tipoRaster);
        /**
        * Calcola il campo elettrico o magnetico per file .dbf di punti
        *
        * @param strFileIndicatori Nome del file dbf che contiene gli indicatori da valutare (es. indic.dbf)
        * @param tipoCampo Tipo campo da calcolare (0 elettrico, 1 magnetico)
        *
        * @return Ritorna <code>0</code> se l'operazione è andata a buon fine, <><code>0</code> altrimenti. 
        * In dettaglio:
        * 1001 - campo X non trovato nel file di input
        * 1002 - campo Y non trovato nel file di input
        * 1003 - campo Z non trovato nel file di input
        * 1004 - campo CAMPO non trovato nel file di input
        * 1005 - problemi scrittura valore campo
        */
        int calcoloPunti(char* strFileIndicatori,int tipoCampo);

};
#endif // MODULO_CALCOLO_H
