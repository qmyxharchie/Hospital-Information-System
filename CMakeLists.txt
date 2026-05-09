cmake_minimum_required(VERSION 3.10)

project(HospitalInformationSystem VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ============ 解决 VS2022 GBK 编码报错 ============
if(MSVC)
    add_compile_options(/utf-8)      # 强制 UTF-8 读取源文件
    add_compile_options(/wd4996)     # 关闭 strcpy 安全警告
    add_compile_options(/wd4819)     # 关闭编码警告
    add_compile_options(/wd6031)     # 关闭"返回值被忽略"分析警告
endif()

# ============ 源文件（main() 在 ui.c 中） ============
set(SOURCES
    src/ui.c
    src/login.c
    src/patient.c
    src/doctor.c
    src/registration.c
    src/medicine.c
    src/hospitalization.c
    src/bed.c
    src/file_io.c
    src/utils.c
)

# ============ 头文件路径 ============
include_directories(${CMAKE_SOURCE_DIR}/include)

# ============ 生成可执行文件 ============
add_executable(HospitalSys ${SOURCES})

# ============ Windows 配置 ============
if(WIN32)
    target_compile_options(HospitalSys PRIVATE /W3)
    set_target_properties(HospitalSys PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
else()
    target_compile_options(HospitalSys PRIVATE -Wall -Wextra -g)
endif()

# ============ VS 文件夹分组 ============
source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCES})

# ============ 数据文件自动复制 ============
add_custom_command(TARGET HospitalSys POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:HospitalSys>/data
    COMMENT "复制数据文件到输出目录"
)