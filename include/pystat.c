#define PY_SSIZE_T_CLEAN
#define mdi 2147483647
#include  <stdio.h>
#include  "Python.h"
#include  "odbdump.h"

#define SMAX 50 




#define MEGA ((double)1048576.0)

#define PRINT_TIMING(txt, nrows, ncols) \
if (print_title && print_newline) { \
  double wthis  = util_walltime_(); \
  double wdelta = wthis - wlast; \
  Bool packed = true ; \
  /*if (txt) printf("# %s in %.3f secs\n", txt, wdelta); */\
if (nrows > 0 && ncols > 0) { \
    double rows_per_sec = nrows/wdelta; \
    ll_t nbytes = nrows * ((ll_t) ncols) * ((ll_t) sizeof(double)); \
    double MBytes_per_sec = (nbytes/wdelta)/MEGA; \
    char *dotified = lld_dotify(nbytes); \
    fprintf(fp, "# Total %lld row%s, %d col%s, %s %sbytes in %.3f secs : %.0f rows/s, %.0f MB/s\n", \
            (long long int)nrows, (nrows != 1) ? "s" : "",              \
            ncols, (ncols != 1) ? "s" : "", \
            dotified, packed ? "packed-" : "", \
            wdelta, rows_per_sec, MBytes_per_sec); \
    FREE(dotified); \
  } \
  if (txt || (nrows > 0 && ncols > 0)) wlast = wthis; \
}

