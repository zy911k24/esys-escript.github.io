# Copyright 2002 - 2008, 2010, 2011 National Technology Engineering
# Solutions of Sandia, LLC (NTESS). Under the terms of Contract
# DE-NA0003525 with NTESS, the U.S. Government retains certain rights
# in this software.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
# 
#     * Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials provided
#       with the distribution.
# 
#     * Neither the name of NTESS nor the names of its contributors
#       may be used to endorse or promote products derived from this
#       software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 


SET(HEADERS "")
SET(SOURCES "")


FILE(GLOB MESH_CLONE_HEADERS mesh_clone/*.hpp)
FILE(GLOB MESH_CLONE_SOURCES mesh_clone/*.cpp)
FILE(GLOB MESH_TOOLS_HEADERS mesh_tools/*.hpp)
FILE(GLOB MESH_TOOLS_SOURCES mesh_tools/*.cpp)
FILE(GLOB BLOCK_EXTRACTOR_HEADERS block_extractor/*.hpp)
FILE(GLOB BLOCK_EXTRACTOR_SOURCES block_extractor/*.cpp)
FILE(GLOB TRANSFER_UTILS_HEADERS transfer_utils/*.hpp)
FILE(GLOB TRANSFER_UTILS_SOURCES transfer_utils/*.cpp)
LIST(REMOVE_ITEM BLOCK_EXTRACTOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/block_extractor/main.cpp)

#
# C) Define the targets for package's library(s)
#

TRIBITS_ADD_LIBRARY(
  stk_transfer_utils_lib
  NOINSTALLHEADERS ${TRANSFER_UTILS_HEADERS}
  SOURCES ${TRANSFER_UTILS_SOURCES}
  )

TRIBITS_ADD_LIBRARY(
  stk_tools_lib
  NOINSTALLHEADERS ${MESH_CLONE_HEADERS} ${MESH_TOOLS_HEADERS} ${BLOCK_EXTRACTOR_HEADERS}
  SOURCES ${MESH_CLONE_SOURCES} ${MESH_TOOLS_SOURCES} ${BLOCK_EXTRACTOR_SOURCES}
  )

target_include_directories(stk_transfer_utils_lib PUBLIC
  $<BUILD_INTERFACE:${STK_TOPLEVEL_SOURCE_DIR}/stk_tools>
  $<INSTALL_INTERFACE:include>
)
target_include_directories(stk_tools_lib PUBLIC
  $<BUILD_INTERFACE:${STK_TOPLEVEL_SOURCE_DIR}/stk_tools>
  $<INSTALL_INTERFACE:include>
)

INSTALL(FILES ${MESH_CLONE_HEADERS} DESTINATION
    ${${PROJECT_NAME}_INSTALL_INCLUDE_DIR}/stk_tools/mesh_clone)

INSTALL(FILES ${MESH_TOOLS_HEADERS} DESTINATION
    ${${PROJECT_NAME}_INSTALL_INCLUDE_DIR}/stk_tools/mesh_tools)

INSTALL(FILES ${BLOCK_EXTRACTOR_HEADERS} DESTINATION
    ${${PROJECT_NAME}_INSTALL_INCLUDE_DIR}/stk_tools/block_extractor)

INSTALL(FILES ${TRANSFER_UTILS_HEADERS} DESTINATION
    ${${PROJECT_NAME}_INSTALL_INCLUDE_DIR}/stk_tools/transfer_utils)

TRIBITS_ADD_EXECUTABLE(
  stk_block_extractor
  NOEXEPREFIX
  NOEXESUFFIX
  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/block_extractor/main.cpp
  COMM serial mpi
  )

INSTALL(TARGETS stk_block_extractor COMPONENT ${PACKAGE_NAME} RUNTIME DESTINATION ${${PROJECT_NAME}_INSTALL_RUNTIME_DIR})
