# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/xtrem/OneDrive/Documents/pico-sdk-master/tools/pioasm"
  "C:/Users/xtrem/CLionProjects/SimiaCNC/simiacore/drivers/cmake-build-debug/pioasm"
  "C:/Users/xtrem/CLionProjects/SimiaCNC/simiacore/drivers/cmake-build-debug/pioasm"
  "C:/Users/xtrem/CLionProjects/SimiaCNC/simiacore/drivers/cmake-build-debug/pioasm/tmp"
  "C:/Users/xtrem/CLionProjects/SimiaCNC/simiacore/drivers/cmake-build-debug/pioasm/src/PioasmBuild-stamp"
  "C:/Users/xtrem/CLionProjects/SimiaCNC/simiacore/drivers/cmake-build-debug/pioasm/src"
  "C:/Users/xtrem/CLionProjects/SimiaCNC/simiacore/drivers/cmake-build-debug/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/xtrem/CLionProjects/SimiaCNC/simiacore/drivers/cmake-build-debug/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/xtrem/CLionProjects/SimiaCNC/simiacore/drivers/cmake-build-debug/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
