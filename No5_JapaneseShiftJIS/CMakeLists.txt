##############################################################################

if(USE_ICONV)
    set(iconv_USE_STATIC_LIBS ON)
    find_package(Iconv REQUIRED)
endif()

##############################################################################

add_executable(JapaneseShiftJIS JapaneseShiftJIS.cpp)

if(USE_ICONV)
    target_include_directories(JapaneseShiftJIS PUBLIC ${ICONV_INCLUDE_DIRS})
    target_link_libraries(JapaneseShiftJIS ${Boost_LIBRARIES} ${ICONV_LIBRARIES})
else()
    target_link_libraries(JapaneseShiftJIS ${Boost_LIBRARIES})
endif()

##############################################################################

add_executable(wave_utf8_test wave_utf8_test.cpp)
add_test(NAME wave_utf8_test COMMAND wave_utf8_test)

if(USE_ICONV)
    target_include_directories(wave_utf8_test PUBLIC ${ICONV_INCLUDE_DIRS})
    target_link_libraries(wave_utf8_test ${Boost_LIBRARIES} ${ICONV_LIBRARIES})
else()
    target_link_libraries(wave_utf8_test ${Boost_LIBRARIES})
endif()

##############################################################################
