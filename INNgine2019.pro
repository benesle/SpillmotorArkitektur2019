QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++17

TARGET      = INNgine2019
#CONFIG   += cmdline PRECOMPILED_HEADER

PRECOMPILED_HEADER = innpch.h

PRECOMPILED_HEADER:!isEmpty(PRECOMPILED_HEADER) {
DEFINES += USING_PCH
}

INCLUDEPATH +=\
./GSL\
./Components\
./Objects\
./Sound\
./Shaders


mac{
LIBS += -framework OpenAL\
#-framework glut\
#-framework glut32

}
win32{
#I can't say that I am surprised that this turned out miserably

#    INCLUDEPATH += $(GLUT_HOME)\\include\\glut32
#    LIBS *= $(GLUT_HOME)\\glutdlls37beta\\glut32.dll

#    INCLUDEPATH += $(GLUT_HOME)\\include\\glut
#    LIBS *= $(GLUT_HOME)\\glutdlls37beta\\glut.dll

    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a

# Copy required DLLs to output directory
    contains(QT_ARCH, x86_64){
        CONFIG(debug, debug|release) {
            OpenAL32.commands = copy /Y \"$(OPENAL_HOME)\\bin\\Win64\\OpenAL32.dll\" debug
            OpenAL32.target = debug/OpenAL32.dll

            QMAKE_EXTRA_TARGETS += OpenAL32
            PRE_TARGETDEPS += debug/OpenAL32.dll
        } else:CONFIG(release, debug|release) {
            OpenAL32.commands = copy /Y \"$(OPENAL_HOME)\\bin\\Win64\\OpenAL32.dll\" release
            OpenAL32.target = release/OpenAL32.dll

            QMAKE_EXTRA_TARGETS += OpenAL32
            PRE_TARGETDEPS += release/OpenAL32.dll release/OpenAL32.dll
        } else {
            error(Unknown set of dependencies.)
        }
    }

#    contains(QT_ARCH, x86_64){
#        CONFIG(debug, debug|release) {
#            glut.commands = copy /Y \"$(GLUT_HOME)\\glutdlls37beta\\glut.dll\" debug
#            glut.target = debug/glut.dll

#            QMAKE_EXTRA_TARGETS += glut
#            PRE_TARGETDEPS += debug/glut.dll
#        } else:CONFIG(release, debug|release) {
#            glut.commands = copy /Y \"$(GLUT_HOME)\\glutdlls37beta\\glut.dll\" release
#            glut.target = release/glut.dll

#            QMAKE_EXTRA_TARGETS += glut
#            PRE_TARGETDEPS += release/glut.dll release/glut.dll
#        } else {
#            error(Unknown set of dependencies.)
#        }
#    }

#    contains(QT_ARCH, x86_64){
#        CONFIG(debug, debug|release) {
#            glut32.commands = copy /Y \"$(GLUT_HOME)\\glutdlls37beta\\glut32.dll\" debug
#            glut32.target = debug/glut32.dll

#            QMAKE_EXTRA_TARGETS += glut32
#            PRE_TARGETDEPS += debug/glut32.dll
#        } else:CONFIG(release, debug|release) {
#            glut32.commands = copy /Y \"$(GLUT_HOME)\\glutdlls37beta\\glut32.dll\" release
#            glut32.target = release/glut32.dll

#            QMAKE_EXTRA_TARGETS += glut32
#            PRE_TARGETDEPS += release/glut32.dll release/glut32.dll
#        } else {
#            error(Unknown set of dependencies.)
#        }
#    }
}


#INCLUDEPATH += $(GLUT_HOME)\\include\\glut

#HEADERS += \
#glut.h

HEADERS += \
    Components/soundmanager.h \
    GSL/mathcollection.h \
    Components/collisionmanager.h \
    coreengine.h \
    entitywidget.h \
    innpch.h\
    mainwindow.h \
    mousepicker.h \
    Components/movementsystem.h \
    renderwindow.h \
    Components/entitymanager.h

HEADERS += \
    GSL/matrix2x2.h \
    GSL/matrix3x3.h \
    GSL/matrix4x4.h \
    GSL/vector2d.h \
    GSL/vector3d.h \
    GSL/vector4d.h \
    GSL/vertex.h

HEADERS += \
GSL/gltypes.h \
GSL/math_constants.h

HEADERS += \
GSL/gsl_math.h

HEADERS += \
    Components/collisioncomponent.h \
    Components/transformcomponent.h \
    Components/materialcomponent.h \
    Components/meshcomponent.h \
    Components/soundcomponent.h \
    Components/rendercomponent.h

HEADERS += \
    Sound/audio.h \
    Sound/soundsource.h

HEADERS += \
        factory.h \
        game.h \
        wavefilehandler.h \
        openal.h \
        input.h \
        constants.h

HEADERS += \
    camera.h \
    skybox.h

HEADERS += \
    Objects/xyz.h \
    Objects/visualobject.h \
    Objects/octahedronball.h \
    Objects/trianglesurface.h \
    Objects/triangle.h \
    Objects/objmesh.h \
    entity.h \
    Objects/billboard.h

HEADERS += \
    Shaders/colorshader.h \
    Shaders/textureshader.h \
    Shaders/phongshader.h \
    Shaders/shader.h \
    Shaders/texture.h \
    Shaders/material.h \
    Shaders/light.h

SOURCES += \
    Components/soundmanager.cpp \
    Components/collisionmanager.cpp\
    coreengine.cpp \
    entitywidget.cpp \
    main.cpp \
    mainwindow.cpp \
    mousepicker.cpp \
    Components/movementsystem.cpp \
    renderwindow.cpp \
    Components/entitymanager.cpp

SOURCES +=  \
    GSL/matrix2x2.cpp \
    GSL/matrix3x3.cpp \
    GSL/matrix4x4.cpp \
    GSL/vector2d.cpp \
    GSL/vector3d.cpp \
    GSL/vector4d.cpp \
    GSL/vertex.cpp

SOURCES += \
GSL/gsl_math.cpp

SOURCES += \
     Components/collisioncomponent.cpp \
     Components/transformcomponent.cpp \
     Components/materialcomponent.cpp \
     Components/meshcomponent.cpp \
     Components/soundcomponent.cpp \
     Components/rendercomponent.cpp

SOURCES += \
    Sound/audio.cpp \
    Sound/soundsource.cpp

SOURCES += \
    factory.cpp \
    game.cpp \
    wavefilehandler.cpp \
    input.cpp

SOURCES +=  \
      camera.cpp \
      skybox.cpp

 SOURCES +=  \
        Objects/xyz.cpp \
        Objects/visualobject.cpp \
        Objects/octahedronball.cpp \
        Objects/trianglesurface.cpp \
        Objects/triangle.cpp \
        Objects/objmesh.cpp \
        entity.cpp \
        Objects/billboard.cpp

 SOURCES +=  \
     Shaders/colorshader.cpp \
     Shaders/textureshader.cpp \
     Shaders/phongshader.cpp\
     Shaders/shader.cpp\
     Shaders/texture.cpp \
     Shaders/material.cpp \
     Shaders/light.cpp
#   Objects/octahedronball.cpp

FORMS += \
    entitywid.ui \
    mainwindow.ui

DISTFILES += \
    Shaders/phongshader.frag \
    Shaders/phongshader.vert \
    Shaders/plainshader.frag \
    Shaders/plainshader.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert

RESOURCES += \




