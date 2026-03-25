cmake_minimum_required(VERSION 3.10)
project(Secure_Personnel_Record_System C)

set(CMAKE_C_STANDARD 11)

find_package(OpenSSL REQUIRED)

add_executable(Secure_Personnel_Record_System
        main.c
        crypto.c)

target_include_directories(Secure_Personnel_Record_System PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(Secure_Personnel_Record_System OpenSSL::SSL OpenSSL::Crypto)
