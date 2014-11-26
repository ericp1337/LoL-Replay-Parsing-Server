# specifying common dirs

unix {
    TEMPDIR         = /tmp/$$TARGET
    macx:TEMPDIR    = /tmp/$$TARGET
    QMAKE_CFLAGS    += -std=gnu99
    QMAKE_CXXFLAGS  += -std=c++11
}

!unix:error("your platform is not supported!")


DESTDIR         = $$PRJDIR/bin
MOC_DIR         = $$TEMPDIR
OBJECTS_DIR     = $$TEMPDIR
RCC_DIR         = $$TEMPDIR
UI_DIR          = $$TEMPDIR/Ui
