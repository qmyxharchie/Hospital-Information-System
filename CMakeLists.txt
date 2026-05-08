# CMakeLists.txt — 医院信息管理系统
# 最低 CMake 版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称 + 版本
project(HospitalInformationSystem VERSION 1.0 LANGUAGES C)

# 设置 C 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ============ 源文件收集 ============
# 手动列出所有 .c 文件（推荐，控制精确）
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

# ============ 平台特定配置 ============
if(WIN32)
    # Windows 下可能需要关闭某些警告
    target_compile_options(HospitalSys PRIVATE /W3)
    
    # 如果需要 system("pause") 和 system("cls") 的支持（已存在）
    # Windows 控制台程序
    set_target_properties(HospitalSys PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
else()
    # Linux/macOS 编译选项
    target_compile_options(HospitalSys PRIVATE -Wall -Wextra -g)
endif()

# ============ VS2022 文件夹组织 ============
# 让 .c 文件在 Solution Explorer 中按目录分组
source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCES})

# ============ 数据文件复制（可选） ============
# 构建时自动将 data/ 文件夹复制到输出目录
add_custom_command(TARGET HospitalSys POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:HospitalSys>/data
    COMMENT "复制数据文件到输出目录"
)