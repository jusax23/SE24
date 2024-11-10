# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (ImageGenerator)
add_executable(ImageGenerator
  main.cc
  logger.cc
)

set_property(TARGET ImageGenerator PROPERTY CXX_STANDARD 17)
target_compile_options(ImageGenerator PRIVATE -Wall)