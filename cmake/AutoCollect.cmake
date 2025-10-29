# Example: CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} source_files)
function(CollectSourceFiles current_dir output_variable)
    file(GLOB_RECURSE source_files
            ${current_dir}/*.c
            ${current_dir}/*.cc
            ${current_dir}/*.cpp
    )
    set(${output_variable} ${source_files} PARENT_SCOPE)
endfunction()
