/** @file modulo_calcolo.cpp
  * @brief Metodi per il calcolo del campo strZ e verticale
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

/**
* FUNZIONI
*    INTERNE (e quindi private)
*    1)  calcoloModulo(() - calcola il modulo del campo elettrico al quadrato dovuto ad un'antenna in un punto
*    2)  calcoloVersori() - calcola i versori da usare per il calcolo vettoriale
*    3)  calcoloVett()    - calcola gli addendi per determinare il valore efficace del campo in un punto come somma quadratica dei vettori di campo
*    4)  calculateField() - calcola il campo (elettrico o magnetico) in un punto (modifica le variabili globali potTotPunto e potTotPuntoH)
*    ESPORTATE
*    5)  calcoloPunti() - calcola il campo (elettrico e/o magnetico) per file .dbf di punti
*    6)  calcoloOriz() - crea file txt per il campo (elettrico e/o magnetico) strZ
*    7)  calcoloVert() - crea file txt per il campo (elettrico e/o magnetico) verticale
*
*/

#include <stdio.h>
#include <stdlib.h>             // serve per le funzioni atof() e atoi()
#include <string.h>             // serve qui per le funzioni strcpy e strtok
#include <math.h>               // serve per le funzioni trigonometriche, sqrt() e ceil() che arrotonda
#include <locale.h>             // per la funzione setlocale per il separatore decimale
#include "modulo_calcolo.h"     // header principale del progetto
#include "shapefil.h"
#include "geotiff_writer.h"
#include "asc_writer.h"

//#include <ctime>
#include <chrono> // per prove di velocità di calcolo
#include <fstream>


using namespace std;

moduloCalcolo::moduloCalcolo() {
    gruppoImp = NULL;
    setlocale(LC_NUMERIC, "C"); // forza il . come separatore decimale
}

gruppoImpianti * moduloCalcolo::getGruppoImpianti(){
    return gruppoImp;
}

double moduloCalcolo::getPotTotPunto(){
    return potTotPunto;
}

double moduloCalcolo::getPotTotPuntoH(){
    return potTotPuntoH;
}

int moduloCalcolo::getIndexString(){
    return indexString;
}

long moduloCalcolo::getHandleWinProgrMain(){
    return handleWinProgrMain;
}

void moduloCalcolo::setGruppoImpianti(gruppoImpianti * gruppoImpianto){
    gruppoImp=gruppoImpianto;
}

void moduloCalcolo::setPotTotPunto(double valDato){
    potTotPunto=valDato;
}

void moduloCalcolo::setPotTotPuntoH(double valDato){
    potTotPuntoH=valDato;
}
void moduloCalcolo::setIndexString(int valDato){
    indexString=valDato;
}
void moduloCalcolo::setHandleWinProgrMain(long valDato){
    handleWinProgrMain=valDato;
}


double moduloCalcolo::calcoloModulo(antenna* pAnt, modello* pMod, double zImp, double xyz_punto[], double xyzUno[], double xyzZero[], double r2, double grad[],double dirant[], int rot){
    double xyzUno_prec[3] ={xyzUno[0],xyzUno[1],xyzUno[2]};
    int angoli[3] = {0,0,0}; // fi_or, tilt, rot
    angoli[0] =(int)(pAnt->getDirezione()); // fi_or,
    angoli[1]=(int)(pAnt->getTilt()); // tilt
    angoli[2]=rot; // antenna montata ruotata sul proprio asse di un angolo rot in senso orario

    // riporto tutti gli angoli a valori tra 0 e 359 compresi
    for(int i=0;i<3;i++){
        if(angoli[i]!=0){ 
            while(angoli[i]>359) angoli[i] = angoli[i]-360;
            while(angoli[i]<0) angoli[i] = angoli[i]+360; // si possono mettere anche angoli negativi che poi vengono riportati a positivi (es. -90 -> 270)
        }
    }
    // Coordinate del punto nel sistema di coordinate con centro nell'antenna e asse y in direzione di puntamento dell'antenna
    xyzZero[0] = xyz_punto[0]-(pAnt->getX());
    xyzZero[1] = xyz_punto[1]-(pAnt->getY());
    xyzZero[2] = xyz_punto[2]-((pAnt->getH())+zImp);

    double r2_prec = r2;
    r2 = xyzZero[0]*xyzZero[0]+xyzZero[1]*xyzZero[1]+xyzZero[2]*xyzZero[2];

    double efficace2 = 0.0; //valore efficace al quadrato del campo elettrico generato dall'antenna

    if (r2!=0){ // r!=0

        dirant[0] = SENO[angoli[0]]; // senof - 
        dirant[1] = COSENO[angoli[0]]; // cosenof
        dirant[2] = SENO[angoli[1]]; // senot
        dirant[3] = COSENO[angoli[1]]; // cosenot

        xyzUno[0] = (dirant[0]*dirant[2]* xyzZero[0]+dirant[1]*dirant[2]*xyzZero[1]+dirant[3]*xyzZero[2])*SENO[angoli[2]]+(dirant[1]*xyzZero[0]-dirant[0]*xyzZero[1])*COSENO[angoli[2]]; // nuova x dopo anche rotazione
        xyzUno[1] = dirant[0]*dirant[3]*xyzZero[0]+dirant[1]*dirant[3]*xyzZero[1]-dirant[2]*xyzZero[2];
        xyzUno[2] = (dirant[0]*dirant[2]*xyzZero[0]+dirant[1]*dirant[2]*xyzZero[1]+dirant[3]*xyzZero[2])*COSENO[angoli[2]]-(dirant[1]*xyzZero[0]-dirant[0]*xyzZero[1])*SENO[angoli[2]]; // nuova z dopo anceh rotazione
        // Le antenne di un impianto dovrebbero essere ordinate da z, x, y (prima quelle con z uguale se ci sono ecc.)
        // questa parte serve a trovare figrad e tetagrad che servono per Dh e DV
        int precedente = (int)(xyzUno[0]==xyzUno_prec[0] && xyzUno[1]==xyzUno_prec[1])+2*(int)(xyzUno[2]==xyzUno_prec[2] && r2==r2_prec);
        switch(precedente){
            case 0:{ // zr_uguali falso e xy_uguali falso - tutto il calcolo di nuovo
                // dev'essere d!=0 per fi_grad e r2!=0 per tetagrad!!!!
                double xy = xyzUno[0]*xyzUno[0]+xyzUno[1]*xyzUno[1];
                if(xy!=0) {
                    double figrad = acos(xyzUno[1]/sqrt(xy))*180./PIGRECO; // azimut in gradi sessagesimali
                    grad[0] = figrad; 
                }
                else grad[0]= 0.0; // uguale a 0 (siamo nel sistema di riferimento dell'antenna = direzione di puntamento)
                double tetagrad = asin(-xyzUno[2]/sqrt(r2))*180./PIGRECO; // viene sempre un risultato tra -90° e 90°
                if(tetagrad<0) tetagrad = 360.+tetagrad; // se ad es. è -pigreco/2 diventa 3pigreco/2 (cioè 270°)
                grad[1] = tetagrad;
            }
            break;
            case 1:{ // zr_uguali falso e xy_uguali vero - recupero figrad
                double xy = xyzUno[0]*xyzUno[0]+xyzUno[1]*xyzUno[1];
                if(xy!=0) {
                    double figrad = acos(xyzUno[1]/sqrt(xy))*180./PIGRECO; // azimut in gradi sessagesimali
                    grad[0] = figrad; 
                }
                else grad[0]= 0.0;
            }
            break;
            case 2:{ // zr_uguali vero e xy_uguali falso - recupero tetagrad
                double tetagrad = asin(-xyzUno[2]/sqrt(r2))*180./PIGRECO; // viene sempre un risultato tra -90° e 90°
                if(tetagrad<0) tetagrad = 360.+tetagrad; // se ad es. è -pigreco/2 diventa 3pigreco/2 (cioè 270°)
                grad[1] = tetagrad;
            }
            break;
        } // fine
        // Calcolo di Dh
        int f=(int)(grad[0]); // azimut intero in gradi sessagesimali arrotondato per difetto
        double att_f = pMod->getAttO(f);
        double a_or=0.0;
        switch (f) {
            case 359: a_or=(pMod->getAttO(0)-att_f);
            break;
            default: a_or=(pMod->getAttO(f+1)-att_f);
            break;
        }
        double att_or1 = (a_or*grad[0])+att_f-f*a_or; // Dh(f)

        // calcolo di Dv(t)
        int t=(int)(grad[1]); // abbassamento intero in gradi sessagesimali arrotondato per difetto
        double att_t = pMod->getAttV(t);
        double a_ver;
        switch (t) {
            //case 90: a_ver=(pMod->getAttV(271)-att_t); // caso aggiunto perche' se t e' 90 non passo a leggere il diagramma a 91 ma quello a 271
            //break;
            case 359: a_ver=(pMod->getAttV(0)-att_t);
            break;
            default: a_ver=(pMod->getAttV(t+1)-att_t);
            break;
        }
        double att_ver1=(a_ver*grad[1])+att_t - t*a_ver; // Dv(t)
        // nella prossima espressione va messa la potenza effettiva (con o senza alfa24 secondo le scelte... invece di PotenzaTx)
        //double erp = (pAnt -> getNumTx())*(pAnt -> getPotenzaTx())*(pMod -> getGuadagno()); //n_port*pp*guadagno = potenza per guadagno (ERP) in W
        double erp = ( pAnt->getPot() )* ( pMod->getGuadagno() ); //potenza (ridotta o no secondo la selezione dell'alfa24) per guadagno (ERP) in W
        efficace2 = (ZCAR*erp*att_or1*att_ver1)/4./PIGRECO/r2; // valore efficace al quadrato del campo elettrico nel punto dovuto alla singola antenna
    }
    return efficace2; // restituisce e2
}

