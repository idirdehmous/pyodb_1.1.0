# -*- coding: utf-8 -*- 

# THE pyodb MODULE CAN'T BE IMPORTED DIRECTLY ! IT NEEDS libodb.so AS A DYNAMICAL LIB
# ONE HAS TO LOAD THE SHARED LIBRARY FIRST 

import os  , sys 
from ctypes  import cdll  


file_found=False 
env_found =False 
if os.path.isfile ( "odb_install_dir" ):
   odbpath_file="odb_install_dir"
   file_found=True 
   print("-"*50)
   print( "** odb_install_dir file FOUND !" )
   print(" "  )

else:
   file_found=False 
   print ( "** odb_install_dir file NOT FOUND !" )  
   print ( "\n** Trying to get ODB_INSTALL_DIR from environment variable ... \n" )

   odb_install_dir=os.getenv( "ODB_INSTALL_DIR" )
   if odb_install_dir != None:
      env_found=True 
      print("\n** The path to the ODB_install dir found :"  , odb_install_dir, "\n") 
   else:
      print("** Can't find libodb.so location.   'export ODB_INSTALL_DIR=/path/to/../odb_install_dir' \n")
      env_found =False  
      sys.exit(0)

if file_found:
   _file_ =  open( odbpath_file  , "r" )
   odb_install_dir = _file_.readline().strip()
   libname="libodb.so"
   libpath="/".join( (odb_install_dir,  "/lib/" , libname ) )
   status= cdll.LoadLibrary(   libpath   )
   if status is not None:
      print(  "** libodb.so has been successfully loaded !" )

if env_found:
   libname="libodb.so"
   libpath="/".join( (odb_install_dir,  "/lib/" , libname ) )
   status= cdll.LoadLibrary(   libpath   )
   if status is not None:
      print(  "** libodb.so has been successfully loaded " )


# pyodb CAN NOW BE IMPORTED !
try:
   import  pyodb
   __methods__= [  m for m in dir( pyodb )  if not m.startswith( "__"  ) ]
   print("Main methods  :")
   for m in __methods__: 
       print("                   :",  m ) 
   

   import pyodb_extra 
   print("pyodb_extra modules and classes :")
   __ext_modules__= [  m for m in dir( pyodb_extra )  if not m.startswith( "__"  ) ]

   for m in __ext_modules__:
       print("                   :",  m )

   print("     ")
   print("** Waiting for a complete documentation on :https://docs.readthedocs.io  !\n")

except:
   ModuleNotFoundError 
   print ("** Something went wrong during importing modules !" )

quit()


