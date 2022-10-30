cmake_minimum_required(VERSION 2.8.4)
project(eb_tgbot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

add_executable(eb_tgbot main.cpp main.hpp snippets.hpp 
    Config.cpp Config.hpp CoordsData.hpp LegendWords.cpp LegendWords.hpp
    ../third_party/json-modern/json.hpp)

target_link_libraries(eb_tgbot /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})