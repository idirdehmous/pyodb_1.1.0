#-*-coding utf-8 -*- 
import os  , io, sys , re 
from   distutils.core import setup ,Extension
import sysconfig
from   Cython.Distutils import build_ext
from   distutils.sysconfig import customize_compiler, get_config_vars
from   Cython.Build import cythonize


# VERSION 
__version__="1.1.0" 


# COMPILER FLAGS 
extra_compile_args = sysconfig.get_config_var('CFLAGS').split()
extra_compile_args += [ "-fPIC" ,"-Wall", "-Wextra", 
                        "-Wsign-compare","-Waddress",
                        "-Wunused-variable","-Wunused-variable"]
# EXTENSION SUFFIX 
sfx  =  sysconfig.get_config_var('EXT_SUFFIX')

# CWD 
pwd=os.getcwd()

# The path to the ODB install directory is supposed to be written already by cmake. 
odbpath_file=pwd+"/odb_install_dir"
if os.path.isfile( odbpath_file ):
   print("-"*50)
   print( "--odb_install_dir file FOUND !" , pwd+"/odb_install_dir" )
   print(" "  )
else:
   print("--Cmake failed to find '"+odbpath_file \
        +"' file.\n--Please check the odb installation" )
   sys.exit(0)



# GET THE PATH FROM THE FILE 
_file_ =  open( odbpath_file  , "r" )
odb_install_dir = _file_.readline().rstrip()


# PATHS
odb_env    =odb_install_dir
libname    ="libodb.so"
libpath    =odb_env+"/lib/"+libname


if os.path.isfile( libpath ):
   print("-"*50)
   print( "--{} FOUND !".format( libpath  )    )
   print(" "  )
else:
   print("--Path to {} NOT FOUND. Please check the odb"\
        +" installation, or the file ".format( odb_install_dir ) )


# SOURCE AND INCLUDE 
pyc_src="pycc"
include="include"


class BuildModule:
    def __init__ (self ,name   ):
        self.name = name
        return None 
    def Module ( self , src , include, libs , *args):
        self.src=src 
        self.inc=include 
        self.lib=libs 
        self.language='c'
        self.maj_version=0
        self.min_version=1
        self.patch=0 
        m=Extension( self.name, [ self.src ],
                  include_dirs =[ self.inc ],
                  extra_objects=[ self.lib ], 
                  extra_compile_args=extra_compile_args,
                  language     =self.language )
        return m  


class NoSuffixBuilder(build_ext):
    def get_ext_filename(self, ext_name):
        filename = super().get_ext_filename(ext_name)
        suffix = sysconfig.get_config_var('EXT_SUFFIX')
        ext = os.path.splitext(filename)[1]
        return filename.replace(suffix, "") + ext


def read(path):
    file_path = os.path.join(os.path.dirname(__file__), *path.split("/"))
    return io.open(file_path, encoding="utf-8").read()

def parse_version_from(path):
    version_file = read(path)
    version_match = re.search(r'^__version__ = "(.*)"', version_file, re.M)
    if version_match is None or len(version_match.groups()) > 1:
        raise ValueError("couldn't parse version")
    return version_match.group(1)



# INSTANTIATE  MODULES BY NAME !
m      =BuildModule("pyodb")
pyodb  =m.Module(   "./pycc/main.c"     , include , libpath)
module_list=[ pyodb ]

# SETUP  
version="_1.1.0"
setup(
    ext_modules = cythonize( module_list )     , 
    cmdclass={"build_ext": NoSuffixBuilder}    
)
quit()
