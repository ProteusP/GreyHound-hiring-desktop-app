cmake_minimum_required(VERSION 3.16)

project(GreyHound-hiring-desktop-app VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Sql Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Sql Test)

# ------------------------ приложение ------------------------
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
    src/loginpage.cpp   src/loginpage.h   src/loginpage.ui
    src/registerstatus.cpp  src/registerstatus.h  src/registerstatus.ui
    src/passwordwarningdialog.cpp src/passwordwarningdialog.h
    src/registerpageforcandidate.cpp src/registerpageforcandidate.h src/registerpageforcandidate.ui
    src/registerpageforemployer.cpp src/registerpageforemployer.h src/registerpageforemployer.ui
    src/flowlayout.cpp   src/flowlayout.h
    src/mainpage.cpp     src/mainpage.h   src/mainpage.ui
    src/profilepageforcandidate.cpp src/profilepageforcandidate.h src/profilepageforcandidate.ui
    src/profilepageforemployer.cpp src/profilepageforemployer.h src/profilepageforemployer.ui
    src/resume.cpp  src/resume.h
    src/vacancycard.cpp src/vacancycard.h src/vacancycard.ui
    src/hashing.cpp  src/hashing.h
    src/validation.cpp src/validation.h
    src/candidatecard.cpp src/candidatecard.h
    src/candidatedetailswindow.cpp src/candidatedetailswindow.h src/candidatedetailswindow.ui
    src/vacancydetailswindow.cpp src/vacancydetailswindow.h src/vacancydetailswindow.ui
    resource/icons.qrc
)

qt_add_executable(GreyHound-hiring-desktop-app
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_include_directories(GreyHound-hiring-desktop-app PRIVATE src)

target_link_libraries(GreyHound-hiring-desktop-app PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Test
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GreyHound-hiring-desktop-app)
endif()

# ------------------------ тесты ------------------------
enable_testing()
function(add_my_test name)
    add_executable(${name} ${ARGN})
    target_include_directories(${name} PRIVATE src)
    target_link_libraries(${name} PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Sql
        Qt${QT_VERSION_MAJOR}::Test
    )
    set_target_properties(${name} PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
    )
    add_test(NAME ${name} COMMAND ${name})
endfunction()
add_my_test(test_hashing            tests/test_hashing.cpp            src/hashing.cpp)
add_my_test(test_validation         tests/test_validation.cpp         src/validation.cpp)
add_my_test(test_loginpage          tests/test_loginpage.cpp          src/loginpage.cpp src/hashing.cpp)
add_my_test(test_vacancycard        tests/test_vacancycard.cpp        src/vacancycard.cpp src/vacancydetailswindow.cpp)
add_my_test(test_vacancydetailswindow tests/test_vacancydetailswindow.cpp src/vacancydetailswindow.cpp)
add_my_test(test_registeremployer   tests/test_registeremployer.cpp   src/registerpageforemployer.cpp src/validation.cpp src/hashing.cpp src/passwordwarningdialog.cpp)
add_my_test(test_registercandidate  tests/test_registercandidate.cpp  src/registerpageforcandidate.cpp src/passwordwarningdialog.cpp src/validation.cpp src/hashing.cpp)
add_my_test(test_profilepageforemployer tests/test_profilepageforemployer.cpp src/profilepageforemployer.cpp)
add_my_test(test_profilepageforcandidate tests/test_profilepageforcandidate.cpp src/profilepageforcandidate.cpp)
add_my_test(test_mainwindow         tests/test_mainwindow.cpp         src/mainwindow.cpp src/loginpage.cpp src/mainpage.cpp
                                                              src/registerpageforcandidate.cpp src/registerpageforemployer.cpp
                                                              src/profilepageforcandidate.cpp src/profilepageforemployer.cpp
                                                              src/registerstatus.cpp src/passwordwarningdialog.cpp src/validation.cpp
                                                              src/hashing.cpp src/vacancycard.cpp src/vacancydetailswindow.cpp
                                                              src/flowlayout.cpp src/candidatecard.cpp src/candidatedetailswindow.cpp)
add_my_test(test_mainpage           tests/test_mainpage.cpp           src/mainpage.cpp src/flowlayout.cpp src/vacancycard.cpp
                                                              src/candidatecard.cpp src/vacancydetailswindow.cpp src/candidatedetailswindow.cpp)
add_my_test(test_candidatedetailswindow tests/test_candidatedetailswindow.cpp src/candidatedetailswindow.cpp)
add_my_test(test_candidatecard      tests/test_candidatecard.cpp      src/candidatecard.cpp src/candidatedetailswindow.cpp)
