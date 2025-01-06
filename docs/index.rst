pyodb documentation 
=====================
This is the BETA release 1.1.0 of pyodb.
An interface written in C/Python API to access the  `ECMWF ODB <https://example.com>`_ databases.
Its aim is to access the ODB tables/columns data and meta data using a direct SQL request embadded in python scripts.

The main source code is written in pure C and the routines handling the ODB1 format has been 'pruned' from ECMWF ODB_API bundle-0.18.1 to build only the needed libraries

The original C code has been developed by "Sami Saarinen et al" at ECMWF during last 90s and 2008. 
Some modifictions have been done to make it compatible with `C/Python API https://docs.python.org/3/c-api/index.html`_.
