CPPFLAGS=-Wall
XMLH = -I/mingw64/include/cereal/external/rapidxml
GEOTIFFH = -I/usr/include
PYH = -I/usr/include
PYL = -L/usr/lib
LIB = -lcrypto -lgeotiff -ltiff -ljpeg -lz -llzma -lshp

DEPSH = antenna.h gruppo_antenne.h impianto.h \
asc_writer.h geotiff_writer.h gruppo_impianti.h modello.h \
get_md5_file.h gruppo_modelli.h modulo_calcolo.h

OBJ = gruppo_modelli.o gruppo_impianti.o \
gruppo_antenne.o modello.o impianto.o antenna.o get_md5_file.o \
libcampocls_wrap.o modulo_calcolo.o geotiff_writer.o asc_writer.o

libcampocls.dll:$(OBJ)
	$(CXX) -Wall -shared -static -static-libgcc -static-libstdc++  $(OBJ) $(PYL) -lpython36 $(LIB) -o $@
libcampocls_wrap.cpp: libcampocls.java.i $(DEPSH)
	swig -c++ -java -o libcampocls_wrap.cpp libcampocls.java.i
libcampocls_wrap.o:libcampocls_wrap.cpp
	$(CXX) -c $(CPPFLAGS) $< $(PYH) -I"/c/Program Files/Java/jdk-10/include" -I"/c/Program Files/Java/jdk-10/include/win32"
modello.o:modello.cpp modello.h
	$(CXX) -c $(CPPFLAGS) $<
gruppo_modelli.o:gruppo_modelli.cpp gruppo_modelli.h modello.h get_md5_file.h
	$(CXX) -c $(CPPFLAGS) $< $(XMLH)
antenna.o:antenna.cpp antenna.h modello.h
	$(CXX) -c $(CPPFLAGS) $<
gruppo_antenne.o:gruppo_antenne.cpp gruppo_antenne.h antenna.h modello.h
	$(CXX) -c $(CPPFLAGS) $<
impianto.o:impianto.cpp impianto.h gruppo_antenne.h antenna.h modello.h
	$(CXX) -c $(CPPFLAGS) $<
gruppo_impianti.o:gruppo_impianti.cpp gruppo_impianti.h impianto.h gruppo_antenne.h antenna.h gruppo_modelli.h modello.h get_md5_file.h
	$(CXX) -c $(CPPFLAGS) $< $(XMLH)
asc_writer.o:asc_writer.cpp asc_writer.h
	$(CXX) -c $(CPPFLAGS) $<
geotiff_writer.o:geotiff_writer.cpp geotiff_writer.h
	$(CXX) -c $(CPPFLAGS) $< $(GEOTIFFH)
modulo_calcolo.o:modulo_calcolo.cpp modulo_calcolo.h gruppo_impianti.h impianto.h gruppo_antenne.h \
 antenna.h gruppo_modelli.h modello.h
	$(CXX) -c $(CPPFLAGS) $< $(GEOTIFFH)
get_md5_file.o:get_md5_file.cpp get_md5_file.h
	$(CXX) -c $(CPPFLAGS) $<


.PHONY: clean
clean:
	-rm libcampocls_wrap.cpp
	-rm *.o
	-rm libcampocls.py
	-rm _libcampocls.so
	-rm _libcampocls.pyd

