cmake_minimum_required(VERSION 3.16)
project("AlogrithmAnalyseCourse")

include_directories(./deps/doctest/include)

enable_testing()

file(GLOB hw_groups "./src/hw**")
foreach(hw_group ${hw_groups})
    get_filename_component(hw_group_name ${hw_group} NAME)
    file(GLOB hw_group_items "${hw_group}/*.cpp")
    foreach(hw_group_item ${hw_group_items})
        get_filename_component(hw_group_item_name ${hw_group_item} NAME_WLE)
        set(hw_item_full_name "${hw_group_name}___${hw_group_item_name}")
        add_executable(${hw_item_full_name} "${hw_group_item}")
        add_test(${hw_item_full_name} ${hw_item_full_name})
    endforeach()
endforeach()