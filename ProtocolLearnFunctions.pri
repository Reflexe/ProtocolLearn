PL_BUILD_DIR = $$PWD/build
PL_LIBRARYS_DIR = $$PL_BUILD_DIR
PL_SOURCE_DIR = $$PWD

defineTest(includeProtocolLearnUtility) {
    # Include first the depended libraries.
    LIBS += -L$$PL_LIBRARYS_DIR/$$1 -l$$basename(1)

    # After that, include the undepend libraries.
    include($$PL_SOURCE_DIR/$${1}/$$basename(1).pro)

    export(LIBS)
}

INCLUDEPATH *= $$PWD/include
