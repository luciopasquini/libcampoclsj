/** @file impianto.cpp
  * @brief Metodi della classe impianto
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

#include <string.h>
#include <cstdio>
#include <iostream>
#include "impianto.h"

impianto::impianto()
{
    grImp = NULL;
    x = 0;
    y = 0;
    z = 0;
    idImp = 0;

    idGestore = 0;
    idProv = 0;
    idComune = 0;
    idStato= 0;  

    nome[0] = '\0';
    codImp[0] = '\0';
    indirizzo[0] = '\0';
    respImp[0] = '\0';
    note1[0] = '\0';
    note2[0] = '\0';
    immagine[0] = '\0';
    creazione[0] = '\0';
    dataModifica[0] = '\0';
    parere[0] = '\0';
    comunicazione[0] = '\0';
    dataDismissione[0] = '\0';    
    
    strVuota[0] = '\0';
    
    idResp = 0;
    idPadre = 0;
    idPonte = 0;
    idPost = 0;

    comDir = false;
    attEdif = false;
    ricDiretta = false;
    calcoloImpianto = true;
    impiantoAttivo = true;
    influente = true;
}
impianto::impianto(impianto * oImp):gruppoAntenne(oImp) {
    
    if (oImp == NULL)
        return;
    // Copia delle variabili di classe
    //grImp = oImp->grImp;
    x = oImp->x;
    y = oImp->y;
    z = oImp->z;
    idImp = oImp->idImp; 
    idGestore = oImp->idGestore;
    idProv = oImp->idProv;
    idComune = oImp->idComune;
    idStato = oImp->idStato;

    idResp = oImp->idResp;
    idPadre = oImp->idPadre;
    idPonte = oImp->idPonte;
    idPost = oImp->idPost;

    comDir = oImp->comDir;
    attEdif = oImp->attEdif;
    ricDiretta = oImp->ricDiretta;
    calcoloImpianto = oImp->calcoloImpianto;
    impiantoAttivo = oImp->impiantoAttivo;
    influente = oImp->influente;
    
    Modified = oImp->Modified;
    
    //copia delle stringhe
    setNome(oImp->getNome());
    setCodImp(oImp->getCodImp());
    setIndirizzo(oImp->getIndirizzo());
    setRespImp(oImp->getRespImp());
    setNote1(oImp->getNote1());
    setNote2(oImp->getNote2());
    setImmagine(oImp->getImmagine());
    setCreazione(oImp->getCreazione());
    setDataModifica(oImp->getDataModifica());
    setParere(oImp->getParere());
    setComunicazione(oImp->getComunicazione());
    setDataDismissione(oImp->getDataDismissione());

}
void impianto::setIdImp(int id) {
    idImp = id;
}
int impianto::setIdImp(char* valDato){
    //sscanf(variabileDestinazione, formato dato da copiare, valore da copiare)
    // se la copia è riuscita ritorna 1
    if(sscanf(valDato, "%d", &idImp) != 1) {
        idImp = 0;
        string err("Inserimento ID Impianto (Impianto) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err);        
        return 1;   
    }
    return 0;
}

int impianto::setNome(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(nome,"");
        else
            strcpy(nome,valDato);
        return 0; 
    }
    else {
        nome[0] = '\0';
        string err("Inserimento Nome Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}

int impianto::setCodImp(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(codImp,"");
        else
            strcpy(codImp,valDato);
        return 0; 
    }
    else {
        codImp[0] = '\0';
        string err("Inserimento Codice Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setIndirizzo(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(indirizzo,"");
        else
            strcpy(indirizzo,valDato);
        return 0; 
    }
    else {
        indirizzo[0] = '\0';
        string err("Inserimento Indirizzo Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}

int impianto::setX(char* valDato){
    if(sscanf(valDato, "%lf", &x) != 1) {
        x = 0;
        string err("Inserimento coordinata X Impianto fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
    return 0; 
}
int impianto::setY(char* valDato){
    if(sscanf(valDato, "%lf", &y) != 1) {
        y = 0;
        string err("Inserimento coordinata Y Impianto fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
    return 0; 
}
int impianto::setZ(char* valDato){
    if(sscanf(valDato, "%lf", &z) != 1) {
        z = 0;
        string err("Inserimento coordinata Z Impianto fallito, valore non double: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
    return 0; 
}

int impianto::setRespImp(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(respImp,"");
        else
            strcpy(respImp,valDato);
        return 0; 
    }
    else {
        respImp[0] = '\0';
        string err("Inserimento Responsabile Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}

int impianto::setIdGestore(char* valDato){
    if(sscanf(valDato, "%d", &idGestore) != 1) {
        idGestore = 0;
        string err("Inserimento ID Gestore (Impianto) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;  
    }
    return 0; 
}
int impianto::setIdProv(char* valDato){
    if(sscanf(valDato, "%d", &idProv) != 1) {
        idProv = 0;
        string err("Inserimento ID Provincia (Impianto) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;  
    }
    return 0; 
}
int impianto::setIdComune(char* valDato){
    if(sscanf(valDato, "%d", &idComune) != 1) {
        idComune = 0;
        string err("Inserimento ID Provincia (Impianto) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;  
    }
    return 0; 
}
int impianto::setIdStato(char* valDato){
    if(sscanf(valDato, "%d", &idStato) != 1){
        idStato = 0;
        string err("Inserimento ID Stato (Impianto) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;  
    }
    return 0; 
}
int impianto::setNote1(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(note1,"");
        else
            strcpy(note1,valDato);
        return 0; 
    }
    else {
        note1[0] = '\0';
        string err("Inserimento Note1 Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}

int impianto::setNote2(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(note2,"");
        else
            strcpy(note2,valDato);
        return 0; 
    }
    else {
        note2[0] = '\0';
        string err("Inserimento Note2 Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setImmagine(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(immagine,"");
        else
            strcpy(immagine,valDato);
        return 0; 
    }
    else {
        immagine[0] = '\0';
        string err("Inserimento Immagine Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setIdResp(char* valDato){
    if(sscanf(valDato, "%d", &idResp) != 1) {
        idResp = 0;
        string err("Inserimento ID Responsabile (Impianto) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;  
    }
    return 0; 
}
int impianto::setIdPadre(char* valDato){
    if(sscanf(valDato, "%d", &idPadre) != 1) {
        idPadre = 0;
        string err("Inserimento ID Padre (Impianto) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
    return 0; 
}
int impianto::setCreazione(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(creazione,"");
        else
            strcpy(creazione,valDato);
        return 0; 
    }
    else {
        creazione[0] = '\0';
        string err("Inserimento Data creazione Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setDataModifica(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(dataModifica,"");
        else
            strcpy(dataModifica,valDato);
        return 0; 
    }
    else {
        dataModifica[0] = '\0';
        string err("Inserimento Data modifica Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setParere(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(parere,"");
        else
            strcpy(parere,valDato);
        return 0; 
    }
    else {
        parere[0] = '\0';
        string err("Inserimento Data parere Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setComunicazione(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4))
            strcpy(comunicazione,"");
        else
            strcpy(comunicazione,valDato);
        return 0; 
    }
    else {
        comunicazione[0] = '\0';
        string err("Inserimento Data comunicazione Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setDataDismissione(char* valDato){
    if(strlen(valDato)< STR_FLD) {
        if (!strcmp(dataDismissione,"NULL") || !strncmp(valDato,"None",4))
            strcpy(dataDismissione,"");
        else
            strcpy(dataDismissione,valDato);
        return 0; 
    }
    else {
        dataDismissione[0] = '\0';
        string err("Inserimento Data dismissione Impianto fallito, dimensione >= STR_FLD: ");
        err.append(valDato, 20);//unisce alla stringa i primi 20 caratteri del valore inserito
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}

int impianto::setIdPonte(char* valDato){
    if(sscanf(valDato, "%d", &idPonte) != 1) {
        idPonte = 0;
        string err("Inserimento ID Ponte (Impianto) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
    return 0; 
}
int impianto::setIdPost(char* valDato){
    if(sscanf(valDato, "%d", &idPost) != 1) {
        idPost = 0;
        string err("Inserimento ID Postazione (Impianto) fallito, valore non intero: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
    return 0; 
}

int impianto::setComDir(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)){
        comDir = false;
        return 0;
    }
    if(strcmp(valDato,"TRUE")==0 || strcmp(valDato,"True")==0 || strcmp(valDato,"SI")==0 || strcmp(valDato,"Si")==0 || strcmp(valDato,"si")==0) {
        comDir = true;
        return 0;
    }
    if (strcmp(valDato,"FALSE")==0 || strcmp(valDato,"False")==0 || strcmp(valDato,"NO")==0 || strcmp(valDato,"No")==0 || strcmp(valDato,"no")==0) {
        comDir = false;
        return 0;
    }
    else {
        comDir = false;
        string err("Inserimento flag comunicazione diretta (Impianto) fallito, valore non TRUE-True-FALSE-False-SI-NO-si-no-Si-No: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setAttEdif(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)){
        attEdif = false;
        return 0;
    }
    if(strcmp(valDato,"TRUE")==0 || strcmp(valDato,"True")==0 || strcmp(valDato,"SI")==0 || strcmp(valDato,"Si")==0 || strcmp(valDato,"si")==0) {
        attEdif = true;
        return 0;
    }
    if (strcmp(valDato,"FALSE")==0 || strcmp(valDato,"False")==0 || strcmp(valDato,"NO")==0 || strcmp(valDato,"No")==0 || strcmp(valDato,"no")==0) {
        attEdif = false;
        return 0;
    }
    else {
        attEdif = false;
        string err("Inserimento flag attenuazione edifici (Impianto) fallito, valore non TRUE-True-FALSE-False-SI-NO-si-no-Si-No: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setRicDiretta(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)){
        ricDiretta = false;
        return 0;
    }
    if(strcmp(valDato,"TRUE")==0 || strcmp(valDato,"True")==0 || strcmp(valDato,"SI")==0 || strcmp(valDato,"Si")==0 || strcmp(valDato,"si")==0) {
        ricDiretta = true;
        return 0;
    }
    if (strcmp(valDato,"FALSE")==0 || strcmp(valDato,"False")==0 || strcmp(valDato,"NO")==0 || strcmp(valDato,"No")==0 || strcmp(valDato,"no")==0) {
        ricDiretta = false;
        return 0;
    }
    else{
        ricDiretta = false;
        string err("Inserimento flag riconfigurazione (Impianto) fallito, valore non TRUE-True-FALSE-False-SI-NO-si-no-Si-No: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}

int impianto::setCalcoloImpianto(char* valDato){
    if(strcmp(valDato,"TRUE")==0 || strcmp(valDato,"True")==0 || strcmp(valDato,"SI")==0 || strcmp(valDato,"Si")==0 || strcmp(valDato,"si")==0) {
        calcoloImpianto = true;
        return 0;
    }
    if (strcmp(valDato,"FALSE")==0 || strcmp(valDato,"False")==0 || strcmp(valDato,"NO")==0 || strcmp(valDato,"No")==0 || strcmp(valDato,"no")==0) {
        calcoloImpianto = false;
        return 0;
    }
    else{
        calcoloImpianto = true;
        string err("Inserimento flag per il calcolo dell'impianto (Impianto) fallito, valore non TRUE-True-FALSE-False-SI-NO-si-no-Si-No: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setInfluente(char* valDato){
    if(strcmp(valDato,"TRUE")==0 || strcmp(valDato,"True")==0 || strcmp(valDato,"SI")==0 || strcmp(valDato,"Si")==0 || strcmp(valDato,"si")==0) {
        influente = true;
        return 0;
    }
    if (strcmp(valDato,"FALSE")==0 || strcmp(valDato,"False")==0 || strcmp(valDato,"NO")==0 || strcmp(valDato,"No")==0 || strcmp(valDato,"no")==0) {
        influente = false;
        return 0;
    }
    else{
        influente = true;
        string err("Inserimento flag di influenza dell'impianto (Impianto) fallito, valore non TRUE-True-FALSE-False-SI-NO-si-no-Si-No: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}
int impianto::setImpiantoAttivo(char* valDato){
    if (!strncmp(valDato,"NULL",4) || !strncmp(valDato,"None",4)){
        impiantoAttivo = false;
        return 0;
    }
    if(strcmp(valDato,"TRUE")==0 || strcmp(valDato,"True")==0 || strcmp(valDato,"SI")==0 || strcmp(valDato,"Si")==0 || strcmp(valDato,"si")==0) {
        impiantoAttivo = true;
        return 0;
    }
    if (strcmp(valDato,"FALSE")==0 || strcmp(valDato,"False")==0 || strcmp(valDato,"NO")==0 || strcmp(valDato,"No")==0 || strcmp(valDato,"no")==0) {
        impiantoAttivo = false;
        return 0;
    }
    else{
        impiantoAttivo = false;
        string err("Inserimento flag impianto attivo (Impianto) fallito, valore non TRUE-True-FALSE-False-SI-NO-si-no-Si-No: ");
        err.append(valDato);
        err.append("\n");
        antenna::debugAntenne.append(err); 
        return 1;
    }
}

int impianto::setDI(char * dbFld, char * valDato) {  
        if (!strcmp(dbFld,"IDSITO")) 
            return setIdImp(valDato);
        if (!strcmp(dbFld,"NOME")) 
            return setNome(valDato);
        if (!strcmp(dbFld,"CODSITO")) 
             return setCodImp(valDato);
        if (!strcmp(dbFld,"INDIRIZZO")) 
            return setIndirizzo(valDato);
        if (!strcmp(dbFld,"X_SITO")) 
            return setX(valDato);
        if (!strcmp(dbFld,"Y_SITO")) 
            return setY(valDato);
        if (!strcmp(dbFld,"Z_SITO")) 
             return setZ(valDato);
        if (!strcmp(dbFld,"RESPIMP")) 
            return setRespImp(valDato);
        if (!strcmp(dbFld,"IDGEST")) 
            return setIdGestore(valDato);
        if (!strcmp(dbFld,"IDPROV")) 
            return setIdProv(valDato);
        if (!strcmp(dbFld,"IDCOMUNE")) 
            return setIdComune(valDato);
        if (!strcmp(dbFld,"IDSTATO")) 
            return setIdStato(valDato);
        if (!strcmp(dbFld,"NOTE1"))
            return setNote1(valDato);
        if (!strcmp(dbFld,"NOTE2")) 
            return setNote2(valDato);
        if (!strcmp(dbFld,"IMMAGINE")) 
             return setImmagine(valDato);
        if (!strcmp(dbFld,"IDRESP")) 
            return setIdResp(valDato);
        if (!strcmp(dbFld,"ID_PADRE")) 
            return setIdPadre(valDato);
        if (!strcmp(dbFld,"CREAZIONE")) 
            return setCreazione(valDato);
        if (!strcmp(dbFld,"DATAMODIFY")) 
            return setDataModifica(valDato);
        if (!strcmp(dbFld,"PARERE")) 
            return setParere(valDato);
        if (!strcmp(dbFld,"COMUNICAZ")) 
            return setComunicazione(valDato);
        if (!strcmp(dbFld,"DISMISSION")) 
            return setDataDismissione(valDato);
        if (!strcmp(dbFld,"IDPONTE"))
            return setIdPonte(valDato);
        if (!strcmp(dbFld,"IDPOST"))
            return setIdPost(valDato); 
        if (!strcmp(dbFld,"COMDIR")) 
            return setComDir(valDato);
        if (!strcmp(dbFld,"ATTEDIF")) 
            return setAttEdif(valDato);
        if (!strcmp(dbFld,"RICDIRETTA")) 
            return setRicDiretta(valDato);
        if (!strcmp(dbFld,"CHECKED")) 
            return setCalcoloImpianto(valDato);
        if (!strcmp(dbFld,"ATTIVO")) 
            return setImpiantoAttivo(valDato);
        if (!strcmp(dbFld,"INFLUENTE"))
            return setInfluente(valDato);
        else{
            string err("Non esiste nessun campo dati della classe Impianto con questo nome: ");
            err.append(dbFld);
            err.append("\n");
            antenna::debugAntenne.append(err);
            return 0;
        }
    /*       
    }
    catch(string err){
        antenna::debugAntenne.append(err);
        antenna::debugAntenne.append("\n");
        return 1;      
    }   
    */  
}

