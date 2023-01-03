add_executable(server main.cpp)

target_compile_features(server PRIVATE cxx_std_20)

target_link_libraries(server PRIVATE tcpChat_lib)

# target_compile_options(
#   server 
#   PRIVATE -Wall
#           -Wextra
#           -Wshadow
#           -Wconversion
#           -Wpedantic
#           -Werror
#           -Wnon-virtual-dtor
#           -pedantic
#           )
