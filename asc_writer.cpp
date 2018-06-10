/** @file asc_writer.cpp
  * @brief Metodi della classe asc_writer.h per scrivere il file Arc/Info ASCII Grid
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

#include <string>
#include "asc_writer.h"

ascWriter::ascWriter(char * nomeFile,double x,double y,int nx,int ny,double dx,double dy) {
    columns = nx;
    std::string nomeF=nomeFile;
    nomeF += ".asc";
    
    //"w+b" apre il file come file binario in modalità scrittura 
    openFile = fopen(nomeF.c_str(), "w+b" );
    //openFile contiene il puntatore al file aperto
    if(openFile != NULL) {
        fprintf(openFile,"ncols %d\n",nx);
        fprintf(openFile,"nrows %d\n",ny);
        fprintf(openFile,"xllcorner %9.2f\n",x);
        fprintf(openFile,"yllcorner %9.2f\n",y);
        fprintf(openFile,"cellsize  %f\n",dx);
        fprintf(openFile,"NODATA_VALUE  -9999\n");
    }
}

void ascWriter::printRow(float * rowValue)
{
    for (int j = 0; j < columns;j++)
        fprintf(openFile,"%f ",*rowValue++);
    fprintf(openFile,"\n");      
}

void ascWriter::close()
{
    fclose(openFile);
}

