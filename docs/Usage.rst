Usage
=====

Open a CCMA database 
--------------------
The package pyodb is written in C/Python API. It contains a set of functions as an interface between python and ODB softeware. 
In order to open a given database one has to import the module "odbConnect". Ths function returns positive value for a seccessful connection and negative if it fails.
We consider the use of an CCMA odb (The same code  works also for ECMA )

.. code-block:: python

   from pyodb import odbConnect   
   iret  = odbConnect(odbdir ="imaginary/../CCMA" , verbose=True, npools="1"  )
   if iret < 0 :
      print("Failed  to open the odb"  , dbpath )
      sys.exit(0)



Create the DCA (Direct Column Access) files 
---------------------------------------------
In order to fetch the columns and rows, the ODB creates the DCA files first. The latters contain the necessary information about how the data are written on the disk.Such as the starting memory segment and the end of a given table, the size and the offsets between the different tables and columns.

The DCA files can be created on the fly when any of the binaries such as odbdump.x or odbsql.x are used. In the pyodb package a C/python function is used to create the files before the data fetch. Creating DCA files is relatiely simple,the user has to provide the path the ODB and the number of CPUs used by the task. The ODB path is requiered however, the number of CPUs and the verbosity arguments are optional. The DCA files are written uin a subdirectory  called "dca" inside the ECMA or CCMA directory. 

.. code-block:: python 

   # Check if the dca directory is already there
   dbpath = "/path to/imaginary"
   db_type= "CCMA" 
   dcapath= "/".join( ( dbpath,db_type, dcapth) )

   if not os.path.isdir (dcapath ):
      os.environ["IOASSIGN "] =dbpath+"/"+db_type+"/IOASSIGN"
      ic=odbDca ( dbpath=dbpath, db= db_type, ncpu=8  )  # in this example 8 CPUs are used 
      # 'ic' is used to check whether the process has successfully ended 
      # Returns a negative value if it fails and postive in success. 



Send and fetch data from a given ECMA or CCMA 
----------------------------------------------
 
