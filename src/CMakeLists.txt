set(executable_name bin)

set(bin_source_files 
    program.cpp
        core/corefunctions.h)

add_executable(${executable_name} ${bin_source_files})

target_compile_features(${executable_name} PRIVATE cxx_std_20)

target_link_libraries(${executable_name} PRIVATE restfulapi plog::plog CURL::libcurl)