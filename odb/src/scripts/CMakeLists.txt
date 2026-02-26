# WE NEED ONLY dcgan and other dcagen/dcagen.x dependencies 
list(APPEND _odb_scripts
    create_ioassign
    dcafix
    dcagen
    dcaquick
    odb_prune
    odbprune
    set_env.sh  
)


install(PROGRAMS ${_odb_scripts} DESTINATION bin)

foreach(script ${_odb_scripts})
    configure_file(${script} ${CMAKE_BINARY_DIR}/bin COPYONLY)
endforeach()
