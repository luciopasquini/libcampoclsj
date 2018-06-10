/** @file gruppo_impianti.cpp
  * @brief Metodi della classe gruppoImpianti
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
#include "gruppo_impianti.h"
#include "gruppo_modelli.h"
#include "antenna.h"
#include "get_md5_file.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <iostream>
using namespace rapidxml;

gruppoImpianti::gruppoImpianti() {
        calcolaAlfa24 = false;
        for(int i = 0;i<40;i++)
            md5FileImp[i] = 0;
        grpModelli = new gruppoModelli();
}
gruppoImpianti::gruppoImpianti(gruppoImpianti * gImp){
    calcolaAlfa24 = gImp->calcolaAlfa24;
    setMd5(gImp->getMd5());
    grpModelli = new gruppoModelli(gImp->grpModelli);
    for (vector<impianto*>::const_iterator it = gImp->impianti.begin();it != gImp->impianti.end();it++)
        addImpianto(new impianto(*it));
}
gruppoImpianti::~gruppoImpianti(){
    for (vector<impianto*>::const_iterator it = impianti.begin();it != impianti.end();it++)
        delete *it;
    delete grpModelli;
}

void gruppoImpianti::creaFileErrori(){ 
    ofstream fileErrori;
    fileErrori.open("C:/fileErrori.txt");
    if(fileErrori.is_open()) {
        fileErrori << antenna::debugAntenne;
        fileErrori << "\n";
        fileErrori << modello::debugModelli;
        fileErrori.close(); 
    }
}
void gruppoImpianti::azzeraStrErrori() {
    antenna::debugAntenne = "";
    modello::debugModelli = "";
}
char * gruppoImpianti::getStrErrori(){
    //string strErrori;
    
    string strErrori(antenna::debugAntenne + "\n" + modello::debugModelli);
    if(strErrori.size() < 100000)
        strcpy(strErr,strErrori.c_str());
    else
        strcpy(strErr,"Troppi errori!");
    return (char *) strErr;
    
}
void gruppoImpianti::deleteImpianti(){
    for(vector<impianto*>::const_iterator it=impianti.begin();it!=impianti.end();++it)
        delete *it;//cancello gli elementi puntati
    impianti.clear();//svuoto il vector
}

int gruppoImpianti::addImpianto(impianto * newImp){
    newImp->grImp = this;
    impianti.push_back(newImp);
    return impianti.size();
}

int gruppoImpianti::associaModelli() {
    antenna * impAnt;
    int numNoMod = 0;

    //per ciascun impianto presente in lista scorro tutte le antenne di cui è composto l'impianto.
    //Da ciascuna antenna ricavo l'ID che identifica il modello dell'antenna.
    //Con l'ID del modello dell'antenna il modello viene estratto e associato all'antenna.
    //Se non esiste nessun modello con l'ID cercato, all'antenna non viene associato nessun
    //modello e viene incrementato il contatore del numero di antenne senza modello associato.

    for (vector<impianto*>::iterator itrI = impianti.begin();itrI != impianti.end();itrI++) {
        vector<antenna*> antImp = (*itrI)->antenne;
        for (vector<antenna*>::iterator itrA = antImp.begin();itrA != antImp.end();itrA++) {
            impAnt = *itrA;
            modello * modAnt = grpModelli->getModById(impAnt->getIdModello());
            impAnt->setModello(modAnt);

            if (!impAnt->getModello())
                numNoMod++;
        }
    }
    return numNoMod;
}


void gruppoImpianti::beginIter() {
    itI = impianti.begin();
}

impianto * gruppoImpianti::nextItem() {
        impianto * outImp;
        if (itI == impianti.end()) 
            return NULL;
        else {
            outImp = *itI;
            itI++;
            return outImp;
        }
}

impianto * gruppoImpianti::getImpById(int id){
    for (vector<impianto*>::iterator it = impianti.begin();it != impianti.end();it++)
        if ((*it)->getIdImp() == id)
            return *it;
    return NULL;
}
int gruppoImpianti::removeImpById(int id){
    for (vector<impianto*>::iterator it = impianti.begin();it != impianti.end();it++)
        if ((*it)->getIdImp() == id) {
            delete *it;
            impianti.erase(it);
            return 0;
        }
    return 1;
}
antenna * gruppoImpianti::getAntById(int id) {
    for (vector<impianto*>::iterator itrI = impianti.begin();itrI != impianti.end();itrI++) {
        vector<antenna*> antImp = (*itrI)->antenne;
        for (vector<antenna*>::iterator itrA = antImp.begin();itrA != antImp.end();itrA++)
            if ((*itrA)->getIdAntenna() == id) 
                return *itrA;
    }
    return NULL;
}
int gruppoImpianti::removeAntById(int id) {
    
    for (vector<impianto*>::iterator itrI = impianti.begin();itrI != impianti.end();itrI++) {
        if (!(*itrI)->removeAntById(id))
            return 0;
    }
    return 1;
}
int gruppoImpianti::leggiModelliDiagrammiXML(char * nomeFile) {
    azzeraStrErrori();
    int presenzaErrori;//flag che indica se sono stati caricati dati inesatti
    
    grpModelli->deleteModelli();
    presenzaErrori = grpModelli->leggiModelliDiagrammiXML(nomeFile);
    associaModelli();
    return presenzaErrori;
}
int gruppoImpianti::scriviModelliDiagrammiXML(char * nomeFile) {
    int presenzaErrori;//flag che indica se sono stati caricati dati inesatti
    
    if (grpModelli)
        presenzaErrori = grpModelli->scriviModelliDiagrammiXML(nomeFile);
    else {
        presenzaErrori = 1;
        modello::debugModelli.append("Non esiste una classe di gruppoModelli");
    }
    return presenzaErrori;
}
int gruppoImpianti::leggiImpiantiAntenneXML(char * nomeFile) {
    azzeraStrErrori();
    antenna::debugAntenne.append("\n gruppoImpianti::leggiImpiantiAntenneXML: ");//da togliere
    antenna::debugAntenne.append(nomeFile);//da togliere
    
    int presenzaErrori=0;//flag che indica se sono stati caricati dati inesatti

    xml_node<> * siti_node;
    xml_node<> * property_node;
    xml_node<> * alfa24_node;
    impianto * newImp;
    antenna * newAnt;
    const char * chAlfa;
    const char * modify;

    fstream fin;
    fin.open(nomeFile, ios::in);
    if (fin.is_open()){
        fin.close();//perchè sotto viene aperto con la libreria rapidxml 

        file<> xmlFile(nomeFile);
        xml_document<> doc;
        doc.parse<0>(xmlFile.data());


        // Cancella eventuali impianti presenti e, con una chiamata 
        //implicita al distruttore ridefinito, le antenne collegate 
        deleteImpianti();


        // inizio estrazione del contenuto del file xml
        siti_node = doc.first_node("siti");
        alfa24_node = siti_node->first_node("alfa24");
        if (alfa24_node) {
            calcolaAlfa24 = false;
            chAlfa = (const char *)alfa24_node->first_attribute("value");
            if(!strncmp(chAlfa,"TRUE",4) || !strncmp(chAlfa,"True",4))
                calcolaAlfa24 = true;
        }
        for (xml_node<> * sito_node = siti_node->first_node("sito"); sito_node; sito_node = sito_node->next_sibling()) {
            newImp = new impianto();
            property_node = sito_node->first_node("property");
            if (property_node)
                for (xml_node<> * pr_node = property_node->first_node(); pr_node; pr_node = pr_node->next_sibling()) {

                    int rightValue=newImp->setDI(pr_node->name(),pr_node->first_attribute("value")->value());
                    if(rightValue==1){ // errore durante l'inserimento
                        string err("\n errore metodo setDI \n");
                        err.append(pr_node->name());
                        err.append(" ");
                        err.append(pr_node->first_attribute("value")->value());
                        antenna::debugAntenne.append(err);
                        presenzaErrori=1;
                    }

                    if(pr_node->first_attribute("mod")) {
                        modify = pr_node->first_attribute("mod")->value();
                        if(!strcmp(modify,"TRUE") || !strcmp(modify,"True"))
                            newImp->Modified.insert(pr_node->name());
                    }
                }
            for (xml_node<> * antenna_node = sito_node->first_node("antenna"); antenna_node; antenna_node = antenna_node->next_sibling()){
                newAnt = new antenna();
                for (xml_node<> * ar_node = antenna_node->first_node(); ar_node; ar_node = ar_node->next_sibling()) {
                    int rightValue=newAnt->setDA(ar_node->name(),ar_node->first_attribute("value")->value());
                    if(rightValue==1){ // errore durante l'inserimento
                        string err("\n errore metodo setDA \n");
                        err.append(ar_node->name());
                        err.append(" ");
                        err.append(ar_node->first_attribute("value")->value());
                        antenna::debugAntenne.append(err);
                        presenzaErrori=1;
                    }
                    if(ar_node->first_attribute("mod")) {
                        modify = ar_node->first_attribute("mod")->value();
                        if(!strcmp(modify,"TRUE") || !strcmp(modify,"True"))
                            newAnt->Modified.insert(ar_node->name());
                    }
                }
                newImp->addAntenna(newAnt);
            }
            addImpianto(newImp);
        }
        getMd5File(nomeFile,md5FileImp);
        antenna::debugAntenne.append("\n Terminato caricamento file xml");//da togliere
        if(presenzaErrori==1) {
            antenna::debugAntenne.append("\n Errore in leggiImpiantiAntenneXML( ");
            antenna::debugAntenne.append(nomeFile);
            antenna::debugAntenne.append( " ): \n" );
            creaFileErrori();
        }
       
        return presenzaErrori;
    }
    else //file non aperto
        return 2;


/** RapidXML, se il file è inesistente (o non riesce ad aprirlo) lancia un'eccezione di tipo run time
 *  QGis, quando  RapidXML lancia l'eccezione, crasha (nonostante qui sotto sia gestita correttamente) 
 *
 *    }
 *   catch (const std::runtime_error& e){
 *       antenna::debugAntenne.append("Runtime error was: ");
 *        antenna::debugAntenne.append(e.what());//e.what() contiene la descrizione dell'errore
 *       creaFileErrori();
 *       return 2;
 *   }
 *   catch(rapidxml::parse_error &e){
 *       antenna::debugAntenne.append("Parse error was: ");
 *       antenna::debugAntenne.append(e.what());//e.what() contiene la descrizione dell'errore
 *       creaFileErrori();
 *       return 2;
 *   }
 */  
}

