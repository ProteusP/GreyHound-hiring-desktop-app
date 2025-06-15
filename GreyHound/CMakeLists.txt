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
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    loginpage.cpp   loginpage.h   loginpage.ui
    registerstatus.cpp  registerstatus.h  registerstatus.ui
    passwordwarningdialog.cpp passwordwarningdialog.h
    registerpageforcandidate.cpp registerpageforcandidate.h registerpageforcandidate.ui
    registerpageforemployer.cpp registerpageforemployer.h registerpageforemployer.ui
    flowlayout.cpp   flowlayout.h
    mainpage.cpp     mainpage.h   mainpage.ui
    profilepageforcandidate.cpp profilepageforcandidate.h profilepageforcandidate.ui
    profilepageforemployer.cpp profilepageforemployer.h profilepageforemployer.ui
    resume.cpp  resume.h
    vacancycard.cpp vacancycard.h vacancycard.ui
    hashing.cpp  hashing.h
    validation.cpp validation.h
    candidatecard.cpp candidatecard.h
    candidatedetailswindow.cpp candidatedetailswindow.h candidatedetailswindow.ui
    vacancydetailswindow.cpp   vacancydetailswindow.h   vacancydetailswindow.ui
    icons.qrc
)

qt_add_executable(GreyHound-hiring-desktop-app
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

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

# --- test_hashing ---
add_executable(test_hashing
    test_hashing.cpp
    hashing.cpp
)
set_target_properties(test_hashing PROPERTIES AUTOMOC ON AUTOUIC OFF)
target_link_libraries(test_hashing PRIVATE Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME HashingTest COMMAND test_hashing)

# --- test_validation ---
add_executable(test_validation
    test_validation.cpp
    validation.cpp
)
set_target_properties(test_validation PROPERTIES AUTOMOC ON AUTOUIC OFF)
target_link_libraries(test_validation PRIVATE Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME ValidationTest COMMAND test_validation)

# --- test_loginpage ---
add_executable(test_loginpage
    test_loginpage.cpp
    loginpage.cpp
    hashing.cpp
)
set_target_properties(test_loginpage PROPERTIES AUTOMOC ON AUTOUIC ON)
target_link_libraries(test_loginpage PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME LoginPageTest COMMAND test_loginpage)
# --- test_vacancycard ---
add_executable(test_vacancycard test_vacancycard.cpp vacancycard.cpp vacancydetailswindow.cpp)
target_link_libraries(test_vacancycard PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)
set_target_properties(test_vacancycard PROPERTIES AUTOMOC ON)
add_test(NAME VacancyCardTest COMMAND test_vacancycard)
# --- test_vacancydetailswindow ---
add_executable(test_vacancydetailswindow
    test_vacancydetailswindow.cpp
    vacancydetailswindow.cpp
)
target_link_libraries(test_vacancydetailswindow PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)
set_target_properties(test_vacancycard PROPERTIES AUTOMOC ON AUTOUIC ON)
add_test(NAME VacancyDetailsWindowTest COMMAND test_vacancycard)
# --- test_registeremployer ---
add_executable(test_registeremployer
    test_registeremployer.cpp
    registerpageforemployer.cpp
    validation.cpp
    hashing.cpp
    passwordwarningdialog.cpp
)

set_target_properties(test_registeremployer PROPERTIES AUTOMOC ON AUTOUIC ON)

target_include_directories(test_registeremployer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(test_registeremployer PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

add_test(NAME RegisterEmployerTest COMMAND test_registeremployer)
# --- test_registercandidate ---
add_executable(test_registercandidate
    test_registercandidate.cpp
    registerpageforcandidate.cpp
    passwordwarningdialog.cpp
    validation.cpp
    hashing.cpp
)

set_target_properties(test_registercandidate PROPERTIES AUTOMOC ON)
target_link_libraries(test_registercandidate PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)
add_test(NAME RegisterCandidateTest COMMAND test_registercandidate)
# --- test_profilepageforemployer ---
add_executable(test_profilepageforemployer test_profilepageforemployer.cpp profilepageforemployer.cpp)
set_target_properties(test_profilepageforemployer PROPERTIES AUTOMOC ON AUTOUIC ON)
target_link_libraries(test_profilepageforemployer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME ProfilePageForEmployerTest COMMAND test_profilepageforemployer)
# --- test_profilepageforcandidate ---
add_executable(test_profilepageforcandidate test_profilepageforemployer.cpp profilepageforemployer.cpp)
set_target_properties(test_profilepageforcandidate PROPERTIES AUTOMOC ON AUTOUIC ON)
target_link_libraries(test_profilepageforcandidate PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME ProfilePageForCandidateTest COMMAND test_profilepageforcandidate)
# --- test_mainwindow ---
add_executable(test_mainwindow
    test_mainwindow.cpp
    mainwindow.cpp
    loginpage.cpp
    mainpage.cpp
    registerpageforcandidate.cpp
    registerpageforemployer.cpp
    profilepageforcandidate.cpp
    profilepageforemployer.cpp
    registerstatus.cpp
    passwordwarningdialog.cpp
    validation.cpp
    hashing.cpp
    vacancycard.cpp
    vacancydetailswindow.cpp
    flowlayout.cpp
    candidatecard.cpp
    candidatedetailswindow.cpp
)

set_target_properties(test_mainwindow PROPERTIES AUTOMOC ON AUTOUIC ON)
target_link_libraries(test_mainwindow
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Sql
)

add_test(NAME MainWindowTest COMMAND test_mainwindow)
# --- test_mainpage ---
add_executable(test_mainpage
    test_mainpage.cpp
    mainpage.cpp
    flowlayout.cpp
    vacancycard.cpp
    candidatecard.cpp
    vacancydetailswindow.cpp
    candidatedetailswindow.cpp
)
set_target_properties(test_mainpage PROPERTIES AUTOMOC ON AUTOUIC ON)
target_link_libraries(test_mainpage
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Test
)
add_test(NAME MainPageTest COMMAND test_mainpage)
# --- test_candidatedetailswindow ---
add_executable(test_candidatedetailswindow
    test_candidatedetailswindow.cpp
    candidatedetailswindow.cpp
)

set_target_properties(test_candidatedetailswindow PROPERTIES AUTOMOC ON AUTOUIC ON)

target_link_libraries(test_candidatedetailswindow PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Test
)

add_test(NAME CandidateDetailsWindowTest COMMAND test_candidatedetailswindow)
# --- test_candidatecard ---
add_executable(test_candidatecard
    test_candidatecard.cpp
    candidatecard.cpp
    candidatedetailswindow.cpp
)

set_target_properties(test_candidatecard PROPERTIES AUTOMOC ON AUTOUIC ON)

target_link_libraries(test_candidatecard PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Test
)

add_test(NAME CandidateCardTest COMMAND test_candidatecard)
