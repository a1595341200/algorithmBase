macro(BUILD curdir)
  file(GLOB children ${curdir}/*.cpp)
  foreach(child ${children})
  get_filename_component(FILENAME ${child} NAME_WE)
  add_executable(${FILENAME} ${child})
  target_link_libraries(${FILENAME} 
      pthread 
      gflags 
      ${GLOG_LIBRARIES}
      ${PCL_LIBRARIES}
      ${OpenCV_LIBS}
      algorithmBase::algorithmBase
  )
  endforeach()
endmacro()

macro(header_directories return_list includes_base_folder extention )
    FILE(GLOB_RECURSE new_list ${includes_base_folder}/*.${extention})
    SET(dir_list "")
    foreach(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    endforeach()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
endmacro()

macro(add_compilation_options_check)
add_compile_options(
    -Werror
    -Wconversion
    -Wunused
    -Wall
    -Wextra
    -Wuninitialized
    -Weffc++
    -Wno-non-virtual-dtor
    -Wdelete-non-virtual-dtor
    -Wzero-as-null-pointer-constant
    -Wparentheses
    -Wempty-body
    -Wsign-compare
    -Wsign-conversion
    -Wold-style-cast
    # -Wrange-loop-construct
    -Wformat
    -Wstrict-aliasing=3
    -Wrestrict
    -Winvalid-memory-model
    -Wsuggest-override
    -Wattributes
    -Wmissing-declarations
    -Warray-bounds
    # -Wdangling-pointer=2
    -Wreturn-local-addr
    # -Wuse-after-free
    # -Wmismatched-new-delete
    -Wvla
    -Walloca
    -Wreorder
    -Woverflow
    -Werror=return-type
    -Wswitch
    -Wimplicit-fallthrough
    -Wshadow=local
    -Werror
    -Wall
    -Wextra
    -Weffc++
    -Wunused
    -Wunused-but-set-variable
    -Wunused-but-set-parameter
    -Wunused-const-variable
    -Wunused-value
    -Werror=unused-result
    # -Werror=use-after-free=3
    # -Werror=string-compare
    -Werror=return-type
    -Werror=deprecated-copy-dtor
    -Werror=deprecated-copy
    -Wnon-virtual-dtor
    -Werror=delete-non-virtual-dtor
    # -Werror=mismatched-new-delete
    -Werror=suggest-override
    -Werror=conversion-null
    -Werror=zero-as-null-pointer-constant
    -Werror=reorder
    -Werror=class-memaccess
    -Wparentheses
    -Wconversion
    -Werror=float-conversion
    # -Wenum-conversion
    -Wsign-conversion
    -Wsign-compare
    -Wsign-promo
    -Wold-style-cast
    # -Wrange-loop-construct
    -Werror=uninitialized
    -Werror=maybe-uninitialized
    -Wmissing-field-initializers
    -Wmissing-declarations
    -Wempty-body
    -Wlogical-op
    -Wlogical-not-parentheses
    -Werror=address
    -Werror=multichar
    -Werror=overflow
    -Werror=restrict
    -Werror=strict-aliasing
    -Werror=invalid-memory-model
    -Werror=vla
    -Wattributes
    -Wstrict-overflow
    -Werror=stringop-overflow
    -Werror=shift-overflow
)
endmacro()

