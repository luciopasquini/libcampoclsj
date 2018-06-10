/** @file modello.cpp
  * @brief Metodi della classe modello
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
#include <iostream> 
#include "modello.h"

string  modello::debugModelli = "Elenco eccezioni: \n";
//inizializzo campo dati statico


modello::modello(){
    idMod = 0;
    frequenza= 0;
    tilt = 0;
    guadagno = 0;
    lunghezza = 0;
    numOgradi=0;
    numVgradi=0;

    for (int j = 0;j<STR_FLD_MOD;j++) {
        nomeMod[j] = '\0'; 
        marca[j] = '\0';    
        note1[j] = '\0';
        immagine[j] = '\0';
    }
}
modello::modello(modello * oMod){
    
    if (oMod == NULL)
        return;
    // Copia delle variabili di classe
    idMod = oMod->idMod;
    frequenza = oMod->frequenza;
    tilt = oMod->tilt;
    guadagno = oMod->guadagno;
    lunghezza = oMod->lunghezza;
    numOgradi = oMod->numOgradi;
    numVgradi = oMod->numVgradi;
    
    //copia delle stringhe
    setNomeMod(oMod->getNomeMod());
    setMarca(oMod->getMarca());
    setNote1(oMod->getNote1());
    setImmagine(oMod->getImmagine());
    
    //Copia dei vettori
    int vSize;
    vSize = oMod->attO.size();
    for(int i = 0; i<vSize; i++)
        attO.push_back(oMod->attO[i]);
    vSize = oMod->grdO.size();
    for(int i = 0; i<vSize; i++) 
        grdO.push_back(oMod->grdO[i]);        
    vSize = oMod->attV.size();
    for(int i = 0; i<vSize; i++) 
        attV.push_back(oMod->attV[i]);
    vSize = oMod->grdV.size();
    for(int i = 0; i<vSize; i++) 
        grdV.push_back(oMod->grdV[i]);
}
int modello::setIdMod(char* valDato) {
    if(sscanf(valDato, "%d", &idMod) != 1) {
        idMod = 0;
        string err("Inserimento ID Modello fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        debugModelli.append(err);
        return 1;  
    }
     return 0; 
}
void modello::setIdMod(int id) {
    idMod = id;
}
int modello::setNomeMod(char* valDato) {
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        strcpy(nomeMod,"");
        return 0;
    }
    if(strlen(valDato)< STR_FLD_MOD) {
        strcpy(nomeMod,valDato);
        return 0;  
    } 
    else {
        nomeMod[0] = '\0';
        string err("Inserimento Nome Modello fallito, dimensione >= STR_FLD_MOD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        debugModelli.append(err);
        return 1;
    }
}

int modello::setMarca(char* valDato) {
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        strcpy(marca,"");
        return 0;
    }
    if(strlen(valDato)< STR_FLD_MOD) {
            strcpy(marca,valDato);
             return 0; 
    } 
    else{
        marca[0] = '\0';
        string err("Inserimento Marca Modello fallito, dimensione >= STR_FLD_MOD: ");
        err.append(valDato, 20);/*unisce alla stringa i primi 20 caratteri valore inserito*/
        err.append("\n");
        debugModelli.append(err);
        return 1;
    }
}
int modello::setFrequenza(char* valDato) {
    if(sscanf(valDato, "%lf", &frequenza) != 1) {
        frequenza = 0;
        string err("Inserimento Frequenza Modello fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugModelli.append(err);
        return 1;
    }
     return 0; 
}

int modello::setTilt(char* valDato) {
    if(sscanf(valDato, "%lf", &tilt) != 1) {
        tilt=0;
        string err("Inserimento Tilt elettrico Modello fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugModelli.append(err);
        return 1;
    }
     return 0;
}
int modello::setGuadagno(char* valDato) {
    if(sscanf(valDato, "%lf", &guadagno) != 1) {
        guadagno = 0;
        string err("Inserimento Guadagno Modello fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugModelli.append(err);
        return 1;
    }
     return 0; 
}
int modello::setLunghezza(char* valDato) {
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        lunghezza = 0;
        return 0;
    }
    if(sscanf(valDato, "%lf", &lunghezza) != 1) {
        lunghezza = 0;
        string err("Inserimento Lunghezza Modello fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        debugModelli.append(err);
        return 1;
    }
     return 0; 
}
int modello::setNote1(char* valDato) {
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        strcpy(note1,"");
        return 0;
    }
    if(strlen(valDato)< STR_FLD_MOD) {
        strcpy(note1,valDato);
         return 0; 
    } 
    else{
        note1[0] = '\0';
        string err("Inserimento Note1 Modello fallito, dimensione >= STR_FLD_MOD: ");
        err.append(valDato, 20);/*unisce alla stringa i primi 20 caratteri valore inserito*/
        err.append("\n");
        debugModelli.append(err);
        return 1;
    }
}
int modello::setImmagine(char* valDato) {
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)) {
        strcpy(immagine,"");
        return 0;
    }
    if(strlen(valDato)< STR_FLD_MOD) {
        strcpy(immagine,valDato);
         return 0;
    } 
    else{
        immagine[0] = '\0';
        string err("Inserimento Immagine Modello fallito, dimensione >= STR_FLD_MOD: ");
        err.append(valDato, 20);/*unisce alla stringa i primi 20 caratteri valore inserito*/
        err.append("\n");
        debugModelli.append(err);
        return 1;
    } 
}

