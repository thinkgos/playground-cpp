add_library(container OBJECT)



file(GLOB_RECURSE SRC_FILES "*.cc")
file(GLOB_RECURSE HEADER_FILES "*.h")
target_sources(container
    PRIVATE
     ${SRC_FILES}
    INTERFACE
      FILE_SET HEADERS
      FILES
        ${HEADER_FILES}
)
