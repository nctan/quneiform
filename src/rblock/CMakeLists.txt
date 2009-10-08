include_directories(BEFORE 
sources/h 
sources/main 
../hhh/tigerh
../hhh/tigerh/h)

add_library(rblock ${LIBTYPE}
${SNP_SRC}
_adaptive.cpp
_debug.cpp
_dll.cpp
_dlldummy.cpp
_graphics.cpp
_loadroots.cpp
_mymem.cpp
_partlayout.cpp
_partstring.cpp
_pic_n_table.cpp
_rblock.cpp
compdata.cpp
ltblglue.cpp
ltblocks.cpp
ltbreak.cpp
lterrors.cpp
ltexcomp.cpp
lthand.cpp
lthlundo.cpp
lthyst.cpp
ltibreak.cpp
ltinclin.cpp
ltmain.cpp
ltpage.cpp
ltremove.cpp
ltroots.cpp
ltseps.cpp
ltsmart.cpp
lttrees.cpp
ltwstrip.cpp
madebug.cpp
msgerr.cpp
qsort.cpp
scalar.cpp
seblocks.cpp
seform.cpp
sehbreak.cpp
sehyst.cpp
semain.cpp
sestring.cpp
sevbreak.cpp
)
set_property(TARGET rblock PROPERTY COMPILE_DEFINITIONS __RBLOCK__)

target_link_libraries(rblock cpage loc32 exc cstr cline rstr evn32)

install_dll(rblock)