#-*-coding utf-8 -*- 
import os  , io, sys , re 
from   distutils.core import setup 

# Detect ReadTheDocs build 
if os.environ.get("READTHEDOCS") == "True":
    print("If in  ReadTheDocs build — skipping C extensions")
    setup(
        name="pyodb",
        version="1.1.0",
        packages=["pyodb_extra"],
        description="Fast ODB1 reader interface (doc-only mode)",
    )
    raise SystemExit(0)



from   distutils.core import setup ,Extension
import sysconfig
from   distutils.sysconfig import customize_compiler, get_config_vars
from   Cython.Build import cythonize
from   distutils import log
import numpy as np  
from pathlib import Path

try:
    from Cython.Distutils import build_ext
except ImportError:
    from setuptools.command.build_ext import build_ext



# VERSION 
__version__="1.1.0"



# PATH TO libodb.so
odb_install_dir = Path(__file__).parent.resolve()


# Compilation verbosity  !
log.set_verbosity(log.DEBUG)



class BuildModule:
    def __init__(self, name):
        self.name = name

    def Module(self, src, include):
        lib_dir = _auto_load_lib()
        print(f"Using ODB lib in: {lib_dir}")

        extra_compile_args = [
         "-fPIC", "-Wall", "-Wextra",
            "-Wsign-compare", "-Waddress",
            "-Wunused-variable", "-v"
        ]

        extra_link_args = [
            f"-Wl,-rpath,{lib_dir}"
        ]

        m = Extension(
            self.name,
            [src],
            include_dirs=[include, np.get_include()],
            library_dirs=[lib_dir],
            libraries=["odb"],  # libodb.so   lodb  . Without lib  and .so  
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args,
            language="c",
        )
        return m


class NoSuffixBuilder(build_ext):
    def get_ext_filename(self, ext_name):
        filename = super().get_ext_filename(ext_name)
        suffix = sysconfig.get_config_var('EXT_SUFFIX')
        ext = os.path.splitext(filename)[1]
        return filename.replace(suffix, "") + ext




def _auto_load_lib():
    odblib_path = os.getenv("ODB_INSTALL_DIR")
    if odblib_path is not None:
       odblib_path = odblib_path +"/lib"
    else:
        _thisdir_ = Path(__file__).parent.resolve()
        install_file = _thisdir_ / "odb_install_dir"
        if install_file.exists():
            odb_install_path = install_file.read_text().strip()
            libpath = Path(odb_install_path) / "lib"
            libfile = libpath / "libodb.so"
            if libfile.exists():
                return str(libpath)
            else:
                raise FileNotFoundError(f"libodb.so not found in {libpath}")
        else:
            raise FileNotFoundError(f"{install_file} does not exist")
    return odblib_path



def read(path):
    file_path = os.path.join(os.path.dirname(__file__), *path.split("/"))
    return io.open(file_path, encoding="utf-8").read()



def parse_version_from(path):
    version_file = read(path)
    version_match = re.search(r'^__version__ = "(.*)"', version_file, re.M)
    if version_match is None or len(version_match.groups()) > 1:
        raise ValueError("couldn't parse version")
    return version_match.group(1)



# EXTENSION SUFFIX  (only .so)
sfx  =  sysconfig.get_config_var('EXT_SUFFIX')

# SOURCE AND INCLUDE
pyc_src="pycc"
include="include"


# INSTANTIATE  MODULES BY NAME !
m      =BuildModule("pyodb")
pyodb  =m.Module(   "./pycc/main.c"     , include   )
module_list=[ pyodb ]

setup(  ext_modules = cythonize( module_list )     ,     
        cmdclass={"build_ext": NoSuffixBuilder}    
             )

quit()
