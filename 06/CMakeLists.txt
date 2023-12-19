get_filename_component(DayName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(D${DayName}P01 P01.cc)
add_executable(D${DayName}P02 P01.cc)
target_compile_definitions(D${DayName}P02 PRIVATE PART_2)
