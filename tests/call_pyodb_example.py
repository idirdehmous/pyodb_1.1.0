# -*- coding: utf-8 -*-
import os
import sys
import ctypes 
from   ctypes import cdll , CDLL
from   ctypes.util import find_library 
import pandas as  pd 


# pyodb .py    MODULES 
from pyodb_extra.environment  import  OdbEnv
from pyodb_extra.odb_ob       import  OdbObject
from pyodb_extra.parser       import  StringParser  


# Init env first  ( path to libodb.so, no  /lib/ ! )
env= OdbEnv ("/home/idehmous/Desktop/rmib_dev/github/pkg", "libodb.so")
env.InitEnv ()

# --> NOW pyodb could be imported  !
from pyodb   import  odbFetch   , odbDict
from pyodb   import  odbConnect , odbClose
from pyodb   import  odbDca




# GET ARGS from COMMAND LINE 
nargv = len(sys.argv)
if nargv > 1 :
  dbpath   = sys.argv[1]
  if not os.path.exists(dbpath) :
    
      print("--Odb " + dbpath + " not found.")
      exit(1)
else :
  print("--You need to provide the odb path !\n")
  print("--Usage:")
  print("  python   call_odb_example.py   dbpath  (ECMA.<obstype>  or CCMA )\n")
  exit(1)



# Some needed attributes 
db      = OdbObject ( dbpath )
db_attr = db.GetAttrib()
db_type = db_attr["type"]
db_name = db_attr["name"]

# Connect 
iret  = odbConnect ( odbdir =dbpath+"/"+db_name   )
if iret < 0 :
   print("Failed  to open the odb"  , dbpath )
   sys.exit(0)



# Direct Column Access files 
# CREATE DCA IF THEY ARN'T ALREADY THERE 
# IF THE ODB HAS BEEN OPENED ONCE THESE LINES CAN BE SWITCHED !
if not os.path.isdir (dbpath+"/dca"):
   os.environ["IOASSIGN "]           =dbpath+"/IOASSIGN"
   ic=odbDca ( dbpath=dbpath, db= db_type, ncpu=8  )


# SIMPLE SQL QUERY (lat,lon,obsvalue )
sql_query="select   statid ,degrees(lat) ,  degrees(lon) , varno, obsvalue   FROM  hdr, body" 
# CHECK THE query 
p      =StringParser()
nfunc  =p.ParseTokens ( sql_query )    # Parse sql statement and get the number of functions  ( In C code , N pure columns = N All columns - N functions in sql query)
sql    =p.CleanString ( sql_query  )   # Check and clean before sending !



# THE METHOD COULD BE CALLED WITH sql QUERY or  sql file  ( one  is Required  !)
# args :
nfunctions=nfunc    # (type == integer ) Number of columns considring the functions in the sql statement  (degrees, rad, avg etc ...)
query_file=None     # (type == str     ) The sql file if used rather than sql request string 
poolmask   =None    # (type ==str      ) The pool number (  must be a string  "1" , "2", "33" ...  , etc   )
progress  =True     # (type == bool    ) Progress bar (we need to see what happends in the case of "huge " ODBs)
float_fmt = 5       # (type == str     ) Number of digits for floats (same syntax  as in C  )
verbose   =False    # (type == bool    ) Verbosity   on/off  


data =odbDict  (dbpath ,sql, nfunctions ,float_fmt,query_file , poolmask , progress    )
print( data     )
odbClose( iret )


