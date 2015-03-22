cmake_minimum_required(VERSION 2.6)
set(HOME $ENV{HOME})
set(CMAKE_INSTALL_PREFIX "${HOME}/Software" CACHE PATH
    "Choose the install location."
    )
project(RANDOM_FOREST)

set(BUILD_SHARED_LIBS True)
set(CMAKE_BUILD_TYPE Release)

install(FILES forest.hpp DESTINATION include/random-forest)
install(FILES tree.hpp DESTINATION include/random-forest)
install(FILES node.hpp DESTINATION include/random-forest)
install(FILES node-gini.hpp DESTINATION include/random-forest)
install(FILES node-matrix.hpp DESTINATION include/random-forest)
install(FILES common-libraries.hpp DESTINATION include/random-forest)
