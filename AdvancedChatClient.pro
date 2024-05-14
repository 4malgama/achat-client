QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

TARGET = AChatClient

DESTDIR = build

win32:VERSION = 1.0.0.0
else:VERSION = 1.0.0

OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = ui
UI_HEADERS_DIR = ui_h
UI_SOURCES_DIR = ui_s

LIBS += -L"C:\Program Files (x86)\OpenSSL-Win32\lib\MinGW" -llibssl -llibcrypto
LIBS += -L"C:\Program Files\OpenSSL-Win64\lib\MinGW" -llibssl -llibcrypto

SOURCES += \
	free.cpp \
	main.cpp \
	client.cpp \
	network/account.cpp \
	network/network.cpp \
	network/packet_factory.cpp \
	network/packets/auth_accept_packet.cpp \
	network/packets/auth_packet.cpp \
	network/packets/auth_reject_packet.cpp \
	network/packets/check_avatar_hash_packet.cpp \
	network/packets/client_hello_packet.cpp \
	network/packets/client_ready_packet.cpp \
	network/packets/get_init_messages_packet.cpp \
	network/packets/init_chats_packet.cpp \
	network/packets/init_location_packet.cpp \
	network/packets/init_messages_packet.cpp \
	network/packets/init_profile_packet.cpp \
	network/packets/ipacket.cpp \
	network/packets/new_message_packet.cpp \
	network/packets/register_packet.cpp \
	network/packets/send_message_packet.cpp \
	network/packets/server_hello_packet.cpp \
	network/packets/server_ready_packet.cpp \
	network/packets/update_avatar_packet.cpp \
	network/packets/update_profile_packet.cpp \
	network/stream_builder.cpp \
	network/stream_parser.cpp \
	resource_manager/resource_manager.cpp \
	secure/certification/certification_manager.cpp \
	secure/encryption/aes.cpp \
	settings/settings_manager.cpp \
	test/widgets/ripple_button_widget.cpp \
	utils/CryptoUtils.cpp \
	utils/json_utils.cpp \
	widgets/advert_widget.cpp \
	widgets/authorization_widget.cpp \
	widgets/chat_message_widget.cpp \
	widgets/chat_row_widget.cpp \
	widgets/chats_widget.cpp \
	widgets/message_widget.cpp \
	widgets/profile_widget.cpp \
	widgets/server_message_widget.cpp

HEADERS += \
	client.h \
	network/account.h \
	network/network.h \
	network/packet_factory.h \
	network/packets.h \
	network/packets/all.h \
	network/packets/auth_accept_packet.h \
	network/packets/auth_packet.h \
	network/packets/auth_reject_packet.h \
	network/packets/check_avatar_hash_packet.h \
	network/packets/client_hello_packet.h \
	network/packets/client_ready_packet.h \
	network/packets/get_init_messages_packet.h \
	network/packets/init_chats_packet.h \
	network/packets/init_location_packet.h \
	network/packets/init_messages_packet.h \
	network/packets/init_profile_packet.h \
	network/packets/ipacket.h \
	network/packets/new_message_packet.h \
	network/packets/register_packet.h \
	network/packets/send_message_packet.h \
	network/packets/server_hello_packet.h \
	network/packets/server_ready_packet.h \
	network/packets/update_avatar_packet.h \
	network/packets/update_profile_packet.h \
	network/stream_builder.h \
	network/stream_parser.h \
	resource_manager/resource_manager.h \
	secure/certification/certification_manager.h \
	secure/encryption/aes.h \
	settings/settings_manager.h \
	test/widgets/ripple_button_widget.h \
	types/classes.h \
	types/types_variables.h \
	utils/json_utils.h \
	widgets/advert_widget.h \
	widgets/authorization_widget.h \
	widgets/chat_message_widget.h \
	widgets/chat_row_widget.h \
	widgets/chats_widget.h \
	widgets/message_widget.h \
	widgets/profile_widget.h \
	widgets/server_message_widget.h

FORMS += \
	client.ui \
	widgets/authorization_widget.ui \
	widgets/chats_widget.ui \
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