char * impianto::getDI(char * dbFld)
{

    /*Confronta il valore in input con il nome dei campi dati e, se esiste una corrispondenza, 
    ritorna il valore contenuto nel campo dato*/

    if (!strcmp(dbFld,"IDSITO")) {
        sprintf(strIdImp, "%d", idImp);
        return strIdImp;
    }
    else if (!strcmp(dbFld,"NOME"))
        return nome;
    else if (!strcmp(dbFld,"CODSITO"))
        return codImp;
    else if (!strcmp(dbFld,"INDIRIZZO"))
        return indirizzo;
    else if (!strcmp(dbFld,"X_SITO")) {
        sprintf(strX, "%.2f", x);
        return strX;
    }
    else if (!strcmp(dbFld,"Y_SITO")) {
        sprintf(strY, "%.2f", y);
        return strY;
    }
    else if (!strcmp(dbFld,"Z_SITO")) {
        sprintf(strZ, "%.2f", z);
        return strZ;
    }
    else if (!strcmp(dbFld,"RESPIMP"))
        return respImp;
    else if (!strcmp(dbFld,"IDGEST")) {
        sprintf(strIdGestore, "%d", idGestore);
        return strIdGestore;
    }
    else if (!strcmp(dbFld,"IDPROV")) {
        sprintf(srtIdProv, "%d", idProv);
        return srtIdProv;
    }
    else if (!strcmp(dbFld,"IDCOMUNE")) {
        sprintf(strIdComune, "%d", idComune);
        return strIdComune;
    }
    else if (!strcmp(dbFld,"IDSTATO")) {
        sprintf(strIdStato, "%d", idStato);
        return strIdStato;
    }
    else if (!strcmp(dbFld,"NOTE1"))
        return note1;
    else if (!strcmp(dbFld,"NOTE2"))
        return note2;
    else if (!strcmp(dbFld,"IMMAGINE"))
        return immagine;
    else if (!strcmp(dbFld,"IDRESP")) {
        sprintf(strIdResp, "%d", idResp);
        return strIdResp;
    }
    else if (!strcmp(dbFld,"ID_PADRE")) {
        sprintf(strIdPadre, "%d", idPadre);
        return strIdPadre;
    }
    else if (!strcmp(dbFld,"CREAZIONE"))
        return creazione;
    else if (!strcmp(dbFld,"DATAMODIFY"))
        return dataModifica;
    else if (!strcmp(dbFld,"PARERE"))
        return parere;
    else if (!strcmp(dbFld,"COMUNICAZ"))
        return comunicazione;
    else if (!strcmp(dbFld,"DISMISSION"))
        return dataDismissione;
    else if (!strcmp(dbFld,"IDPONTE")) {
        sprintf(strIdPonte, "%d", idPonte);
        return strIdPonte;
    }
    else if (!strcmp(dbFld,"IDPOST")) {
        sprintf(strIdPost, "%d", idPost);
        return strIdPost;
    }
    else if (!strcmp(dbFld,"COMDIR")) {
        if (comDir)
            strcpy(strComDir,"True");
        else
            strcpy(strComDir,"False");
        return strComDir;
    }
    else if (!strcmp(dbFld,"ATTEDIF")) {
        if (attEdif)
            strcpy(strAttEdif,"True");
        else
            strcpy(strAttEdif,"False");
        return strAttEdif;
    }
    else if (!strcmp(dbFld,"RICDIRETTA")) {
        if (ricDiretta)
            strcpy(strRicDiretta,"True");
        else
            strcpy(strRicDiretta,"False");
        return strRicDiretta;
    }
     else if (!strcmp(dbFld,"CHECKED")) {
        if (calcoloImpianto)
            strcpy(strCalcoloImpianto,"True");
        else
            strcpy(strCalcoloImpianto,"False");
        return strCalcoloImpianto;
    }
    else if (!strcmp(dbFld,"ATTIVO")) {
        if (impiantoAttivo)
            strcpy(strImpiantoAttivo,"True");
        else
            strcpy(strImpiantoAttivo,"False");
        return strImpiantoAttivo;
    }
    else if (!strcmp(dbFld,"INFLUENTE")) {
        if (influente)
            strcpy(strInfluente,"True");
        else
            strcpy(strInfluente,"False");
        return strInfluente;
    }
    else{ 
    // Non esiste alcun campo dato con il nome ricercato.
        return strVuota;
    }

}

