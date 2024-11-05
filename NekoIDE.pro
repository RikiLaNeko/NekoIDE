TEMPLATE = app
TARGET = NekoIDE
INCLUDEPATH += .
INCLUDEPATH += /usr/include/gtk-3.0
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/glib-2.0/include
INCLUDEPATH += /usr/include/pango-1.0
INCLUDEPATH += /usr/include/harfbuzz
INCLUDEPATH += /usr/include/cairo/
INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
INCLUDEPATH += /usr/include/atk-1.0

QT += core gui widgets
PKGCONFIG += gtk+-3.0

LIBS += -lgtk-3 -lgdk-3 -lglib-2.0 -lgobject-2.0 -lgthread-2.0 \
        -latk-1.0 -lgdk_pixbuf-2.0 -lpango-1.0 -lpangocairo-1.0 \
        -lcairo -lgio-2.0

SOURCES += main.cpp neko.c

