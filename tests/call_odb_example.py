# -*- coding: utf-8 -*-
import os
import sys
import ctypes 
from   ctypes import cdll , CDLL
from   ctypes.util import find_library 
import argparse 
sys.path.insert( 0,   "/mnt/HDS_ALD_TEAM/ALD_TEAM/idehmous/hpca_backup/pyodb_releases/pyodb_1.1.0/build/lib.linux-x86_64-cpython-39" )

# python  MODULES 
from pyodb_extra.odb_glossary import  OdbLexic
from pyodb_extra.parser       import  StringParser
from pyodb_extra.environment  import  OdbEnv
from pyodb_extra.odb_ob       import  OdbObject
from pyodb_extra.exceptions   import  *


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


# WHERE libodb.so  is installed  ? 
odb_install_dir=os.getenv( "ODB_INSTALL_DIR" )
if odb_install_dir== None:
   # SET THE PATH EXPLICITY 
   odb_install_dir="/hpcperm/cvah/odb/odb_env"


# INIT ENV 
env= OdbEnv(odb_install_dir , "libodb.so")
env.InitEnv ()

# --> NOW pyodb could be imported  !
from pyodb   import  odbFetch
from pyodb   import  odbConnect , odbClose 
from pyodb   import  odbDca  



db      = OdbObject ( dbpath )
db_attr = db.GetAttrib()

db_type = db_attr["type"]
db_name = db_attr["name"]



# CREATE DCA IF THEY ARN'T ALREADY THERE 
# ALWAYS EXPORT IOASSIGN FILE , OTHERWISE dcagen WILL CRASH WITOUT FINISHING !!!!!
if not os.path.isdir (dbpath+"/dca"):
   os.environ["IOASSIGN "]           =dbpath+"/IOASSIGN"
   ic=odbDca ( dbpath=dbpath, db= db_type, ncpu=8  )

# CONNECT 
iret  = odbConnect ( odbdir=dbpath+"/"+db_name  , mode="r")
if iret < 0 :
   print("Failed  to open the odb"  , dbpath )
   sys.exit(0)


# SQL QUERY 
sql_query="select seqno, date,time, varno ,lat, lon , obsvalue from hdr,body"

# CHECK THE query 
p      =StringParser()
nfunc  =p.ParseTokens ( sql_query )    # Parse sql statement and get the number of functions  
sql    =p.CleanString ( sql_query  )   # Check and clean before sending !

#sqlfile="sql/ccma_view.sql"

# THE METHOD COULD BE CALLED WITH sql QUERY or  sql file  (ONE IS MANDATORY !)
# The arguments are positional , should be replaced by keyword args 
# args :
nfunctions=nfunc    # (type == integer )Number of columns considring the functions in the sql statement  (degrees, rad, avg etc ...)
query_file=None     # (type == str     )The sql file if used rather than sql request string 
pool      =None     # (type == str     )The pool number (  must be a string  "2", "33"   , etc   )
progress  =False  
float_fmt =None     # (type == str     )Number of digits for floats (same syntax  as in C  )
verbose   =True     # (type == bool    )Verbosity 
header    =True     # (type == bool    )Get the columns names 


rows =odbFetch(dbpath ,sql, nfunctions ,query_file, pool,float_fmt , progress   , verbose, header)
for row in rows :
    print( row  )


