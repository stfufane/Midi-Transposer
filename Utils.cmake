# Macro to include a github project
include(FetchContent QUIET)
macro (retrieve_from_git name repo branch subdir)
    message("Retrieving ${name} from git...")
    FetchContent_Declare(${name}
            GIT_REPOSITORY ${repo}
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
            GIT_TAG ${branch}
            SOURCE_SUBDIR ${subdir}
    )
    message("${name} update finished")
endmacro()

