cmake_minimum_required(VERSION 3.15)
project(strace2ds)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O4")
find_package(LibXml2 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include /usr/local/dataseries/include ${LIBXML2_INCLUDE_DIR})
set(STRACE2DS_SRCS
    src/DataSeriesOutputModule.cpp
    src/MakeCloseArgsMap.cpp
    src/MakeOpenArgsMap.cpp
    src/MakeOpenAtArgsMap.cpp
    src/MakeReadArgsMap.cpp
    src/MakeMmapPreadArgsMap.cpp
    src/MakeMmapPwriteArgsMap.cpp
    src/MakeWriteArgsMap.cpp
    src/MakeChdirArgsMap.cpp
    src/MakeChrootArgsMap.cpp
    src/MakeRmdirArgsMap.cpp
    src/MakeUnlinkArgsMap.cpp
    src/MakeUnlinkAtArgsMap.cpp
    src/MakeMkdirArgsMap.cpp
    src/MakeMkdirAtArgsMap.cpp
    src/MakeCreatArgsMap.cpp
    src/MakeChmodArgsMap.cpp
    src/MakeUmaskArgsMap.cpp
    src/MakeSetxAttrArgsMap.cpp
    src/MakeLSetxAttrArgsMap.cpp
    src/MakeGetxAttrArgsMap.cpp
    src/MakeLGetxAttrArgsMap.cpp
    src/MakeFSetxAttrArgsMap.cpp
    src/MakeFGetxAttrArgsMap.cpp
    src/MakeListxAttrArgsMap.cpp
    src/MakeLListxAttrArgsMap.cpp
    src/MakeFListxAttrArgsMap.cpp
    src/MakeFLockArgsMap.cpp
    src/MakeRemovexAttrArgsMap.cpp
    src/MakeLRemovexAttrArgsMap.cpp
    src/MakeFRemovexAttrArgsMap.cpp
    src/MakeFChmodArgsMap.cpp
    src/MakeFChmodAtArgsMap.cpp
    src/MakeFChdirArgsMap.cpp
    src/MakeLinkArgsMap.cpp
    src/MakeLinkAtArgsMap.cpp
    src/MakeSymLinkArgsMap.cpp
    src/MakeSymlinkAtArgsMap.cpp
    src/MakeTruncateArgsMap.cpp
    src/MakeAccessArgsMap.cpp
    src/MakeFAccessAtArgsMap.cpp
    src/MakeLSeekArgsMap.cpp
    src/MakePReadArgsMap.cpp
    src/MakePWriteArgsMap.cpp
    src/MakeSetpgidArgsMap.cpp
    src/MakeSetrlimitArgsMap.cpp
    src/MakeSetsidArgsMap.cpp
    src/MakeStatArgsMap.cpp
    src/MakeStatfsArgsMap.cpp
    src/MakeFStatfsArgsMap.cpp
    src/MakeFTruncateArgsMap.cpp
    src/MakeChownArgsMap.cpp
    src/MakeReadlinkArgsMap.cpp
    src/MakeReadvArgsMap.cpp
    src/MakeWritevArgsMap.cpp
    src/MakeUtimeArgsMap.cpp
    src/MakeLStatArgsMap.cpp
    src/MakeFStatArgsMap.cpp
    src/MakeFStatatArgsMap.cpp
    src/MakeUtimesArgsMap.cpp
    src/MakeUtimesatArgsMap.cpp
    src/MakeRenameArgsMap.cpp
    src/MakeFsyncArgsMap.cpp
    src/MakeFdatasyncArgsMap.cpp
    src/MakeFallocateArgsMap.cpp
    src/MakeReadaheadArgsMap.cpp
    src/MakeMknodArgsMap.cpp
    src/MakeMknodatArgsMap.cpp
    src/MakePipeArgsMap.cpp
    src/MakeDupArgsMap.cpp
    src/MakeDup2ArgsMap.cpp
    src/MakeDup3ArgsMap.cpp
    src/MakeFcntlArgsMap.cpp
    src/MakeExitArgsMap.cpp
    src/MakeExecveArgsMap.cpp
    src/MakeMmapArgsMap.cpp
    src/MakeUnmapArgsMap.cpp
    src/MakeGetdentsArgsMap.cpp
    src/MakeGetrlimitArgsMap.cpp
    src/MakeIoctlArgsMap.cpp
    src/MakeCloneArgsMap.cpp
    src/MakeVForkArgsMap.cpp
    src/MakeSocketArgsMap.cpp
    src/MakeEpollCreateArgsMap.cpp
    src/MakeEpollCreate1ArgsMap.cpp
    src/MakeConnectArgsMap.cpp
    src/MakeBindArgsMap.cpp
    src/MakeAcceptArgsMap.cpp
    src/MakeAccept4ArgsMap.cpp
    src/MakeGetpeernameArgsMap.cpp
    src/MakeGetsocknameArgsMap.cpp
    src/MakeListenArgsMap.cpp
    src/MakeShutdownArgsMap.cpp
    src/MakeGetsockoptArgsMap.cpp
    src/MakeSetsockoptArgsMap.cpp
    src/MakeSocketpairArgsMap.cpp
    src/MakeRecvArgsMap.cpp
    src/MakeRecvfromArgsMap.cpp
    src/MakeRecvmsgArgsMap.cpp
    src/MakeSendArgsMap.cpp
    src/MakeSendtoArgsMap.cpp
    src/MakeSendmsgArgsMap.cpp
    src/strace2ds.cpp)
set(STRACE2DS_INCLUDES
    include/DataSeriesOutputModule.hpp
    include/strace2ds-enums.h
    include/strace2ds.h)
add_library(strace2ds STATIC ${STRACE2DS_SRCS} ${STRACE2DS_INCLUDES})
add_library(strace2ds_shared SHARED ${STRACE2DS_SRCS} ${STRACE2DS_INCLUDES})
set_target_properties(strace2ds_shared PROPERTIES OUTPUT_NAME strace2ds)
target_link_libraries(strace2ds DataSeries Lintel tcmalloc ${LIBXML2_LIBRARIES})
target_link_libraries(strace2ds_shared DataSeries Lintel tcmalloc ${LIBXML2_LIBRARIES})
install(TARGETS strace2ds ARCHIVE DESTINATION /usr/local/strace2ds/lib)
install(TARGETS strace2ds_shared LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/strace2ds/lib)
