cmake_minimum_required(VERSION 3.5)
project(eosjustgame1)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( eosjustgame1 eosjustgame1 eosjustgame1.cpp )
target_include_directories( eosjustgame1 PUBLIC ${CMAKE_SOURCE_DIR}/../include )