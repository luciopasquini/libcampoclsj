/** @file gruppo_antenne.cpp
  * @brief Metodi della classe gruppoAntenne
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
//#include "gruppo_antenne.h"
#include "gruppo_impianti.h"
 #include <algorithm> 
gruppoAntenne::gruppoAntenne(){
    grImp = NULL;
}
gruppoAntenne::~gruppoAntenne(){
    for (vector<antenna*>::const_iterator it= antenne.begin();it!= antenne.end();it++)
        delete *it;
}
gruppoAntenne::gruppoAntenne(gruppoAntenne * gAnt){
    if (gAnt == NULL){
        grImp = NULL;
        return;
    }
    for (vector<antenna*>::const_iterator it = gAnt->antenne.begin();it != gAnt->antenne.end();it++)
        addAntenna(new antenna(*it));
}

int gruppoAntenne::getMinNegID() {
    int minId,numAnt;
        minId = 0;
        
    if (grImp)
        return (((gruppoImpianti*)grImp)->getMinNegAntID());

    numAnt = antenne.size();
    for (int i = 0; i < numAnt; i++)
        if (antenne[i]->getIdAntenna() < minId)
            minId = antenne[i]->getIdAntenna();
    return minId;
}
int gruppoAntenne::somma(antenna * newAnt){
    if (newAnt == NULL)
        return 1;
    if (newAnt->getIdAntenna() > 0) {
        if (getAntById(newAnt->getIdAntenna()))
            return 1;
    }
    else if (newAnt->getIdAntenna() < 0) {
        if (getAntById(newAnt->getIdAntenna()))
            newAnt->setIdAnt(getMinNegID()-1);
    }
    else
        newAnt->setIdAnt(getMinNegID()-1);

    addAntenna(new antenna(newAnt));
    return 0;

}

int gruppoAntenne::somma(gruppoAntenne * aGA){
    int numErr;
    
    numErr = 0;
    if(aGA == NULL)
        return 1;
    for (vector<antenna*>::const_iterator it = aGA->antenne.begin();it != aGA->antenne.end();it++)
        numErr += somma(*it);
    return numErr;
}

void gruppoAntenne::deleteAntenne()
{
    for(vector<antenna*>::const_iterator it=antenne.begin();it!=antenne.end();++it)
        delete *it;//cancello gli elementi puntati
    antenne.clear();//svuoto il vector
}

int gruppoAntenne::size(){
    return antenne.size();
}

int gruppoAntenne::addAntenna(antenna * newAnt){
    antenne.push_back(newAnt);
    return antenne.size();
}
antenna * gruppoAntenne::getAntById(int id) {
    for (vector<antenna*>::iterator itrA = antenne.begin();itrA != antenne.end();itrA++)
        if ((*itrA)->getIdAntenna() == id)
            return *itrA;
    return NULL;
}
int gruppoAntenne::removeAntById(int id) {
    for (vector<antenna*>::iterator itrA = antenne.begin();itrA != antenne.end();itrA++)
        if ((*itrA)->getIdAntenna() == id) {
            delete *itrA;
            antenne.erase(itrA);
            return 0;
        }
    return 1;
}
void gruppoAntenne::beginIter() {
    itA = antenne.begin();
}

antenna *  gruppoAntenne::nextItem(){
    antenna * outAnt;
    if (itA == antenne.end()) 
        return NULL;
    else {
        outAnt = *itA;
        itA++;
        return outAnt;
    }
}

antenna * gruppoAntenne::newAntenna() {
    antenna * nuovaAntenna = new antenna();
    nuovaAntenna->setIdAnt(getMinNegID()-1);
    addAntenna(nuovaAntenna);
    return nuovaAntenna;
}
void gruppoAntenne::ordina() {
    sort(antenne.begin(),antenne.end(),ordinaAntenne());
}



