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

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/qcustomplot
        ${CMAKE_CURRENT_LIST_DIR}/Matrix
        ${CMAKE_CURRENT_LIST_DIR}/mtx_method
        ${CMAKE_CURRENT_LIST_DIR}/data_weights
        ${CMAKE_CURRENT_LIST_DIR}/Parser
        ${CMAKE_CURRENT_LIST_DIR}/Perceptron
        ${CMAKE_CURRENT_LIST_DIR}/ThreadPool
        ${CMAKE_CURRENT_LIST_DIR}/Timer
        ${CMAKE_CURRENT_LIST_DIR}/learn_answer
)

# build for learning
set(LEARNING_HEADERS
        Matrix/my_matrix.h
        Matrix/neuron_matrix.h
        mtx_method/perzeptron_mtx.h
        Timer/timer.h
        Parser/parser.h
        )

set(LEARNING_SOURCES
        learning_test.cc
        Matrix/neuron_matrix.cc
        mtx_method/perzeptron_mtx.cc
        Timer/timer.cc
        Parser/parser.cc
        )

add_executable(learning
        ${LEARNING_SOURCES}
        ${LEARNING_HEADERS}
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
        Matrix/neuron_matrix.h
        mtx_method/perzeptron_mtx.h
        Timer/timer.h
        )

set(TEST_SOURCES
        tests.cpp
        Matrix/neuron_matrix.cc
        mtx_method/perzeptron_mtx.cc
        Timer/timer.cc
        )

add_executable(test
        ${TEST_SOURCES}
        ${TEST_HEADERS}
        )

target_link_libraries(test
        gtest_main
        )

# build MLP
set(SOURCES
        main.cpp
        controller.cpp
        paintscene.cpp
        assistant.cpp
        mtx_method/perzeptron_mtx.cc
        Parser/parser.cc
        Timer/timer.cc
        qcustomplot/qcustomplot.cpp
        learn_answer/learnanswer.cpp
        )

set(HEADERS
        controller.h
        paintscene.h
        assistant.h
        Matrix/my_matrix.h
        mtx_method/perzeptron_mtx.h
        Parser/parser.h
        Perceptron/perceptron_abstract.h
        ThreadPool/thread_pool.h
        Timer/timer.h
        qcustomplot/qcustomplot.h
        learn_answer/learnanswer.h
)

set(RESOURCES
        resource.qrc
        )

set(FORMS
        controller.ui
        learn_answer/learnanswer.ui
        custom_graph.ui
        )

# create neuron_matrix library
set(LIB_NAME neuronmatrix)
set(NEURO Matrix/neuron_matrix.cc Matrix/neuron_matrix.h)
add_library(${LIB_NAME} STATIC ${NEURO})

add_executable(MLP
        ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES}
)

target_link_libraries(MLP
        PUBLIC Qt::Multimedia
        PUBLIC Qt6::Widgets
        PUBLIC Qt6::Core
        PUBLIC Qt6::PrintSupport
        PUBLIC neuronmatrix
        )
