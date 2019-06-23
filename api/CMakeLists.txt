set(GKICK_API_SOURCES
${GKICK_API_DIR}/src/audio_output.c
${GKICK_API_DIR}/src/compressor.c
${GKICK_API_DIR}/src/distortion.c
${GKICK_API_DIR}/src/envelope.c
${GKICK_API_DIR}/src/filter.c
${GKICK_API_DIR}/src/geonkick.c
${GKICK_API_DIR}/src/gkick_audio.c
${GKICK_API_DIR}/src/gkick_buffer.c
${GKICK_API_DIR}/src/gkick_jack.c
${GKICK_API_DIR}/src/gkick_log.c
${GKICK_API_DIR}/src/oscillator.c
${GKICK_API_DIR}/src/synthesizer.c)

set(GKICK_API_HEADERS
${GKICK_API_DIR}/src/audio_output.h
${GKICK_API_DIR}/src/compressor.h
${GKICK_API_DIR}/src/distortion.h
${GKICK_API_DIR}/src/envelope.h
${GKICK_API_DIR}/src/filter.h
${GKICK_API_DIR}/src/geonkick.h
${GKICK_API_DIR}/src/geonkick_internal.h
${GKICK_API_DIR}/src/gkick_audio.h
${GKICK_API_DIR}/src/gkick_buffer.h
${GKICK_API_DIR}/src/gkick_jack.h
${GKICK_API_DIR}/src/gkick_log.h
${GKICK_API_DIR}/src/oscillator.h
${GKICK_API_DIR}/src/synthesizer.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
add_library(api_standalone STATIC ${GKICK_API_HEADERS} ${GKICK_API_SOURCES})
set_target_properties(api_standalone PROPERTIES COMPILE_FLAGS "${GKICK_API_FLAGS}")
