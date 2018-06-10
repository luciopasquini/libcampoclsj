/** @file gruppo_modelli.cpp
  * @brief Metodi della classe gruppoModelli
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
#include "gruppo_modelli.h"
#include "rapidxml.hpp" 
#include "rapidxml_utils.hpp" 
#include "rapidxml_print.hpp" 
#include "get_md5_file.h" 

using namespace rapidxml;


gruppoModelli::gruppoModelli(){
    for(int i = 0;i<40;i++)
        md5FileMod[i] = 0;
}
gruppoModelli::gruppoModelli(gruppoModelli * gMod){
    if (gMod == NULL)
        return;
    for (vector<modello*>::const_iterator it = gMod->modelli.begin();it != gMod->modelli.end();it++)
        addModello(new modello(*it));
}
gruppoModelli::~gruppoModelli(){
    for (vector<modello*>::const_iterator it = modelli.begin();it != modelli.end();it++)
        delete *it;        
}
int gruppoModelli::getMinNegID() {
    int minId,numMod;
    
    minId = 0;
    numMod = modelli.size();
    for (int i = 0; i < numMod; i++)
        if (modelli[i]->getIdModello() < minId)
            minId = modelli[i]->getIdModello();
    return minId;
}

int gruppoModelli::somma(modello * newMod){
    if (newMod == NULL)
        return 1;
        
    if (newMod->getIdModello() > 0) {
        if (getModById(newMod->getIdModello()))
            return 1;
    }
    else if (newMod->getIdModello() < 0){
        if (getModById(newMod->getIdModello()))
            newMod->setIdMod(getMinNegID()-1);
    }
    else
        newMod->setIdMod(getMinNegID()-1);
    addModello(new modello(newMod));
    return 0;
}

int gruppoModelli::somma(gruppoModelli * aGM){
    int numErr;
    
    numErr = 0;
    if(aGM == NULL)
        return 1;
    for (vector<modello*>::const_iterator it = aGM->modelli.begin();it != aGM->modelli.end();it++)
        numErr += somma(*it);
    return numErr;
}

void gruppoModelli::deleteModelli()
{
    for(vector<modello*>::const_iterator it=modelli.begin();it!=modelli.end();++it)
        delete *it;//cancello gli elementi puntati
    modelli.clear();//svuoto il vector
}

int gruppoModelli::size(){
    return modelli.size();
}

int gruppoModelli::addModello(modello * newMod){
    modelli.push_back(newMod);
    return modelli.size();
}

modello * gruppoModelli::getModById(int id){
    for (vector<modello*>::iterator it = modelli.begin();it != modelli.end();it++)
        if ((*it)->getIdModello() == id)
            return *it;
    return NULL;    
}
int gruppoModelli::removeModById(int id){
    for (vector<modello*>::iterator it = modelli.begin();it != modelli.end();it++)
        if ((*it)->getIdModello() == id) {
            delete *it;
            modelli.erase(it);
            return 0;
        }
    return 1;    
}
void gruppoModelli::beginIter(){
    itM = modelli.begin();
}

modello * gruppoModelli::nextItem(){
        modello * outMod;
        if (itM == modelli.end()) 
            return NULL;
        else {
            outMod = *itM;
            itM++;
            return outMod;
        }
}


int gruppoModelli::leggiModelliDiagrammiXML(char * nomeFile){
    modello::debugModelli.append("\n gruppoModelli::leggiModelliDiagrammiXML: ");//da togliere
    modello::debugModelli.append(nomeFile);//da togliere

    int presenzaErrori=0;//flag che indica se sono stati caricati dati inesatti

    xml_node<> * modelli_node;
    xml_node<> * property_node;
    modello * newMod;
    char * strGrado;
    char * strAttV;
    char * strAttH;

    fstream fin;
    fin.open(nomeFile, ios::in);
    if (fin.is_open()){
        fin.close();//perchè sotto viene aperto con la libreria rapidxml 

        file<> xmlFile(nomeFile); 
        xml_document<> doc; 
        doc.parse<0>(xmlFile.data());

        deleteModelli();//distruzione profonda vector modelli

        modelli_node = doc.first_node("modelli");

        for (xml_node<> * modello_node = modelli_node->first_node("modello"); modello_node; modello_node = modello_node->next_sibling()) {
            newMod = new modello();
            property_node = modello_node->first_node("property");
            if (property_node)
                for (xml_node<> * pr_node = property_node->first_node(); pr_node; pr_node = pr_node->next_sibling()) {
                    int rightValue= newMod->setDM(pr_node->name(),pr_node->first_attribute("value")->value());
                    if(rightValue==1){ // errore durante l'inserimento

                        string err("\n errore metodo setDM ");
                        err.append(pr_node->name());
                        err.append(" ");
                        err.append(pr_node->first_attribute("value")->value());
                        modello::debugModelli.append(err);
                        presenzaErrori=1;
                    }
                }
            int j = 0;
            for (xml_node<> * dia_node = modello_node->first_node("diagram"); dia_node; dia_node = dia_node->next_sibling()){
                for (xml_node<> * ar_node = dia_node->first_node(); ar_node; ar_node = ar_node->next_sibling()) {
                    if (j<360) {
                        strGrado = ar_node->first_attribute("grad")->value();
                        strAttV = ar_node->first_attribute("att_v")->value();  
                        strAttH = ar_node->first_attribute("att_o")->value();
                        double grad = 0;
                        double dAtt_o = 0.;
                        double dAtt_v = 0.;
                        sscanf(strGrado, "%lf", &grad);
                        sscanf(strAttV, "%lf", &dAtt_v);
                        sscanf(strAttH, "%lf", &dAtt_o);
                        newMod->addGrdV(grad);
                        newMod->addGrdO(grad);
                        newMod->addAttV(dAtt_v);
                        newMod->addAttO(dAtt_o);
                        j++;
                    }
                    newMod->numOgradi = j;
                    newMod->numVgradi = j;
                }
            }
            addModello(newMod);
        }
        getMd5File(nomeFile,md5FileMod);
        modello::debugModelli.append("\n Terminato caricamento file xml");//da togliere
        if(presenzaErrori==1){
            modello::debugModelli.append("\n Errore in leggiModelliDiagrammiXML( ");
            modello::debugModelli.append(nomeFile);
            modello::debugModelli.append( " ): \n" );
        }

        return presenzaErrori;
    }
    else //file non aperto
        return 2;       
    /*
    catch(string e) {
        modello::debugModelli.append("\n Errore in leggiModelliDiagrammiXML( ");
        modello::debugModelli.append(nomeFile);
        modello::debugModelli.append( " ): \n");

        deleteModelli();//svuoto il vettore modelli
        return 1;
    }
    */
}

