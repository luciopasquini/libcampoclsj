/** @file geotiff_writer.cpp
  * @brief Metodi della classe geotiff_writer.h per scrivere il file geotiff
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

#include "geotiff_writer.h"

geotiffWriter::geotiffWriter(char * nomeFile,double x,double y,int nx,int ny,double dx,double dy) {

    double tiepoints[6]={0,0,0,x,y,0.0};
    double pixscale[3]={dx,dy,0}; 
    prjNum = 3003;
    
    std::string strNomeFile = nomeFile;
    strNomeFile += ".tif";
    
    //tif contiene il file strNomeFile aperto in modalità scrittura
    tif=XTIFFOpen(strNomeFile.c_str(),"w");
    if (!tif)
        return;

    //Tag personalizzati
    const TIFFFieldInfo xtiffFieldInfo[] = {{
    TIFFTAG_GDAL_METADATA,  1, 1, TIFF_ASCII,  FIELD_CUSTOM, 0, 0, const_cast<char*>("Gdal Metadata") }};
    strMetaData = "<GDALMetadata>\n";

    //TIFFMergeFieldInfo - imposta i tag personalizzati nel file TIFF aperto
    TIFFMergeFieldInfo(tif, xtiffFieldInfo, N(xtiffFieldInfo));

    //TIFFSetField - imposta il valore di un tag nel file TIFF aperto in modalità scrittura
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, nx);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, ny);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 2);
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
    TIFFSetField(tif, TIFFTAG_GEOTIEPOINTS, 6,tiepoints);
    TIFFSetField(tif, TIFFTAG_GEOPIXELSCALE, 3,pixscale);
    
}

void geotiffWriter::addMetaData(char * nomeMetaData, char * valMetaData) {
    strMetaData += "<Item name=\"";
    strMetaData += nomeMetaData;
    strMetaData += "\"> ";
    strMetaData += valMetaData;
    strMetaData += "</Item>\n";
}

int geotiffWriter::writeMetaData() {
    strMetaData += "</GDALMetadata>";
    //TIFFSetField - imposta il valore di un tag nel file TIFF aperto in modalità scrittura
    //TIFFSetField - ritorna 1 se l'operazione ha successo, altrimenti 0.
    return TIFFSetField(tif, TIFFTAG_GDAL_METADATA,strMetaData.c_str());
}

int geotiffWriter::printRow(int i,float * rowValue)
{
    if (tif)
        //TIFFWriteScanline - consente di scrivere una scanline in un file TIFF aperto
        //TIFFWriteScanline ritorna 1 se la scrittura avviene correttamente, altrimenti -1.
        return TIFFWriteScanline(tif, rowValue, i, 0); 
    return -1;   
}

void geotiffWriter::close()
{
    //GTIFNew () - prende un descrittore di file TIFF esistente e crea un file GTIF GeoTIFF.
    gtif = GTIFNew(tif);
    if (gtif) {
        //GTIFKeySet - definisce e imposta i valori chiave specificati
        GTIFKeySet(gtif, ProjectedCSTypeGeoKey,TYPE_SHORT, 1, prjNum);
        GTIFKeySet(gtif, GTModelTypeGeoKey, TYPE_SHORT, 1, ModelProjected);
        GTIFKeySet(gtif, GeogLinearUnitsGeoKey, TYPE_SHORT,  1, Linear_Meter);
        GTIFKeySet(gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1, RasterPixelIsArea);
        GTIFKeySet(gtif, GTCitationGeoKey, TYPE_ASCII, 0, "Monte Mario / Italy zone 1");
        GTIFKeySet(gtif, GeogAngularUnitsGeoKey, TYPE_SHORT,  1, Angular_Degree);
        //GTIFWriteKeys - da chiamare solo dopo aver impostato tutti i valori 
        GTIFWriteKeys(gtif);
        //GTIFFree - Distrugge il descrittore di file GeoTIFF
        GTIFFree(gtif);
    }
    if (tif) 
        TIFFClose(tif);
}
