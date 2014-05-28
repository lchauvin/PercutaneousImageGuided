cmake_minimum_required(VERSION 2.8.9)

project(PercutaneousImageGuided)

#-----------------------------------------------------------------------------
set(EXTENSION_HOMEPAGE "http://www.slicer.org/slicerWiki/index.php/Documentation/4.3/Extensions/PercutaneousImageGuided")
set(EXTENSION_CATEGORY "IGT")
set(EXTENSION_CONTRIBUTORS "Laurent Chauvin (SNR)")
set(EXTENSION_DESCRIPTION "This is an example of extension bundling a loadable module")
set(EXTENSION_ICONURL "http://viewvc.slicer.org/viewvc.cgi/Slicer4/trunk/Extensions/Testing/PercutaneousImageGuided/PercutaneousImageGuided.png?revision=21746&view=co")
set(EXTENSION_SCREENSHOTURLS "http://wiki.slicer.org/slicerWiki/images/4/42/Slicer-r19441-PercutaneousImageGuided-screenshot.png")

#-----------------------------------------------------------------------------
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

#-----------------------------------------------------------------------------
add_subdirectory(PercutaneousImageGuided)

#-----------------------------------------------------------------------------
include(${Slicer_EXTENSION_CPACK})