int gruppoModelli::scriviModelliDiagrammiXML(char * nomeFile){

xml_attribute<> *attr;
modello * Modello;
char* strDat; 

ofstream xmlFile(nomeFile);
//controllo che il file sia aperto
if(xmlFile.is_open()){
    vector<char *> va;
	xml_document<> doc;
	xml_node<> *root_node = doc.allocate_node(node_element, "modelli");
	doc.append_node(root_node);

	 // Scrittura modelli
	for (vector<modello*>::iterator it = modelli.begin();it != modelli.end();it++) {
		Modello = *it;
		xml_node<> *modello_node = doc.allocate_node(node_element, "modello");
		strDat = Modello->getDM((char*)"IDMOD");
		attr = doc.allocate_attribute("id",strDat);
		modello_node->append_attribute(attr);
		root_node->append_node(modello_node);

	    // Scrittura dati modello
		xml_node<> *prop_node = doc.allocate_node(node_element, "property");
		modello_node->append_node(prop_node);
		for (int j= 0; j < NUM_MOD_FLD;j++) {
			xml_node<> *x_node = doc.allocate_node(node_element, MOD_FLD[j]);
			prop_node->append_node(x_node);

			strDat = Modello->getDM((char*)MOD_FLD[j]);
			attr = doc.allocate_attribute("value", strDat);
			x_node->append_attribute(attr);

		}
	    // Scrittura diagramma
		xml_node<> *diag_node = doc.allocate_node(node_element, "diagram");
		modello_node->append_node(diag_node);
		if ( Modello->getSizeAttO() == Modello->getSizeGrdO() &&
			Modello->getSizeAttV() == Modello->getSizeGrdV() &&
			Modello->getSizeGrdO() == Modello->getSizeGrdV()    ) {
			int numGrd = Modello->getSizeGrdO();
			for(int j = 0; j < numGrd; j++) {
				xml_node<> *att_node = doc.allocate_node(node_element, "ATT");
				diag_node->append_node(att_node);


				strDat = (char*) malloc(50);
                va.push_back(strDat);
                sprintf(strDat, "%lf", Modello->getAttO(j));
				attr = doc.allocate_attribute("att_o",strDat);
				att_node->append_attribute(attr);
                

				strDat = (char*) malloc(50);
				sprintf(strDat, "%lf", Modello->getAttV(j));
				attr = doc.allocate_attribute("att_v",strDat);
				att_node->append_attribute(attr);


				strDat = (char*) malloc(50);
                va.push_back(strDat);
				sprintf(strDat, "%lf", Modello->getGrdO(j));
				attr = doc.allocate_attribute("grad",strDat);
				att_node->append_attribute(attr);
			}
		}    
	}
	xmlFile <<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	xmlFile << doc;
	xmlFile.close();
    for (unsigned int i = 0; i < va.size();i++)
        free(va[i]);
    va.clear();
	getMd5File(nomeFile,md5FileMod);
	return 0;
}
else //non si può aprire il file
    return 1;
}
