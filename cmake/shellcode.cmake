# Set the project root directory
set( PROJECT_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../" )

# Is this not clang? NOTE: Do a architecture check as well for the arches
# from LLVM-MINGW as a safety check
if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	message( FATAL_ERROR "Clang is required to build this project." )
else()
        # Build flags for the final shellcode
        set( CMAKE_SHELLCODE_C_FLAGS -Os -fno-asynchronous-unwind-tables -fuse-ld=bfd -fno-exceptions -fno-unwind-tables )
	set( CMAKE_SHELLCODE_C_FLAGS "${CMAKE_SHELLCODE_C_FLAGS}" -fPIC -fno-ident -fpack-struct=8 -falign-functions=1 )
        set( CMAKE_SHELLCODE_C_FLAGS "${CMAKE_SHELLCODE_C_FLAGS}" -s -ffunction-sections -falign-jumps=1 -w )
        set( CMAKE_SHELLCODE_C_FLAGS "${CMAKE_SHELLCODE_C_FLAGS}" -falign-labels=1 -fdata-sections -mno-sse )
        set( CMAKE_SHELLCODE_C_FLAGS "${CMAKE_SHELLCODE_C_FLAGS}" -fms-extensions -fno-jump-tables -mno-stack-arg-probe )

        # Linker flags for the final shellcode
	set( CMAKE_SHELLCODE_L_FLAGS -fuse-ld=bfd -Wl,-s,--no-seh,--enable-stdcall-fixup -nostdlib -nostartfiles )
        set( CMAKE_SHELLCODE_L_FLAGS "${CMAKE_SHELLCODE_L_FLAGS}" "-Wl,-T${PROJECT_ROOT_DIR}/rsrc/SectionLink.ld,--no-seh,-s,--gc-sections" )

	# Adjust the entrypoint depending on the incoming architecture
	if ( "${CMAKE_SIZEOF_VOID_P}" STREQUAL "8" )
		set( CMAKE_SHELLCODE_L_FLAGS "${CMAKE_SHELLCODE_L_FLAGS}" -Wl,-eEfiMain )
	else()
		set( CMAKE_SHELLCODE_L_FLAGS "${CMAKE_SHELLCODE_L_FLAGS}" -Wl,-e_EfiMain )
	endif()

        # Add the compile flags
        add_compile_options( ${CMAKE_SHELLCODE_C_FLAGS} )

        # Add the linker flags
        add_link_options( ${CMAKE_SHELLCODE_L_FLAGS} )
endif()

# Create the virtual environment if it does not exist
find_package( Python3 COMPONENTS Interpreter REQUIRED )
message( STATUS "Creating virtual environment" )
set( VENV_DIR "${CMAKE_CURRENT_BINARY_DIR}/venv" )
execute_process(
	COMMAND "${Python3_EXECUTABLE}" -m venv "${VENV_DIR}"
	ECHO_OUTPUT_VARIABLE
	ECHO_ERROR_VARIABLE
	COMMAND_ERROR_IS_FATAL ANY
)

# Switch to the virtual environment
set(ENV{VIRTUAL_ENV} "${VENV_DIR}")
set(Python3_FIND_VIRTUALENV FIRST)
unset(Python3_EXECUTABLE)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Install the dependencies
message(STATUS "Installing virtualenv environment dependencies" )
execute_process(
	COMMAND "${Python3_EXECUTABLE}" -m pip install -r "${PROJECT_ROOT_DIR}/scripts/requirements.txt" --disable-pip-version-check
	ECHO_OUTPUT_VARIABLE
	ECHO_ERROR_VARIABLE
	COMMAND_ERROR_IS_FATAL ANY
)

function(add_shellcode_executable tgt)
	# Create an executable we will extract the bitcode from
	add_executable(${tgt} ${ARGN})

	# Remove from the install target
	set_target_properties(${tgt} PROPERTIES INSTALL_EXCLUDE_FROM_ALL TRUE)

	# Create the target base name
	set(TARGET_BASE_NAME "$<TARGET_FILE_DIR:${tgt}>/$<TARGET_FILE_BASE_NAME:${tgt}>")

	# Extracting the shellcode
	add_custom_command(TARGET ${tgt}
		POST_BUILD
		USES_TERMINAL
		COMMENT "Extracting the shellcode from the executable"
		COMMAND "${Python3_EXECUTABLE}" "${PROJECT_ROOT_DIR}/scripts/extractsc.py" "${TARGET_BASE_NAME}.exe" "${TARGET_BASE_NAME}.bin"
		VERBATIM
	)

	# Install it to the appropriate directories 
	if ( "${CMAKE_SIZEOF_VOID_P}" STREQUAL "8" )
		install( FILES ${TARGET_BASE_NAME}.bin DESTINATION "${CMAKE_INSTALL_PREFIX}" RENAME "$<TARGET_FILE_BASE_NAME:${tgt}>.x64.bin" )
	else()
		install( FILES ${TARGET_BASE_NAME}.bin DESTINATION "${CMAKE_INSTALL_PREFIX}" RENAME "$<TARGET_FILE_BASE_NAME:${tgt}>.x86.bin" )
	endif()
endfunction()
