##############################################################################

if(USE_ICONV)
    set(iconv_USE_STATIC_LIBS ON)
    find_package(Iconv REQUIRED)
    
    if(ICONV_SECOND_ARGUMENT_IS_CONST)
        add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
    endif()
    message("ICONV_SECOND_ARGUMENT_IS_CONST is ${ICONV_SECOND_ARGUMENT_IS_CONST}")
endif()

##############################################################################

add_executable(JapaneseEncoding JapaneseEncoding.cpp)

if(USE_ICONV)
    target_include_directories(JapaneseEncoding PUBLIC ${ICONV_INCLUDE_DIRS})
    target_link_libraries(JapaneseEncoding ${Boost_LIBRARIES} ${ICONV_LIBRARIES})
else()
    target_link_libraries(JapaneseEncoding ${Boost_LIBRARIES})
endif()

##############################################################################

add_executable(JapaneseEncodingTest JapaneseEncodingTest.cpp)
add_test(NAME JapaneseEncodingTest COMMAND JapaneseEncodingTest)

if(USE_ICONV)
    target_include_directories(JapaneseEncodingTest PUBLIC ${ICONV_INCLUDE_DIRS})
    target_link_libraries(JapaneseEncodingTest ${Boost_LIBRARIES} ${ICONV_LIBRARIES})
else()
    target_link_libraries(JapaneseEncodingTest ${Boost_LIBRARIES})
endif()

##############################################################################
