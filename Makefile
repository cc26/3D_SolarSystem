#############################################################################
# Makefile for building: final
# Generated by qmake (2.01a) (Qt 4.7.0) on: Thu Dec 13 01:22:14 2012
# Project:  final.pro
# Template: app
# Command: /course/cs123/bin/cs123_qmake -o Makefile final.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/course/cs123/lib/qt/qt-4.7.0/mkspecs/default -I. -I/course/cs123/lib/qt/qt-4.7.0/include/QtCore -I/course/cs123/lib/qt/qt-4.7.0/include/QtGui -I/course/cs123/lib/qt/qt-4.7.0/include/QtOpenGL -I/course/cs123/lib/qt/qt-4.7.0/include -I/usr/X11R6/include -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1 -Wl,-rpath,/course/cs123/lib/qt/qt-4.7.0/lib
LIBS          = $(SUBLIBS)  -L/course/cs123/lib/qt/qt-4.7.0//lib -L/usr/X11R6/lib -lQtOpenGL -L/course/cs123/lib/qt/qt-4.7.0//lib -L/usr/X11R6/lib -lQtGui -lQtCore -lGLU -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /course/cs123/bin/cs123_qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		mainwindow.cpp \
		view.cpp \
		resourceloader.cpp \
		targa.cpp \
		glm.cpp \
		camera.cpp \
		particleemitter.cpp \
		common.cpp \
		bcurve.cpp moc_mainwindow.cpp \
		moc_view.cpp \
		qrc_textures.cpp
OBJECTS       = main.o \
		mainwindow.o \
		view.o \
		resourceloader.o \
		targa.o \
		glm.o \
		camera.o \
		particleemitter.o \
		common.o \
		bcurve.o \
		moc_mainwindow.o \
		moc_view.o \
		qrc_textures.o
DIST          = /gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/common/g++.conf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/common/unix.conf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/common/linux.conf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/qconfig.pri \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/qt_functions.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/qt_config.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/exclusive_builds.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/default_pre.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/release.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/default_post.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/warn_on.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/qt.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/unix/opengl.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/unix/thread.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/moc.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/resources.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/uic.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/yacc.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/lex.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/include_source_dir.prf \
		final.pro
QMAKE_TARGET  = final
DESTDIR       = 
TARGET        = final

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_mainwindow.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: final.pro  /course/cs123/lib/qt/qt-4.7.0/mkspecs/default/qmake.conf /gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/common/g++.conf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/common/unix.conf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/common/linux.conf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/qconfig.pri \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/qt_functions.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/qt_config.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/exclusive_builds.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/default_pre.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/release.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/default_post.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/warn_on.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/qt.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/unix/opengl.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/unix/thread.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/moc.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/resources.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/uic.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/yacc.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/lex.prf \
		/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/include_source_dir.prf \
		/course/cs123/lib/qt/qt-4.7.0/lib/libQtOpenGL.prl \
		/course/cs123/lib/qt/qt-4.7.0/lib/libQtGui.prl \
		/course/cs123/lib/qt/qt-4.7.0/lib/libQtCore.prl
	$(QMAKE) -o Makefile final.pro
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/common/g++.conf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/common/unix.conf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/common/linux.conf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/qconfig.pri:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/qt_functions.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/qt_config.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/exclusive_builds.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/default_pre.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/release.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/default_post.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/warn_on.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/qt.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/unix/opengl.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/unix/thread.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/moc.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/resources.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/uic.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/yacc.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/lex.prf:
/gpfs/main/sys/shared/psfu/contrib/projects/qt/qt-4.7.0/mkspecs/features/include_source_dir.prf:
/course/cs123/lib/qt/qt-4.7.0/lib/libQtOpenGL.prl:
/course/cs123/lib/qt/qt-4.7.0/lib/libQtGui.prl:
/course/cs123/lib/qt/qt-4.7.0/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile final.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/final1.0.0 || $(MKDIR) .tmp/final1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/final1.0.0/ && $(COPY_FILE) --parents mainwindow.h view.h camera.h vector.h resourceloader.h targa.h glm.h particleemitter.h common.h bcurve.h .tmp/final1.0.0/ && $(COPY_FILE) --parents textures/textures.qrc .tmp/final1.0.0/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp view.cpp resourceloader.cpp targa.cpp glm.cpp camera.cpp particleemitter.cpp common.cpp bcurve.cpp .tmp/final1.0.0/ && $(COPY_FILE) --parents mainwindow.ui .tmp/final1.0.0/ && (cd `dirname .tmp/final1.0.0` && $(TAR) final1.0.0.tar final1.0.0 && $(COMPRESS) final1.0.0.tar) && $(MOVE) `dirname .tmp/final1.0.0`/final1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/final1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check:

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_mainwindow.cpp moc_view.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_view.cpp
moc_mainwindow.cpp: mainwindow.h
	/course/cs123/lib/qt/qt-4.7.0//bin/moc $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_view.cpp: camera.h \
		vector.h \
		resourceloader.h \
		glm.h \
		particleemitter.h \
		common.h \
		view.h
	/course/cs123/lib/qt/qt-4.7.0//bin/moc $(DEFINES) $(INCPATH) view.h -o moc_view.cpp

