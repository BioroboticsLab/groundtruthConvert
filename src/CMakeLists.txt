
set(LIBS
#    ${OPENGL_LIBRARIES}
	)

set(CMAKE_INCLUDE_CURRENT_DIR OFF)


message(STATUS "test")

#add_dependencies(${EXE_NAME} libbpgMod)

file(GLOB_RECURSE groundtruthConvertSrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h *.hpp)
#add_dependencies(${EXE_NAME} halideYuvConvPrec)
#target_link_libraries(${EXE_NAME} ${ALL_LIBS} ${LIBS} halideYuv420Conv)
set(EXE_NAME groundtruthConvert)
add_executable(${EXE_NAME} ${groundtruthConvertSrc} )
message("Done!")

