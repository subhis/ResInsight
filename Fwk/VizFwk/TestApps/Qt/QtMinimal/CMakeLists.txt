cmake_minimum_required(VERSION 2.8.12)

project(QtMinimal)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ${CEE_BASE_CXX_FLAGS}")

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wno-long-long")
endif()


find_package(OpenGL)

if (CEE_USE_QT5)
	find_package(Qt5 COMPONENTS Core Gui OpenGL Widgets REQUIRED)
else()
	find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
    include(${QT_USE_FILE})
	if (QT4_FOUND)
		message(STATUS "Found Qt4")
	endif(QT4_FOUND)
endif(CEE_USE_QT5)

include_directories(${LibCore_SOURCE_DIR})
include_directories(${LibGeometry_SOURCE_DIR})
include_directories(${LibRender_SOURCE_DIR})
include_directories(${LibViewing_SOURCE_DIR})
include_directories(${LibGuiQt_SOURCE_DIR})

set(CEE_LIBS LibGuiQt LibViewing LibRender LibGeometry LibCore)
 

set(CEE_SOURCE_FILES
QMMain.cpp
QMMainWindow.cpp
QMWidget.cpp
)

# Headers that need MOCing
set(MOC_HEADER_FILES
QMMainWindow.h
QMWidget.h
)

# Run MOC on the headers
add_definitions(-DCVF_USING_CMAKE)
set(MOC_SOURCE_FILES)
if (CEE_USE_QT5)
	qt5_wrap_cpp(MOC_SOURCE_FILES ${MOC_HEADER_FILES} )
    set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Widgets)
else()
	qt4_wrap_cpp(MOC_SOURCE_FILES ${MOC_HEADER_FILES} )
endif()



add_executable(${PROJECT_NAME} ${CEE_SOURCE_FILES} ${MOC_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CEE_LIBS} ${OPENGL_LIBRARIES} ${QT_LIBRARIES})

if (CEE_USE_QT5)
    foreach (qtlib ${QT_LIBRARIES})
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${qtlib}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
	endforeach(qtlib)
	# Copy Qt Dlls	
else()
	# Copy Qt Dlls
	if (MSVC)
	    set (QTLIBLIST QtCore QtGui QtOpenGl)
	    foreach (qtlib ${QTLIBLIST})
	        
	        # Debug
	        execute_process(COMMAND cmake -E copy_if_different ${QT_BINARY_DIR}/${qtlib}d4.dll ${CMAKE_BINARY_DIR}/Debug/${qtlib}d4.dll)
	        
	        # Release
	        execute_process(COMMAND cmake -E copy_if_different ${QT_BINARY_DIR}/${qtlib}4.dll ${CMAKE_BINARY_DIR}/Release/${qtlib}4.dll)
	    endforeach( qtlib )
	endif(MSVC)
endif(CEE_USE_QT5)
