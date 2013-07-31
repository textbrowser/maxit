purge.commands = rm -f *~ && rm -f include.d/*~ && rm -f ui.d/*~

TEMPLATE	= app
LANGUAGE	= C++
CONFIG         += qt thread release warn_on app_bundle
QT		+= opengl
ICON            = images.d/maxit.icns
QMAKE_LFLAGS_RELEASE =
QMAKE_LFLAGS_RPATH =
QMAKE_CLEAN	+= Maxit
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -Wall -Werror -Wextra -Wstack-protector -fPIE -fstack-protector-all -pie
QMAKE_EXTRA_TARGETS = purge
INCLUDEPATH	+= include.d

FORMS           = ui.d/maxit.ui

HEADERS		= include.d/maxit.h \
		  include.d/glpiece.h \
		  include.d/computer.h

SOURCES		= maxit.cc \
		  glpiece.cc \
		  computer.cc \
		  maxit_main.cc

TARGET		= Maxit
PROJECTNAME	= Maxit
