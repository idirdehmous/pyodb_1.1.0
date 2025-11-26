Usage
=====

Example
-------
The package pyodb contains a C functions as an interface between python and ODB softeware. 
in order to open a given database one has to import the module "odbConnect". Ths function returns positive value for a seccessful connection and negative if it fails.
Wwe consider the use of an CCMA odb (The same code  works also for ECMA )
.. code-block:: python

   from pyodb import odbConnect   

   iret  = odbConnect(odbdir ="imaginary/../CCMA" , verbose=True, npools="1"  )
   if iret < 0 :
      print("Failed  to open the odb"  , dbpath )
      sys.exit(0)
    
