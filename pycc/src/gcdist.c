#define PY_SSIZE_T_CLEAN
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "Python.h"
// THIS CODE IS BASED ON THE Roger Bivand  (2005-2009)  
// DEVELOPED ORIGINALLY FOR THE sp R PACKAGE !
/*  Copyright by Roger Bivand (C) 2005-2009  */

/* Adapted to Python/C API by   IDIR DEHMOUS */ 
// 28-11-2024 
/* Copyright  by Idir DEHMOUS (C)  2024 */


#define   EPSILON  pow(1, -52)
#define POWDI(x,i) pow(x,i)
#define pythag(a,b) sqrt(a*a+b*b)





static PyObject* gcMatrix_method    (PyObject* Py_UNUSED(self), PyObject *args) {

int N =0       ; 
PyObject *lats ;
PyObject *lons ;


if (!PyArg_ParseTuple (args , "OOi", &lons, &lats, &N )) {
        return NULL ;
}


PyObject *dist_list =PyList_New(0); 


    double degrees2radians = 3.14159/180.0;
    double F, G, L, sinG2, cosG2, sinF2, cosF2, sinL2, cosL2, S, C; 
    double lon1R , lat1R , lon2R , lat2R   ;
    double w, R ;
    double D,H1 , H2 ,a  ,f ; 
    double dist[1] ;


    

    a = 6378.137;
    f = 1.0/298.257223563;

    Py_ssize_t ii;
    Py_ssize_t jj;    
    for(ii=0; ii < N ; ii++){
       for(jj=0; jj < N; jj++){


        PyObject* lon1=  PyList_GetItem(  lons, ii  ) ;
        PyObject* lon2=  PyList_GetItem(  lons, jj  ) ;
	    
	PyObject* lat1=  PyList_GetItem(  lats, ii  ) ; 
	PyObject* lat2=  PyList_GetItem(  lats, jj  ) ; 

        double llon1 = PyFloat_AsDouble(lon1 );
        double llon2 = PyFloat_AsDouble(lon2 );

        double llat1 = PyFloat_AsDouble(lat1 );
        double llat2 = PyFloat_AsDouble(lat2 );


     if (fabs(llat1 - llat2) <   EPSILON) {
        if (fabs(fmod(llon1 - llon2 , 360.0)) <  EPSILON) {
            dist[0] = 0.0;
        }
    }



        lon1R = llon1 * degrees2radians;
        lat1R = llat1 * degrees2radians;
        lon2R = llon2 * degrees2radians;
        lat2R = llat2 * degrees2radians;

    	F = ( lat1R + lat2R )/2.0;
        G = ( lat1R - lat2R )/2.0;
        L = ( lon1R - lon2R )/2.0;
       sinG2 = POWDI( sin( G ), 2 );
       cosG2 = POWDI( cos( G ), 2 );
       sinF2 = POWDI( sin( F ), 2 );
       cosF2 = POWDI( cos( F ), 2 );
       sinL2 = POWDI( sin( L ), 2 );
       cosL2 = POWDI( cos( L ), 2 );


       S = sinG2*cosL2 + cosF2*sinL2;
       C = cosG2*cosL2 + sinF2*sinL2;

       w = atan( sqrt( S/C ) );
       R = sqrt( S*C )/w;

      if (  isnan ( R) ) {
           R = 0.0  ; 
               }

      D  =  2*w*a;
      H1 = ( 3*R - 1 )/( 2*C );
      H2 = ( 3*R + 1 )/( 2*S );

      if ( isinf ( H2 )  ) {
         H2 =0.0  ; 
      }
      if ( isinf ( H1 ) )  {
         H1 = 0.0 ; 
      }

      dist[0] = D*( 1 + f*H1*sinF2*cosG2 - f*H2*cosF2*sinG2 ); 

      PyObject*   dd   =PyFloat_FromDouble(   dist[0]  ) ;

      PyList_Append( dist_list  , dd   )  ; 


	    }//jj
	}//ii




/*   Haversine formula  
        dlon = radlon2 - radlon1;
        dlat = radlat2 - radlat1;
	double term1=pow (sin (dlat/2.0)  , 2.)  ;  
        double term2=pow ((dlon/2.) ,2.);
	double t=  term1  + cos(radlat1)*cos(radlat2) * term2   ; 
	d  =  2.0 * 6367.0  * asin (   sqrt(  t  ) ) ; 
        printf (  "dist :%f\n" ,  d  )  ;
        d = pow(sin(dlat/2.0), 2.0)  + cos(iter_lat1) * cos(iter_lat2) * pow(sin(dlon/2.0), 2.0);
        PyObject*  d =PyFloat_FromDouble((double)( km_conversion * asin(sqrt(km))) ) ;
        PyList_Append( dists, d     ) ; */

return dist_list ; 
 }

