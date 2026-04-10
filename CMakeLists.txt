cmake_minimum_required(VERSION 3.10)
project(HospitalManagement CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows平台链接EasyX图形库
if(WIN32)
    link_libraries(EasyX.lib)
endif()

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/StellarX/StellarX)
include_directories(${CMAKE_SOURCE_DIR}/src/StellarX)

# 源文件列表
set(SOURCES
    src/main_gui.cpp
    src/patient.c
    src/doctor.c
    src/registration.c
    src/medicine.c
    src/hospitalization.c
    src/bed.c
    src/file_io.c
    src/utils.c
    src/login.c
)

# 添加StellarX源文件
file(GLOB_RECURSE STELLARX_SOURCES
    "${CMAKE_SOURCE_DIR}/src/StellarX/*.cpp"
)
list(APPEND SOURCES ${STELLARX_SOURCES})

# 生成可执行文件
add_executable(hospital_management ${SOURCES})

# 设置输出目录
set_target_properties(hospital_management 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
