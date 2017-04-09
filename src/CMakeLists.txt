cmake_minimum_required(VERSION 3.6)
set(CMAKE_C_STANDARD 99)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
set(SOURCE_FILES acls.c
        attrib.c
        attrlist.c
        bitmap.c
        bootsect.c
        cache.c
        collate.c
        compat.c
        compress.c
        debug.c
        device.c
        dir.c
        ea.c
        efs.c
        index.c
        inode.c
        ioctl.c
        lcnalloc.c
        logfile.c
        logging.c
        mft.c
        misc.c
        mst.c
        object_id.c
        realpath.c
        reparse.c
        runlist.c
        security.c
        unistr.c
        volume.c
        xattrs.c)
ADD_DEFINITIONS("-DHAVE_CONFIG_H")
add_library(ntfs STATIC ${SOURCE_FILES})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