void moduloCalcolo::calcoloVersori(double u[], double xyzZero[],double dirant[], double r2, int rot) {
    double uxv = 0.0;
    double uyv = 0.0;
    double uzv = 0.0;
    double uxh = 0.0;
    double uyh = 0.0; 
    double uzh = 0.0;

    double d = sqrt(xyzZero[0]*xyzZero[0]+xyzZero[1]*xyzZero[1]);
    double r = sqrt(r2); // Distanza r del punto, dal centro elettrico dell'antenna

    int nonasse_z = int(d!=0.0); // se 0 d=0 (punto sull'asse z) altrimenti d!=0
    switch (nonasse_z) {
        case 0: { // sull'asse z (prendo i versori calcolati da abbassameno e direzione dell'antenna)
            uxh =   dirant[1]; // cos(fi) - non serve salvarlo in dirant... perché fi=fi_or è intero e si possono usare i vettori SENO[] e COSENO[]
            uyh =  -dirant[0]; // -sen(fi)
            uxv =  -1*uyh*dirant[2]; // sen(fi)*sen(teta)
            uyv =  uxh*dirant[2]; // cos(fi)*sen(teta)
            uzv =  0.0;
            uzh =  0.0;
            uxh = uzh*SENO[rot]+uxh*COSENO[rot]; // se c'è rotazione dell'antenna
            uzh = uzh*COSENO[rot]-uxh*SENO[rot]; // se c'è rotazione dell'antenna
            uxv = uzv*SENO[rot]+uxv*COSENO[rot]; // se c'è rotazione dell'antenna
            uzv = uzv*COSENO[rot]-uxv*SENO[rot]; // se c'è rotazione dell'antenna
        }
        break;
        default: { // d<>0
            uxh =  xyzZero[1] / d; // lo calcolo anche se la polarizzazione e' vert...
            uyh = -xyzZero[0] / d; // lo calcolo anche se la polarizzazione e' vert...
            uxv = xyzZero[2] * uyh / r; // lo calcolo anche se la polarizzazione e' orizz...
            uyv = -xyzZero[2] * uxh / r; // lo calcolo anche se la polarizzazione e' orizz...
            uzv =  d / r; // lo calcolo anche se la polarizzazione e' orizz...
            uzh = 0;
        }
        break;
    }  // fine versori                            
    u[0] =  uxv;
    u[1] =  uyv;
    u[2] =  uzv;
    u[3] =- uxh;
    u[4] = -uyh;
    u[5] =  uzh; // VERSORE COMPLESSIVO
}

void moduloCalcolo::calcoloVett(antenna* pAnt, double r2, double efficace2, double freqz, double ck[], double u[], int rot, int tipo){
    double CM = sqrt(efficace2*2); // modulo del vettore campo (valore efficace * radice di 2)
    double fase_a = pAnt->getFase()/180.*PIGRECO;  // fase di alimentazione in radianti
    double kdr = sqrt(r2) * freqz * PIGRECO/150.; // fase da diverso percorso - freqz trovata per la prima antenna dell'impianto!
    double ksin = sin(kdr-fase_a); // serve in tutti i casi - si potrebbe usare una funzione con lo sfasamento... (intero come getFase())
    double kcos = cos(kdr-fase_a); // serve in tutti i casi
    // CALCOLO DEL CONTRIBUTO AL CAMPO DELL'ANTENNA k-esima, secondo il tipo di polarizzazione
    int anticipo = 0; // anticipo di fase della componente del campo parallela alla verticale a quella parallela all'orizzontale (se 0 il Campo non ruota)
    int alfapol = tipo*(-90); // rotazione del vettore campo sul piano ortogonale ad r e rispetto alla direzione della polarizzazione verticale del campo elettrico in senso orario
    // se tipo 0 campo elettrico  e alfapol=0, se tipo=1 allora alfapol=-90
    // alfapol = +90; per campo magnetico (è ruotato di 90° in senso orario rispetto al campo elettrico)
    // tipo = 0 (elettrico), 1 (magnetico)
    switch(pAnt->getIdPolar()) {
        case 3 : // Polarizzazione circolare destrorsa (rif. campo elettrico)
        {
            anticipo += 90; // i 45 gradi servono per ottenere E diviso per radice di 2 nelle componenti
            alfapol = rot+45;
        }
        break;
        case 2 : // Polarizzazione ORIZZONTALE (anticipo rimane 0) - (rif. campo elettrico)
            alfapol += -90+rot;
        break;
        default: // Polarizzazione VERTICALE (anticipo rimane 0) - (rif. campo elettrico)
            alfapol += rot;
        break;
    }
    // ------ Parte per far sì che gli angoli (interi e in gradi) siano sempre da 0 a 359 --------------------//
    while(alfapol<0) alfapol+=360;
    while(alfapol>359) alfapol+=-360;
    while(anticipo<0) anticipo+=360;
    while(anticipo>359) anticipo+=-360;
    // ------ incremento dei termini generali che, al termine del ciclo sulle antenne dell'impianto,
    // ------ sommati quadraticamente daranno il campo dovuto all'impianto nel punto dato --------------//
    ck[0] += CM*COSENO[alfapol]*u[0] *ksin + CM*SENO[alfapol]*u[3]*(ksin*COSENO[anticipo]-kcos*SENO[anticipo]) ; // sommatoria Ex*ksin
    ck[1] += CM*COSENO[alfapol]*u[0] *kcos + CM*SENO[alfapol]*u[3]*(kcos*COSENO[anticipo]-ksin*SENO[anticipo]) ; // sommatoria Ex*kcos
    ck[2] += CM*COSENO[alfapol]*u[1] *ksin + CM*SENO[alfapol]*u[4]*(ksin*COSENO[anticipo]-kcos*SENO[anticipo]) ; // sommatoria Ex*ksin
    ck[3] += CM*COSENO[alfapol]*u[1] *kcos + CM*SENO[alfapol]*u[4]*(kcos*COSENO[anticipo]-ksin*SENO[anticipo]) ; // sommatoria Ex*kcos
    ck[4] += CM*COSENO[alfapol]*u[2] *ksin + CM*SENO[alfapol]*u[5]*(ksin*COSENO[anticipo]-kcos*SENO[anticipo]) ; // sommatoria Ex*ksin
    ck[5] += CM*COSENO[alfapol]*u[2] *kcos + CM*SENO[alfapol]*u[5]*(kcos*COSENO[anticipo]-ksin*SENO[anticipo]) ; // sommatoria Ex*kcos
}


