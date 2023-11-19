set(api_name restfulapi)

set(lib_header_files
    restfulapi.h
)

set(lib_source_files 
    restfulapi.cpp
)

add_library(restfulapi STATIC ${lib_source_files} ${lib_header_files})
add_library(${PROJECT_NAME}::${api_name} ALIAS ${api_name})

set_target_properties(
    PROPERTIES
        PUBLIC_HEADER "${lib_header_files}" 
        FOLDER "restfulapi"
        POSITION_INDEPENDENT_CODE ON  
)

target_include_directories(${api_name}
PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_link_libraries(${api_name} PRIVATE spdlog::spdlog CURL::libcurl)
#target_link_libraries(${api_name} PRIVATE CURL::libcurl)

target_compile_features(${api_name} PUBLIC cxx_std_20)
target_compile_options(${api_name} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/MP>")

source_group(
    TREE "${PROJECT_SOURCE_DIR}/src/restfulapi"
    PREFIX "Header Files"
    FILES ${lib_header_files}
)