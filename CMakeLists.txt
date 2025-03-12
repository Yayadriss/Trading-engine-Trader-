cmake_minimum_required(VERSION 3.5)

# Define the project name
project(TradingInterface)

# **     Specify the path where Qt is installed    **
# CMAKE_PREFIX_PATH is used to tell CMake where to find Qt's configuration files
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.1/mingw_64/lib/cmake")

# **Find necessary Qt components**
# We ask CMake to locate Qt6 and the required modules for this project:
# - `Widgets`: For creating graphical user interfaces
# - `Charts`: For adding and managing charts
# - `Sql`: For database management
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Sql)

# **Define the executable**
# This tells CMake that our project will generate an executable named "TradingInterface"
# The necessary source files for compilation are `main.cpp` and `DatabaseManager.cpp`
add_executable(TradingInterface main.cpp DatabaseManager.cpp)

# **Link the executable with Qt libraries**
# We link our executable with the specific Qt libraries it needs:
# - `Qt6::Widgets`: Provides basic GUI elements like buttons, tables,..
# - `Qt6::Charts`: Enables the use of interactive charts in the application
# - `Qt6::Sql`: Allows interaction with relational databases
target_link_libraries(TradingInterface
    PRIVATE
    Qt6::Widgets
    Qt6::Charts
    Qt6::Sql
)
