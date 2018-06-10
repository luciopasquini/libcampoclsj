/** @file asc_writer.h
  * @brief Classe per scrivere il file Arc/Info ASCII Grid
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

#ifndef ASC_WRITER_H
#define ASC_WRITER_H

#include <stdio.h>
#include <stdlib.h>

class ascWriter{
    private:
        FILE * openFile; /**< File su cui scrivere */
        int columns;     /**< Numero di valori per riga nel file stampati con il metodo printRow */

    public:
      /** 
      * Costruttore
      *
      * @param nomeFile Nome del file Arc/Info ASCII Grid 
      * @param x Coordinata x dell'angolo in alto a sinistra
      * @param y Coordinata y dell'angolo in alto a sinistra
      * @param nx Numero di celle in x
      * @param ny Numero di celle in y
      * @param dx Dimensione della cella in x
      * @param dy Dimensione della cella in y
      */
      ascWriter(char * nomeFile,double x,double y,int nx,int ny,double dx,double dy);

      /**
      * Aggiunge una riga di dati nel file puntato da openFile
      *
      * @param rowValue Puntatore ai valori float da aggiungere
      */
      void printRow(float * rowValue); 

      /**
      * Chiude il file puntato da openFile
      */
      void close(); 
};
#endif
