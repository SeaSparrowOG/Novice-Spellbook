set(SRC_DIR "${SRC_DIR}")
set(DST_DIR "${DST_DIR}")
set(PROJECT_NAME "${PROJECT_NAME}")

set(ESP_SRC "${SRC_DIR}/${PROJECT_NAME}.esp")
if(EXISTS "${ESP_SRC}")
    message(STATUS "Copying ${ESP_SRC} to ${DST_DIR}")
    file(COPY "${ESP_SRC}" DESTINATION "${DST_DIR}")
endif()

set(CONFIG_SRC "${SRC_DIR}/SKSE/Plugins/${PROJECT_NAME}")
set(CONFIG_DST "${DST_DIR}/SKSE/Plugins/${PROJECT_NAME}")
if(EXISTS "${CONFIG_SRC}")
    file(REMOVE_RECURSE "${CONFIG_DST}")
    file(MAKE_DIRECTORY "${CONFIG_DST}")
    message(STATUS "Copying config folder from ${CONFIG_SRC} to ${CONFIG_DST}")
    file(COPY "${CONFIG_SRC}/" DESTINATION "${CONFIG_DST}")
endif()

set(CONFIG_SRC "${SRC_DIR}/SKSE/CustomConsole")
set(CONFIG_DST "${DST_DIR}/SKSE/CustomConsole")
if(EXISTS "${CONFIG_SRC}")
    file(REMOVE_RECURSE "${CONFIG_DST}")
    file(MAKE_DIRECTORY "${CONFIG_DST}")
    message(STATUS "Copying custom command folder from ${CONFIG_SRC} to ${CONFIG_DST}")
    file(COPY "${CONFIG_SRC}/" DESTINATION "${CONFIG_DST}")
endif()

set(INI_SRC "${SRC_DIR}/SKSE/Plugins/${PROJECT_NAME}.ini")
set(INI_DST "${DST_DIR}/SKSE/Plugins/${PROJECT_NAME}.ini")
if(EXISTS "${INI_SRC}")
    file(MAKE_DIRECTORY "${DST_DIR}/SKSE/Plugins")
    message(STATUS "Copying ${INI_SRC} to ${INI_DST}")
    file(COPY "${INI_SRC}" DESTINATION "${DST_DIR}/SKSE/Plugins")
endif()
