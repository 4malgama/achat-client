# AChat Client

Кроссплатформенный клиент мессенджера, написанный на **C++ / Qt**.  
Поддерживает TCP/WebSocket соединения и шифрование через OpenSSL.

---

## Возможности

- Подключение к серверу администратора
- Работа через TCP / WebSocket
- Шифрование соединения (OpenSSL)
- Системный трей
- Кроссплатформенность (Windows / Linux)

---

## Зависимости

### Обязательные
- C++17
- Qt (5 или 6)

### Встроенные
- OpenSSL (уже включён в `third_party`)

---

## Windows (MinGW)

### Сборка

```bash
qmake
mingw32-make
```

### Запуск

После сборки рядом с `.exe` должны находиться:
- libssl-*.dll
- libcrypto-*.dll

Если их нет — скопируй из:

`third_party/openssl/windows/OpenSSL-Win64/bin/`

или добавь путь в `PATH`.

## Linux

### Установка зависимостей

### Arch

```bash
sudo pacman -S qt6-base openssl pkgconf
```

### Ubuntu / Debian

```bash
sudo apt install qtbase5-dev libssl-dev pkg-config
```

### Сборка

```bash
qmake
make
```

### Запуск

```bash
./AChatClient
```

## OpenSSL

- Windows → используется локальный `third_party`
- Linux → используется системный OpenSSL (`pkg-config`)

## Безопасность

Клиент использует:
- TLS / OpenSSL
- AES после handshake
- Проверку сертификатов