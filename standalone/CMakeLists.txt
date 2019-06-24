set(GKICK_STANDALONE_SOURCES ${GKICK_STANDALONE_DIR}/main.cpp)

add_executable(geonkick
  ${GKICK_COMMON_HEADERS}
  ${GKICK_COMMON_RESOURCES}
  ${GKICK_STANDALONE_SOURCES})

add_dependencies(geonkick api_standalone)
target_link_libraries(geonkick  geonkick_common)
target_link_libraries(geonkick "-mwindows -lredkite -lgeonkick -ld3d11 -ld2d1 -lstdc++ -lm")
#target_link_libraries(geonkick "-lstdc++fs")
#target_link_libraries(geonkick "-lredkite -lX11 -ljack -lsndfile -lrt -lm -ldl -lpthread -lcairo")
#target_link_libraries(geonkick "-lredkite -lrt -lm -ldl -lpthread")

install(TARGETS geonkick DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

