cmake_minimum_required(VERSION 3.5)
project(payout VERSION 1.0.0)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

### generate both the wasm and abi
add_contract( payout payout payout.cpp )
target_include_directories( payout PUBLIC ${CMAKE_SOURCE_DIR}/../include )