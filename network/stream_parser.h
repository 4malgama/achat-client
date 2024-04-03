#ifndef STREAMPARSER_H
#define STREAMPARSER_H

#include <QString>

class StreamParser
{
	QByteArray streamData;
	int I;
	int length;

public:
	StreamParser();
	StreamParser(const StreamParser&) = delete;
	StreamParser(StreamParser&&) noexcept = delete;
	StreamParser(const QByteArray& data);

	int8_t parseByte(bool* ok = nullptr);
	int16_t parseInt16(bool* ok = nullptr);
	int32_t parseInt32(bool* ok = nullptr);
	int64_t parseInt64(bool* ok = nullptr);
	quint16 parseUInt16(bool* ok = nullptr);
	quint32 parseUInt32(bool* ok = nullptr);
	quint64 parseUInt64(bool* ok = nullptr);
	QString parseString(int length, bool* ok = nullptr);
	QByteArray parseData(int length, int size, bool* ok = nullptr);

	inline bool isEmpty() const { return length == 0; }
	inline int size() const { return length; }
	inline int index() const { return I; }
	
	void shift(int shift);

	void reset();
	void load(const QByteArray& data);

private:
	inline int16_t swapInt16(int16_t value) { return (value << 8) | (value >> 8); }
	inline int32_t swapInt32(int32_t value) { return (value << 24) | ((value << 8) & 0xFF0000) | ((value >> 8) & 0xFF00) | (value >> 24); }
	inline int64_t swapInt64(int64_t value) { return (value << 56) | ((value << 40) & 0xFF000000) | ((value << 24) & 0xFF0000) | ((value << 8) & 0xFF00) | (value >> 56); }
	inline quint16 swapUInt16(quint16 value) { return (value << 8) | (value >> 8); }
	inline quint32 swapUInt32(quint32 value) { return (value << 24) | ((value << 8) & 0xFF0000) | ((value >> 8) & 0xFF00) | (value >> 24); }
	inline quint64 swapUInt64(quint64 value) { return (value << 56) | ((value << 40) & 0xFF000000) | ((value << 24) & 0xFF0000) | ((value << 8) & 0xFF00) | (value >> 56); }
	QByteArray swapData(const QByteArray& data, int size);
};

#endif // STREAMPARSER_H
