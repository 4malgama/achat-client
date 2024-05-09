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
	QByteArray swapData(const QByteArray& data, int size);
};

#endif // STREAMPARSER_H
