cmake_minimum_required(VERSION 3.5)

# The following lines of boilerplate have to be in your project's CMakeLists.txt
# or it will fail to be a valid project.
set(EXTRA_COMPONENT_DIRS "../../sdk")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32_demo)
