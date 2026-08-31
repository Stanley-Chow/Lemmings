if(NOT DEFINED APP_EXECUTABLE OR NOT DEFINED DEPENDENCY_DIR OR NOT DEFINED DESTINATION)
    message(FATAL_ERROR "Runtime dependency copier received incomplete arguments")
endif()

file(GET_RUNTIME_DEPENDENCIES
    EXECUTABLES "${APP_EXECUTABLE}"
    RESOLVED_DEPENDENCIES_VAR resolved_dependencies
    UNRESOLVED_DEPENDENCIES_VAR unresolved_dependencies
    DIRECTORIES "${DEPENDENCY_DIR}"
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES
        ".*[Ww][Ii][Nn][Dd][Oo][Ww][Ss][/\\\\][Ss][Yy][Ss][Tt][Ee][Mm]32[/\\\\].*"
)

foreach(dependency IN LISTS resolved_dependencies)
    file(COPY "${dependency}" DESTINATION "${DESTINATION}")
endforeach()

if(unresolved_dependencies)
    message(WARNING "Unresolved runtime dependencies: ${unresolved_dependencies}")
endif()
