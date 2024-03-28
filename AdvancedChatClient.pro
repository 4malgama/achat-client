QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = AChatClient

DESTDIR = build

win32:VERSION = 1.0.0.0
else:VERSION = 1.0.0

OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = ui
UI_HEADERS_DIR = ui_h
UI_SOURCES_DIR = ui_s

SOURCES += \
	free.cpp \
	main.cpp \
	client.cpp \
	network/account.cpp \
	network/network.cpp \
	network/packet_factory.cpp \
	network/packets/auth_packet.cpp \
	network/packets/ipacket.cpp \
	settings/settings_manager.cpp \
	widgets/advert_widget.cpp \
	widgets/authorization_widget.cpp \
	widgets/profile_widget.cpp \
	widgets/server_message_widget.cpp

HEADERS += \
	client.h \
	network/account.h \
	network/network.h \
	network/packet_factory.h \
	network/packets.h \
	network/packets/auth_packet.h \
	network/packets/ipacket.h \
	settings/settings_manager.h \
	widgets/advert_widget.h \
	widgets/authorization_widget.h \
	widgets/profile_widget.h \
	widgets/server_message_widget.h

FORMS += \
	client.ui \
	widgets/authorization_widget.ui \
	widgets/profile_widget.ui \
	widgets/server_message_widget.ui

TRANSLATIONS += \
	AdvancedChatClient_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	resources.qrc
