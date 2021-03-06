/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 3.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class gruppoAntenne {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected gruppoAntenne(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(gruppoAntenne obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        libcampoclsJNI.delete_gruppoAntenne(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setGrImp(SWIGTYPE_p_void value) {
    libcampoclsJNI.gruppoAntenne_grImp_set(swigCPtr, this, SWIGTYPE_p_void.getCPtr(value));
  }

  public SWIGTYPE_p_void getGrImp() {
    long cPtr = libcampoclsJNI.gruppoAntenne_grImp_get(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_void(cPtr, false);
  }

  public void setItA(SWIGTYPE_p_std__vectorT_antenna_p_t__iterator value) {
    libcampoclsJNI.gruppoAntenne_itA_set(swigCPtr, this, SWIGTYPE_p_std__vectorT_antenna_p_t__iterator.getCPtr(value));
  }

  public SWIGTYPE_p_std__vectorT_antenna_p_t__iterator getItA() {
    long cPtr = libcampoclsJNI.gruppoAntenne_itA_get(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_std__vectorT_antenna_p_t__iterator(cPtr, false);
  }

  public void setAntenne(SWIGTYPE_p_std__vectorT_antenna_p_t value) {
    libcampoclsJNI.gruppoAntenne_antenne_set(swigCPtr, this, SWIGTYPE_p_std__vectorT_antenna_p_t.getCPtr(value));
  }

  public SWIGTYPE_p_std__vectorT_antenna_p_t getAntenne() {
    long cPtr = libcampoclsJNI.gruppoAntenne_antenne_get(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_std__vectorT_antenna_p_t(cPtr, false);
  }

  public gruppoAntenne() {
    this(libcampoclsJNI.new_gruppoAntenne__SWIG_0(), true);
  }

  public gruppoAntenne(gruppoAntenne gAnt) {
    this(libcampoclsJNI.new_gruppoAntenne__SWIG_1(gruppoAntenne.getCPtr(gAnt), gAnt), true);
  }

  public int getMinNegID() {
    return libcampoclsJNI.gruppoAntenne_getMinNegID(swigCPtr, this);
  }

  public int somma(antenna newAnt) {
    return libcampoclsJNI.gruppoAntenne_somma__SWIG_0(swigCPtr, this, antenna.getCPtr(newAnt), newAnt);
  }

  public int somma(gruppoAntenne aGA) {
    return libcampoclsJNI.gruppoAntenne_somma__SWIG_1(swigCPtr, this, gruppoAntenne.getCPtr(aGA), aGA);
  }

  public antenna getAntById(int id) {
    long cPtr = libcampoclsJNI.gruppoAntenne_getAntById(swigCPtr, this, id);
    return (cPtr == 0) ? null : new antenna(cPtr, false);
  }

  public int removeAntById(int id) {
    return libcampoclsJNI.gruppoAntenne_removeAntById(swigCPtr, this, id);
  }

  public void deleteAntenne() {
    libcampoclsJNI.gruppoAntenne_deleteAntenne(swigCPtr, this);
  }

  public int size() {
    return libcampoclsJNI.gruppoAntenne_size(swigCPtr, this);
  }

  public int addAntenna(antenna newAnt) {
    return libcampoclsJNI.gruppoAntenne_addAntenna(swigCPtr, this, antenna.getCPtr(newAnt), newAnt);
  }

  public void beginIter() {
    libcampoclsJNI.gruppoAntenne_beginIter(swigCPtr, this);
  }

  public antenna nextItem() {
    long cPtr = libcampoclsJNI.gruppoAntenne_nextItem(swigCPtr, this);
    return (cPtr == 0) ? null : new antenna(cPtr, false);
  }

  public antenna newAntenna() {
    long cPtr = libcampoclsJNI.gruppoAntenne_newAntenna(swigCPtr, this);
    return (cPtr == 0) ? null : new antenna(cPtr, false);
  }

  public void ordina() {
    libcampoclsJNI.gruppoAntenne_ordina(swigCPtr, this);
  }

}
