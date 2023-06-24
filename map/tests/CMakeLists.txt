add_executable(map_tests
               map_tests.cpp)

find_package(Threads REQUIRED)

target_link_libraries(map_tests PRIVATE
                      gtest
                      Threads::Threads
                      map)

add_test(map_tests map_tests
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
