/** @file geotiff_writer.h
  * @brief Classe per scrivere il file geotiff
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

#ifndef GEOTIFF_WRITER_H
#define GEOTIFF_WRITER_H

#include <stdlib.h>
#include <string>
#include "xtiffio.h"  /* for TIFF */
#include "geotiffio.h" /* for GeoTIFF */

#define N(a) (sizeof(a) / sizeof (a[0]))
#define TIFFTAG_GDAL_METADATA     42112

class geotiffWriter{
    private:
        uint16 prjNum;              /**<  3003 Codice Proiezione Monte Mario fuso Ovest */
        std::string strMetaData;    /**<  Stringa di un metadato*/
        TIFF *tif=(TIFF*) NULL;     /**<  File TIFF */
        GTIF *gtif=(GTIF*) NULL;    /**<  File GTIF */

    public:

      /** 
      * Costruttore. Apre in modalità scrittura un file TIFF e imposta la struttura del file
      *
      * @param nomeFile Nome del file geotiff 
      * @param x Coordinata x dell'angolo in alto a sinistra
      * @param y Coordinata y dell'angolo in alto a sinistra
      * @param nx Numero di celle in x
      * @param ny Numero di celle in y
      * @param dx Dimensione della cella in x
      * @param dy Dimensione della cella in y
      */       
      geotiffWriter(char * nomeFile,double x,double y,int nx,int ny,double dx,double dy);

      /**
      * Aggiunge alla stringa strMetaData i valori presi in input
      *
      * @param nomeMetaData Nome del campo del metadato
      * @param valMetaData Valore del metadato
      */
      void addMetaData(char * nomeMetaData, char * valMetaData);// aggiunge un metadato al file
        
      /**
      * Assegno ad un tag il contenuto della stringa strMetaData
      *
      * @return Ritorna <code>1</code> se se l'operazione ha successo, <code>0</code> altrimenti.
      */
      int writeMetaData();    

      /**
      * Scrive nella riga i del file TIFF i valori puntati da rowValue  
      *
      * @param i Numero della riga del file TIFF
      * @param rowValue Puntatore ai valori float da scrivere
      *
      * @return Ritorna <code>1</code> se la scrittura ha avuto successo, <code>-1</code> altrimenti
      */
      int printRow(int i,float * rowValue); 

      /**
      * Crea il file GTIF GeoTIFF e chiude il file puntato da openFile
      */
      void close(); // chiude e salva il file
};

#endif