void impianto::setModified(char *dbFld){
    string fieldName(dbFld);
    Modified.insert(fieldName);
}

void impianto::noModified(char *dbFld){
    string fieldName(dbFld);
    Modified.erase(fieldName);
}

bool impianto::isModified(char *dbFld){
    string fieldName(dbFld);
    if (Modified.find(fieldName) == Modified.end())
        return false;
    else
        return true;
}

int impianto::getIdImp(){
    return idImp;
}

double impianto::getX(){
    return x;
}
double impianto::getY(){
    return y;
}
double impianto::getZ(){
    return z;
}
char * impianto::getNome(){
    return nome;
}
char * impianto::getCodImp(){
    return codImp;
}
char * impianto::getIndirizzo(){
    return indirizzo;
}
char * impianto::getRespImp(){
    return respImp;
}
char * impianto::getNote1(){
    return note1;
}
char * impianto::getNote2(){
    return note2;
}
char * impianto::getImmagine(){
    return immagine;
}
char * impianto::getCreazione(){
    return creazione;
}
char * impianto::getDataModifica(){
    return dataModifica;
}
char * impianto::getParere(){
    return parere;
}
char * impianto::getComunicazione(){
    return comunicazione;
}
char * impianto::getDataDismissione(){
    return dataDismissione;
}
bool impianto::getCalcoloImpianto() {
    return calcoloImpianto;
}
bool impianto::getInfluente() {
    return influente;
}
void impianto::setInfluente(bool isInflu){
    influente = isInflu;
}
