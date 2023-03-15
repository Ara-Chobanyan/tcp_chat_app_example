file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS
     "${tcpChat_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS
     "${tcpChat_SOURCE_DIR}/src/*.cpp")
add_library(tcpChat_lib SHARED ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(tcpChat_lib PUBLIC ../include)
target_link_libraries(tcpChat_lib PUBLIC Boost::asio)
target_compile_features(tcpChat_lib PUBLIC cxx_std_20)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header files"
  FILES ${HEADER_LIST})
