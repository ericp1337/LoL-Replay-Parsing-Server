# specifying common dirs

unix {
    TEMPDIR         = $$PRJDIR/tmp/$$TARGET
    QMAKE_CFLAGS    += -std=gnu99
    QMAKE_CXXFLAGS  += -std=c++11
}

win32 {
    warning("The windows portion is not guaranteed to work properly.")
    TEMPDIR         = $$PRJDIR/tmp/win32/$$TARGET
    CONFIG         += c++11
    DEFINES        += _WINDOWS WIN32_LEAN_AND_MEAN NOMINMAX
}

DESTDIR         = $$PRJDIR/bin
MOC_DIR         = $$TEMPDIR
OBJECTS_DIR     = $$TEMPDIR
RCC_DIR         = $$TEMPDIR
UI_DIR          = $$TEMPDIR/Ui