int gruppoImpianti::scriviImpiantiAntenneXML(char * nomeFile) {

xml_attribute<> *attr;
impianto * Sito;
antenna * Antenna;
char* strDat; 
    
ofstream xmlFile(nomeFile);
//controllo che il file sia aperto
if(xmlFile.is_open()){

    
    xml_document<> doc;
    xml_node<> *root_node = doc.allocate_node(node_element, "siti");
    doc.append_node(root_node);
    xml_node<> *alfa24_node = doc.allocate_node(node_element, "alfa24");
    if(calcolaAlfa24)
        attr = doc.allocate_attribute("value","True");
    else
        attr = doc.allocate_attribute("value","False");
    alfa24_node->append_attribute(attr);
    root_node->append_node(alfa24_node);
    // Scrittura impianti
    for (vector<impianto*>::iterator it = impianti.begin();it != impianti.end();it++) {
        Sito = *it;
        xml_node<> *sito_node = doc.allocate_node(node_element, "sito");

        strDat = Sito->getDI((char*)"IDSITO");
        attr = doc.allocate_attribute("id",strDat);
        sito_node->append_attribute(attr);
        root_node->append_node(sito_node);

        // Scrittura dati impianto
        xml_node<> *prop_node = doc.allocate_node(node_element, "property");
        sito_node->append_node(prop_node);
        for (int j= 0; j < NUM_IMP_FLD;j++) {
            xml_node<> *x_node = doc.allocate_node(node_element, IMP_FLD[j]);
            prop_node->append_node(x_node);
            
            if (Sito->Modified.find(IMP_FLD[j]) == Sito->Modified.end())
                attr = doc.allocate_attribute("mod", "False");
            else
                attr = doc.allocate_attribute("mod", "True");
            x_node->append_attribute(attr);

            strDat = Sito->getDI((char*)IMP_FLD[j]);
            attr = doc.allocate_attribute("value", strDat);
            x_node->append_attribute(attr);

        }
        // Scrittura antenne
        for (vector<antenna*>::iterator itA = Sito->antenne.begin();itA != Sito->antenne.end();itA++) {
            Antenna = *itA;
            xml_node<> *antenna_node = doc.allocate_node(node_element, "antenna");

            strDat = Antenna->getDA((char*)"IDANT");
            attr = doc.allocate_attribute("id",strDat);
            antenna_node->append_attribute(attr);
            sito_node->append_node(antenna_node);

            // Scrittura dati antenna
            for (int j= 0; j < NUM_ANT_FLD;j++) {
                xml_node<> *a_node = doc.allocate_node(node_element, ANT_FLD[j]);
                antenna_node->append_node(a_node);

                if (Antenna->Modified.find(ANT_FLD[j]) == Antenna->Modified.end())
                    attr = doc.allocate_attribute("mod", "False");
                else
                    attr = doc.allocate_attribute("mod", "True");
                a_node->append_attribute(attr);

                strDat = Antenna->getDA((char*)ANT_FLD[j]);
                attr = doc.allocate_attribute("value", strDat);
                a_node->append_attribute(attr);              
            }
        }    
    }

    xmlFile <<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xmlFile << doc;
    xmlFile.close();
    getMd5File(nomeFile,md5FileImp);
    return 0;
}
else //non si può aprire il file
    return 1;
}
int gruppoImpianti::getMinNegAntID() {
    int minId,numAnt,numImp;
    impianto * imp;
    
    minId = 0;
    numImp = impianti.size();
    for(int j = 0; j < numImp; j++) {
        imp = impianti[j];
        numAnt = imp->antenne.size();
        for (int i = 0; i < numAnt; i++)
            if (imp->antenne[i]->getIdAntenna() < minId)
                minId = imp->antenne[i]->getIdAntenna();
    }
    return minId;
}
int gruppoImpianti::getMinNegImpID() {
    int minId,numImp;
    
    minId = 0;
    numImp = impianti.size();
    for(int j = 0; j < numImp; j++) {
            if (impianti[j]->getIdImp() < minId)
                minId = impianti[j]->getIdImp();
    }
    return minId;
}

