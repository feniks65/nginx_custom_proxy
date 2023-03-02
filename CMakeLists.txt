cmake_minimum_required(VERSION 3.10)
project(ngx_http_custom_proxy_module)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(NGINX_SRC_PATH "/path/to/nginx/src") define path to nginx source code
set(NGINX_CORE_PATH "${NGINX_SRC_PATH}/core")
set(NGINX_HTTP_PATH "${NGINX_SRC_PATH}/http")
set(NGINX_OS_PATH "${NGINX_SRC_PATH}/os/unix")

include_directories(${NGINX_CORE_PATH} ${NGINX_HTTP_PATH} ${NGINX_OS_PATH})

set(SOURCE_FILES ngx_http_custom_proxy_module.cpp)

add_library(ngx_http_custom_proxy_module SHARED ${SOURCE_FILES})

set_target_properties(ngx_http_custom_proxy_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PREFIX ""
    SUFFIX ".so"
)
