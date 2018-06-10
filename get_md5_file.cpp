/** @file get_md5_file.cpp
  * @brief Funzione per calcolare l'MD5 del contenuto di un file
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

#include <openssl/md5.h>
#include <fstream>
#include <string.h>
#include <malloc.h>
#include "get_md5_file.h"

using namespace std;

void getMd5File(char * nomeFile,char * strMd5) {
    
    unsigned char *result =(unsigned char *) malloc(MD5_DIGEST_LENGTH);
    strMd5[0] = 0;
    char nB[10];
    ifstream file(nomeFile, ifstream::binary);
    MD5_CTX md5Context;
    MD5_Init(&md5Context);
    char buf[1024 * 16];
    while (file.good()) {
        file.read(buf, sizeof(buf));
        MD5_Update(&md5Context, buf, file.gcount());
    }
    MD5_Final(result, &md5Context);
    
    for(int i=0; i <MD5_DIGEST_LENGTH; i++) {
        sprintf(nB,"%02x",result[i]);
        strcat(strMd5,nB);
    }
    strMd5[32] = 0;
    file.close();
    free(result);
}

