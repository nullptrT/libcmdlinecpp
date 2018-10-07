
if( BUILD_CMDLINECPP_TESTS )
    add_executable( cmdlinecpp_test main.cpp )
	target_link_libraries( cmdlinecpp_test cmdlinecpp )
	
    add_executable( cmdlinecpp_test2 main2.cpp )
	target_link_libraries( cmdlinecpp_test2 cmdlinecpp )
endif()
