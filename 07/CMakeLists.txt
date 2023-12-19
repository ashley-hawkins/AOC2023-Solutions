get_filename_component(DayName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Find Range v3
find_package(range-v3 REQUIRED)
add_executable(D${DayName}P01 P01.cc)
# link range v3 to p01
target_link_libraries(D${DayName}P01 PUBLIC range-v3)
add_executable(D${DayName}P02 P01.cc)
target_compile_definitions(D${DayName}P02 PRIVATE PART_2)