/****************************************************************************
    FUNZIONE:       calculateField()
    DESCRIZIONE :   calcolo del campo elettrico e/o magnetico in un punto;
                    versione modificata per calcolo vettoriale se il idServizio
                    (della prima antenna dell'impianto) è radio o TV: 4 = UHF , 5 = VHF , 6 = FM
    RETURN:         codice errore <>0 oppure 0

****************************************************************************/

int  moduloCalcolo::calculateField (double x_punto, double y_punto, double z_punto, char* strD2Lambda, int scalavm, int tipoCampo)
{
    // dichiarazione e inizializzazione variabili del metodo calculateField()
    double e2_sum = 0.0; // azzeramemto valore rms del campo elettrico totale al quadrato
    double h2_sum = 0.0; // 21/03/2013 - azzeramento valore rms del campo magnetico totale al quadrato
    double s_sum = 0.0; // 17/05/2018 - azzeramento valore della densità di potenza
    //-----------------------------------------------------------------
    double xyz_punto[3] = {x_punto, y_punto, z_punto}; // array delle coordinate del punto
    // Inizializzazioni variabili della classe
    potTotPunto = 0.0; // azzeramento valore campo elettrico totale nel punto
    potTotPuntoH = 0.0; // 21/03/2013 - azzeramento valore campo magnetico totale nel punto
    // forse potrei creare un'unica funzione con parametri tipoCampo
    switch(tipoCampo) {
        case 0: // Solo campo elettrico
        {
            // Ciclo sui siti
            for (vector<impianto*>::iterator itrI = gruppoImp->impianti.begin();itrI != gruppoImp->impianti.end();itrI++)
            {
                double zImp = (*itrI)->getZ(); // quota della base dell'impianto (m slm)
                vector<antenna*> antImp = (*itrI)->antenne;
                double idSer = antImp[0]->getIdServizio(); // prendo  il servizio dalla prima antenna
                double freqz = antImp[0]->getFrequenza(); // prendo la frequenza dalla prima antenna (se è 0 considero che sia una SRB altrimenti non posso fare il calcolo vettoriale)
                int contantenne = antImp.size(); // potrebbe esserci una radio-tv con una sola antenna
                bool tiposrb = ((freqz==0.0) || (idSer != 4 && idSer != 5 && idSer != 6 && idSer != 7 && idSer != 8 && idSer != 9 && idSer != 15 && idSer != 17 && idSer != 18 && idSer != 21));
                int tipocalcolo = 2;
                if(contantenne<=1 || tiposrb) tipocalcolo = 0; // caso di nessuna antenna (non fa nessun ciclo sulle antenne) oppure di un'antenna oppure di più antenne ma srb
                else tipocalcolo = 1; // caso di più antenne i cui campi vanno sommati vettorialmente
                // vettori con i parametri da conservare per recuperare eventualmente dati dai calcoli per l'antenna precedente
                double dirant[4] = {2.0,2.0,2.0,2.0}; // {sen(fi), cos(fi) , sen(teta), cos(teta)} - parametri dalla direzione di puntamento dell'antenna
                double xyzUno[3] = {-9999.9, -9999.9, -9999.9}; // coordinate del punto nel sistema di riferimento dell'antenna per ricavare le attenuazioni Dh e Dv
                double xyzZero[3] = {-9999.9, -9999.9, -9999.9};
                double grad[2]={-9999.9,-9999.9}; // figrad , tetagrad; // calcolati con arcsin e arccos... utile memorizzarli
                double r2 = -1.0; // ...
                switch(tipocalcolo) { // somma quadratica o somma vettoriale...dipende dal tipo di impianto (SRB, radio-tv...)
                    case 0: { // una o nessuna antenna e/o più antenne di una srb
                        for (vector<antenna*>::iterator itrA = antImp.begin();itrA != antImp.end();itrA++) {
                            antenna * pAnt = *itrA; // antenna corrente
                            modello * pMod = pAnt->getModello(); // modello d'antenna dell'antenna corrente
                            int rot = 0; // rotazione dell'antenna in senso orario attorno all'asse della sua direzione di puntamento
                            double e2 = calcoloModulo(pAnt, pMod, zImp, xyz_punto, xyzUno, xyzZero, r2, grad, dirant, rot);
                            e2_sum += e2; // aggiornamento della somma quadratica dei campi nel punto (caso SRB)
                        }
                    }
                    break;
                    case 1: { // più antenne e somma vettoriale dei campo elettrico
                        double ck[6] = {0.0,0.0,0.0,0.0,0.0,0.0};//{xSsin,xScos,ySsin,yScos,zSsin,zScos}; // per calcolo con funzione standard
                        double u[6] = {0.0,0.0,0.0,0.0,0.0,0.0};//versori standard - serve a memorizzarli
                        for (vector<antenna*>::iterator itrA = antImp.begin();itrA != antImp.end();itrA++) {
                            antenna * pAnt = *itrA; // antenna corrente
                            modello * pMod = pAnt->getModello(); // modello d'antenna dell'antenna corrente
                            int rot = 0; // rotazione dell'antenna in senso orario attorno all'asse della sua direzione di puntamento
                            double e2 = calcoloModulo(pAnt, pMod, zImp, xyz_punto, xyzUno, xyzZero, r2, grad, dirant, rot); // valore efficace del campo elettrico al quadrato generato da pAnt nel punto
                            // Calcolo generale dei versori                            
                            calcoloVersori(u,xyzZero,dirant,r2,rot); // aggiorna i versori cioè u[]
                            calcoloVett(pAnt, r2, e2, freqz, ck, u, rot, 0); // 0 per campo elettrico - qui forse si possono recuperare alcuni versori passando da un'antenna alla successiva
                            // Incremento delle somme per il campo dovuto a tutto l'impianto, con le somme vettoriali
                            e2_sum += (ck[0]*ck[0]+ck[1]*ck[1]+ck[2]*ck[2]+ck[3]*ck[3]+ck[4]*ck[4]+ck[5]*ck[5])/2.; //e' +=(xSsin*xSsin+xScos*xScos+ySsin*ySsin+yScos*yScos+zSsin*zSsin+zScos*zScos)/2
                        } // fine ciclo sulle antenne
                    }
                    break;
                    default:{
                        potTotPunto = -9999;
                        potTotPuntoH = -9999;
                        potTotPuntoS = -9999;
                        return 1; // se succede c'è un errore perché non sono stati concordati altri casi di polarizzazione da inserire in dbNir...
                    }
                    break;
                }

            } // FINE CICLO SUI SITI
        }
        break;
        case 1: // Solo campo magnetico
        {
            // Ciclo sui siti
            for (vector<impianto*>::iterator itrI = gruppoImp->impianti.begin();itrI != gruppoImp->impianti.end();itrI++)
            {
                double zImp = (*itrI)->getZ(); // quota della base dell'impianto (m slm)
                vector<antenna*> antImp = (*itrI)->antenne;
                double idSer = antImp[0]->getIdServizio(); // prendo  il servizio dalla prima antenna
                double freqz = antImp[0]->getFrequenza(); // prendo la frequenza dalla prima antenna (se è 0 considero che sia una SRB altrimenti non posso fare il calcolo vettoriale)
                int contantenne = antImp.size(); // potrebbe esserci una radio-tv con una sola antenna
                bool tiposrb = ((freqz==0.0) || (idSer != 4 && idSer != 5 && idSer != 6 && idSer != 7 && idSer != 8 && idSer != 9 && idSer != 15 && idSer != 17 && idSer != 18 && idSer != 21));
                int tipocalcolo = 2;
                if(contantenne<=1 || tiposrb) tipocalcolo = 0; // caso di nessuna antenna (non fa nessun ciclo sulle antenne) oppure di un'antenna oppure di più antenne ma srb
                else tipocalcolo = 1; // caso di più antenne i cui campi vanno sommati vettorialmente
                // vettori con i parametri da conservare per recuperare eventualmente dati dai calcoli per l'antenna precedente
                double dirant[4] = {2.0,2.0,2.0,2.0}; // {sen(fi), cos(fi) , sen(teta)} - parametri dalla direzione di puntamento dell'antenna
                double xyzUno[3] = {-9999.9, -9999.9, -9999.9}; // coordinate del punto nel sistema di riferimento dell'antenna per ricavare le attenuazioni Dh e Dv
                double xyzZero[3] = {-9999.9, -9999.9, -9999.9}; // coordinate del punto nel sistema di riferimento con centro nell'antenna
                double grad[2]={-9999.9,-9999.9}; // figrad  tetagrad calcolati;
                double r2 = -1.0; // ...
                switch(tipocalcolo) { // somma quadratica o somma vettoriale...dipende dal tipo di impianto (SRB, radio-tv...)
                    case 0: { // una o nessuna antenna e/o più antenne di una srb
                        for (vector<antenna*>::iterator itrA = antImp.begin();itrA != antImp.end();itrA++) {
                            antenna * pAnt = *itrA; // antenna corrente
                            modello * pMod = pAnt->getModello(); // modello d'antenna dell'antenna corrente
                            int rot = 0; // rotazione dell'antenna in senso orario attorno all'asse della sua direzione di puntamento
                            double h2 = calcoloModulo(pAnt, pMod, zImp, xyz_punto, xyzUno, xyzZero, r2, grad, dirant, rot)/ZCAR/ZCAR;
                            h2_sum += h2; // aggiornamento della somma quadratica dei campi nel punto (caso SRB)
                        }
                    }
                    break;
                    case 1: { // più antenne e somma vettoriale dei campi
                        double ck[6] = {0.0,0.0,0.0,0.0,0.0,0.0};//{xSsin,xScos,ySsin,yScos,zSsin,zScos}; // per calcolo con funzione standard
                        double u[6] = {0.0,0.0,0.0,0.0,0.0,0.0};//versori standard - serve a memorizzarli
                        for (vector<antenna*>::iterator itrA = antImp.begin();itrA != antImp.end();itrA++) {
                            antenna * pAnt = *itrA; // antenna corrente
                            modello * pMod = pAnt->getModello(); // modello d'antenna dell'antenna corrente
                            int rot = 0; // rotazione dell'antenna in senso orario attorno all'asse della sua direzione di puntamento
                            double h2 = calcoloModulo(pAnt, pMod, zImp, xyz_punto, xyzUno, xyzZero, r2, grad, dirant, rot)/ZCAR/ZCAR;
                            calcoloVersori(u,xyzZero,dirant,r2,rot); // aggiorna i versori cioè u[]
                            calcoloVett(pAnt, r2, h2, freqz, ck, u, rot, 1); // 1 per campo magnetico - qui forse si possono recuperare alcuni versori passando da un'antenna alla successiva
                            h2_sum += (ck[0]*ck[0]+ck[1]*ck[1]+ck[2]*ck[2]+ck[3]*ck[3]+ck[4]*ck[4]+ck[5]*ck[5])/2.; //e' +=(xSsin*xSsin+xScos*xScos+ySsin*ySsin+yScos*yScos+zSsin*zSsin+zScos*zScos)/2
                        } // fine ciclo sulle antenne
                    }
                    break;
                    default:{
                        potTotPunto = -9999;
                        potTotPuntoH = -9999;
                        potTotPuntoS = -9999;
                        return 1; // se succede c'è un errore perché non sono stati concordati altri casi di polarizzazione da inserire in dbNir...
                    }
                    break;
                }

            } // FINE CICLO SUI SITI
        }
        break;
        case 2: // campo elettrico, campo magnetico e densità di potenza
        {
            // Ciclo sui siti
            for (vector<impianto*>::iterator itrI = gruppoImp->impianti.begin();itrI != gruppoImp->impianti.end();itrI++)
            {
                double zImp = (*itrI)->getZ(); // quota della base dell'impianto (m slm)
                vector<antenna*> antImp = (*itrI)->antenne;
                double idSer = antImp[0]->getIdServizio(); // prendo  il servizio dalla prima antenna
                double freqz = antImp[0]->getFrequenza(); // prendo la frequenza dalla prima antenna (se è 0 considero che sia una SRB altrimenti non posso fare il calcolo vettoriale)
                int contantenne = antImp.size(); // potrebbe esserci una radio-tv con una sola antenna
                bool tiposrb = ((freqz==0.0) || (idSer != 4 && idSer != 5 && idSer != 6 && idSer != 7 && idSer != 8 && idSer != 9 && idSer != 15 && idSer != 17 && idSer != 18 && idSer != 21));
                int tipocalcolo = 2;
                if(contantenne<=1 || tiposrb) tipocalcolo = 0; // caso di nessuna antenna (non fa nessun ciclo sulle antenne) oppure di un'antenna oppure di più antenne ma srb
                else tipocalcolo = 1; // caso di più antenne i cui campi vanno sommati vettorialmente
                // vettori con i parametri da conservare per recuperare eventualmente dati dai calcoli per l'antenna precedente
                double dirant[4] = {2.0,2.0,2.0,2.0}; // {sen(fi), cos(fi) , sen(teta)} - parametri dalla direzione di puntamento dell'antenna
                double xyzUno[3] = {-9999.9, -9999.9, -9999.9}; // coordinate del punto nel sistema di riferimento dell'antenna per ricavare le attenuazioni Dh e Dv
                double xyzZero[3] = {-9999.9, -9999.9, -9999.9};
                double grad[2]={-9999.9,-9999.9}; // figrad  tetagrad;
                double r2 = -1.0; // ...
                switch(tipocalcolo) { // somma quadratica o somma vettoriale...dipende dal tipo di impianto (SRB, radio-tv...)
                    case 0: { // una o nessuna antenna e/o più antenne di una srb
                        for (vector<antenna*>::iterator itrA = antImp.begin();itrA != antImp.end();itrA++) {
                            antenna * pAnt = *itrA; // antenna corrente
                            modello * pMod = pAnt->getModello(); // modello d'antenna dell'antenna corrente
                            int rot = 0; // rotazione dell'antenna in senso orario attorno all'asse della sua direzione di puntamento
                            double e2 = calcoloModulo(pAnt, pMod, zImp, xyz_punto, xyzUno, xyzZero, r2, grad, dirant, rot);
                            e2_sum += e2; // aggiornamento della somma quadratica dei campi nel punto (caso SRB)
                            h2_sum += e2/ZCAR/ZCAR; // aggiornamento della somma quadratica dei campi nel punto (caso SRB) 
                            s_sum += e2/ZCAR; // incremento della densità di potenza (si aggiunge il prodotto dei valori efficaci-evito la radice quadrata poiché e ed h sono proporzionali)
                        }
                    }
                    break;
                    case 1: { // più antenne e somma vettoriale dei campi
                        double ckE[6] = {0.0,0.0,0.0,0.0,0.0,0.0};//{xSsin,xScos,ySsin,yScos,zSsin,zScos}; // per calcolo campo elettrico con funzione standard
                        double ckH[6] = {0.0,0.0,0.0,0.0,0.0,0.0};//{xSsin,xScos,ySsin,yScos,zSsin,zScos}; // per calcolo campo magnetico con funzione standard
                        double u[6] = {0.0,0.0,0.0,0.0,0.0,0.0};//versori standard - serve a memorizzarli
                        for (vector<antenna*>::iterator itrA = antImp.begin();itrA != antImp.end();itrA++) {
                            antenna * pAnt = *itrA; // antenna corrente
                            modello * pMod = pAnt->getModello(); // modello d'antenna dell'antenna corrente
                            int rot = 0; // rotazione dell'antenna in senso orario attorno all'asse della sua direzione di puntamento
                            double e2 = calcoloModulo(pAnt, pMod, zImp, xyz_punto, xyzUno, xyzZero, r2, grad, dirant, rot);
                            double h2 = e2/ ZCAR/ ZCAR;
                            calcoloVersori(u,xyzZero,dirant,r2,rot); // aggiorna i versori cioè u[]
                            calcoloVett(pAnt, r2, e2, freqz, ckE, u, rot, 0); // 0 per campo elettrico - qui forse si possono recuperare alcuni versori passando da un'antenna alla successiva
                            e2_sum += (ckE[0]*ckE[0]+ckE[1]*ckE[1]+ckE[2]*ckE[2]+ckE[3]*ckE[3]+ckE[4]*ckE[4]+ckE[5]*ckE[5])/2.; //e' +=(xSsin*xSsin+xScos*xScos+ySsin*ySsin+yScos*yScos+zSsin*zSsin+zScos*zScos)/2
                            calcoloVett(pAnt, r2, h2, freqz, ckH, u, rot, 1); // 1 per campo magnetico - qui forse si possono recuperare alcuni versori passando da un'antenna alla successiva
                            h2_sum += (ckH[0]*ckH[0]+ckH[1]*ckH[1]+ckH[2]*ckH[2]+ckH[3]*ckH[3]+ckH[4]*ckH[4]+ckH[5]*ckH[5])/2.; //e' +=(xSsin*xSsin+xScos*xScos+ySsin*ySsin+yScos*yScos+zSsin*zSsin+zScos*zScos)/2
                            s_sum += sqrt(e2*h2); // incremento della densità di potenza (si aggiunge il prodotto dei valori efficaci)
                        } // fine ciclo sulle antenne
                    }
                    break;
                    default:{
                        potTotPunto = -9999;
                        potTotPuntoH = -9999;
                        potTotPuntoS = -9999;
                        return 1; // se succede c'è un errore perché non sono stati concordati altri casi di polarizzazione da inserire in dbNir...
                    }
                    break;
                }

            } // FINE CICLO SUI SITI
        }
        break;
    }

    // Assegno i valori calcolati
    switch (tipoCampo) {
        case 0: potTotPunto = sqrt(e2_sum) * (double)scalavm; // VALORE EFFICACE DEL CAMPO ELETTRICO TOTALE NEL PUNTO (DOVUTO A TUTTI I SISTEMI CONSIDERATI)
        break;
        case 1: potTotPuntoH = sqrt(h2_sum) * (double)scalavm; // VALORE EFFICACE DEL CAMPO MAGNETICO TOTALE NEL PUNTO (DOVUTO A TUTTI I SISTEMI CONSIDERATI)
        break;
        case 2: {
            potTotPunto = sqrt(e2_sum) * (double)scalavm; // VALORE EFFICACE DEL CAMPO ELETTRICO TOTALE NEL PUNTO (DOVUTO A TUTTI I SISTEMI CONSIDERATI)
            potTotPuntoH = sqrt(h2_sum) * (double)scalavm; // VALORE EFFICACE DEL CAMPO MAGNETICO TOTALE NEL PUNTO (DOVUTO A TUTTI I SISTEMI CONSIDERATI)
            potTotPuntoS = s_sum * (double)scalavm;
            // aggiungere la densità di potenza
        }
        break;
        default: {
            potTotPunto = -9999;
            potTotPuntoH = -9999;
            potTotPuntoS = -9999;
            return 1; // sbagliata la selezione del campo
        }
        break;
    }

    return 0;
} // FINE FUNZIONE calculateField

