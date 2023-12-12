get_filename_component(DayName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(D${DayName}P01)
target_sources(D${DayName}P01
    PUBLIC
        P01.cc)
add_executable(D${DayName}P02)
target_sources(D${DayName}P02
    PUBLIC
        P02.cc)
