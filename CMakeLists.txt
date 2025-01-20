cmake_minimum_required(VERSION 3.10)
project(CompositeRender)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify vendor libraries
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")

# ----------------------
# GLFW (Precompiled)
# ----------------------
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${VENDOR_DIR}/glfw/lib-vc2022/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${VENDOR_DIR}/glfw/include"
)

# ----------------------
# Glad
# ----------------------
add_library(glad STATIC
    ${VENDOR_DIR}/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    ${VENDOR_DIR}/glad/include
)

# ----------------------
# ImGui
# ----------------------
add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)
set(IMGUI_SRC
    ${VENDOR_DIR}/imgui/imgui.cpp
    ${VENDOR_DIR}/imgui/imgui_draw.cpp
    ${VENDOR_DIR}/imgui/imgui_widgets.cpp
    ${VENDOR_DIR}/imgui/imgui_tables.cpp
    ${VENDOR_DIR}/imgui/imgui_filedialog.cpp
    ${VENDOR_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${VENDOR_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
    ${VENDOR_DIR}/imgui
    ${VENDOR_DIR}/imgui/backends
)
target_link_libraries(imgui PRIVATE glfw glad)

# ----------------------
# TinyXML2
# ----------------------
add_library(tinyxml2 STATIC
    ${VENDOR_DIR}/tinyxml2/tinyxml2.cpp
)
target_include_directories(tinyxml2 PUBLIC
    ${VENDOR_DIR}/tinyxml2/
)
target_link_libraries(tinyxml2 PRIVATE imgui glfw glad)

# ----------------------
# zlib
# ----------------------
add_library(zlib STATIC
    ${VENDOR_DIR}/zlib/compress.c
    ${VENDOR_DIR}/zlib/crc32.c
    ${VENDOR_DIR}/zlib/deflate.c
    ${VENDOR_DIR}/zlib/gzclose.c
    ${VENDOR_DIR}/zlib/gzlib.c
    ${VENDOR_DIR}/zlib/gzread.c
    ${VENDOR_DIR}/zlib/gzwrite.c
    ${VENDOR_DIR}/zlib/infback.c
    ${VENDOR_DIR}/zlib/inffast.c
    ${VENDOR_DIR}/zlib/inflate.c
    ${VENDOR_DIR}/zlib/inftrees.c
    ${VENDOR_DIR}/zlib/trees.c
    ${VENDOR_DIR}/zlib/uncompr.c
    ${VENDOR_DIR}/zlib/zutil.c
    ${VENDOR_DIR}/zlib/adler32.c
)
target_include_directories(zlib PUBLIC
    ${VENDOR_DIR}/zlib/
)
target_link_libraries(zlib PRIVATE imgui glfw glad tinyxml2)

# ----------------------
# libjpeg-turbo
# ----------------------
add_library(libjpeg STATIC
    ${VENDOR_DIR}/libjpeg-turbo/src/cdjpeg.c
    ${VENDOR_DIR}/libjpeg-turbo/src/cjpeg.c
    ${VENDOR_DIR}/libjpeg-turbo/src/djpeg.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jaricom.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcapimin.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcapistd.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcinit.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcarith.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jccoefct.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jccolor.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcdctmgr.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcdiffct.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jchuff.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcicc.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jclhuff.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jclossls.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcmainct.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcmarker.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcmaster.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcomapi.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcparam.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcphuff.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcprepct.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jcsample.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jctrans.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdapimin.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdapistd.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdarith.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdatadst-tj.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdatadst.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdatasrc-tj.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdatasrc.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdcoefct.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdcolor.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jddctmgr.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jddiffct.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdhuff.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdicc.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdinput.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdlhuff.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdlossls.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdmainct.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdmarker.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdmerge.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdphuff.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdpostct.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdsample.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jdtrans.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jerror.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jfdctflt.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jfdctfst.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jfdctint.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jidctflt.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jidctfst.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jidctint.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jidctred.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jmemmgr.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jmemnobs.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jpegtran.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jpeg_nbits.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jquant1.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jquant2.c
    ${VENDOR_DIR}/libjpeg-turbo/src/jutils.c
    ${VENDOR_DIR}/libjpeg-turbo/src/rdbmp.c
    ${VENDOR_DIR}/libjpeg-turbo/src/rdcolmap.c
    ${VENDOR_DIR}/libjpeg-turbo/src/rdgif.c
    ${VENDOR_DIR}/libjpeg-turbo/src/rdjpgcom.c
    ${VENDOR_DIR}/libjpeg-turbo/src/rdppm.c
    ${VENDOR_DIR}/libjpeg-turbo/src/rdswitch.c
    ${VENDOR_DIR}/libjpeg-turbo/src/rdtarga.c
    ${VENDOR_DIR}/libjpeg-turbo/src/tjbench.c
    ${VENDOR_DIR}/libjpeg-turbo/src/tjcomp.c
    ${VENDOR_DIR}/libjpeg-turbo/src/tjdecomp.c
    ${VENDOR_DIR}/libjpeg-turbo/src/tjtran.c
    ${VENDOR_DIR}/libjpeg-turbo/src/tjutil.c
    ${VENDOR_DIR}/libjpeg-turbo/src/transupp.c
    ${VENDOR_DIR}/libjpeg-turbo/src/turbojpeg.c
    ${VENDOR_DIR}/libjpeg-turbo/src/wrbmp.c
    ${VENDOR_DIR}/libjpeg-turbo/src/wrgif.c
    ${VENDOR_DIR}/libjpeg-turbo/src/wrjpgcom.c
    ${VENDOR_DIR}/libjpeg-turbo/src/wrppm.c
    ${VENDOR_DIR}/libjpeg-turbo/src/wrtarga.c
)

target_include_directories(libjpeg PUBLIC
    ${VENDOR_DIR}/libjpeg-turbo/src/
)
target_link_libraries(libjpeg PRIVATE imgui glfw glad tinyxml2 zlib)

# ----------------------
# libpng
# ----------------------
add_library(libpng STATIC
    ${VENDOR_DIR}/libpng/png.c
    ${VENDOR_DIR}/libpng/pngerror.c
    ${VENDOR_DIR}/libpng/pngget.c
    ${VENDOR_DIR}/libpng/pngmem.c
    ${VENDOR_DIR}/libpng/pngread.c
    ${VENDOR_DIR}/libpng/pngpread.c
    ${VENDOR_DIR}/libpng/pngrio.c
    ${VENDOR_DIR}/libpng/pngrtran.c
    ${VENDOR_DIR}/libpng/pngrutil.c
    ${VENDOR_DIR}/libpng/pngset.c
    ${VENDOR_DIR}/libpng/pngtrans.c
    ${VENDOR_DIR}/libpng/pngwio.c
    ${VENDOR_DIR}/libpng/pngwrite.c
    ${VENDOR_DIR}/libpng/pngwtran.c
    ${VENDOR_DIR}/libpng/pngwutil.c
)
target_include_directories(libpng PUBLIC
    ${VENDOR_DIR}/libpng/
    ${VENDOR_DIR}/libpng/scripts
)
target_link_libraries(libpng PRIVATE imgui glfw glad tinyxml2 zlib)

# ----------------------
# PenguinV
# ----------------------
add_compile_definitions(PENGUINV_ENABLED_JPEG_SUPPORT)
add_compile_definitions(PENGUINV_ENABLED_PNG_SUPPORT)
add_library(penguinv STATIC
    ${VENDOR_DIR}/penguinv/src/image_function_helper.cpp
    ${VENDOR_DIR}/penguinv/src/image_function.cpp
    ${VENDOR_DIR}/penguinv/src/penguinv/penguinv.cpp
    ${VENDOR_DIR}/penguinv/src/file/file_image.cpp
    ${VENDOR_DIR}/penguinv/src/file/bmp_image.cpp
    ${VENDOR_DIR}/penguinv/src/file/png_image.cpp
    ${VENDOR_DIR}/penguinv/src/file/jpeg_image.cpp
)
target_include_directories(penguinv PUBLIC
    ${VENDOR_DIR}/penguinv/src/
    ${VENDOR_DIR}/penguinv/src/penguinv/
    ${VENDOR_DIR}/stb/
)
target_link_libraries(penguinv PRIVATE imgui glfw glad tinyxml2 zlib libpng)

# ----------------------
# Main Application
# ----------------------
file(GLOB_RECURSE SRC "src/*.cpp" "src/*/*.cpp")

add_executable(CompositeRenderer ${SRC})

# ----------------------
# STB IMAGE
# ----------------------
target_include_directories(CompositeRenderer PUBLIC
    ${VENDOR_DIR}/stb
)

# Link libraries to the application
target_link_libraries(CompositeRenderer PRIVATE glfw glad imgui tinyxml2 zlib libpng penguinv)