/***********************************************************************************/



/****************************************************************************
    FUNZIONE: calcoloOrizz(
                x_0+" "y_o  :coord. x e y grid del punto iniziale a sinistra
                lato_x+" "+lato_y : base (x) e altezza (y) del rettangolo di misura
                cellsize :  passoGrid
                z_o :       quota(z)
                file output per campo elettrico
                file output per campo magnetico
                file output per la densità di potenza
                tipoCampo (0 elettrico, 1 magnetico, 2 entrambi)
                tipoRaster ( 0 Arc/Info ASCII Grid, 1 GeoTiff, 2 entrambi)
    RETURN:   codice errore <>0 oppure 0
****************************************************************************/

int moduloCalcolo::calcoloOrizz(double xGB,double yGB, double dx,double dy,double CellSize,
                        double Altitudine, char* strFileOutputE, char* strFileOutputH,char* strFileOutputS,int tipoCampo,int tipoRaster)
{
    //-----------------------------------------------------------------
    // Dichiarazione variabili locali
    //-----------------------------------------------------------------


    // puntatori ai file non sevono se si utilizzano le classi ascWriter e geotiff_writer
    ascWriter * ascGrE;
    ascWriter * ascGrH;
    ascWriter * ascGrS;
    geotiffWriter * gtGrE;
    geotiffWriter * gtGrH;
    geotiffWriter * gtGrS;
    //float * vl_lineE;
    //float * vl_lineH;
    //float * vl_lineS;
    
    bool rstAsc;
    bool rstGT;
    
    int intReturnValue;
    int scalavm=1;
    char strD2Lambda[2]; // voglio il calcolo sempre anche in campo vicino
    strD2Lambda[0] = 'S';
    strD2Lambda[1] = 'I';
    int i,j;


    //---Variabili GRID
    double x_origin = xGB;
    double y_origin = yGB;
    double z_origin = Altitudine;
    double lato_x = dx;
    double lato_y = dy;
    double cellsize = CellSize;
    char strAltitudine[32];
    //--------------------------------------------------------------------
    // Calcolo num celle nella direz. x e y e passo grid
    //--------------------------------------------------------------------
    int n_punti_x = (int) ceil(lato_x/cellsize); // ceil arrotonda per difetto
    int n_punti_y = (int) ceil(lato_y/cellsize); // ceil arrotonda per difetto

    //---Variabili punto
    //  Inizio calcolo nell'angolo in alto a sinistra
    //  x_origin, y_origin + lato_y
    //-----------------------------------------------------------------------
    double x_punto = x_origin;
    double y_punto = y_origin + lato_y;
    double z_punto = z_origin;
    

    switch(tipoRaster){
        case 0:{ // solo ASCII (asc)
            rstAsc = true;
            rstGT = false;        
        }
        break;
        case 1: { // solo geoTiff
            rstAsc = false;
            rstGT = true;
        }
        break;
        case 2:{ // sia ASCII (asc) sia geoTiff
            rstAsc = true;
            rstGT = true;            
        }
        break;
        default: { // solo ASCII (asc)
            rstAsc = true;
            rstGT = false;
        }
        break;
    }
    sprintf(strAltitudine, "%f", Altitudine);

    //--------------------------------------------------------------------
    // Apertura file ASCII grid dove memorizzare il risultato del campo
    // All'inizio c'è l'intestazione:
    //--------------------------------------------------------------------
    //---Apertura file
    // e scrittura della prima riga dove ci sono le informazioni generali
    // tra cui le coordinate dell'angolo in alto a sinistra (xmin,ymax) da dove partono i valori
    // di campo calcolati e riportati nella griglia/raster
    // Affinché i valori di campo calcolati siano associati al centro di ciascuna cella e non
    // all'angolo in alto a sinistra della griglia, cambio l'angolo di partenza della griglia indicato nel raster/asc:
    // xmin = xmin-1/2*passo e ymax = ymax+1/2*passo
    
    double xmin = x_origin-cellsize/2.0;
    double ymax = y_origin+lato_y+cellsize/2.0;
    double ymin = ymax - n_punti_y*cellsize;

    //-----------------------------------------------------------------------
    //  Calcolo del campo elettrico in tutti i punti
    //  calcolo riga per riga
    //  Inizio calcolo nell'angolo in alto a sinistra
    //  x_origin, y_origin + lato_y
    //-----------------------------------------------------------------------

    switch(tipoCampo) {
        case 0:{ // campo elettrico
            //if(rstAsc) ascGrE = new ascWriter(strFileOutputE,xmin,ymax,n_punti_x,n_punti_y,cellsize,cellsize); 
            if(rstAsc) ascGrE = new ascWriter(strFileOutputE,xmin,ymin,n_punti_x,n_punti_y,cellsize,cellsize); // parte dall'angolo in basso a sinistra
            if(rstGT) { // Se raster geoTiff, scrive anche i metadata
                gtGrE = new geotiffWriter(strFileOutputE,xmin,ymax,n_punti_x,n_punti_y,cellsize,cellsize); // per il geoTiff parte dall'angolo in alto a sinistra
                gtGrE->addMetaData((char*)"SLM",strAltitudine);
                if (gruppoImp) gtGrE->addMetaData((char*)"MD5",gruppoImp->md5FileImp);
                gtGrE->writeMetaData(); 
            }          
            //vl_lineE = (float *) malloc(n_punti_x * sizeof(float));
            //float vl_lineE[n_punti_x];
            // parte dall'angolo in alto a sinistra: (xmin,ymin) per convenzione dei raster
            // scorre sulle righe partendo da quella con ymax
            for (i=0;i<n_punti_y;i++)
            {
                // scorre prima sulle colonne
                float vl_lineE[n_punti_x];
                for(j=0;j<n_punti_x;j++)
                {
                    // Calcolo campo nel punto dato dal contributo di tutte le antenne
                    intReturnValue = calculateField(x_punto,y_punto,z_punto,strD2Lambda,scalavm,tipoCampo);
                    if (intReturnValue>0) potTotPunto = -9999; // in realtà è già così nella funzione calculateField... è un controllo inutile, servirebbe se si interrompesse la sezione
                    vl_lineE[j] = potTotPunto; // qui c'è una conversione implicita da double (potTotPunto) a float
                    x_punto+=cellsize; //Passa al punto successivo della riga
                }
                if(rstAsc) ascGrE->printRow(vl_lineE); // printRow è molto lenta perché ripete nuovamente il ciclo su tutti i punti della riga... invece di scrivere direttamente la riga nel file .asc!
                if(rstGT) gtGrE->printRow(i,vl_lineE);

                //------------------------------------------------
                //Passaggio alla riga successiva : y = y - step_y
                //------------------------------------------------
                x_punto=x_origin;
                y_punto-=cellsize;
            }            
            //free(vl_lineE);
            if(rstAsc) {
                ascGrE->close(); // Chiusura file
                delete ascGrE; // Pulisco la memoria con delete
            }
            if(rstGT) {
                gtGrE->close(); // Chiusura file
                delete gtGrE; // Pulisco la memoria con delete
            }
        }
        break;
        case 1:{ // campo magnetico
            if(rstAsc) ascGrH = new ascWriter(strFileOutputH,xmin,ymin,n_punti_x,n_punti_y,cellsize,cellsize); // parte dall'angolo in basso a sinistra
            if(rstGT){ // Se raster, scrive anche i metadata
                gtGrH = new geotiffWriter(strFileOutputH,xmin,ymax,n_punti_x,n_punti_y,cellsize,cellsize);
                gtGrH->addMetaData((char*)"SLM",strAltitudine);
                if (gruppoImp) gtGrH->addMetaData((char*)"MD5",gruppoImp->md5FileImp);
                gtGrH->writeMetaData();                
            }      
            //vl_lineH = (float *) malloc(n_punti_x * sizeof(float));
            // parte dall'angolo in alto a sinistra: (xmin,ymin) per convenzione dei raster
            // scorre sulle righe partendo da quella con ymax
            for (i=0;i<n_punti_y;i++)
            {
                float vl_lineH[n_punti_x];
                // scorre prima sulle colonne
                for(j=0;j<n_punti_x;j++)
                {
                    // Calcolo campo nel punto dato dal contributo di tutte le antenne
                    intReturnValue = calculateField(x_punto,y_punto, z_punto,strD2Lambda,scalavm,tipoCampo);
                    if (intReturnValue>0) potTotPuntoH = -9999;
                    vl_lineH[j] = potTotPuntoH;                       
                    x_punto+=cellsize; //Passa al punto successivo della riga
                }
                if(rstAsc) ascGrH->printRow(vl_lineH);
                if(rstGT) gtGrH->printRow(i,vl_lineH);
                //------------------------------------------------
                //Passaggio alla riga successiva : y = y - step_y
                //------------------------------------------------
                x_punto=x_origin;
                y_punto-=cellsize;
            }            
            //free(vl_lineH);
            if(rstAsc) {
                ascGrH->close(); // Chiusura file
                delete ascGrH; // Pulisco la memoria con delete
            }
            if(rstGT) {
                gtGrH->close(); // Chiusura file
                delete gtGrH; // Pulisco la memoria con delete
            }
        }
        break;
        case 2:{ // campo elettrico, campo magnetico, densità di potenza
            if(rstAsc) {
                ascGrE = new ascWriter(strFileOutputE,xmin,ymin,n_punti_x,n_punti_y,cellsize,cellsize); // parte dall'angolo in basso a sinistra
                ascGrH = new ascWriter(strFileOutputH,xmin,ymin,n_punti_x,n_punti_y,cellsize,cellsize); // parte dall'angolo in basso a sinistra
                ascGrS = new ascWriter(strFileOutputS,xmin,ymin,n_punti_x,n_punti_y,cellsize,cellsize); // parte dall'angolo in basso a sinistra
            }
            if(rstGT) { // Se raster, scrive i metadata
                gtGrE = new geotiffWriter(strFileOutputE,xmin,ymax,n_punti_x,n_punti_y,cellsize,cellsize);
                gtGrH = new geotiffWriter(strFileOutputH,xmin,ymax,n_punti_x,n_punti_y,cellsize,cellsize);
                gtGrS = new geotiffWriter(strFileOutputS,xmin,ymax,n_punti_x,n_punti_y,cellsize,cellsize);
                gtGrE->addMetaData((char*)"SLM",strAltitudine);
                gtGrH->addMetaData((char*)"SLM",strAltitudine);
                gtGrS->addMetaData((char*)"SLM",strAltitudine);
                if (gruppoImp) {
                    gtGrE->addMetaData((char*)"MD5",gruppoImp->md5FileImp);
                    gtGrH->addMetaData((char*)"MD5",gruppoImp->md5FileImp);
                    gtGrS->addMetaData((char*)"MD5",gruppoImp->md5FileImp);
                }
                gtGrE->writeMetaData();                
                gtGrH->writeMetaData();                
                gtGrS->writeMetaData();                
            }            
            //vl_lineE = (float *) malloc(n_punti_x * sizeof(float));
            //vl_lineH = (float *) malloc(n_punti_x * sizeof(float));
            //vl_lineS = (float *) malloc(n_punti_x * sizeof(float));
            // parte dall'angolo in alto a sinistra: (xmin,ymin) per convenzione dei raster
            // scorre sulle righe partendo da quella con ymax
            for (i=0;i<n_punti_y;i++)
            {
                // scorre prima sulle colonne
                float vl_lineE[n_punti_x];
                float vl_lineH[n_punti_x];
                float vl_lineS[n_punti_x];
                for(j=0;j<n_punti_x;j++)
                {
                    // Calcolo campo nel punto dato dal contributo di tutte le antenne
                    intReturnValue = calculateField(x_punto,y_punto, z_punto,strD2Lambda,scalavm,tipoCampo);
                    if (intReturnValue>0) { // valore non calcolabile -9999
                        potTotPunto = -9999;
                        potTotPuntoH = -9999;
                        potTotPuntoS = -9999;
                    }
                    vl_lineE[j] = potTotPunto;
                    vl_lineH[j] = potTotPuntoH;                      
                    vl_lineS[j] = potTotPuntoS;
                    x_punto+=cellsize; //Passa al punto successivo della riga
                }
                if(rstAsc) {
                    ascGrE->printRow(vl_lineE);
                    ascGrH->printRow(vl_lineH);
                    ascGrS->printRow(vl_lineS);
                }
                if(rstGT) {
                    gtGrE->printRow(i,vl_lineE);
                    gtGrH->printRow(i,vl_lineH);
                    gtGrS->printRow(i,vl_lineS);
                }
                //------------------------------------------------
                //Passaggio alla riga successiva : y = y - step_y
                //------------------------------------------------
                x_punto=x_origin;
                y_punto-=cellsize;
            }            
            //free(vl_lineE);
            //free(vl_lineH);
            //free(vl_lineS);
            if(rstAsc) {
                ascGrE->close(); // Chiusura file
                delete ascGrE; // Pulisco la memoria con delete
                ascGrH->close(); // Chiusura file
                delete ascGrH; // Pulisco la memoria con delete
                ascGrS->close(); // Chiusura file
                delete ascGrS; // Pulisco la memoria con delete
            }
            if(rstGT) {
                gtGrE->close(); // Chiusura file
                delete gtGrE; // Pulisco la memoria con delete                
                gtGrH->close(); // Chiusura file
                delete gtGrH; // Pulisco la memoria con delete
                gtGrS->close(); // Chiusura file
                delete gtGrS; // Pulisco la memoria con delete            
            }
        }
        break;
        default: return 1;
        break;                
    }
    return 0;
}





