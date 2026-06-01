# -----------------------------------------------------------------------------
# Developer Touchpoint: Entry Point Policy For All Examples
# -----------------------------------------------------------------------------
option(ND_USE_DEFAULT_MAIN
    "Link examples against Nodens::DefaultMain (framework-provided main)" ON)

add_library(NodensExamplesEntry INTERFACE)
add_library(Nodens::ExamplesEntry ALIAS NodensExamplesEntry)

target_compile_definitions(NodensExamplesEntry INTERFACE
    $<$<BOOL:${ND_USE_DEFAULT_MAIN}>:ND_EXCLUDE_APP_MAIN>
)

target_link_libraries(NodensExamplesEntry INTERFACE
    $<$<BOOL:${ND_USE_DEFAULT_MAIN}>:Nodens::DefaultMain>
)

# -----------------------------------------------------------------------------
# Developer Touchpoint: Enabled Examples
# -----------------------------------------------------------------------------
add_subdirectory(circularwave3d)
add_subdirectory(jobsystem)
add_subdirectory(asyncevent)