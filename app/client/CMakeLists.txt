add_executable(client main.cpp)

target_compile_features(client PRIVATE cxx_std_20)

target_link_libraries(client PRIVATE tcpChat_lib)

# target_compile_options(
#   client 
#   PRIVATE -Wall
#           -Wextra
#           -Wshadow
#           -Wconversion
#           -Wpedantic
#           -Werror
#           -Wnon-virtual-dtor
#           -pedantic
#           )
