# Macro to include a github project
include(FetchContent QUIET)
macro (update_from_git name repo branch populate)
    message("Updating ${name} from git...")
    FetchContent_Declare(${name}
            GIT_REPOSITORY ${repo}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
            GIT_TAG ${branch})

    # Somehow juce does not want to work with MakeAvailable :(
    if (${populate})
        FetchContent_GetProperties(${name})

        if (NOT ${name}_POPULATED)
            FetchContent_Populate(${name})
        endif ()
    else()
        FetchContent_MakeAvailable(${name})
    endif()
    message("${name} update finished")
endmacro()

# Function to build the binary data from a folder of images
function(add_images_from_directory target imagesSubFolder)
    set (ImagesDir ${CMAKE_CURRENT_LIST_DIR}/${imagesSubFolder})
    file(GLOB_RECURSE images
            "${ImagesDir}/*.jpg"
            "${ImagesDir}/*.png"
            "${ImagesDir}/*.jpeg")

    if(NOT images STREQUAL "")
        set (ImagesTarget "${target}-Images")
        juce_add_binary_data(${ImagesTarget} SOURCES ${images})
        target_link_libraries(${target} PRIVATE ${ImagesTarget})
    endif()
endfunction()