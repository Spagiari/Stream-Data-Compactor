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
    ${PROJECT_SOURCE_DIR}/src/istreambin.cpp
    ${PROJECT_SOURCE_DIR}/src/ostreambin.cpp
    )

SET(MOVETOFRONT
    ${PROJECT_SOURCE_DIR}/src/MoveToFront.cpp
    ${PROJECT_SOURCE_DIR}/src/MoveToFrontMain.cpp
    )

SET(BURROWSWHEELER
    ${PROJECT_SOURCE_DIR}/src/BurrowsWheeler.cpp
    ${PROJECT_SOURCE_DIR}/src/BurrowsWheelerMain.cpp
    )

SET(HUFFMAN
    ${PROJECT_SOURCE_DIR}/src/Huffman.cpp
    ${PROJECT_SOURCE_DIR}/src/HuffmanMain.cpp
    ${PROJECT_SOURCE_DIR}/src/istreambin.cpp
    ${PROJECT_SOURCE_DIR}/src/ostreambin.cpp
    )

add_executable (bw main.cpp ${ALGS})
add_executable (MoveToFront ${MOVETOFRONT})
add_executable (BurrowsWheeler ${BURROWSWHEELER})
add_executable (Huffman ${HUFFMAN})

TARGET_LINK_LIBRARIES( MoveToFront
    ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})

TARGET_LINK_LIBRARIES( BurrowsWheeler
    ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})

TARGET_LINK_LIBRARIES( Huffman
    ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})
