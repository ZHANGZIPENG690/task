















file(GLOB_RECURSE srcs
    ./application/*.c
    ./event/*.c
    ./core/*.c
    ../core/*.c
    ../drive/*.c
)
MESSAGE( STATUS ${srcs} )
idf_component_register(SRCS 
                                        
                    ${srcs}

                    INCLUDE_DIRS "../"
                    
                    )
                    
