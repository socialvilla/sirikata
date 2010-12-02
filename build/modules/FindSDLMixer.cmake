#Sirikata
#FindSDL.cmake
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer in the documentation
#      and/or other materials provided with the distribution.
#    * Neither the name of the Sirikata nor the names of its contributors
#      may be used to endorse or promote products derived from this software
#      without specific prior written permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
#ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
#ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Searches for an sdl installation
#
# Defines:
#
#   SDL_mixer_FOUND          True if sdl was found, else false
#   SDL_mixer_LIBRARIES      Libraries to link (without full path)
#   SDL_mixer_LIBRARY_DIRS   Directories containing the libraries (-L option)
#   SDL_mixer_LDFLAGS        All required linker flags
#   SDL_mixer_INCLUDE_DIRS   The directories containing header files (-I option)
#   SDL_mixer_CFLAGS         All required compiler flags
#
# If you set sdl_MINIMUM_VERSION, only a version equal to or higher than this will be searched for. (This feature does not
# currently work on all systems.)
#
# On Windows, to specify the root directory of the sdl installation, set sdl_ROOT.
#
# Author: Siddhartha Chaudhuri, 2008
#

IF(WIN32)  # Windows

#  SET(sdl_FOUND FALSE)
#
#  IF(sdl_ROOT AND EXISTS "${sdl_ROOT}")
#    SET(sdl_LIBRARY_DIRS ${sdl_ROOT}/lib)
#    SET(sdl_LIBRARIES
#        debug sdl_d.lib
#        optimized sdl.lib)
#    SET(sdl_LDFLAGS)
#    SET(sdl_INCLUDE_DIRS ${sdl_ROOT}/include ${sdl_ROOT}/samples/include)
#    SET(sdl_CFLAGS)
#
#    SET(sdl_FOUND TRUE)
#
#  ENDIF(sdl_ROOT AND EXISTS "${sdl_ROOT}")
#
#ELSEIF(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")  # OS X
#
#  SET(sdl_FOUND FALSE)
#
#  #INCLUDE(CMakeFindFrameworks)
#  #CMAKE_FIND_FRAMEWORKS(sdl) 
#  SET(sdl_FRAMEWORKS ${sdl_ROOT}/Frameworks/SDL.framework)
#  IF(EXISTS ${sdl_FRAMEWORKS})
#    LIST(GET sdl_FRAMEWORKS 0 sdl_LIBRARIES)
#    SET(sdl_INCLUDE_DIRS ${sdl_LIBRARIES}/Headers)
#    
#    # Unset other variables
#    SET(sdl_LDFLAGS -F${sdl_LIBRARIES}/.. -framework SDL)
#
#    SET(sdl_LIBRARY_DIRS)
#    SET(sdl_LIBRARIES)
#    SET(sdl_CFLAGS)
#    SET(sdl_FOUND TRUE)
#  ENDIF(EXISTS ${sdl_FRAMEWORKS})
#
ELSE(WIN32)  # Linux etc
  FIND_PACKAGE(PkgConfig)
  IF(NOT PKG_CONFIG_FOUND)
    MESSAGE("Could not find pkg-config (to search for SDL_mixer)")
  ELSE(NOT PKG_CONFIG_FOUND)
		MESSAGE(STATUS " sdl mixer root = ${SDL_mixer_ROOT}")
    SET(ENV{PKG_CONFIG_PATH} ${SDL_mixer_ROOT}/../lib/pkgconfig:${SDL_mixer_ROOT}/lib/pkgconfig)
		PKG_CHECK_MODULES(SDL_mixer SDL_mixer)  # will set sdl_FOUND
		MESSAGE(STATUS "Found sdl mixer: headers at ${SDL_mixer_INCLUDE_DIRS}, libraries at ${SDL_mixer_LIBRARY_DIRS}")
		MESSAGE(STATUS "Found sdl mixer: SDL_mixer_CFLAGS at ${SDL_mixer_CFLAGS}") 
  ENDIF(NOT PKG_CONFIG_FOUND)
  IF(SDL_mixer_FOUND)
		MESSAGE(STATUS "Found sdl mixer: ")
  ENDIF()
ENDIF(WIN32)
