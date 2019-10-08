message(STATUS "Checking for LV2 library...")
pkg_check_modules(LV2 lv2)
if (NOT LV2_FOUND)
  message(FATAL_ERROR "can't find LV2 library")
endif(NOT LV2_FOUND)

set(GKICK_LV2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/plugin/lv2)
set(GKICK_LV2_SOURCES ${GKICK_LV2_DIR}/geonkick_lv2.cpp)

add_library(geonkick_lv2 SHARED
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_RESOURCES}
  ${GKICK_LV2_SOURCES})

target_link_libraries(geonkick_lv2  geonkick_common api_lv2)
target_link_libraries(geonkick_lv2 "-lstdc++fs")
target_link_libraries(geonkick_lv2 "-lredkite -lX11 -lsndfile -lrt -lm -ldl -lpthread -lcairo")
add_dependencies(geonkick_lv2 api_lv2)

install(DIRECTORY ${GKICK_LV2_DIR}/geonkick.lv2 DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2)
install(TARGETS geonkick_lv2 DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lv2/geonkick.lv2)
