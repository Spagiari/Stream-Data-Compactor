add_definitions (-D_GNU_SOURCE)

FIND_PACKAGE( Boost 1.55 COMPONENTS system filesystem program_options REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

# add the executable
SET(ALGS
    ${PROJECT_SOURCE_DIR}/src/Quick3stringEx.cpp
    ${PROJECT_SOURCE_DIR}/src/CircularSuffixArray.cpp
    ${PROJECT_SOURCE_DIR}/src/MoveToFront.cpp
    ${PROJECT_SOURCE_DIR}/src/BurrowsWheeler.cpp
    ${PROJECT_SOURCE_DIR}/src/Huffman.cpp
    )

SET(MOVETOFRONT
    ${PROJECT_SOURCE_DIR}/src/MoveToFront.cpp
    ${PROJECT_SOURCE_DIR}/src/MoveToFrontMain.cpp
    )

SET(BURROWSWHEELER
    ${PROJECT_SOURCE_DIR}/src/BurrowsWheeler.cpp
    ${PROJECT_SOURCE_DIR}/src/BurrowsWheelerMain.cpp
    )

add_executable (bw main.cpp ${ALGS})
add_executable (MoveToFront ${MOVETOFRONT})
add_executable (BurrowsWheeler ${BURROWSWHEELER})

TARGET_LINK_LIBRARIES( MoveToFront
    ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})

TARGET_LINK_LIBRARIES( BurrowsWheeler
    ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})
