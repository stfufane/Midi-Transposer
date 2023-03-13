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

