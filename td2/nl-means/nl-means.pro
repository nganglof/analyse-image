#les fichiers temporaires nécessaires a QT (en mode caché)

MOC_DIR = .moc
OBJECTS_DIR = .obj

TEMPLATE = app
CONFIG += qt warn_on release thread
#possibilité d'ajouter des entetes avec la variable HEADERS
SOURCES = nl-means.cpp
TARGET = nl-means
CONFIG -= app_bundle

#inclusion des entetes opencv, directives speciales en fonction du systeme
enseirb {
	INCLUDEPATH += /opt/opencv/include/opencv2
	QMAKE_LFLAGS += -Wl, -R/opt/opencv/lib -L/opt/opencv/lib
}

#les lib pour l'édition des liens
LIBS += \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_features2d \
-lopencv_calib3d