project(fracgm_src)
include(GNUInstallDirs)

add_library(fracgm SHARED src/registration.cpp src/utils.cpp)
target_include_directories(fracgm PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMALE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(fracgm Eigen3::Eigen)
add_library(FracGM::fracgm ALIAS fracgm)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION include
)
install(TARGETS fracgm
  EXPORT FracGMConfig
  LIBRARY DESTINATION lib
)

export(TARGETS fracgm
  NAMESPACE FracGM::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/FracGMConfig.cmake"
)
install(EXPORT FracGMConfig
  DESTINATION "${CMAKE_INSTALL_DATADIR}/FracGM/cmake"
  NAMESPACE FracGM::
)