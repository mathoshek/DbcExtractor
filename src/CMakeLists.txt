file(GLOB DbcExtractor_sources *.cpp *.h)

include_directories(
  ${CMAKE_SOURCE_DIR}/dep/StormLib
)

add_executable(DbcExtractor ${DbcExtractor_sources})

if( WIN32 )
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(DbcExtractor StormLib)

install(TARGETS DbcExtractor DESTINATION ${CMAKE_INSTALL_PREFIX})

