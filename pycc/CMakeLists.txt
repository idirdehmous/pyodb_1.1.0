set(PYTHON_C_EXTENSION_SRCS    "info_module.c dump_module.c info_module.c" )
set(PYTHON_C_MODULE_NAME        pyodb)

# COMPILATION VERBOSITY LEVEL
set (ENV{VERBOSE}  "0" )
if     ( VERBOSE MATCHES "0"   )
        set (VERBOSE  --quiet  )
elseif ( VERBOSE MATCHES "1"   )
        set ( VERBOSE "" )
elseif  (VERBOSE MATCHES "2"   )
        set (VERBOSE --verbose )
endif ()

# TARGET 
Python_add_library(${CMAKE_PROJECT_NAME} MODULE WITH_SOABI
                  "${CMAKE_CURRENT_BINARY_DIR}/${f2py_module_c}"
                  "${F2PY_INCLUDE_DIR}/fortranobject.c"
                  "${fortran_src_file}"
                   )

# DEPENDANCIES 
target_link_libraries     (${CMAKE_PROJECT_NAME} PRIVATE Python::NumPy)
add_dependencies          (${CMAKE_PROJECT_NAME} genpyf)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${F2PY_INCLUDE_DIR}")