int modello::setDM(char * dbFld, char * valDato){
    //try{
        if (!strcmp(dbFld,"IDMOD")) 
            return setIdMod(valDato);
        if (!strcmp(dbFld,"MODELLO")) 
             return setNomeMod(valDato);
        if (!strcmp(dbFld,"MARCA")) 
            return setMarca(valDato);
        if (!strcmp(dbFld,"FREQUENZA")) 
            return setFrequenza(valDato);
        if (!strcmp(dbFld,"TILT_EL")) 
            return setTilt(valDato);
        if (!strcmp(dbFld,"GUADAGNO")) 
            return setGuadagno(valDato);
        if (!strcmp(dbFld,"LUNGHEZZA")) 
            return setLunghezza(valDato);
        if (!strcmp(dbFld,"NOTE1")) 
            return setNote1(valDato);
        if (!strcmp(dbFld,"IMMAGINE")) 
            return setImmagine(valDato);
        else{
            string err("Non esiste nessun campo dati della classe Modello con questo nome");
            err.append(dbFld);
            err.append("\n");
            debugModelli.append(err);
            return 0;
        }
    /*
    }
    catch(string err){
        debugModelli.append(err);
        debugModelli.append("\n");
        return 1;      
    }  
    */      
}

char * modello::getDM(char * dbFld) {
    
    /*Confronta il valore in input con il nome dei campi dati e, se esiste una corrispondenza, 
    ritorna il valore contenuto nel campo dato*/
    if (!strcmp(dbFld,"IDMOD")){
        sprintf(strIdMod, "%d", idMod);
        return strIdMod;
        }
    else if (!strcmp(dbFld,"MODELLO"))
        return nomeMod;
    else if (!strcmp(dbFld,"MARCA"))
        return marca;
    else if (!strcmp(dbFld,"FREQUENZA")){
        sprintf(strFrequenza, "%f", frequenza);
        return strFrequenza;
        }
    else if (!strcmp(dbFld,"TILT_EL")){
        sprintf(strTilt, "%lf", tilt);
        return strTilt;
        }
    else if (!strcmp(dbFld,"GUADAGNO")){
        sprintf(strGuadagno, "%lf", guadagno);
        return strGuadagno;
        }
    else if (!strcmp(dbFld,"LUNGHEZZA")){
        sprintf(strLunghezza, "%lf", lunghezza);
        return strLunghezza;
        }
    else if (!strcmp(dbFld,"NOTE1"))
        return note1;
    else if (!strcmp(dbFld,"IMMAGINE"))
        return immagine;
    else{ 
        // Non esiste alcun campo dato con il nome ricercato.
        return strVuota;
    }
}

int modello::getIdModello(){
    return idMod;
}

double modello::getGuadagno(){
    return guadagno;
}

double modello::getLunghezza(){
    return lunghezza;
}
double modello::getFrequenza(){
    return frequenza;
}
char * modello::getNomeMod(){
    return nomeMod;
}
char * modello::getMarca(){
    return marca;
}
char * modello::getNote1(){
    return note1;
}
char * modello::getImmagine(){
    return immagine;
}
int modello::getSizeAttO(){
    return attO.size();
}

int modello::getSizeGrdO(){
    return grdO.size();
}

int modello::getSizeAttV(){
    return attV.size();
}

int modello::getSizeGrdV(){
    return grdV.size();
}

double modello::getAttO(unsigned int i){
    if(i<attO.size())
        return attO[i];
    else
        return 0;
}
double modello::getGrdO(unsigned int i){
    if(i<grdO.size())
        return grdO[i];
    else
        return 0;
}
double modello::getAttV(unsigned int i){
    if(i<attV.size())
        return attV[i];
    else
        return 0;
}
double modello::getGrdV(unsigned int i){
    if(i<grdO.size())
        return grdO[i];
    else
        return 0;    
}

void modello::setAttO(unsigned int i, double valDato){
    if(i<attO.size())
        attO[i]=valDato;
}

void modello::setGrdO(unsigned int i, double valDato){
    if(i<grdO.size())
        grdO[i]=valDato;
}
void modello::setAttV(unsigned int i, double valDato){
    if(i<attV.size())
        attV[i]=valDato;
}
void modello::setGrdV(unsigned int i, double valDato){
    if(i<grdV.size())
        grdV[i]=valDato;
}

void modello::addAttO(double valDato){
    attO.push_back(valDato);
}
void modello::addGrdO(double valDato){
    grdO.push_back(valDato);
}
void modello::addAttV(double valDato){
    attV.push_back(valDato);
}
void modello::addGrdV(double valDato){
    grdV.push_back(valDato);
}

