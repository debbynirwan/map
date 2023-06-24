add_subdirectory(tests)
add_library(map INTERFACE)

target_include_directories(map
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/map>
)

set_target_properties(map PROPERTIES PUBLIC_HEADER "map.h;map.hpp")

include(GNUInstallDirs)

install(TARGETS map
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/map)
