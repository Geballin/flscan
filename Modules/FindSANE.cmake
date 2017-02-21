#
# Find the SANE includes and library
#


FIND_PATH ( SANE_INCLUDE_PATH sane/sane.h
    /usr/local/include
    /usr/include
)

FIND_LIBRARY ( SANE_LIB_PATH sane
    PATHS /usr/lib /usr/local/lib
)
