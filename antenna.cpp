/** @file antenna.cpp
  * @brief Metodi della classe antenna
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

#include <cstdlib>
#include <string.h>
#include <cstdio>
#include "antenna.h"


string  antenna::debugAntenne = "Elenco eccezioni: \n";

/**
 * Funzioni classe antenna
*/

antenna::antenna(){
    idAnt=0;
    idPadre=0;
    x = 0.;
    y = 0.;
    h = 0.;
    numTx=0;
    potTx=0.;
    tilt=0.;
    direzione=0.;
    antCalc = true;
    attiva = true;

    idImpianto=0;
    idModello=0;        
    idServizio=0;      
    idStato=0;         
    idPolar=0;

    freqMin=0.;       
    freqMax=0.;       
    frequenza=0.;     
    fase=0.;

    potenzaPerc = 1.;
    alfa24 = 1.;
    
    attPC = 1.;
    attDTX = 1.;
    attTDD = 1.;

    pot = 0;

    dataIniz[0] = '\0';
    dataFin[0] = '\0';
    note1[0] = '\0';
    immagine[0] = '\0';
    strVuota[0] = '\0';
    aMod=NULL;
}
antenna::antenna(antenna * oAnt){
    if (oAnt == NULL)
        return;
        
    //copia delle variabili di classe
    idAnt = oAnt->idAnt;
    idPadre = oAnt->idPadre;
    x = oAnt->x;
    y = oAnt->y;
    h = oAnt->h;
    numTx = oAnt->numTx;
    potTx = oAnt->potTx;
    tilt = oAnt->tilt;
    direzione = oAnt->direzione;
    antCalc = oAnt->antCalc;
    attiva = oAnt->attiva;

    idImpianto = oAnt->idImpianto;
    idModello = oAnt->idModello;
    idServizio = oAnt->idServizio;
    idStato = oAnt->idStato;
    idPolar = oAnt->idPolar;

    freqMin = oAnt->freqMin;
    freqMax = oAnt->freqMax;
    frequenza = oAnt->frequenza;
    fase = oAnt->fase;

    potenzaPerc = oAnt->potenzaPerc;
    alfa24 = oAnt->alfa24;
    attPC = oAnt->attPC;
    attDTX = oAnt->attDTX;
    attTDD = oAnt->attTDD;
    aMod = oAnt->aMod;
    Modified = oAnt->Modified;
    
    //copia delle stringhe
    setDataIn(oAnt->getDataIn());
    setDataFin(oAnt->getDataFin());
    setNote1(oAnt->getNote1());
    setImmagine(oAnt->getImmagine());


}

