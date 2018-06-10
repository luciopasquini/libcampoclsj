%module libcampocls


%{
    #define SWIG_FILE_WITH_INIT

        #include "modello.h"
        #include "antenna.h" 
        #include "gruppo_antenne.h"  
        #include "impianto.h"
		#include "gruppo_modelli.h"   
        #include "gruppo_impianti.h"
        #include "modulo_calcolo.h"
%}
%inline %{
class StopIterator {};
%}
%include "modello.h"
%include "antenna.h"
%include "gruppo_modelli.h"
%include "gruppo_antenne.h" 
%include "impianto.h"  
%include "gruppo_impianti.h"
%include "modulo_calcolo.h"
%include "exception.i"
%include "std_vector.i"
namespace std {
   %template(IntVector) vector<int>;
   %template(DoubleVector) vector<double>;
}




