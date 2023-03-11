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
function(add_binary_data_from_directory target imagesSubFolder fontsSubFolder)
    set (ImagesDir ${CMAKE_CURRENT_LIST_DIR}/${imagesSubFolder})
    file(GLOB_RECURSE images
            "${ImagesDir}/*.jpg"
            "${ImagesDir}/*.png"
            "${ImagesDir}/*.jpeg")

    set (FontsDir ${CMAKE_CURRENT_LIST_DIR}/${fontsSubFolder})
    file (GLOB_RECURSE fonts
            "${FontsDir}/*.ttf")

    set (DataTarget "${target}-Data")
    juce_add_binary_data(${DataTarget} SOURCES ${images} ${fonts})
    target_link_libraries(${target} PRIVATE ${DataTarget})
endfunction()
