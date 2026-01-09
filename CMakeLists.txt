cmake_minimum_required(VERSION 3.10)
project(OnlineStore)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(shop 
    src/main.cpp 
    src/menu.cpp 
    src/database.cpp
)

# ← КРИТИЧНО ДЛЯ MINGW!
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)
target_link_libraries(shop ${LIBPQXX_LIBRARIES})
target_include_directories(shop PRIVATE ${LIBPQXX_INCLUDE_DIRS})
