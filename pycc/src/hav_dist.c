#define PY_SSIZE_T_CLEAN
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
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





static  PyObject*  Haversine_method(PyObject *self, PyObject* args ) 
{

int N   ;  // list length 
PyObject*  lons  ;
PyObject*  lats  ;

PyObject * dist_list =PyList_New(0) ; 
if (!PyArg_ParseTuple (args , "OOi" , &lons , &lats , &N)) {
        return NULL ;
    }


//*kms  = PyList_New(0)  ;

  /*  if (   lon1 == NULL 
        || lon2 == NULL 
        || lat1 == NULL 
        || lat2 == NULL
        || kms == NULL){
        return -1;
    }*/

    double d, dlon, dlat;
    double radlon1, radlon2, radlat1, radlat2;

    double km_conversion = 2.0 * 6367.0; 
    double degrees2radians = 3.14159/180.0;


    double F, G, L, sinG2, cosG2, sinF2, cosF2, sinL2, cosL2, S, C; 
    double lon1R , lat1R , lon2R , lat2R   ;
    double w, R ;
    double D,H1 , H2 ,a  ,f ; 
    double dist[1] ;


    a = 6378.137;
    f = 1.0/298.257223563;


    Py_ssize_t ii;
    Py_ssize_t jj ; 
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
//            return PyLong_FromLong( 0)  ;
        }
    }

//       printf ( "lon1=  %f     lon2=  %f      lat1=  %f      lat2=  %f \n" ,     llon1 , llon2 , llat1   ,llat2  ); 


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

      // printf (   "%f   %f   %f  %f  %f  %f      %f  %f  %f   %ld     %ld\n" , sinG2, cosG2, sinF2 , cosF2 , sinL2, cosL2 , F, G , L , ii , jj  ); 
      // printf (   "S %f   C %f    w %f    R %f     D %f     H1 %f      H2 %f    %ld    %ld\n", S, C , w, R  , D , H1 , H2, ii, jj ) ; 
      // printf (   "Distance  %f    %ld     %ld \n " , dist[0] , ii , jj)  ; 
       
      PyObject*   dd  =PyFloat_FromDouble(   dist[0]  ) ;
      PyList_Append( dist_list ,   dd   )  ; 


/*d= 2r arcsin sqrt(sin**2( (lat2-lat1 )/2)+ cos(lat1)cos(lat2)*sin**2( (lon2-lon1)/2.)      	
 *  Haversine formula  
        dlon = radlon2 - radlon1;
        dlat = radlat2 - radlat1;
	double term1=pow (sin (dlat/2.0)  , 2.)  ;  
        double term2=pow ((dlon/2.) ,2.);
	double t=  term1  + cos(radlat1)*cos(radlat2) * term2   ; 
	d  =  2.0 * 6367.0  * asin (   sqrt(  t  ) ) ; 
        printf (  "dist :%f\n" ,  d  )  ; */
        //d = pow(sin(dlat/2.0), 2.0)  + cos(iter_lat1) * cos(iter_lat2) * pow(sin(dlon/2.0), 2.0);
        //PyObject*  d =PyFloat_FromDouble((double)( km_conversion * asin(sqrt(km))) ) ;
        
       //PyList_Append( dists, d     ) ; 
    }
    }
    return dist_list  ;
}





/*void  gdists(double *u, double *v, double *uout, double *vout, int *n ){
	int N = *n, i  , j;
	double gc[1];
         for (  i=0 ; i < N ; i++ ) {
              	for (j = 0; j < N; j++) {
                       gcdist(u+i, uout+j, v+i, vout+j, gc);

		       printf ( "Distances: %d  %d   %f\n"  , i , j , gc[0] ) ;
		}
	     }
}*/

/* http://en.wikipedia.org/wiki/World_Geodetic_System#A_new_World_Geodetic_System:_WGS_84 */
void gcdist(double *lon1, double *lon2, double *lat1, double *lat2,
		double *dist) {


    double F, G, L, sinG2, cosG2, sinF2, cosF2, sinL2, cosL2, S, C;
    double w, R, a, f, D, H1, H2;
    double lat1R, lat2R, lon1R, lon2R, DE2RA;

    DE2RA = M_PI/180;
    a = 6378.137;              /* WGS-84 equatorial radius in km */
    f = 1.0/298.257223563;     /* WGS-84 ellipsoid flattening factor */

    if (fabs(lat1[0] - lat2[0]) <   EPSILON) {
        if (fabs(fmod(lon1[0] - lon2[0], 360.0)) <  EPSILON) {
            dist[0] = 0.0;
            return;
        }
    }
    lat1R = lat1[0]*DE2RA;
    lat2R = lat2[0]*DE2RA;
    lon1R = lon1[0]*DE2RA;
    lon2R = lon2[0]*DE2RA;

    F = ( lat1R + lat2R )/2.0;
    G = ( lat1R - lat2R )/2.0;
    L = ( lon1R - lon2R )/2.0;

	
 /*
  printf("%g %g %g %g; %g %g %g\n",  *lon1, *lon2, *lat1, *lat2, F, G, L);
*/	

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

    D = 2*w*a;
    H1 = ( 3*R - 1 )/( 2*C );
    H2 = ( 3*R + 1 )/( 2*S );


    dist[0] = D*( 1 + f*H1*sinF2*cosG2 - f*H2*cosF2*sinG2 );
     printf("%f  %f   %f  %f   %f\n"   ,  w , R , H1 , H2  , dist[0] ) ;

}




static PyObject *gcDistance_method  (PyObject* Py_UNUSED(self)  , PyObject*  args )
{

Py_Initialize(); 
int N   ;  // list length 
PyObject*  lons  ; 
PyObject*  lats  ; 

if (!PyArg_ParseTuple (args , "OOi" , &lons , &lats , &N)) {            
	return NULL ;
    }

Py_ssize_t  i ;
Py_ssize_t  j ; 

PyObject*  pydist =  PyList_New(0) ; 
PyObject* dd =0 ; 

double gc [1] ;
gc[0]=0. ; 

for (  i=0 ; i < N ; i++ ) {
     for ( j = 0; j < N  ; j++) {
       PyObject* lon1=  PyList_GetItem(  lons, i  ) ; 
       PyObject* lon2=  PyList_GetItem(  lons, j ) ;

       double    llon1 = PyFloat_AsDouble(lon1);
       double    llon2 = PyFloat_AsDouble(lon2);


       PyObject* lat1=  PyList_GetItem(  lats, i  ) ;
       PyObject* lat2=  PyList_GetItem(  lats, j  ) ;

       double   llat1 = PyFloat_AsDouble(lat1 );
       double   llat2 = PyFloat_AsDouble(lat2 );



        gcdist(&llon1, &llon2, &llat1, &llat2, gc);

	if ( gc[0] == 0.0 ) {
//        dd = PyFloat_FromDouble( (double)gc[0]);
//	PyList_Append(  pydist , dd  )  ;  

//        printf ( "lon1= %f  lon2= %f    lat1= %f    lat2= %f    Distance :%f\n" , llon1,llon2 , llat1 , llat2  , gc[0]) ; 
	break ; 
	}
          }  /* j  */
             
      }  /* i */ 
return  pydist  ;
}
