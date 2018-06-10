//import file necessari:

//aggiungere file creato da swig
//import java.util.*;
//import java.lang.*;
//import java.io.*;

// one class needs to have a main() method
public class Test
{
 // arguments are passed using the text field below this editor
  static {
    try {
        System.loadLibrary("libcampocls");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\n" + e);
      System.exit(1);
    }
  }

 public static void main(String[] args)
 {
   gruppoImpianti GI= new gruppoImpianti();
   String str = "c:\\SitiAntenne.xml";//indirizzo e nome file
   char[] nomeFile = str.toCharArray();
   int x=GI.leggiImpiantiAntenneXML(str);
   GI.beginIter();
   impianto imp;
   imp = GI.nextItem();
   System.out.print("\nNumero ID primo imp: ");
   System.out.print(imp.getDI("IDSITO"));
   System.out.print("\nFine test\n");
 }
}
