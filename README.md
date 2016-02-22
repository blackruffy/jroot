
This software is released under the LGPL/GPL License, see LICENCE

* [JROOT](#jroot)
  * [Javadoc](#javadoc)
  * [Download](#download)
  * [Installation](#installation)
    * [Setup environments](#setup-environments)
    * [Building the libraries](#building-the-libraries)
  * [Preparing before using JROOT](#preparing-before-using-jroot)
  * [Defferences from ROOT](#defferences-from-root)
    * [Creating Instance of the ROOT object](#creating-instance-of-the-root-object)
    * [Deleting instance of the ROOT object](#deleting-instance-of-the-root-object)
    * [Using java method naming conventions](#using-java-method-naming-conventions)
    * [Cast from TObject](#cast-from-tobject)
  * [Handling the pointer](#handling-the-pointer)
    * [Import Pointer type](#import-pointer-type)
    * [Allocate memory](#allocate-memory)
    * [Increment pointer address](#increment-pointer-address)
    * [Get pointer of pointer](#get-pointer-of-pointer)
    * [Get int value from pointer](#get-int-value-from-pointer)
    * [Set int value to pointer](#set-int-value-to-pointer)
    * [Get int array from pointer](#get-int-array-from-pointer)
    * [Set int array to pointer](#set-int-array-to-pointer)
  * [Examples](#examples)
    * [Draw TH1F](#draw-th1f)
    * [Draw TF1](#draw-tf1)
    * [Draw TGraph](#draw-tgraph)
    * [Draw color reference](#draw-color-reference)
    * [Fill data to TNtuple](#fill-data-to-tntuple)
    * [Read data from TNtuple](#read-data-from-tntuple)
    * [Fill data to TTree](#fill-data-to-ttree)
    * [Read data from TTree](#read-data-from-ttree)
    * [Fill data to TTree with TClonesArray](#fill-data-to-ttree-with-tclonesarray)
    * [Read data from TTree with TClonesArray](#read-data-from-ttree-with-tclonesarray)
    * [Fill structured data to TTree](#fill-structured-data-to-ttree)
    * [Read structured data from TTree](#read-structured-data-from-ttree)


# JROOT

JROOT is a Java binding for the [ROOT](https://root.cern.ch). JROOT provides you a lot of interfaces to the ROOT objects from Java. Take a look at the [Javadoc](http://java-root.appspot.com/javadoc_v0.5/index.html) to see which ROOT objects are supported.

Note that JROOT is under development. You need to do enough tests before using JROOT in your project.

## Javadoc

[Javadoc](http://java-root.appspot.com/javadoc_v0.5/index.html)

## Download

Source

[jroot_6.06.00_v0.5.1.tgz](http://java-root.appspot.com/jroot_6.06.00_v0.5.1.tgz)

## Supported ROOT versions

- 6.04
- 6.05
- 6.06

## Installation

This build has been tested on

- Ubuntu 15.10 x86_64
- Java 1.8.0_65
- gcc 5.2.1
- root v6.04/14

### Setup environments

- Setup the JAVA_HOME environment variable to installed java directory.

```
$ export JAVA_HOME=/path/to/javadir
```

For OS X users, you may be able to find the java directory at /Library/Java/JavaVirtualMachines/jdk1.8.x_xx.jdk/Contents/Home.

- Setup the ROOTSYS environment variable to installed ROOT directory.

```
$ export ROOTSYS=/path/to/rootdir
```

- Add $JAVA_HOME/bin and $ROOTSYS/bin to the PATH environment variable.

```
$ export PATH=$JAVA_HOME/bin:$ROOTSYS/bin:$PATH
```

### Building the libraries

```
$ tar zxvf jroot_6.04.06_v0.5.tgz
$ cd jroot
$ make
```      

This process creates a shared library and a jar file.

- libJRoot.so (libJRoot.dylib for mac) in target/lib
- jroot_6.04.06_v0.5.jar in target/lib

## Preparing before using JROOT

- Adding shared libraries to LD_LIBRARY_PATH

```
export LD_LIBRARY_PATH=/path/to/jroot/build/lib:$LD_LIBRARY_PATH
```

NOTE that for OS X, use DYLD_LIBRARY_PATH instead of LD_LIBRARY_PATH.
 
- Adding jar file to CLASSPATH

```
export CLASSPATH=/path/to/jroot/build/lib/jroot_6.04.06_v0.5.jar:$CLASSPATH
```

NOTE: use '-XstartOnFirstThread' option when you run JROOT application on OS X.

## Defferences from ROOT

JROOT can be used in the same way as ROOT except a few differences below.

### Creating Instance of the ROOT object

The all ROOT objects are implemented as Java interfaces in JROOT. So you can not instanciate ROOT objects directly. In order to instaciate ROOT objects, use static factory method 'new + <class name>' in class JRoot.

```java
import net.balckruffy.root.*;
import static net.balckruffy.root.JRoot.*;
TH1F h = newTH1F( "name", "title", 100, 0, 100 );
```

### Deleting instance of the ROOT object

Since the JROOT objects are not managed by JAVA garbage collection, you should release memory of the objects manually. Use 'delete'  method. (if 'delete' method is not exists, use 'destroy' method instead.)

```java
h.delete();
```

### Using java method naming conventions

The name of methods in ROOT begins with capital letter, which is different from java method naming conventions, so all names of ROOT method are converted to java method naming conventions.

```java
h.fill( x ); // not h.Fill(x)
h.draw();    // not h.Draw()
```

### Cast from TObject

If you want to cast from TObject to JROOT object, do not use default cast method of Java. You have to use special cast methods of JRoot class. For example, in order to cast TObject to TH1F, use method TH1F().

```java
TH1F h = TH1F(list.findObject("name"));
```

## Handling the pointer

Some ROOT functions take pointers or return pointers. So you need to know how JRoot can handle the C/C++ pointers.

### Import Pointer type

```java
// Java
import net.blackruffy.root.Pointer;
import static net.blackruffy.root.Pointer.*;
```

### Allocate memory

```java
// Java
Pointer p = allocate(bytes);
```

This is equivalent to

```c++
// C/C++
void* p = malloc(bytes);
```

### Increment pointer address

```java
// Java
Pointer q = p.at(4);
```

This is equivalent to

```c++
// C/C++
void* q = p + 4;
```

### Get pointer of pointer

```java
// Java
Pointer q = p.getReference();
```

This is equivalent to

```c++
// C/C++
void* q = &p;
```

### Get int value from pointer

```java
// Java
Pointer p = newInt(10);
int x = p.getIntValue();
```

This is equivalent to

```c++
// C/C++
void* p = new int(10);
int x = *(int*)p;
```

### Set int value to pointer

```java
// Java
Pointer p = allocate(4); // or Pointer p = newInt();
p.setIntValue(100);
```

This is equivalent to

```c++
// C/C++
void* p = malloc(4);
*(int*)p = 100;
```

### Get int array from pointer

```java
// Java
int[] xs = p.getIntArray(size);
```

This is equivalent to

```c++
// C/C++
int* xs = (int*)p;
```

### Set int array to pointer

```java
// Java
int[] xs = {1, 2, 3};
p.setIntArray(xs);
```

This is equivalent to

```c++
// C/C++
int xs[3] = {1, 2, 3};
void* p = xs;
```

## Examples

### Draw TH1F

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;

public class DrawTH1F {

  public static void main( String[] args ) {

    TApplication app = newTApplication( "app", Pointer.newInt(0), new String[]{} );

    TH1F h = newTH1F( "h", "h", 100, -10., 10. );
    
    h.fillRandom("gaus", 10000);
    
    TCanvas c = newTCanvas( "c", "c", 800, 600 );
    gPad().setGrid();

    h.setFillColor(38);
    h.draw();

    c.update();
    
    app.run();
    
  }

}
```

### Draw TF1

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class DrawTF1 {

  public static void main( String[] args ) {

    TApplication app = newTApplication( "app", newInt(0), new String[]{} );
    
    TCanvas c = newTCanvas( "c", "c", 800, 600 );

    TF1 f = newTF1("f", "sin(x)", 0.0, Math.PI*2);
    f.draw();

    c.update();
    
    app.run();

  }

}
```

### Draw TGraph

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;

public class DrawTGraph {

  public static void main( String[] args ) {

    final TApplication app = newTApplication( "app", newInt(0), new String[]{} );

    final int size = 100;
    final double[] xs = new double[size];
    final double[] ys = new double[size];
    for( int i=0; i<size; i++ ) {
      xs[i] = Math.PI/50*i;
      ys[i] = Math.sin(xs[i]);
    }

    TGraph g = newTGraph(size, xs, ys);

    double[] as = g.getX().getDoubleArray(g.getN());

    for( int i=0; i<size; i++ ) {
      System.out.printf("%f, %f\n", xs[i], as[i]);
    }
    
    TCanvas c = newTCanvas( "c", "c", 800, 600 );
    c.setGrid();
    
    g.draw();

    c.update();
    
    app.run();
    
    c.destroy();
    g.destroy();
  }

}
```

### Draw color reference

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;

public class DrawColors {

  public static void main( String[] args ) {
    TApplication app = newTApplication( "app", newInt(0), new String[]{} );
    TCanvas c = newTCanvas();
    c.divide(10, 10);
    for( int i=0; i<100; i++ ) {
      c.cd(i+1);
      gPad().setFillColor(i+1);
      gPad().draw();
      TText text = newTText(0.1, 0.1, Integer.toString(i+1));
      text.setTextSize(0.7f);
      if( i == 0 ) text.setTextColor(10);
      text.draw();
      gPad().update();
    }
    c.update();
    app.run();
  }

}
```

### Fill data to TNtuple

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class FillTNtuple {

  public static void main( String[] args ) {

    TFile f = newTFile("tuple.root", "RECREATE");
    TNtuple tuple = newTNtuple("tuple", "tuple", "x:y:z");

    TRandom ran = newTRandom();

    int nev = 10000;
    for( int ev=0; ev<nev; ev++ ) {
      float x = (float)ran.gaus();
      float y = (float)ran.gaus();
      float z = (float)ran.gaus();
      tuple.fill(x, y, z);
    }

    f.write();
    f.close();

  }

}
```

### Read data from TNtuple

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class ReadTNtuple {

  public static void main( String[] args ) throws Exception {

    final TFile file = newTFile("tuple.root", "READ");
    final TNtuple tree = TNtuple(file.get("tuple"));

    long nev = tree.getEntries();

    for( long ev=0; ev<nev; ev++ ) {
      tree.getEntry(ev);
      float[] xs = tree.getArgs().getFloatArray(tree.getNvar());
      out.printf("%f, %f, %f\n", xs[0], xs[1], xs[2]);
    }

    tree.delete();
    file.close();

  }

}
```

### Fill data to TTree

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class FillTTree {

  public static void main( String[] args ) {

    TFile f = newTFile("tree.root", "RECREATE");
    TTree tree = newTTree("tree", "tree");

    Pointer px = Pointer.newInt();
    TLorentzVector vec = newTLorentzVector(0, 0, 0, 0);

    tree.branch("x", px, "x/I");
    tree.branch("vec", "TLorentzVector", vec);

    TRandom ran = newTRandom();
    int nev = 100000;
    for( int ev=0; ev<nev; ev++ ) {
      if( ev%1000 == 0 ) out.println(ev);
      px.setIntValue((int)(ran.gaus()*100));
      vec.setX(ran.gaus());
      tree.fill();
      //f.flush();
    }

    f.write();
    f.close();

  }

}
```

### Read data from TTree

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class ReadTTree {

  public static void main( String[] args ) throws Exception {

    TApplication app = newTApplication("app", newInt(0), new String[]{});
    final TH1F h = newTH1F("", "h", 100, -10, 10);

    final Pointer px = newInt(0);
    final TLorentzVector vec = newTLorentzVector(0, 0, 0, 0);

    final TFile file = newTFile("tree.root", "READ");
    final TTree tree = TTree(file.get("tree"));

    long nev = tree.getEntries();
    
    tree.setBranchAddress("x", px);
    tree.setBranchAddress("vec", vec);

    double sum = 0;
    for( long ev=0; ev<nev; ev++ ) {
      tree.getEntry(ev);
      int x = px.getIntValue();
      double lx = vec.x();
      h.fill(lx);
      out.printf("%d: %d, %f\n", ev, x, lx);
    }

    tree.delete();
    file.close();

    TCanvas c = newTCanvas();
    gPad().setGrid();
    h.setFillColor(38);
    h.draw();
    c.update();
    app.run();

  }

}
```

### Fill data to TTree with TClonesArray

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class FillTClonesArray {

  public static void main( String args[] ) {
    TFile file = newTFile("clones.root", "RECREATE");
    TTree tree = newTTree("tree", "tree");

    TClonesArray xs = newTClonesArray("TLorentzVector", 10);

    tree.branch("xs", "TClonesArray", xs);

    int nev = 1000;
    for( int ev=0; ev<nev; ev++ ) {
      for( int n=0; n<10; n++ ) {
        TLorentzVector v = TLorentzVector(xs.constructedAt(n));
        v.setXYZT(
          gRandom().gaus(),
          gRandom().gaus(),
          gRandom().gaus(),
          gRandom().gaus()
        );
      }
      tree.fill();
      xs.clear();
    }

    file.write();
    file.close();

  }

}
```

### Read data from TTree with TClonesArray

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class ReadTClonesArray {

  public static void main( String args[] ) {
    TH1F h = newTH1F("", "h", 100, 0, 5);

    TFile file = newTFile("clones.root", "READ");
    TTree tree = TTree(file.get("tree"));

    TClonesArray xs = newTClonesArray("TLorentzVector");

    tree.setBranchAddress("xs", xs);

    long nev = tree.getEntries();
    for( int ev=0; ev<nev; ev++ ) {
      out.printf("%d/%d\n", ev, nev);
      tree.getEntry(ev);
      for( int n=0; n<xs.getEntries(); n++ ) {
        TLorentzVector v = TLorentzVector(xs.at(n));
        h.fill(v.p());
      }
      xs.clear();
    }

    file.close();

    TApplication app = newTApplication("app", newInt(0), new String[]{});
    TCanvas c = newTCanvas();
    gPad().setGrid();
    h.draw();
    c.update();
    app.run();

  }

}
```

### Fill structured data to TTree

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class FillStruct {

  public static void main( String args[] ) {
    TFile f = newTFile("struct.root", "RECREATE");
    TTree tree = newTTree("tree", "tree");

    // allocate memory with 32 bytes and get its pointer.
    // in this case, you are going to save a int data, a float data, a double data and a int array with 4 data in an event.
    // the total bytes is
    // int(4 bytes) + float(4 bytes) + double(8 bytes) + int array(4*4 bytes) = 32 bytes
    Pointer data = allocate(32);

    tree.branch("data", data, "x/I:y/F:z/D:xs[4]/I");

    TRandom ran = gRandom();
    int nev = 1000;
    for( int ev=0; ev<nev; ev++ ) {
      out.println(ev);
      data.setIntValue(ev);
      // set a float value at 4 bytes from the address of pointer 'data'.
      data.at(4).setFloatValue(Double.valueOf(ran.gaus()).floatValue());
      // set a double value at 8 bytes from the address of pointer 'data'.
      data.at(8).setDoubleValue(ran.gaus());

      int xs[] = new int[4];
      for( int i=0; i<4; i++ ) xs[i] = ev + i;

      // set an array at 16 bytes from the address of pointer 'data'
      data.at(16).setIntArray(xs);
      tree.fill();
      //f.flush();
    }

    f.write();
    f.close();
  }

}
```

### Read structured data from TTree

```java
import net.blackruffy.root.*;
import static net.blackruffy.root.JRoot.*;
import static net.blackruffy.root.Pointer.*;
import static java.lang.System.*;
import static java.lang.String.format;

public class ReadStruct {

  public static void main( String args[] ) {

    final TFile file = newTFile("struct.root", "READ");
    final TTree tree = TTree(file.get("tree"));

    long nev = tree.getEntries();

    // allocate memory to store data in branches.
    // you need 32 bytes(int, float, double, int array with 4 elements).
    Pointer data = allocate(32);

    tree.setBranchAddress("data", data);

    double sum = 0;
    for( long ev=0; ev<nev; ev++ ) {
      tree.getEntry(ev);
      // get int value at the first address of pointer.
      int x = data.getIntValue();
      // get float value at 4 bytes from address of pointer.
      float y = data.at(4).getFloatValue();
      double z = data.at(8).getDoubleValue();
      // get int array with 4 element at 16 bytes from address of pointer.
      int[] xs = data.at(16).getIntArray(4);

      out.printf("%d, %f, %f, [%d, %d, %d, %d]\n", x, y, z, xs[0], xs[1], xs[2], xs[3]);
    }

    tree.delete();
    file.close();

  }

}
```

Copyright (c) 2016 Taichi Kubo
All Rights Reserved.