cmake_minimum_required(VERSION 3.5)

project(qtools)

set(SRC_LIST ${PROJECT_SOURCE_DIR}/src/QAccount.cpp
    ${PROJECT_SOURCE_DIR}/src/QCalendar.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(qtools_so SHARED ${SRC_LIST})
set_target_properties(qtools_so PROPERTIES OUTPUT_NAME "qtools")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
