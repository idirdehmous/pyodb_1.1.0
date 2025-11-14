# -*- coding: utf-8 -*- 
import os, sys 
import cartopy.crs as ccrs
import cartopy.feature as cfeature
import matplotlib.pyplot as plt
from   mpl_toolkits.axes_grid1 import make_axes_locatable
from   pandas import  DataFrame   


sys.path.insert(0,"/home/micro/test/pyodb_1.1.0/build/lib.linux-x86_64-cpython-312" )

from pyodb_extra.environment  import  OdbEnv
from pyodb_extra.odb_ob       import  OdbObject
from pyodb_extra.parser       import  StringParser





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
  print("  python   odb_plotter.py   dbpath  (ECMA.<obstype>  or CCMA )\n")
  exit(1)


# INIT ENV ( path to libodb.so, no  /lib/ ! )
env= OdbEnv ("/home/micro/test/pkg", "libodb.so")
env.InitEnv ()


# NOW pyodb modules could be imported !
from pyodb       import   odbDict  
from pyodb       import   odbConnect , odbClose 
from pyodb       import   odbVarno 

# IN CASE THE DCA FILE HAVE BEEN CREATED odbDca IS NOT NEED !
from pyodb     import     odbDca  



# Get odb type  and name  
db      = OdbObject ( dbpath )
db_attr = db.GetAttrib()
db_type = db_attr["type"]
db_name = db_attr["name"]


# SIMPLE SQL QUERY (lat,lon,obsvalue  from upper air T AMDAR ) 
sql_query="select   statid ,degrees(lat) ,  degrees(lon) , varno, obsvalue   FROM  hdr, body WHERE obstype ==2 AND varno ==2"
# CHECK THE query 
p      =StringParser()
nfunc  =p.ParseTokens ( sql_query )    # Parse sql statement and get the number of functions  ( In C code , N pure columns = N All columns - N functions in sql query)
sql    =p.CleanString ( sql_query  )   # Check and clean before sending !



iret  = odbConnect ( odbdir=dbpath+"/"+db_name  )
if iret < 0:
   print("Failed to connect to the ODB")
   sys.exit ()


# dcagen MUST FIND IOASSIGN FILE (export is as env variable)
env.OdbVars["CCMA.IOASSIGN"]="/".join(  (dbpath, "CCMA.IOASSIGN" ) )

# Don't create DCA if they are already there 
if not  os.path.isdir( dbpath+"/dca" ):
   stat = odbDca   ( dbpath=dbpath  ,  db=db_name  , ncpu =8 )
   if stat <0 :   
      print("--Failed to create DCA files !" );   
      sys.exit(2)

nfunctions =nfunc    
query_file =None      
poolmask   =None    
progress   =True     
float_fmt  = 5
verbose    =False    

# Fetch  the rows 
odb_data =odbDict  (dbpath ,sql, nfunctions ,float_fmt,query_file , poolmask , progress    )

# The data can be easily converted to pandas df
# Now the variable to handle can also been requested from the dataframe 
df= DataFrame( odb_data )
#print( df.head())  
lats=df["degrees(lat)" ]
lons=df["degrees(lon)" ]
obs =df["obsvalue@body"]  

# DOMAIN BOUNDARIES 
if len(lats) != 0: 
   ulat=max(lats)+1. 
   llat=min(lats)-1.
if len(lons) != 0: 
   ulon=max(lons)+1. 
   llon=min(lons)-1.

# PLOT 
fig = plt.figure(figsize=(10, 15))
ax  = fig.add_subplot(111,projection=ccrs.Mercator())
ax.autoscale(True)
ax.coastlines()
ax.set_extent([llon, ulon  ,llat ,ulat], crs=ccrs.PlateCarree())
ax.add_feature(cfeature.BORDERS, linewidth=0.5, edgecolor='blue')
ax.gridlines(draw_labels=True)
sc=plt.scatter ( lons, lats ,c=obs , cmap=plt.cm.jet ,marker='o',s=20, zorder =111,transform=ccrs.PlateCarree() )

divider = make_axes_locatable(ax)
ax_cb = divider.new_horizontal(size="5%", pad=0.9, axes_class=plt.Axes)
fig.add_axes(ax_cb)
plt.colorbar(sc, cax=ax_cb)
plt.show()

