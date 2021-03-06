cmake_minimum_required(VERSION 2.8)

project(FaceRecognizer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

cmake_policy(SET CMP0020 NEW)

# 建立要删除的文件
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
# 建立删除目标
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

SET(BUILD_VERSION "v0.0.2")
# Find Git Version Patch
IF(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    if(NOT GIT)
        SET(GIT $ENV{GIT})
    endif()
    if(NOT GIT)
        FIND_PROGRAM(GIT NAMES git git.exe git.cmd)
    endif()
    IF(GIT)
        EXECUTE_PROCESS(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT} describe --tags
            OUTPUT_VARIABLE GIT_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(NOT GIT_VERSION)
            EXECUTE_PROCESS(
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND ${GIT} rev-parse --short HEAD
                OUTPUT_VARIABLE GIT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE
            )
        endif()
        SET(BUILD_VERSION ${GIT_VERSION})
    ENDIF()
ENDIF()
message("BUILD_VERSION:${BUILD_VERSION}")
set(VERSION ${BUILD_VERSION})

if("Debug" STREQUAL CMAKE_BUILD_TYPE)
    add_definitions(-D_DEBUG)
endif()

#需要的QT组件
get_filename_component(QT_INSTALL_DIR "${Qt5_DIR}/../../.." ABSOLUTE)
message("QT_INSTALL_DIR:${QT_INSTALL_DIR}")
SET(QT_COMPONENTS Core Gui Widgets Xml Network Multimedia Sql)
find_package(Qt5 COMPONENTS ${QT_COMPONENTS})
FOREACH(_COMPONENT ${QT_COMPONENTS})
    SET(QT_LIBRARIES ${QT_LIBRARIES} Qt5::${_COMPONENT})
ENDFOREACH()

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

IF(MSVC)
    # This option is to enable the /MP switch for Visual Studio 2005 and above compilers
    OPTION(WIN32_USE_MP "Set to ON to build with the /MP option (Visual Studio 2005 and above)." ON)
    MARK_AS_ADVANCED(WIN32_USE_MP)
    IF(WIN32_USE_MP)
        #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        add_compile_options(/MP)
    ENDIF(WIN32_USE_MP)
    add_compile_options(/utf-8)
ENDIF(MSVC)
IF(MINGW)
    add_compile_options("-std=c++0x")
ENDIF()
SET(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs")
if (BUILD_SHARED_LIBS)
	add_definitions(-DBUILD_SHARED_LIBS)
    if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
       # Just setting CMAKE_POSITION_INDEPENDENT_CODE should be enough to set
       # -fPIC for GCC but sometimes it still doesn't get set, so make sure it
       # does.
       add_definitions("-fPIC")
    endif()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)

#CHECK_INCLUDE_FILE_CXX("string" HAVE_STRING_H)
#check_include_file("math.h" HAVE_MATH_H)

#check_function_exists("fabs" HAVE_FABS)

set(BUILD_PLATFORM "${CMAKE_SYSTEM_NAME}")
# ----------------------------------------------------------------------------
# Detect compiler and target platform architecture
# ----------------------------------------------------------------------------
if(NOT ANDROID)
    if(X86_64 OR CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(BUILD_ARCH x86_64)
    elseif(X86 OR CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(BUILD_ARCH x86)
    endif()
else()
    set(BUILD_ARCH ${CMAKE_SYSTEM_PROCESSOR})
endif()

# 设置安装前缀
if(ANDROID)
    set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/android-build CACHE STRING "Install path prefix" FORCE)
elseif(WIN32)
    SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE STRING "Install path prefix." FORCE)
endif()

if(NOT RabbitCommon_DIR)
    set(RabbitCommon_DIR $ENV{RabbitCommon_DIR})
    if(NOT RabbitCommon_DIR)
        set(RabbitCommon_DIR ${CMAKE_SOURCE_DIR}/../RabbitCommon)
    endif()
endif()
if(DEFINED RabbitCommon_DIR AND EXISTS ${RabbitCommon_DIR}/Src)
    add_subdirectory(${RabbitCommon_DIR}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
else()
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon")
    message("   ag:")
    message("       git clone https://github.com/KangLin/RabbitCommon.git")
    message("2. Then set cmake value or environment variable RabbitCommon_DIR to download root dirctory.")
    message("   ag:")
    message(FATAL_ERROR "       cmake -DRabbitCommon_DIR= ")
endif()
add_subdirectory(Src)
add_subdirectory(App)

INSTALL(FILES Install/Install.nsi DESTINATION "${CMAKE_BINARY_DIR}")
set(OTHER_FILES
    License.md
    Authors.md
    ChangeLog.md
    Authors_zh_CN.md
    ChangeLog_zh_CN.md)
if(ANDROID)
    # Install other files
    INSTALL(FILES ${OTHER_FILES} DESTINATION "assets")
else()
    # Install other files
    INSTALL(FILES ${OTHER_FILES} DESTINATION ".")
endif()
if(UNIX)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/debian/FaceRecognizer.desktop DESTINATION "share/applications")
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Resource/image/FaceRecognizer.png DESTINATION "share/pixmaps")
endif()
message(STATUS "== Build shared library: ${BUILD_SHARED_LIBS}")
message(STATUS "== Build arch: ${BUILD_ARCH}")
