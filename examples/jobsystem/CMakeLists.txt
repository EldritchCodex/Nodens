add_executable(example-jobsystem
    JobSystemApp.cpp
    JobSystemLayer.cpp
)

target_sources(example-jobsystem
    PRIVATE
        FILE_SET example_modules TYPE CXX_MODULES
        FILES
            JobSystemLayer.cppm
)

target_link_libraries(example-jobsystem PRIVATE Nodens::Nodens)