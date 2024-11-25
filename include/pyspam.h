#define PY_SSIZE_T_CLEAN
#include  <stdio.h> 	
#include  "Python.h"
#include  "odbdump.h"

#define SMAX 50 


// Formatting floats
double  format(double value , char *fmt ) {
static  char str [SMAX ] ;
double    x  ;
sprintf(str, fmt , value );
x = atof(str );
return   x ;

}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


void strtonum(char [], int);
void strtonum(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}


extern  int get_strlen( const char*    string  ) ;


int get_strlen (  const char *  string  ) {
int str_len  ;

if ( string  ) {
str_len=  strlen( string );
} else  {
str_len=0   ;
};

return str_len ;
}



//extern void pprint ( const char *  string ) {   }


// Update lists items through pointers 
//static PyObject*  list_reset(PyObject* *list_col , Py_ssize_t ncols )
//{    *list_col = PyList_New (ncols) ;
//     return *list_col ;
//}

//static PyObject* reset_counter  (PyObject*  nrows ) {
//    PyObject* n ;
//    return PyLong_FromLong(n) ; 
//}
