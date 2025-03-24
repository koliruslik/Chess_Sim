# CMakeList.txt: проект CMake для Chess_sim; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Chess_sim "Chess_sim.cpp" "Chess_sim.h" "Menu.h" "Menu.cpp" "MainMenu.h" "MainMenu.cpp" "Button.cpp" "Button.h" "Timer.cpp" "Timer.h" "SettingsMenu.h" "SettingsMenu.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Chess_sim PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
target_link_libraries(Chess_sim raylib)