int antenna::setIdAntenna(char* valDato){
    if(sscanf(valDato, "%d", &idAnt) != 1) {
        idAnt=0;
        string err("Inserimento ID Antenna fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;
    }
    return 0;
}

int antenna::setIdPadre(char* valDato){
    if(sscanf(valDato, "%d", &idPadre) != 1) {
        idPadre=0;
        string err("Inserimento ID Padre Antenna fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;
    }
    return 0;
}

int antenna::setX(char* valDato){
    if(sscanf(valDato, "%lf", &x) != 1) {
        x=0;
        string err("Inserimento coordinata X Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setY(char* valDato){
    if(sscanf(valDato, "%lf", &y) != 1) {
        y=0;
        string err("Inserimento coordinata Y Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setH(char* valDato){
    if(sscanf(valDato, "%lf", &h) != 1) {
        h=0;
        string err("Inserimento coordinata H Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setNumTx(char* valDato){
    if(sscanf(valDato, "%d", &numTx) != 1){
        numTx=0;
        string err("Inserimento numero tensori Antenna fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setPotenzaTx(char* valDato){
    if(sscanf(valDato, "%lf", &potTx) != 1){
        potTx=0;
        string err("Inserimento potenza tensori Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setTilt(char* valDato){
    if(sscanf(valDato, "%lf", &tilt) != 1) {
        tilt=0;
        string err("Inserimento Tilt Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setDirezione(char* valDato){
    if(sscanf(valDato, "%lf", &direzione) != 1) {
        direzione=0;
        string err("Inserimento Direzione Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setAttiva(char* valDato){
    if(strcmp(valDato,"TRUE")==0 || strcmp(valDato,"True")==0 || strcmp(valDato,"SI")==0 || strcmp(valDato,"Si")==0 || strcmp(valDato,"si")==0) {
        attiva = true;
        return 0;
    }
    if (strcmp(valDato,"FALSE")==0 || strcmp(valDato,"False")==0 || strcmp(valDato,"NO")==0 || strcmp(valDato,"No")==0 || strcmp(valDato,"no")==0) {
        attiva = false;
        return 0;
    }
    else{
    string err("Inserimento antenna attiva (Antenna) fallito, valore non TRUE-True-FALSE-False-SI-NO-si-no-Si-No: ");
    err.append(valDato);
    err.append("\n");
    debugAntenne.append(err);
    return 1;  
    }
}

int antenna::setAntCalc(char* valDato){
    if(strcmp(valDato,"TRUE")==0 || strcmp(valDato,"True")==0 || strcmp(valDato,"SI")==0 || strcmp(valDato,"Si")==0 || strcmp(valDato,"si")==0)  {
        antCalc = true;
        return 0;
    }
    if (strcmp(valDato,"FALSE")==0 || strcmp(valDato,"False")==0 || strcmp(valDato,"NO")==0 || strcmp(valDato,"No")==0 || strcmp(valDato,"no")==0) {
        antCalc = false;
        return 0;
    }
    else {
    string err("Inserimento antenna attiva per il calcolo (Antenna) fallito, valore non TRUE-True-FALSE-False-SI-NO-si-no-Si-No: ");
    err.append(valDato);
    err.append("\n");
    debugAntenne.append(err);
    return 1;    
    }  
}

int antenna::setIdImpianto(char* valDato){
    if(sscanf(valDato, "%d", &idImpianto) != 1) {
        idImpianto=0;
        string err("Inserimento ID Impianto (Antenna) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;
    }
    return 0;
}

int antenna::setIdModello(char* valDato){
    if(sscanf(valDato, "%d", &idModello) != 1) {
        idModello=0;
        string err("Inserimento ID Modello (Antenna) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;
    }
    return 0;
}

int antenna::setIdServizio(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        idServizio = 0;
        return 0;
    } 
    if(sscanf(valDato, "%d", &idServizio) != 1) {
        idServizio=0;
        string err("Inserimento ID Servizio (Antenna) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;
    }
    return 0;
}

int antenna::setIdPolar(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        idPolar = 0;
        return 0;
    }    
    if(sscanf(valDato, "%d", &idPolar) != 1){
        idPolar=0;
        string err("Inserimento ID Polarizzazione (Antenna) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;
    }
    return 0;
}

int antenna::setIdStato(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        idStato = 0;
        return 0;
    }
    if(sscanf(valDato, "%d", &idStato) != 1) {
        idStato=0;
        string err("Inserimento ID Stato (Antenna) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;
    }
    return 0;
}

int antenna::setFreqMin(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        freqMin = 0.;
        return 0;
    }
    if(sscanf(valDato, "%lf", &freqMin) != 1) {
        freqMin=0;
        string err("Inserimento Frequenza minima Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}
int antenna::setFreqMax(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        freqMax = 0.;
        return 0;
    }    
    if(sscanf(valDato, "%lf", &freqMax) != 1){
        freqMax=0;
        string err("Inserimento Frequenza massima fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}
int antenna::setFrequenza(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        frequenza = 0.;
        return 0;
    }
    if(sscanf(valDato, "%lf", &frequenza) != 1) {
        frequenza=0;
        string err("Inserimento Frequenza  fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}
int antenna::setFase(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        fase = 0.;
        return 0;
    }
    if(sscanf(valDato, "%lf", &fase) != 1) {
        fase=0;
        string err("Inserimento Fase Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setAlfa24(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)){
        alfa24=1.;
        return 0;
    }
    if(sscanf(valDato, "%lf", &alfa24) != 1) {
        alfa24=1.;
        string err("Inserimento Alfa24 Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setAttPC(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)){
        attPC=1.;
        return 0;
    }
    if(sscanf(valDato, "%lf", &attPC) != 1) {
        attPC=1.;
        string err("Inserimento attenuazione PC Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setAttDTX(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)){
        attDTX=1.;
        return 0;
    }
    if(sscanf(valDato, "%lf", &attDTX) != 1) {
        attDTX=1.;
        string err("Inserimento attenuazione DTX Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setAttTDD(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)){
        attTDD=1.;
        return 0;
    }
    if(sscanf(valDato, "%lf", &attTDD) != 1) {
        attTDD=1.;
        string err("Inserimento attenuazione TDD Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}
int antenna::setPotenzaPerc(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)){
        potenzaPerc=1.;
        return 0;
    }
    if(sscanf(valDato, "%lf", &potenzaPerc) != 1) {
        potenzaPerc=1.;
        string err("Inserimento Potenza percentuale Antenna fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
    return 0;
}

int antenna::setDataIn(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(dataIniz,"");
        else
            strcpy(dataIniz,valDato);
        return 0;
    }
    else{
        dataIniz[0] = '\0';
        string err("Inserimento Data iniziale Antenna fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);/*unisce alla stringa i primi 20 caratteri valore inserito*/
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
}

int antenna::setDataFin(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(dataFin,"");
        else
            strcpy(dataFin,valDato);
        return 0;
    }
    else{
        dataFin[0] = '\0';
        string err("Inserimento Data finale Antenna fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);/*unisce alla stringa i primi 20 caratteri valore inserito*/
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
}
int antenna::setNote1(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(note1,"");
        else
            strcpy(note1,valDato);
        return 0;
    }
    else{
        note1[0] = '\0';
        string err("Inserimento Nota Antenna fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);/*unisce alla stringa i primi 20 caratteri valore inserito*/
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
}
int antenna::setImmagine(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(immagine,"");
        else
            strcpy(immagine,valDato);
        return 0;
    }
    else{
        immagine[0] = '\0';
        string err("Inserimento Immagine Antenna fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);/*unisce alla stringa i primi 20 caratteri valore inserito*/
        err.append("\n");
        debugAntenne.append(err);
        return 1;      
    }
}        



int antenna::setDA(char * dbFld, char * valDato){
    //try{
        if (!strcmp(dbFld,"IDANT")) 
            return setIdAntenna(valDato);
        if (!strcmp(dbFld,"ID_PADRE")) 
            return setIdPadre(valDato);
        if (!strcmp(dbFld,"X_ANT")) 
            return setX(valDato);
        if (!strcmp(dbFld,"Y_ANT")) 
            return setY(valDato);
        if (!strcmp(dbFld,"H")) 
            return setH(valDato);
        if (!strcmp(dbFld,"N_TX")) 
            return setNumTx(valDato);
        if (!strcmp(dbFld,"POT_TX")) 
            return setPotenzaTx(valDato);
        if (!strcmp(dbFld,"ATT_PC")) 
            return setAttPC(valDato);
        if (!strcmp(dbFld,"ATT_DTX")) 
            return setAttDTX(valDato); 
        if (!strcmp(dbFld,"ATT_TDD")) 
            return setAttTDD(valDato); 
        if (!strcmp(dbFld,"TILT")) 
            return setTilt(valDato);
        if (!strcmp(dbFld,"DIREZIONE")) 
            return setDirezione(valDato);
        if (!strcmp(dbFld,"ATTIVA")) 
            return setAttiva(valDato);
        if (!strcmp(dbFld,"CHECKED")) 
            return setAntCalc(valDato);
        if (!strcmp(dbFld,"IDSITO")) 
            return setIdImpianto(valDato);
        if (!strcmp(dbFld,"IDMOD")) 
            return setIdModello(valDato);
        if (!strcmp(dbFld,"IDSERV")) 
            return setIdServizio(valDato);
        if (!strcmp(dbFld,"IDPOLAR")) 
            return setIdPolar(valDato);
        if (!strcmp(dbFld,"IDSTATO")) 
            return setIdStato(valDato);
        if (!strcmp(dbFld,"FMIN")) 
            return setFreqMin(valDato);
        if (!strcmp(dbFld,"FMAX")) 
            return setFreqMax(valDato);
        if (!strcmp(dbFld,"FREQUENZA")) 
            return setFrequenza(valDato);
        if (!strcmp(dbFld,"FASE")) 
            return setFase(valDato);
        if (!strcmp(dbFld,"PPERC")) 
            return setPotenzaPerc(valDato);
        if (!strcmp(dbFld,"ALFA24")) 
            return setAlfa24(valDato);
        if (!strcmp(dbFld,"DATA_INI")) 
            return setDataIn(valDato);
        if (!strcmp(dbFld,"DATA_FIN")) 
            return setDataFin(valDato);
        if (!strcmp(dbFld,"NOTE1")) 
            return setNote1(valDato);
        if (!strcmp(dbFld,"IMMAGINE")) 
            return setImmagine(valDato);
        else{
            string err("Non esiste nessun campo dati della classe Antenna con questo nome ");
            err.append(dbFld);
            err.append("\n");
            debugAntenne.append(err);
            return 0;
        }
    /*  
    }
    catch(string err){
        debugAntenne.append(err);
        debugAntenne.append("\n");
        return 1;      
    }
    */  
}

char * antenna::getDA(char * dbFld){

    /*Confronta il valore in input con il nome dei campi dati e, se esiste una corrispondenza, 
    ritorna il valore contenuto nel campo dato*/

    if (!strcmp(dbFld,"IDANT")) {
        sprintf(strIdAnt, "%d", idAnt);
        return strIdAnt;
    }
    else if (!strcmp(dbFld,"ID_PADRE")) {
        sprintf(strIdPadre, "%d", idPadre);
        return strIdPadre;
    }
    else if (!strcmp(dbFld,"X_ANT")) {
        sprintf(strX, "%.2lf", x);
        return strX;
    }
    else if (!strcmp(dbFld,"Y_ANT")) {
        sprintf(strY, "%.2lf", y);
        return strY;
    }
    else if (!strcmp(dbFld,"H")) {
        sprintf(strH, "%.2lf", h);
        return strH;
    }
    else if (!strcmp(dbFld,"N_TX")) {
        sprintf(strNumTx, "%d", numTx);
        return strNumTx;
    }
    else if (!strcmp(dbFld,"POT_TX")) {
        sprintf(strPotTx, "%.3lf", potTx);
        return strPotTx;
    }
    else if (!strcmp(dbFld,"ATT_PC")) {
        sprintf(strAttPC, "%lf", attPC);
        return strAttPC;
    }
    else if (!strcmp(dbFld,"ATT_DTX")) {
        sprintf(strAttDTX, "%lf", attDTX);
        return strAttDTX;
    }
    else if (!strcmp(dbFld,"TILT")) {
        sprintf(strTilt, "%.2lf", tilt);
        return strTilt;
    }
    else if (!strcmp(dbFld,"DIREZIONE")) {
        sprintf(strDirezione, "%.2lf", direzione);
        return strDirezione;
    }
    else if (!strcmp(dbFld,"ATTIVA")) {
        if (attiva)
            strcpy(strAttiva,"True");
        else
            strcpy(strAttiva,"False");
        return strAttiva;
    }
    else if (!strcmp(dbFld,"CHECKED")) {
        if (antCalc)
            strcpy(strAntCalc,"True");
        else
            strcpy(strAntCalc,"False");
        return strAntCalc;
    }
    else if (!strcmp(dbFld,"IDSITO")) {
        sprintf(strIdImpianto, "%d", idImpianto);
        return strIdImpianto;
    }
    else if (!strcmp(dbFld,"IDMOD")) {
        sprintf(strIdModello, "%d", idModello);
        return strIdModello;
    }
    else if (!strcmp(dbFld,"IDSERV")) {
        sprintf(strIdServizio, "%d", idServizio);
        return strIdServizio;
    }
    else if (!strcmp(dbFld,"IDPOLAR")) {
        sprintf(strIdPolar, "%d", idPolar);
        return strIdPolar;
    }
    else if (!strcmp(dbFld,"IDSTATO")) {
        sprintf(strIdStato, "%d", idStato);
        return strIdStato;
    }
    else if (!strcmp(dbFld,"FMIN")) {
        sprintf(strFreqMin, "%f", freqMin);
        return strFreqMin;
    }
    else if (!strcmp(dbFld,"FMAX")) {
        sprintf(strFreqMax, "%f", freqMax);
        return strFreqMax;
    }
    else if (!strcmp(dbFld,"FREQUENZA")) {
        sprintf(strFrequenza, "%f", frequenza);
        return strFrequenza;
    }
    else if (!strcmp(dbFld,"FASE")) {
        sprintf(strFase, "%.2f", fase);
        return strFase;
    }
    else if (!strcmp(dbFld,"PPERC")) {
        sprintf(strPotenzaPerc, "%.2f", potenzaPerc);
        return strPotenzaPerc;
    }
    else if (!strcmp(dbFld,"ALFA24")) {
        sprintf(strAlfa24, "%.2f", alfa24);
        return strAlfa24;
    }
    else if (!strcmp(dbFld,"DATA_INI"))
        return dataIniz;
    else if (!strcmp(dbFld,"DATA_FIN"))
        return dataFin;
    else if (!strcmp(dbFld,"NOTE1"))
        return note1;
    else if (!strcmp(dbFld,"IMMAGINE"))
        return immagine;
    else{ 
    // Non esiste alcun campo dato con il nome ricercato.
        return strVuota;
    }
}

void antenna::setModified(char *dbFld){
    string fieldName(dbFld);
    Modified.insert(fieldName);
}

void antenna::noModified(char *dbFld){
    string fieldName(dbFld);
    Modified.erase(fieldName);
}

bool antenna::isModified(char *dbFld){
    string fieldName(dbFld);
    if (Modified.find(fieldName) == Modified.end())
        return false;
    else
        return true;
}

int antenna::getIdAntenna(){
    return idAnt;
}
int antenna::getIdPadre(){
    return idPadre;
}
double antenna::getX(){
    return x;
}
double antenna::getY(){
    return y;
}
double antenna::getH(){
    return h;
}
int antenna::getIdModello(){
    return idModello;
}
int antenna::getNumTx(){
    return numTx;
}
double antenna::getPotenzaTx(){
    return potTx;
}
double antenna::getTilt(){
    return tilt;
}
double antenna::getDirezione(){
    return direzione;
}
bool antenna::getAttiva(){
    return attiva;
}
bool antenna::getAntCalc(){
    return antCalc;
}
int antenna::getIdImpianto(){
    return idImpianto;
}

int antenna::getIdServizio(){
    return idServizio;
}
int antenna::getIdPolar(){
    return idPolar;
}
int antenna::getIdStato(){
    return idStato;
}
double antenna::getFreqMin(){
    return freqMin;
}
double antenna::getFreqMax(){
    return freqMax;
}
double antenna::getFrequenza(){
    return frequenza;
}
double antenna::getFase(){
    return fase;
} 
double antenna::getAlfa24(){
    return alfa24;
}
double antenna::getAttPC(){
    return attPC;
}
double antenna::getAttDTX(){
    return attDTX;
}
double antenna::getAttTDD(){
    return attTDD;
}
double antenna::getPotenzaPerc(){
    return potenzaPerc;
}
char * antenna::getDataIn(){
    return dataIniz;
}
char * antenna::getDataFin(){
    return dataFin;
}
char * antenna::getNote1(){
    return note1;
}
char * antenna::getImmagine(){
    return immagine;
}
modello * antenna::getModello(){
    return aMod;
}

void antenna::setModello(modello * mod){
    aMod=mod;
}

/**
* Metodi get e set per le variabili di servizio per il calcolo del campo
*/
void antenna::setIdAnt(int id){
    idAnt = id;
}

void antenna::setPot(double pW){
    pot = pW;
}
double antenna::getPot(){
    return pot;
}
