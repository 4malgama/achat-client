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

RC_ICONS = resources/icons/icon64.ico

LIBS += -L"C:\Program Files (x86)\OpenSSL-Win32\lib\MinGW" -llibssl -llibcrypto
LIBS += -L"C:\Program Files\OpenSSL-Win64\lib\MinGW" -llibssl -llibcrypto

SOURCES += \
	application.cpp \
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
	network/packets/create_chat_with_message_packet.cpp \
	network/packets/download_file_packet.cpp \
	network/packets/get_init_messages_packet.cpp \
	network/packets/init_chats_packet.cpp \
	network/packets/init_location_packet.cpp \
	network/packets/init_messages_packet.cpp \
	network/packets/init_profile_packet.cpp \
	network/packets/ipacket.cpp \
	network/packets/new_message_packet.cpp \
	network/packets/register_packet.cpp \
	network/packets/search_packet.cpp \
	network/packets/send_file_packet.cpp \
	network/packets/send_message_packet.cpp \
	network/packets/send_token_packet.cpp \
	network/packets/server_hello_packet.cpp \
	network/packets/server_ready_packet.cpp \
	network/packets/update_avatar_packet.cpp \
	network/packets/update_chat_id_packet.cpp \
	network/packets/update_profile_packet.cpp \
	network/stream_builder.cpp \
	network/stream_parser.cpp \
	network/web/gpt_service.cpp \
	network/web/request_builder.cpp \
	network/web/web_service.cpp \
	resource_manager/resource_manager.cpp \
	secure/certification/certification_manager.cpp \
	secure/encryption/aes.cpp \
	settings/settings_manager.cpp \
	test/widgets/ripple_button_widget.cpp \
	utils/CryptoUtils.cpp \
	utils/image_utils.cpp \
	utils/json_utils.cpp \
	widgets/advert_widget.cpp \
	widgets/attachment_button_widget.cpp \
	widgets/authorization_widget.cpp \
	widgets/chat_message_widget.cpp \
	widgets/chat_row_widget.cpp \
	widgets/chats_widget.cpp \
	widgets/console_widget.cpp \
	widgets/dropdown_menu.cpp \
	widgets/message_widget.cpp \
	widgets/profile_widget.cpp \
	widgets/search_result_widget.cpp \
	widgets/search_widget.cpp \
	widgets/server_message_widget.cpp

HEADERS += \
	application.h \
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
	network/packets/create_chat_with_message_packet.h \
	network/packets/download_file_packet.h \
	network/packets/get_init_messages_packet.h \
	network/packets/init_chats_packet.h \
	network/packets/init_location_packet.h \
	network/packets/init_messages_packet.h \
	network/packets/init_profile_packet.h \
	network/packets/ipacket.h \
	network/packets/new_message_packet.h \
	network/packets/register_packet.h \
	network/packets/search_packet.h \
	network/packets/send_file_packet.h \
	network/packets/send_message_packet.h \
	network/packets/send_token_packet.h \
	network/packets/server_hello_packet.h \
	network/packets/server_ready_packet.h \
	network/packets/update_avatar_packet.h \
	network/packets/update_chat_id_packet.h \
	network/packets/update_profile_packet.h \
	network/stream_builder.h \
	network/stream_parser.h \
	network/web/gpt_service.h \
	network/web/request_builder.h \
	network/web/web_service.h \
	resource_manager/resource_manager.h \
	secure/certification/certification_manager.h \
	secure/encryption/aes.h \
	settings/settings_manager.h \
	test/widgets/ripple_button_widget.h \
	types/classes.h \
	types/types_variables.h \
	utils/image_utils.h \
	utils/json_utils.h \
	widgets/advert_widget.h \
	widgets/attachment_button_widget.h \
	widgets/authorization_widget.h \
	widgets/chat_message_widget.h \
	widgets/chat_row_widget.h \
	widgets/chats_widget.h \
	widgets/console_widget.h \
	widgets/dropdown_menu.h \
	widgets/message_widget.h \
	widgets/profile_widget.h \
	widgets/search_result_widget.h \
	widgets/search_widget.h \
	widgets/server_message_widget.h

FORMS += \
	client.ui \
	widgets/authorization_widget.ui \
	widgets/chats_widget.ui \
	widgets/console_widget.ui \
	widgets/profile_widget.ui \
	widgets/search_widget.ui \
	widgets/server_message_widget.ui

TRANSLATIONS += \
	AdvancedChatClient_en_GB.ts \
	AdvancedChatClient_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	resources.qrc

DISTFILES += \
	AdvancedChatClient_ru_RU.ts
