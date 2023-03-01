cmake_minimum_required(VERSION 3.0)

project(MLP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# подключение флагов компилляции для gcov репорта
# -g флаг для дебаг отчета
# -O0 режим оптимизации кода компилятором
# -fprofile-arcs
# -ftest-coverage

# собрать отчет командами
# make
# ./test
# lcov -t "test" -o test.info -c -d .
# genhtml -o report test.info
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")

set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets PrintSupport Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets PrintSupport Multimedia REQUIRED)
find_program(LEAKS "leaks")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/qcustomplot
        ${CMAKE_CURRENT_LIST_DIR}/matrix
        ${CMAKE_CURRENT_LIST_DIR}/mtx_method
        ${CMAKE_CURRENT_LIST_DIR}/grph_method
        ${CMAKE_CURRENT_LIST_DIR}/data_weights
        ${CMAKE_CURRENT_LIST_DIR}/parser
        ${CMAKE_CURRENT_LIST_DIR}/perceptron
        ${CMAKE_CURRENT_LIST_DIR}/thread_pool
        ${CMAKE_CURRENT_LIST_DIR}/timer
        ${CMAKE_CURRENT_LIST_DIR}/learn_answer
        ${CMAKE_CURRENT_LIST_DIR}/paintscene
        ${CMAKE_CURRENT_LIST_DIR}/assistant
)

# create static libraries
add_library(neuronmatrix STATIC matrix/neuron_matrix.cc matrix/neuron_matrix.h)
add_library(matrix_method STATIC mtx_method/perzeptron_mtx.h mtx_method/perzeptron_mtx.cc)
add_library(graph_method STATIC grph_method/perzeptron_grph.h grph_method/perzeptron_grph.cc)
add_library(timer STATIC timer/timer.h timer/timer.cc)
add_library(parser STATIC parser/parser.h parser/parser.cc)

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

# build test
add_executable(test
        tests.cpp
        matrix/my_matrix.h
        )

target_link_libraries(test
        PRIVATE gtest_main
        PRIVATE neuronmatrix
        PRIVATE matrix_method
        PRIVATE graph_method
        PRIVATE timer
        PRIVATE parser
        )

# build MLP
set(SOURCES
        main.cpp
        controller.cpp
        paintscene/paintscene.cpp
        assistant/assistant.cpp
        qcustomplot/qcustomplot.cpp
        learn_answer/learnanswer.cpp
        )

set(HEADERS
        controller.h
        paintscene/paintscene.h
        assistant/assistant.h
        matrix/my_matrix.h
        perceptron/perceptron_abstract.h
        thread_pool/thread_pool.h
        qcustomplot/qcustomplot.h
        learn_answer/learnanswer.h
)

set(RESOURCES resource.qrc)
set(FORMS controller.ui learn_answer/learnanswer.ui)

set(MACOSX_BUNDLE_ICON_FILE resources/logo.png)
set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/resources/logo.png")
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

add_executable(stress_test
        matrix/my_matrix.h
        perceptron/perceptron_abstract.h
        thread_pool/thread_pool.h
        stresstest.cc
        )

target_link_libraries(stress_test
        PRIVATE parser
        PRIVATE neuronmatrix
        PRIVATE matrix_method
        PRIVATE timer
        PRIVATE graph_method
        )
