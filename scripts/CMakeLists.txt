find_package(pybind11 REQUIRED)
find_package(Python3 REQUIRED)

pybind11_add_module(map_module map.cpp)

target_link_libraries(map_module PRIVATE
                      map)

include(GNUInstallDirs)

install(TARGETS map_module
    COMPONENT python
    RUNTIME DESTINATION "${Python3_SITEARCH}"
    LIBRARY DESTINATION "${Python3_SITEARCH}"
    ARCHIVE DESTINATION "${Python3_SITEARCH}")
