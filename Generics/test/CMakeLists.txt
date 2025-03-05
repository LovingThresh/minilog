#tests/CMakeLists.txt
add_executable(exampleGenerics_tests test.cpp)
target_link_libraries(exampleGenerics_tests PRIVATE GTest::gtest_main)
add_test(NAME exampleGenerics_tests COMMAND exampleGenerics_tests)