compiler_rcc_make_all: qrc_textures.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_textures.cpp
qrc_textures.cpp: textures/textures.qrc \
		textures/Spacebg_1024.jpg \
		textures/sun.png \
		textures/Spacebg.jpg \
		textures/stranight2_1024.jpg \
		textures/comet.png \
		textures/moonmap2k_normal.jpeg \
		textures/stone_wall_normal_map__.jpg \
		textures/mercurymap.jpg \
		textures/stranight4_1024.jpg \
		textures/earthbump1k_enhance.png \
		textures/moonmap2k_enhance.jpg \
		textures/earthmap1k.jpg \
		textures/particle2.bmp \
		textures/moonmap2k.jpg \
		textures/earthbig.jpg \
		textures/earthbigNormalMap.jpeg \
		textures/earthcloudmap.jpg \
		textures/mercurybump_normal.jpg \
		textures/stranight1_1024.jpg \
		textures/earthbump_normal.jpeg \
		textures/particle1.bmp \
		textures/earthbig_enhance.jpg \
		textures/stone_wall__.jpg \
		textures/earthbig_enhance2.jpg \
		textures/moonmap2k_enhance2.jpg \
		textures/particle3.jpg
	/course/cs123/lib/qt/qt-4.7.0/bin/rcc -name textures textures/textures.qrc -o qrc_textures.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h
ui_mainwindow.h: mainwindow.ui \
		view.h \
		camera.h \
		vector.h \
		resourceloader.h \
		glm.h \
		particleemitter.h \
		common.h
	/course/cs123/lib/qt/qt-4.7.0/bin/uic mainwindow.ui -o ui_mainwindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

main.o: main.cpp mainwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		ui_mainwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

view.o: view.cpp view.h \
		camera.h \
		vector.h \
		resourceloader.h \
		glm.h \
		particleemitter.h \
		common.h \
		bcurve.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o view.o view.cpp

resourceloader.o: resourceloader.cpp resourceloader.h \
		glm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o resourceloader.o resourceloader.cpp

targa.o: targa.cpp targa.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o targa.o targa.cpp

glm.o: glm.cpp glm.h \
		targa.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o glm.o glm.cpp

camera.o: camera.cpp camera.h \
		vector.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o camera.o camera.cpp

particleemitter.o: particleemitter.cpp particleemitter.h \
		common.h \
		camera.h \
		vector.h \
		view.h \
		resourceloader.h \
		glm.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o particleemitter.o particleemitter.cpp

common.o: common.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o common.o common.cpp

bcurve.o: bcurve.cpp bcurve.h \
		vector.h \
		common.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o bcurve.o bcurve.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_view.o: moc_view.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_view.o moc_view.cpp

qrc_textures.o: qrc_textures.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_textures.o qrc_textures.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

