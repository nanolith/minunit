find_package(PkgConfig REQUIRED)

pkg_check_modules(MODELCHECK REQUIRED modelcheck)

if (${MODELCHECK_FOUND})
    SET(OLD_PKG_CONFIG_PATH ENV{PKG_CONFIG_PATH})
    SET(ENV{PKG_CONFIG_PATH} ${MODELCHECK_PREFIX}/lib/pkgconfig)
    pkg_get_variable(MODELCHECK_ALLOCATOR_SOURCES modelcheck allocator_sources)
    SET(ENV{PKG_CONFIG_PATH} ${OLD_PKG_CONFIG_PATH})
endif(${MODELCHECK_FOUND})
