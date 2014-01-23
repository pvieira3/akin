cmake_minimum_required(VERSION 2.6)
project(akin)



# Set path variables
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# Set include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/osgAkin)
include_directories(${OpenSceneGraph_INCLUDE_DIRS})


## TODO: Seperate the graphics out into their own library
file(GLOB lib_source "src/*.cpp" "include/*.h")
list(SORT lib_source)

file(GLOB unit_tests_source "test/*.cpp")
LIST(SORT unit_tests_source)

add_library(${PROJECT_NAME} SHARED ${lib_source})

# Find packages
    # OpenSceneGraph
find_package(OpenSceneGraph)
include_directories(${OpenSceneGraph_INCLUDE_DIRS})
add_definitions(${OpenSourceGraph_DEFINITIONS})
    # URDF_PARSE
option(HAVE_URDF_PARSE "add urdf parsing"  ON)
if( HAVE_URDF_PARSE ) #---------------------------

    file(GLOB parse_source "parsing/*.cpp")
    set(lib_source ${lib_source} ${parse_source} )

    # Check if the user is set up to parse URDFs
    find_package( urdfdom QUIET )
    if( urdfdom_FOUND )
       MESSAGE(STATUS "Found urdfdom -- We will compile a URDF parser")
       find_package( urdfdom_headers REQUIRED )
       include_directories( ${urdfdom_headers_INCLUDE_DIRS} )
       include_directories( ${urdfdom_INCLUDE_DIRS} )

       #message(STATUS "${urdfdom_LIBRARIES}")

       link_libraries( ${urdfdom_LIBRARIES} )

       find_package(Boost REQUIRED COMPONENTS system filesystem thread)
       include_directories(${BOOST_INCLUDE_DIRS})
       link_libraries( ${BOOST_LIBRARIES} )

    else( urdfdom_FOUND )
       MESSAGE(STATUS "Could NOT find urdfdom -- We will NOT compile a URDF parser")
    endif( urdfdom_FOUND )

endif( HAVE_URDF_PARSE ) #---------------------------

if( HAVE_URDF_PARSE )
   if( urdfdom_FOUND )
        set_property( TARGET ${PROJECT_NAME} PROPERTY COMPILE_DEFINITIONS "HAVE_URDF_PARSE" )
    endif( urdfdom_FOUND )
endif( HAVE_URDF_PARSE )


# Create osgAkin library
file(GLOB osg_source "osg/*.cpp" "osgAkin/*.h")
LIST(SORT osg_source)
add_library(osgAkin SHARED ${osg_source})


# Link to OSG and akin libraries
link_libraries(osg osgViewer osgManipulator osgGA osgDB osgAkin)


enable_testing()

# Build test programs
message(STATUS "\n -- UNIT TEST: ")
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
foreach(utest_src_file ${unit_tests_source})
    get_filename_component(test_base ${utest_src_file} NAME_WE)
    message(STATUS "Adding test ${test_base}")
    add_executable(${test_base} ${utest_src_file})
    target_link_libraries(${test_base} ${PROJECT_NAME} ${OpenSceneGraph_LIBRARIES})
    add_test(${test_base} ${EXECUTABLE_OUTPUT_PATH}/${test_base})
    add_custom_target(${test_base}.run ${test_base} ${ARGN})
    add_dependencies(check ${test_base})
endforeach(utest_src_file)