int gruppoImpianti::setMd5(char* valDato) {
    if(strlen(valDato)< 40) {
        strcpy(md5FileImp,valDato);
         return 0;
    } 
    else
        return 1;
}
char * gruppoImpianti::getMd5(){
    return md5FileImp;
}
impianto * gruppoImpianti::newImpianto() {
    impianto * nuovoImpianto= new impianto();
    nuovoImpianto->setIdImp(getMinNegImpID()-1);
    addImpianto(nuovoImpianto);
    return nuovoImpianto;
}
modello * gruppoImpianti::newModello() {
    modello * nuovoModello= new modello();
    nuovoModello->setIdMod(grpModelli->getMinNegID()-1);
    grpModelli->addModello(nuovoModello);
    return nuovoModello;
}
modello * gruppoImpianti::getModById(int id){
    return grpModelli->getModById(id);
}
int gruppoImpianti::removeModById(int id) {
    return grpModelli->removeModById(id);
}
gruppoModelli * gruppoImpianti::getGruppoModelli() {
    return grpModelli;
}
int gruppoImpianti::somma(impianto * newImp){
    impianto * theNewImp;
    int minG,minA;
    
    if (newImp == NULL)
        return 1;
        
    theNewImp = new impianto(newImp);
    if (theNewImp->getIdImp() > 0) {
        if (getImpById(theNewImp->getIdImp())) {
            delete theNewImp;
            return 1;
        }
    }
    else if (theNewImp->getIdImp() < 0) {
        if (getImpById(theNewImp->getIdImp()))
            theNewImp->setIdImp(getMinNegImpID()-1);
    }
    else
        theNewImp->setIdImp(getMinNegImpID()-1);
    
    minG = getMinNegAntID();
    for (int i = 0; i < theNewImp->size();i++) {
        if (theNewImp->antenne[i]->getIdAntenna() > 0) {
            if (getAntById(theNewImp->antenne[i]->getIdAntenna())) {
                delete theNewImp;
                return 1;
            }
        }
        else if (theNewImp->antenne[i]->getIdAntenna()<0) {
            if (getAntById(theNewImp->antenne[i]->getIdAntenna())) {
                minA = theNewImp->getMinNegID();
                theNewImp->antenne[i]->setIdAnt(min(minG,minA)-1);
            }
        }
    }
    addImpianto(theNewImp);
    return 0;
}
int gruppoImpianti::somma(gruppoImpianti * newGrImp){
    int numErr;
    
    numErr = 0;
    if(newGrImp == NULL)
        return 1;
    for (vector<impianto*>::const_iterator it = newGrImp->impianti.begin();it != newGrImp->impianti.end();it++)
        numErr += somma(*it);
    return numErr;

}
int gruppoImpianti::setInfluenti(char * nomeFileShape){
    
    int pnEntities,pnShapeType;
    double padfMinBound[4], padfMaxBound[4]; 
    SHPObject * aInf;
    SHPObject * aCalc;
    
    SHPHandle fShpIn;
    fShpIn = SHPOpen(nomeFileShape,"rb");
    if (!fShpIn)
        return 1;
    SHPGetInfo(fShpIn,&pnEntities,&pnShapeType,padfMinBound,padfMaxBound);
    cout << "Numero di oggetti: " << pnEntities << "\n";
    aCalc = SHPReadObject(fShpIn, 0);
    aInf = SHPReadObject(fShpIn, 1);
    cout << "numero di vertici: " << aInf->nVertices << "\n";
    cout << "min max X: " << aCalc->dfXMin << " - " << aCalc->dfXMax << "\n";
    cout << "min max Y: " << aCalc->dfYMin << " - " << aCalc->dfYMax << "\n";
    SHPClose(fShpIn); 
    setInfluenti(aInf);
   
    return 0;
}
void gruppoImpianti::setInfluenti(SHPObject * aInf){
    int numSitiInf=0;
    for (vector<impianto*>::const_iterator it = impianti.begin();it != impianti.end();it++)
        if (pnpoly(aInf->nVertices,aInf->padfX,aInf->padfY,(*it)->getX(),(*it)->getY())){
            (*it)->setInfluente(true);
            numSitiInf++;
        }
        else
            (*it)->setInfluente(false);
    cout << "Numero siti influenti: " << numSitiInf << "\n";
}
int gruppoImpianti::pnpoly(int nvert, double *vertx, double *verty, double testx, double testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}
void gruppoImpianti::inizializzaCalcolo() {
    antenna * impAnt;
    if(!calcolaAlfa24){
        for (vector<impianto*>::iterator itrI = impianti.begin();itrI != impianti.end();itrI++) {
            vector<antenna*> antImp = (*itrI)->antenne;
            for (vector<antenna*>::iterator itrA = antImp.begin();itrA != antImp.end();itrA++) {
                impAnt = *itrA;
                impAnt-> setPot( (impAnt->getNumTx()) * (impAnt->getPotenzaTx()) );
            }
        }   
    }
    else{
        for (vector<impianto*>::iterator itrI = impianti.begin();itrI != impianti.end();itrI++) {
            vector<antenna*> antImp = (*itrI)->antenne;
            for (vector<antenna*>::iterator itrA = antImp.begin();itrA != antImp.end();itrA++) {
                impAnt = *itrA;
                double ppMedia = impAnt->getPotenzaTx(); // potenza per portante (media se ci sono alfaPC, DTX o TDD)
                double idServ = impAnt->getIdServizio();
                double ntx = impAnt->getNumTx();
                double pMax = ntx*ppMedia; // potenza massima (se non sono previste altre attenuazioni)
                double alfa[4]= {impAnt->getAttPC() , impAnt->getAttDTX() , impAnt->getAttTDD(),impAnt->getAlfa24() }; // alfaPC, alfaDTX, alfaTDD, alfa24
                for(int i=0;i<4;i++) if(alfa[i]==0) alfa[i]=1.0;
                if ((idServ==0 || idServ==2) && alfa[0]*alfa[1]<1.0) pMax = ntx*ntx*ppMedia/(1+alfa[0]*alfa[1]*(ntx-1)); // GSM o DCS
                if (idServ==25 && alfa[2]<1.0) pMax = ntx*ppMedia / alfa[2]; // LTE TDD
                //(*itrA)->
                impAnt->setPot( pMax*alfa[3]);
            }        
        }
    }
}
void gruppoImpianti::ordinaSorgenti(){
        for (vector<impianto*>::iterator it = impianti.begin();it != impianti.end();it++)
            (*it)->ordina();
}
gruppoImpianti * gruppoImpianti::getActiveImp(){
    gruppoImpianti * cpyImp;
   
    cpyImp = new gruppoImpianti(this);
   
    for (vector<impianto*>::iterator itI = cpyImp->impianti.begin();itI != cpyImp->impianti.end();) {
        if ((*itI)->getCalcoloImpianto() && (*itI)->getInfluente()) {
            vector<antenna*> antImp = (*itI)->antenne;
            for (vector<antenna*>::iterator itA = antImp.begin();itA != antImp.end();) {
                if ((*itA)->getAntCalc()) {
                    ++itA;
                }
                else {
                    delete *itA;
                    itA = antImp.erase(itA);
                }
            }    
            ++itI;
        }
        else  {
            delete *itI;
            itI = cpyImp->impianti.erase(itI);
        }
    }
    cpyImp->ordinaSorgenti();
    return cpyImp;
}
int gruppoImpianti::size(){
    return impianti.size();
}
