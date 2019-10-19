set(GKICK_VST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
if (GKICK_VST_SDK_PATH)
  list(APPEND CMAKE_MODULE_PATH "${GKICK_VST_SDK_PATH}/cmake/modules")
  include(Global)
  include(AddVST3Library)
else (GKICK_VST_SDK_PATH)
  message(STATUS "GKICK_VST_SDK_PATH was not set")
endif(GKICK_VST_SDK_PATH)

include_directories(${GKICK_VST_SDK_PATH})
include_directories(${GKICK_REDKITE_SDK_PATH}/include)
link_directories(${GKICK_VST_SDK_PATH}/build/lib/Debug)
link_directories(${GKICK_REDKITE_SDK_PATH}/lib)

set(GKICK_VST_HEADERS
  ${GKICK_VST_DIR}/GKickVstController.h
  ${GKICK_VST_DIR}/VstIds.h
  ${GKICK_VST_DIR}/GKickVstProcessor.h
  ${GKICK_VST_DIR}/GKickVstEditor.h)

set(GKICK_VST_SOURCES
  ${GKICK_VST_DIR}/VstFactory.cpp
  ${GKICK_VST_DIR}/GKickVstController.cpp
  ${GKICK_VST_DIR}/GKickVstProcessor.cpp
  ${GKICK_VST_DIR}/GKickVstEditor.cpp)

smtg_add_vst3plugin(geonkick_vst ${GKICK_VST_HEADERS} ${GKICK_VST_SOURCES})
target_link_libraries(geonkick_vst PRIVATE geonkick_common)
target_link_libraries(geonkick_vst PRIVATE base sdk redkite)
target_sources(geonkick_vst PRIVATE ${GKICK_VST_DIR}/info.rc)
add_dependencies(geonkick_vst geonkick_common)