/****************************************************************************
    FUNZIONE: calcoloVert(
                xGB0,yGB0, coordinate iniziali della sezione verticale
                xGB1,yGB1, coordinate finali della sezione verticale
                cellsize,  dimensione della cella di calcolo
                Altitudine, altitudine iniziale della sezione
                dh, altezza della sezione
                strFileOutput  = file output campo elettrico
                strFileOutputh = file output campo magnetico
                tipoCampo (0 elettrico, 1 magnetico, 2 entrambi)
                tipoRaster ( 0 Arc/Info ASCII Grid, 1 GeoTiff, 2 entrambi)
    RETURN:   codice errore <>0 oppure 0
****************************************************************************/

int moduloCalcolo::calcoloVert(double xGB0,double yGB0, double xGB1,double yGB1, double cellsize,
                        double Altitudine,double dh,char* strFileOutputE,char* strFileOutputH,char* strFileOutputS,int tipoCampo,int tipoRaster)
{

    //-----------------------------------------------------------------
    // Dichiarazione variabili locali
    //-----------------------------------------------------------------

    
    // puntatori ai file non sevono se si utilizzano le classi ascWriter e geotiff_writer
    ascWriter * ascGrE;
    ascWriter * ascGrH;
    geotiffWriter * gtGrE;
    geotiffWriter * gtGrH;
    float * vl_lineE;
    float * vl_lineH;
    
    bool calcE;
    bool calcH;
    bool rstAsc;
    bool rstGT;
    
    
    
    
    int intReturnValue;
    int i,j,numCelleXY;
    char strD2Lambda[16];
    int scalavm;


    //---Variabili punto
    double x_punto, y_punto, z_punto,lungXY,dlX,dlY;

    //---Variabili per RASTER
    double x_origin, y_origin;

    double z_min, z_max;
    double passoGrid , passoGrid_x, passoGrid_y, passoGrid_z;
    int numCelleZ;

    if (cellsize <= 0)
        return 10;
    
    strD2Lambda[0]='S';
    strD2Lambda[1]='I';
    scalavm = 1;
    x_origin=xGB0;
    y_origin=yGB0;

    passoGrid = cellsize;
    dlX = xGB1 - xGB0;
    dlY = yGB1 - yGB0;
    lungXY = sqrt(xGB1*xGB1+yGB1*yGB1);
    passoGrid_x = dlX/passoGrid;
    passoGrid_y = dlY/passoGrid;    
    z_min = Altitudine;
    z_max = Altitudine + dh;

    // Dall tipoCampo e tipoRaster imposta le variabili booleane utilizzate
    // in seguito per il tipo di campo (E o H) e di file (asc o geotiff)
    if ((tipoCampo<0)||(tipoCampo>2)) {
        calcE = true;
        calcH = false;
    }
    if ((tipoCampo==0)||(tipoCampo==2))
        calcE = true;
    else
        calcE = false;
    if ((tipoCampo==1)||(tipoCampo==2))
        calcH = true;
    else
        calcH = false;
    if ((tipoRaster<0)||(tipoRaster>2)) {
        rstAsc = true;
        rstGT = false;
    }
    if ((tipoRaster==0)||(tipoRaster==2))
        rstAsc = true;
    else
        rstAsc = false;
    if ((tipoRaster==1)||(tipoRaster==2))
        rstGT = true;
    else
        rstGT = false;


    //--------------------------------------------------------------------
    // Calcolo num celle nella direz. xy e z
    //--------------------------------------------------------------------
    passoGrid_z = passoGrid;
    //numCelleXY = (int) ceil(fabs(lungXY/passoGrid_z)); // fabs è vecchio C (in C++ abs fa tutto)
    numCelleXY = (int) ceil(abs(lungXY/passoGrid_z)); // ceil approssima per difetto
    numCelleZ = (int) ceil((z_max - z_min)/passoGrid_z); // ceil approssima per difetto



    //--------------------------------------------------------------------
    //---Apertura file asc o geotiff
    //--------------------------------------------------------------------
    // Definisco anche il punto di partenza standard cioè l'angolo in alto a sinistra
    // della griglia/raster.
    // In questo codice ogni cella della griglia rappresenta il calcolo nell'angolo in alto a sx della stessa
    /*if (calcE) {
        if(rstAsc) 
            ascGrE = new ascWriter(strFileOutputE,0.,z_max,numCelleXY,numCelleZ,passoGrid,passoGrid);
        if(rstGT)
            gtGrE = new geotiffWriter(strFileOutputE,0.,z_max,numCelleXY,numCelleZ,passoGrid,passoGrid);
    }
    if (calcH) {
        if(rstAsc) 
            ascGrH = new ascWriter(strFileOutputH,0.,z_max,numCelleXY,numCelleZ,passoGrid,passoGrid);
        if(rstGT)
            gtGrH = new geotiffWriter(strFileOutputH,0.,z_max,numCelleXY,numCelleZ,passoGrid,passoGrid);
    } */   


    //--------------------------------------------------------------------
    //---Apertura file asc o geotiff
    //--------------------------------------------------------------------
    // Definisco anche il punto di partenza standard cioè l'angolo in alto a sinistra
    // della griglia/raster.
    // Poiché voglio che ogni cella della griglia rappresenti il calcolo del campo al centro
    // della cella stessa, devo spostare di mezzo passo a sinistra e mezzo passo in alto
    double lungmin = -passoGrid/2.0; // coordinata minima nel senso della lunghezza della linea di sezione
    double altmax = z_max + passoGrid_z/2.0; // coordianta massima nel senso dell'altezza (in mslm) della sezione
    if (calcE) {
        if(rstAsc) 
            ascGrE = new ascWriter(strFileOutputE,lungmin,altmax,numCelleXY,numCelleZ,passoGrid,passoGrid);
        if(rstGT)
            gtGrE = new geotiffWriter(strFileOutputE,lungmin,altmax,numCelleXY,numCelleZ,passoGrid,passoGrid);
    }
    if (calcH) {
        if(rstAsc) 
            ascGrH = new ascWriter(strFileOutputH,lungmin,altmax,numCelleXY,numCelleZ,passoGrid,passoGrid);
        if(rstGT)
            gtGrH = new geotiffWriter(strFileOutputH,lungmin,altmax,numCelleXY,numCelleZ,passoGrid,passoGrid);
    }    


    //--------------------------------------------------------------------
    //---Istanza del buffer per i valori di una riga
    //--------------------------------------------------------------------
    if (calcE) 
        vl_lineE = (float *) malloc(numCelleXY * sizeof(float));
    if (calcH)
        vl_lineH = (float *) malloc(numCelleXY * sizeof(float));
        
        
    //-----------------------------------------------------------------------
    //  Calcolo del campo in tutti i punti
    //  calcolo riga per riga partendo da z_maz a z_min e da sinistra a destra
    //  Inizio calcolo nell'angolo in alto a sinistra
    //  x = x_0, y = y_0, z = z_max
    //-----------------------------------------------------------------------
    
    z_punto = z_max;

    // ciclo sulle righe (z)
    for (i=0;i<numCelleZ;i++)
    {
        //------------------------------------------------
        //Calcolo campo lungo una riga nel piano Z=cost
        //------------------------------------------------
        x_punto=x_origin;
        y_punto=y_origin;

        for(j=0;j<numCelleXY;j++)
        {
            //-----------------------------------------------------------
            // Calcolo campo nel punto dato dal contributo di tutte
            // le antenne
            //-----------------------------------------------------------
            intReturnValue = calculateField(x_punto,y_punto, z_punto,strD2Lambda,scalavm,tipoCampo);
            /*
            if (intReturnValue>0){
                return intReturnValue;
            }
            */
            
            if (intReturnValue>0){
                // valore non calcolabile -9999
            //      return intReturnValue;
                potTotPunto = -9999;
                potTotPuntoH = -9999;
            }
            if (calcE)
                vl_lineE[j] = potTotPunto;
            if (calcH)
                vl_lineH[j] = potTotPuntoH;
            

            //-----------------------------------------------------------
            //Passa al punto successivo della riga
            //-----------------------------------------------------------
            x_punto += passoGrid_x;
            y_punto += passoGrid_y;
        }
        //------------------------------------------------
        //Passa alla riga successiva
        //------------------------------------------------
        if (calcE) {
            if(rstAsc)
                ascGrE->printRow(vl_lineE);
            if(rstGT)
                gtGrE->printRow(i,vl_lineE);
        }
        if (calcH) {
            if(rstAsc)
                ascGrH->printRow(vl_lineH);
            if(rstGT)
                gtGrH->printRow(i,vl_lineH);
        }
        
        z_punto -= passoGrid_z;
        
    }
    if (calcE)
        free(vl_lineE);
    if (calcH)
        free(vl_lineH);

    //-----------------------------------------------------------------------
    //Chiusura file
    //-----------------------------------------------------------------------
    if (calcE) {
        if(rstAsc) 
            ascGrE->close();
        if(rstGT)
            gtGrE->close();
    }
    if (calcH) {
        if(rstAsc) 
            ascGrH->close();
        if(rstGT)
            gtGrH->close();
    }

    //-----------------------------------------------------------------------
    // Pulisco la memoria con delete
    //-----------------------------------------------------------------------
    if (calcE) {
        if(rstAsc)
            delete ascGrE;
        if(rstGT)
            delete gtGrE;
    }
    if (calcH) {
        if(rstAsc)
            delete ascGrH;
        if(rstGT)
            delete gtGrH;
    }  
    

    return 0;
}

