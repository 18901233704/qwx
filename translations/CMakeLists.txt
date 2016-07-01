set(TRANSLATION_FILES
    zh_CN.ts
)

qt5_add_translation(QM_FILES ${TRANSLATION_FILES})

file(COPY ${QM_FILES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(translation DEPENDS ${QM_FILES})
