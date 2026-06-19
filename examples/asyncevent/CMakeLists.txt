add_executable(example-asyncevent
    AsyncEventApp.cpp
    AsyncEventLayer.cpp
)

target_sources(example-asyncevent
    PRIVATE
        FILE_SET example_modules TYPE CXX_MODULES
        FILES
            AsyncEventLayer.cppm
)

target_link_libraries(example-asyncevent PRIVATE Nodens::Nodens)
