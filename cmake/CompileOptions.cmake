# Example: CompileOptions(app_name)
function(CompileOptions app_name)
    target_compile_options(${app_name} PRIVATE
            # Enabled options
            -Wall
            -Wextra
            -Wpedantic
            -Werror

            # disabled options
            -Wno-unused-function
            -Wno-unused-lambda-capture
            -Wno-unused-parameter
            -Wno-unused-variable
            -Wno-unused-private-field
            -Wno-missing-field-initializers
            -Wno-gnu-zero-variadic-macro-arguments
            -Wno-invalid-noreturn
            -Wno-unused-value
            -Wno-unused-local-typedef
            -Wno-unused-but-set-variable
    )
endfunction()
