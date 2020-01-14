# Public headers

# Egg
FILE (
  COPY "${CMAKE_CURRENT_SOURCE_DIR}/egg/variable.hpp"
  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/egg" )

# Egg public includes
SET (
  Public_Include

  "${CMAKE_CURRENT_BINARY_DIR}/egg/variable.hpp"

  CACHE INTERNAL "Common headers" )


# Install the Egg::Variable library
INSTALL(
  FILES       ${Public_Include}
  DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/egg" )

# End of file
