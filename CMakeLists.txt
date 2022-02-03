cmake_minimum_required(VERSION 3.12)
project(ProjectThor C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

set(CMAKE_EXE_LINKER_FLAGS "-rdynamic")

include_directories(headers)

add_executable(ProjectThor main.c
        comm/server.c
        comm/writer.c
        core/sched.c
        core/thor.c
        database/items_images.c
        database/items_info_db.c
        database/rpc_log_db.c
        database/thor_db.c
        database/usr_info_db.c
        rpc/clight.c
        rpc/db_manager.c
        rpc/interface.c
        security/authorize.c
        security/hash.c
        security/ssl_tls.c
        src/logger.c
        ui/qr.c
        ui/qr_generator.c)

target_link_libraries (ProjectThor ssl sqlite3 pthread crypto)
