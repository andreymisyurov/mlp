cmake_minimum_required(VERSION 3.0)

project(MLP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets PrintSupport Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets PrintSupport Multimedia REQUIRED)
find_program(LEAKS "leaks")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/qcustomplot
        ${CMAKE_CURRENT_LIST_DIR}/Matrix
        ${CMAKE_CURRENT_LIST_DIR}/mtx_method
        ${CMAKE_CURRENT_LIST_DIR}/grph_method
        ${CMAKE_CURRENT_LIST_DIR}/data_weights
        ${CMAKE_CURRENT_LIST_DIR}/Parser
        ${CMAKE_CURRENT_LIST_DIR}/Perceptron
        ${CMAKE_CURRENT_LIST_DIR}/ThreadPool
        ${CMAKE_CURRENT_LIST_DIR}/Timer
        ${CMAKE_CURRENT_LIST_DIR}/learn_answer
)

# create libraries
add_library(neuronmatrix STATIC Matrix/neuron_matrix.cc Matrix/neuron_matrix.h)
add_library(timer STATIC Timer/timer.h Timer/timer.cc)
add_library(matrix_method mtx_method/perzeptron_mtx.h mtx_method/perzeptron_mtx.cc)
add_library(graph_method grph_method/perzeptron_grph.h grph_method/perzeptron_grph.cc)
add_library(parser Parser/parser.h Parser/parser.cc)

# build for learning
set(LEARNING_HEADERS
        Matrix/my_matrix.h
        )

set(LEARNING_SOURCES
        learning_test.cc
        )

add_executable(learning
        ${LEARNING_SOURCES}
        ${LEARNING_HEADERS}
)

target_link_libraries(learning
        PRIVATE neuronmatrix
        PRIVATE matrix_method
        PRIVATE parser
        PRIVATE timer
        )

# build tests
# connect gtest library from github
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

set(TEST_HEADERS
        Matrix/my_matrix.h
        )

set(TEST_SOURCES
        tests.cpp
        )

add_executable(test
        ${TEST_SOURCES}
        ${TEST_HEADERS}
        )

target_link_libraries(test
        PRIVATE gtest_main
        PRIVATE neuronmatrix
        PRIVATE matrix_method
        PRIVATE timer
        PRIVATE parser
        )

# build MLP
set(SOURCES
        main.cpp
        controller.cpp
        paintscene.cpp
        assistant.cpp
        qcustomplot/qcustomplot.cpp
        learn_answer/learnanswer.cpp
#        grph_method/perzeptron_grph.сс
        )

set(HEADERS
        controller.h
        paintscene.h
        assistant.h
        Matrix/my_matrix.h
        Perceptron/perceptron_abstract.h
        ThreadPool/thread_pool.h
        qcustomplot/qcustomplot.h
        learn_answer/learnanswer.h
#        grph_method/perzeptron_grph.h
)

set(RESOURCES
        resource.qrc
        )

set(FORMS
        controller.ui
        learn_answer/learnanswer.ui
        )


set(MACOSX_BUNDLE_ICON_FILE resources/icon.icns)

# And the following tells CMake where to find and install the file itself.
set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(MLP
        MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES} ${app_icon_macos}
)

target_link_libraries(MLP
        PRIVATE Qt::Multimedia
        PRIVATE Qt6::Widgets
        PRIVATE Qt6::Core
        PRIVATE Qt6::PrintSupport
        PRIVATE neuronmatrix
        PRIVATE timer
        PRIVATE matrix_method
        PRIVATE graph_method
        PRIVATE parser
        )

if(LEAKS)
        set(MEMCHECK_COMMAND ${LEAKS} -nocontext)
endif()
add_test(NAME MyTest COMMAND ${MEMCHECK_COMMAND} $<TARGET_FILE:MLP>)
#add_test(NAME MyTest COMMAND ${MEMCHECK_COMMAND} /Users/gonzo/program/school21/general/CPP7_MLP-0/src/Neuro/cmake-build-debug)

add_executable(graph_test
        grph_method/perzeptron_grph.h
        grph_method/perzeptron_grph.cc
        grph_method/test.cc
        )

target_link_libraries(graph_test
        PRIVATE parser
        PRIVATE neuronmatrix
        PRIVATE matrix_method
        PRIVATE timer
)

