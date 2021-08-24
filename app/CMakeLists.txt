# Add Main
add_executable(app-t app.cpp)

# Link Libraries
target_link_libraries(app-t PUBLIC app-t-lib)
