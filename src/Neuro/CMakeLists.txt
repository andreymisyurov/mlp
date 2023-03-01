cmake_minimum_required(VERSION 3.0)

project(MLP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

file(GLOB SOURCES
        main.cpp
        controller.cpp
        paintscene.cpp
        assistant.cpp
        Matrix/neuron_matrix.cc
        mtx_method/perzeptron_mtx.cc
        Parser/parser.cc
        Timer/timer.cc
)

file(GLOB HEADERS
        controller.h
        paintscene.h
        assistant.h
        Matrix/my_matrix.h
        Matrix/neuron_matrix.h
        mtx_method/perzeptron_mtx.h
        Parser/parser.h
        Perceptron/perceptron_abstract.h
        ThreadPool/thread_pool.h
        Timer/timer.h
)

file(GLOB FORMS
        controller.ui
)

file(GLOB RESOURCES
        resources.qrc
)

add_executable(MLP
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCES}
)

target_link_libraries(MLP
    Qt${QT_VERSION_MAJOR}::Widgets)
