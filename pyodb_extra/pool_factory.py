import os , sys 
import math 
import numpy as np 


from   .odb_ob import OdbObject  

# DOC__:
# NOTE: THIS MODULE IS BEING DEVELOPED FOR HUGE  ODB(s) : npools > 100 or 200 

# IF THE NUMBER OF POOLS IS IN 100 --> 8xx RANGE 
# ONLY SOME OF POOLS ARE VISIBLE ,THE OTHER ARE MASKED !
#                         DILEMMA !
# IF WE LOOP OVER THE VISIBLE POOLS : NOT THE FULL DATASET WOULD BE FETCHED , THERE WOULD BE A DATA HOLES (MAYBE THERE IS ANOTHER WAY?)! 
# IF WE LOOP OVER THE WHOLE POOLS   : THERE WILL ARRAY DUPLICATIONS. DIFFERENT PROCESSES WILL READ SAME DATA IN MEMORY  
# PROCESS1 READS DATA FROM 1 byte ----> N bytes      
# PROCESS2 READS DATA FROM  WHEREEVER , LIKE N/2, N/3 ---> N+P  ( OVERLAPPING )    

# TO FIX THE ISSUE, ONE HAS TO CONSIDER THE "linkoffset" and "offset_lenghts" BETWEEN TABLES AND COLUMNS 
# i.e : LOOK FOR THE MEMORY SECTORS WHERE ODB STOPS AND STARTS READ/WRITE AND LINKS BETWEEN THE TABLES  ( 864 POOLS 368 TABLES !!!)

# QUICK FIX:  WITH PARALLELISATION, LOOPING OVER ALL THE POOLS IS PRETTY FAST ,WE HAVE JUSTE TO GET 
# THE DATA ARRAY IN MEMORY BY UNIQUE "entryno" AS A PRIMARY KEY.




__all__=["PoolSlicer"]


class PoolSlicer:
    def __init__(self, path ):
        self.path=path 
        db_obj= OdbObject( self.path )
        attr=db_obj.GetAttrib()
        self.poolmask= attr["poolmask"]
        self.MAX_NPOOL= 864   
        return None 
 
    def PoolRange (self):
        """
        Method : 
        CONVERT POOLS STRING TO INTEGER AND "DECIDES"  HOW THE 
        POOLS WOULD BE DISTRIBUTED ON PARALLEL PROCESSES 
        """

        ipools=[ int(i) for i in self.poolmask ]
    
        if   any(ipools)<99 and any(ipools)>= 1  and  max (ipools) > 99:  # BIG ODBs PRODUCED BY NBPOOL > 100, COULD REACH 864 
             range_=list(np.arange( 1, self.MAX_NPOOL +1  ))
             return range_
        elif min(ipools) >= 1 and  max(ipools) <=99:     # RELATIVELY BIG ODB 
             range_=list(np.arange(min(ipools),   max(ipools) +1 ))
             return range_
        elif min(ipools) == 1 and not any (ipools) >1 : # MONO POOL ODB 
             range_=1 
             return range_ 

    
    def Chunks(self,  l,  n, nslice  ):
        """
        Method:
        SPLIT THE POOL LIST 
        """

        Ndefault = 8
        if   len(l)    >  int(nslice):
           return [l[i:i+n] for i in range(0, len(l) , int(n) )]

        elif len(l)   <= int(nslice):
           n=Ndefault
           return [l[i:i+n] for i in range(0, len(l) , int(n) )]



    def GetPoolSets ( self, nslice =None  ):
        """
        Method :
        This METHOD SLICES THE POOLS SET 
        INTO EVENLY SIZED SLICES 
        """
        
        prange     = self.PoolRange()

        total      = len(prange  )
        chunk_size = round(total / int(nslice) )

        slices = self.Chunks( prange , chunk_size ,nslice   )
        slices.append(-999)
        return  slices 
