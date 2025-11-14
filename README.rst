## pyodb_1.1.0  pre-release 


## Description & prologue 
This is the BETA release 1.1.0 of pyodb.<br />
An interface written in C/Python API to access the ECMWF ODB1 databases.<br />

Its aim is to access the ODB tables/columns data and meta data using a direct SQL request
embadded in python scripts.<br />

The main source code is written in pure C and the routines handling the ODB1
format has been 'pruned' from ECMWF ODB_API bundle-0.18.1 to build only the
needed libraries <br />

Reference:
The original C code has been developed by "Sami Saarinen et al" at ECMWF from 1998 --> 2008.  <br />
Some modifictions have been done to make it compatible with C/Python API ( 3.8 ---> 3.10 ).

## How it works ?

## Dependencies :
        gcc compiler  = 13.2.0    
        cmake        >= 3.15.0   
        3.8.0  <=   python   <= 3.10  (NEEDS SOME READAPTATION FOR python3.11.x, 3.12.x,3.13.x )

==> It has been widely developed and tested on ATOS. <br />
So on ATOS  : <br />
   module load   gcc/13.2.0           <br />
   module load   python3/3.10.10-01   <br />
   module load   cmake/3.25.2         <br />


## installation :  

   ```  
        STEP 0 - download the source  code:
        wget   https://github.com/idirdehmous/pyodb_0.1.0/releases/download/1.1.0/pyodb_1.1.0.tar.gz 
        OR 
        git clone  https://github.com/idirdehmous/pyodb_0.1.0 
        tar -xvf  pyodb_1.1.0.tar.gz
 
        STEP 1 - FIRST, CREATE A BUILD DIRECORY TO INSTALL THE ODB LIBRARIES.
               mkdir -p  build_odb  
               cd build_odb 
               cmake    -DCMAKE_INSTALL_PREFIX=/path/to/../../odb/install/dir         /path/to/../../pyodb_1.1.0 
               make -j ncpu    ( has been tested with a maximum of 4  cpus  ) 
               make install  
```
REMARK:  <br />
-By the end the ODB installation, a file called 'odb_install_dir' will be created by cmake in the directory 'pyodb_1.1.0'. The latter will be used by setup.py <br />

-Once the odb binaries , include and libs are installed, the second step consist on building and installing the python module itself<br />

```
        STEP 2 - cd  /path/to/source/of/.../pyodb_1.1.0  
              python  setup.py   build 
!!Mandatory!: Remove the build directory if any build command is needed ! 
              ** FOR STANDARD INSTALLATION : 
                sudo  python  setup.py   install  

              ** FOR CUSTOM DIRECTORY INSTALLATION  
                 python setup.py   install  --user 
        python will install the module uder   /home/$user/.local/lib  
```

## Testing 
-When a C/Python API extension is imported, the python statement 'import' initialises some functions involved during the compilation (headers,  libraries etc ). One has then, firstly to load the shared objects using 'ctypes' module. <br /> 
-The pyodb installation is tested as follow : 
```
from ctypes import  cdll  
cdll.LoadLibrary(  "/path/to/odb/install dir/../../libodb.so"  )

#-If the shared library ( libodb.so) is loaded successfully then !

import pyodb
print( pyodb.__doc__) 
```
-For a complete import test script, please see 'tests/test_import.py'  

## Epilogue 
-If everything went well,one can use the example scripts under 'pyodb_1.1.0/tests' and reading the ODB(s) samples included in 'pyodb_1.1.0/odb_samples'. <br />

===> REMARK :
It's recommanded to run dcagen and dcagen.x just before archiving your ODB(s). <br /> 
This will allows pyodb to don't create them on the fly( even any odb apps like odbsql ,odbdump.x etc). Having DCA in ODB directory  make fatching the data a bit faster.(Advice from S.Saarinen himself!).  <br />
In the BATOR ,canari , screening , minimisation  scripts, the bash lines hereafter have to be added :  <br />
```

Example for screening :
# NEEDS SCRIPTS  : dcagen, create_ioassign
# BINARY         : dcagen.x 
# SCRIPTS AND BINARIES ARE AVAILABLE IN ARPEGE/IFS CODE (to be compiled )
# OR FROM odb_api_bundle ( ECMWF version=0.18.0)

export PATH=${PATH}:${WORKDIR}
export ODB_FEBINPATH=${WORKDIR}
${MPICMD}  ./dcagen  -l CCMA   -d -n -N ${NPROC} -q  -P -e dca.log -i  ${WORKDIR}/var_db/CCMA

Now CCMA contains DCA files 
cp -rf   ${WORKDIR}/var_db/CCMA     /somewhere/in/archive../../


```




General info        <br />
Used languages      : C / python /Fortran  <br />
Needs installation  : YES                  <br />
Needs compilation   : YES                  <br />
Tested with ODB(s)  : RMI ,CHMZ , MetCoOp and CHMI and ARPEGE  <br />
Handled observations: Conventional, GNSS , Radar , Satallite observations ( IASI, MHS AMSUA/B etc ) <br />

Some limitations    <br />
	  OpenMP                     : NOT YET    <br />
	  Read/Write to ECMA,CCMA    : READ ONLY  <br />
	  conversion to ODB2         : NOT YET   ( On going  ) <br />
	  conversion to SQLite : NOT YET   ( On going  ) <br />


@__DATE              :  2024.07.23    <br />
@__INSTITUTE         :  RMI ( Royale Meteorological Institute )   <br />
@__AUTHOR            :  Idir DEHMOUS    <br />
@__LAST_MODIFICATION :  2024.11.18      <br />
