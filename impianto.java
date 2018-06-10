/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class impianto extends gruppoAntenne {
  private transient long swigCPtr;

  protected impianto(long cPtr, boolean cMemoryOwn) {
    super(libcampoclsJNI.impianto_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(impianto obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        libcampoclsJNI.delete_impianto(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public void setModified(SWIGTYPE_p_setT_string_t value) {
    libcampoclsJNI.impianto_Modified_set(swigCPtr, this, SWIGTYPE_p_setT_string_t.getCPtr(value));
  }

  public SWIGTYPE_p_setT_string_t getModified() {
    return new SWIGTYPE_p_setT_string_t(libcampoclsJNI.impianto_Modified_get(swigCPtr, this), true);
  }

  public impianto() {
    this(libcampoclsJNI.new_impianto__SWIG_0(), true);
  }

  public impianto(impianto oImp) {
    this(libcampoclsJNI.new_impianto__SWIG_1(impianto.getCPtr(oImp), oImp), true);
  }

  public int setDI(String dbFld, String valDato) {
    return libcampoclsJNI.impianto_setDI(swigCPtr, this, dbFld, valDato);
  }

  public String getDI(String dbFld) {
    return libcampoclsJNI.impianto_getDI(swigCPtr, this, dbFld);
  }

  public void setModified(String dbFld) {
    libcampoclsJNI.impianto_setModified(swigCPtr, this, dbFld);
  }

  public void noModified(String dbFld) {
    libcampoclsJNI.impianto_noModified(swigCPtr, this, dbFld);
  }

  public boolean isModified(String dbFld) {
    return libcampoclsJNI.impianto_isModified(swigCPtr, this, dbFld);
  }

  public void setIdImp(int id) {
    libcampoclsJNI.impianto_setIdImp(swigCPtr, this, id);
  }

  public void setInfluente(boolean isInflu) {
    libcampoclsJNI.impianto_setInfluente(swigCPtr, this, isInflu);
  }

  public boolean getInfluente() {
    return libcampoclsJNI.impianto_getInfluente(swigCPtr, this);
  }

  public boolean getCalcoloImpianto() {
    return libcampoclsJNI.impianto_getCalcoloImpianto(swigCPtr, this);
  }

  public int getIdImp() {
    return libcampoclsJNI.impianto_getIdImp(swigCPtr, this);
  }

  public double getX() {
    return libcampoclsJNI.impianto_getX(swigCPtr, this);
  }

  public double getY() {
    return libcampoclsJNI.impianto_getY(swigCPtr, this);
  }

  public double getZ() {
    return libcampoclsJNI.impianto_getZ(swigCPtr, this);
  }

  public String getNome() {
    return libcampoclsJNI.impianto_getNome(swigCPtr, this);
  }

  public String getCodImp() {
    return libcampoclsJNI.impianto_getCodImp(swigCPtr, this);
  }

  public String getIndirizzo() {
    return libcampoclsJNI.impianto_getIndirizzo(swigCPtr, this);
  }

  public String getRespImp() {
    return libcampoclsJNI.impianto_getRespImp(swigCPtr, this);
  }

  public String getNote1() {
    return libcampoclsJNI.impianto_getNote1(swigCPtr, this);
  }

  public String getNote2() {
    return libcampoclsJNI.impianto_getNote2(swigCPtr, this);
  }

  public String getImmagine() {
    return libcampoclsJNI.impianto_getImmagine(swigCPtr, this);
  }

  public String getCreazione() {
    return libcampoclsJNI.impianto_getCreazione(swigCPtr, this);
  }

  public String getDataModifica() {
    return libcampoclsJNI.impianto_getDataModifica(swigCPtr, this);
  }

  public String getParere() {
    return libcampoclsJNI.impianto_getParere(swigCPtr, this);
  }

  public String getComunicazione() {
    return libcampoclsJNI.impianto_getComunicazione(swigCPtr, this);
  }

  public String getDataDismissione() {
    return libcampoclsJNI.impianto_getDataDismissione(swigCPtr, this);
  }

}
