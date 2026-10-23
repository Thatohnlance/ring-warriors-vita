cmake_minimum_required(VERSION 3.10)

project(ring_warriors_vita C)

include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Ring Warriors Vita")
set(VITA_TITLEID "RWVT00001")
set(VITA_VERSION "01.00")

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME}
    SceDebugScreen_stub
    SceCtrl_stub
    SceLibKernel_stub
)

vita_create_self(eboot.bin ${PROJECT_NAME})

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} eboot.bin
    VERSION ${VITA_VERSION}
    NAME ${VITA_APP_NAME}
)
