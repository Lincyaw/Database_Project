# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Program Files\CLion 2020.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Program Files\CLion 2020.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\fay\Documents\Github\Database_Project\extmem-c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\fay\Documents\Github\Database_Project\extmem-c\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\extmem_c.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\extmem_c.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\extmem_c.dir\flags.make

CMakeFiles\extmem_c.dir\extmem.c.obj: CMakeFiles\extmem_c.dir\flags.make
CMakeFiles\extmem_c.dir\extmem.c.obj: ..\extmem.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\fay\Documents\Github\Database_Project\extmem-c\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/extmem_c.dir/extmem.c.obj"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\extmem_c.dir\extmem.c.obj /FdCMakeFiles\extmem_c.dir\ /FS -c C:\Users\fay\Documents\Github\Database_Project\extmem-c\extmem.c
<<

CMakeFiles\extmem_c.dir\extmem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/extmem_c.dir/extmem.c.i"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\extmem_c.dir\extmem.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\fay\Documents\Github\Database_Project\extmem-c\extmem.c
<<

CMakeFiles\extmem_c.dir\extmem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/extmem_c.dir/extmem.c.s"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\extmem_c.dir\extmem.c.s /c C:\Users\fay\Documents\Github\Database_Project\extmem-c\extmem.c
<<

CMakeFiles\extmem_c.dir\test.c.obj: CMakeFiles\extmem_c.dir\flags.make
CMakeFiles\extmem_c.dir\test.c.obj: ..\test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\fay\Documents\Github\Database_Project\extmem-c\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/extmem_c.dir/test.c.obj"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\extmem_c.dir\test.c.obj /FdCMakeFiles\extmem_c.dir\ /FS -c C:\Users\fay\Documents\Github\Database_Project\extmem-c\test.c
<<

CMakeFiles\extmem_c.dir\test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/extmem_c.dir/test.c.i"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe > CMakeFiles\extmem_c.dir\test.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\fay\Documents\Github\Database_Project\extmem-c\test.c
<<

CMakeFiles\extmem_c.dir\test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/extmem_c.dir/test.c.s"
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\extmem_c.dir\test.c.s /c C:\Users\fay\Documents\Github\Database_Project\extmem-c\test.c
<<

# Object files for target extmem_c
extmem_c_OBJECTS = \
"CMakeFiles\extmem_c.dir\extmem.c.obj" \
"CMakeFiles\extmem_c.dir\test.c.obj"

# External object files for target extmem_c
extmem_c_EXTERNAL_OBJECTS =

extmem_c.exe: CMakeFiles\extmem_c.dir\extmem.c.obj
extmem_c.exe: CMakeFiles\extmem_c.dir\test.c.obj
extmem_c.exe: CMakeFiles\extmem_c.dir\build.make
extmem_c.exe: CMakeFiles\extmem_c.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\fay\Documents\Github\Database_Project\extmem-c\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable extmem_c.exe"
	"D:\Program Files\CLion 2020.3\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\extmem_c.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1428~1.293\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\extmem_c.dir\objects1.rsp @<<
 /out:extmem_c.exe /implib:extmem_c.lib /pdb:C:\Users\fay\Documents\Github\Database_Project\extmem-c\cmake-build-debug\extmem_c.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\extmem_c.dir\build: extmem_c.exe

.PHONY : CMakeFiles\extmem_c.dir\build

CMakeFiles\extmem_c.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\extmem_c.dir\cmake_clean.cmake
.PHONY : CMakeFiles\extmem_c.dir\clean

CMakeFiles\extmem_c.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\fay\Documents\Github\Database_Project\extmem-c C:\Users\fay\Documents\Github\Database_Project\extmem-c C:\Users\fay\Documents\Github\Database_Project\extmem-c\cmake-build-debug C:\Users\fay\Documents\Github\Database_Project\extmem-c\cmake-build-debug C:\Users\fay\Documents\Github\Database_Project\extmem-c\cmake-build-debug\CMakeFiles\extmem_c.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\extmem_c.dir\depend

