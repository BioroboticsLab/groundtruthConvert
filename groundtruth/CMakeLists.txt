find_package(Boost 1.54.0 REQUIRED thread system filesystem serialization program_options)

set(LIBS
	${CPM_LIBRARIES}
	${OpenCV_LIBS}
	)

set(CMAKE_INCLUDE_CURRENT_DIR OFF)

file(GLOB_RECURSE groundtruthConvertSrc RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.c *.h *.hpp)

IF (NOT DEFINED BUILD_CLI)
    add_library(${CPM_LIB_TARGET_NAME} SHARED ${groundtruthConvertSrc})
	target_link_libraries(${CPM_LIB_TARGET_NAME} ${LIBS})
ELSE()
	set(EXE_NAME groundtruthConvertCLI)
    add_executable(${EXE_NAME} ${groundtruthConvertSrc})
	target_link_libraries(${EXE_NAME} ${LIBS})
ENDIF()
