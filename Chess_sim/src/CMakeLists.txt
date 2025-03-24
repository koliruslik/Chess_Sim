# CMakeList.txt: проект CMake для Chess_sim; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
include_directories(src/Menu)  
include_directories(src/Button)  
include_directories(src/GamePlay)  


file(GLOB SOURCES
    "src/*.cpp"
    "src/Menu/*.cpp"
    "src/Button/*.cpp"
    "src/GamePlay/*.cpp"
)
# Добавьте источник в исполняемый файл этого проекта.
add_executable (Chess_sim ${SOURCE})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chess_sim PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_link_libraries(Chess_sim raylib)