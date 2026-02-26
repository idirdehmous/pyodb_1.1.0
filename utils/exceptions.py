# -*- coding: utf-8 -*-

__all__ =[  "pyodbErrMessage"        ,
            "pyodbBinError"          ,
            "pyodbLibError"          ,
            "pyodbWarning"           ,
            "pyodbInterfaceError"    ,
            "pyodbDatabaseError"     ,
            "pyodbProgrammingError"  ,
            "pyodbIntegrityError"    ,
            "pyodbDataError"         ,
            "pyodbNotSupportedError" ,
            "pyodbPathError"         ,
            "pyodbEnvError"          ,
            "pyodbInternalError"     ,
            "pyodbInstallError" ]




class pyodbErrMessage:
      def __init__(self):
          return None 
      def _ErrMsg(self):
         msg_list=[ 
       "\n--Path to ODB_INSTALL_DIR is {} is 'None' or not found!\n--Please"+ \
               " export ODB_INSTALL_DIR=/path/../../ where libodb.so is installed",                         # 0

       "\n--Can't find the path to the ODB binaries\n--Set the ODB_INSTALL_DIR nvironement variable",          # 1

       "\n--Something went wrong while loading libodb.so\n--Try to check  the ODB_INSTALL_DIR environment variable",# 2 

       "\n--cdll module can't find shared library libodb.so\n--Please check"+ \
            "that it has been installed  and ODB_INSTALL_LIB var is set",                                   # 3

       "\n--The path {} not found !",                                                                       # 4

       "\n--The ODB env variable{}name not recongnized !",                                                  # 5

       "\n--The ODB doesn't contain .dd   file. Must be : {}.dd",                                           # 6

       "\n--The ODB doesn't contain .sch  file. Must be : {}.sch",                                          # 7

       "\n--{} path not found !"                                                                            # 8
            ]
         return msg_list

class pyodbBinError(Exception):
    """
    ODB needed bin executables 
    """
    pass 
class  pyodbLibError(Exception):
    """
    ODB needed libraries 
    """
    pass 

class  pyodbWarning(Exception):
    """
    Some warnings messages if they exist
    """
    pass 

class  pyodbInterfaceError(Exception):
    """
    C/Python API communication errors 
    """
    pass


class  pyodbDatabaseError(Exception):
    """
    ODB internal structure error 
        corrupted odb 
    """
    pass 

class  pyodbProgrammingError(Exception):
    """
    Programming Error sepcially from the backend side (C language)
    """
    pass 
class  pyodbIntegrityError(Exception):
    """
    ODB data integrity error 
    """
    pass 

class  pyodbDataError(Exception):
    """
    Arrays length , data types  etc  
    """
    pass 
class  pyodbNotSupportedError(Exception):
    """
    Versions and modeules 
    """
    pass 

class pyodbPathError (Exception):
    """
    Checks path and raises Errors !
    """
    pass 
class pyodbEnvError(Exception):
    """
    Raises error if a non environmental odb variables is set !
    """ 
    pass 
class pyodbInternalError(Exception):
    """
    Raises error if a problem is found inside the C code or Python/C communication !
    """
    pass
class pyodbInstallError(Exception):
    """
    Raises an error if something wrong occured during building the modules
    """
    pass 