/****************************************************************************
    FUNZIONE: calcoloPunti(
                strFileIndicatori:  nome del file dbf che contiene gli indicatori da valutare (es. indic.dbf)
                strFileElencAnt:    file di testo con configurazione antenne (di norma sorgenti.txt)
                strTipoCampo: "0" per campo elettrico o "1" per campo magnetico
    RETURN:   codice errore <>0 oppure 0
              1001 - campo X non trovato nel file di input
              1002 - campo Y non trovato nel file di input
              1003 - campo Z non trovato nel file di input
              1004 - campo CAMPO non trovato nel file di input
              1005 - problemi scrittura valore campo
    * ndr (Mattia Scola - 27/08/2014)
****************************************************************************/
int moduloCalcolo::calcoloPunti(char* strFileIndicatori,int tipoCampo)
{
    //-----------------------------------------------------------------
    // Dichiarazione variabili locali
    //-----------------------------------------------------------------
    int intReturnValue;
    int scalavm=1;
    //tipoCampo = atoi(strTipoCampo);
    char strD2lambda[2]; // voglio il calcolo sempre anche in campo vicino
    strD2lambda[0] = 'S';
    strD2lambda[1] = 'I';
    //---Variabili punto
    double x_punto, y_punto, z_punto;
    
    /*
    const char* strCampoPerAntenna = "D:\\NirGis\\CampoPerAntenna.txt";
    FILE* finCampoPerAntenna = NULL;
    finCampoPerAntenna = fopen(strCampoPerAntenna,"w");
    */
    
    //--------------------------------------------------------------------
    //Estrazione parametri in ingresso:
    //--------------------------------------------------------------------
    //fileElencoAnt = strFileElencAnt; // è il file Sorgenti.txt

    //--------------------------------------------------------------------
    // Estrazione informazioni dai file con i dati delle antenne
    //--------------------------------------------------------------------
    /*
    intReturnValue = readDataFile(strFileElencAnt); // legge il file "sorgenti.txt" con i modelli delle antenne
                                                    // è sufficiente che l'argomento sia diverso da NULL
    */

    // if (intReturnValue>0) return intReturnValue;

    // legge il file dbf di input e cicla su su x,y,z
    DBFHandle dbfhIndicatori;
    dbfhIndicatori = DBFOpen(strFileIndicatori,"rb+");
    int numRighe=0;
    numRighe = DBFGetRecordCount(dbfhIndicatori);
    int rec;
    // CAMPO ELETTRICO
    int intCampoX,intCampoY,intCampoZ,intCampoCampo;
    intCampoX = DBFGetFieldIndex(dbfhIndicatori,"X");
    if(intCampoX== -1)
        return 1001;
    intCampoY = DBFGetFieldIndex(dbfhIndicatori,"Y");
    if(intCampoY== -1)
        return 1002;
    intCampoZ = DBFGetFieldIndex(dbfhIndicatori,"Z");
    if(intCampoZ== -1)
        return 1003;
    intCampoCampo = DBFGetFieldIndex(dbfhIndicatori,"CAMPO");
    if(intCampoCampo== -1)
        return 1004;

    for (rec=0;rec<numRighe;rec++)
    {
        x_punto = DBFReadDoubleAttribute(dbfhIndicatori,rec,intCampoX);
        y_punto = DBFReadDoubleAttribute(dbfhIndicatori,rec,intCampoY);
        z_punto = DBFReadDoubleAttribute(dbfhIndicatori,rec,intCampoZ);
        //--------------------------------------------------------------------
        // Calcolo campo nel punto dato dal contributo di tutte le antenne
        //--------------------------------------------------------------------
        intReturnValue = calculateField(x_punto,y_punto,z_punto,strD2lambda,scalavm,tipoCampo);
        // AGGIUNGERE IL CASO DI CAMPO MAGNETICO!!!!! (AGGIUNGERE IL PARAMETRO strTipoCampo alla funzione)

        if (intReturnValue>0){
            // valore non calcolabile -9999
        //      return intReturnValue;
            potTotPunto = -9999;
            potTotPuntoH = -9999;
        }
        if(tipoCampo==0) {
            //fprintf(finCampoPerAntenna,"%f %f %f %lf \r\n",x_punto,y_punto,z_punto, potTotPunto);
            intReturnValue = DBFWriteDoubleAttribute(dbfhIndicatori,rec,intCampoCampo,potTotPunto);
        }
        else {
            //fprintf(finCampoPerAntenna,"%f %f %f %lf \r\n",x_punto,y_punto,z_punto, potTotPuntoH);
            intReturnValue = DBFWriteDoubleAttribute(dbfhIndicatori,rec,intCampoCampo,potTotPuntoH);
        }
        if (intReturnValue == false){
            // problemi di scrittura del punto
            return 1005;
        }

        //--------------------------------------------------------------------
        // Formattazione risultato
        // formato = dddddd.ddd
        //--------------------------------------------------------------------
        //sprintf(resultValue,"%6.3lf ",potTotPunto);
    } // per ogni record
    DBFClose(dbfhIndicatori);
    //fclose(finCampoPerAntenna);
    return 0;
}

