#include "stream_parser.h"
#include <QTextCodec>
#include <QDebug>

StreamParser::StreamParser()
	: I(0)
	, length(0)
{ }

StreamParser::StreamParser(const QByteArray& data)
{
	streamData = data;
	I = 0;
	length = data.size();
}

int8_t StreamParser::parseByte(bool* ok)
{
	if (I >= length)
	{
		if (ok != nullptr)
			*ok = false;
		return -1;
	}

	if (ok != nullptr)
		*ok = true;

	return streamData[I++];
}


int16_t StreamParser::parseInt16(bool* ok)
{
	if (I + 1 >= length)
	{
		if (ok != nullptr)
			*ok = false;
		return -1;
	}

	if (ok != nullptr)
		*ok = true;

	int16_t value = streamData[I++];
	value |= streamData[I++] << 8;

	return swapInt16(value);
}


int32_t StreamParser::parseInt32(bool* ok)
{
	if (I + 3 >= length)
	{
		if (ok != nullptr)
			*ok = false;
		return -1;
	}

	if (ok != nullptr)
		*ok = true;

	int32_t value = streamData[I++];
	value |= streamData[I++] << 8;
	value |= streamData[I++] << 16;
	value |= streamData[I++] << 24;

	return swapInt32(value);
}


int64_t StreamParser::parseInt64(bool* ok)
{
	if (I + 7 >= length)
	{
		if (ok != nullptr)
			*ok = false;
		return -1;
	}

	if (ok != nullptr)
		*ok = true;

	int64_t value = streamData[I++];
	value |= streamData[I++] << 8;
	value |= streamData[I++] << 16;
	value |= streamData[I++] << 24;
	value |= streamData[I++] << 32;
	value |= streamData[I++] << 40;
	value |= streamData[I++] << 48;
	value |= streamData[I++] << 56;

	return swapInt64(value);
}


quint16 StreamParser::parseUInt16(bool* ok)
{
	if (I + 1 >= length)
	{
		if (ok != nullptr)
			*ok = false;
		return -1;
	}

	if (ok != nullptr)
		*ok = true;

	quint16 value = streamData[I++];
	value |= streamData[I++] << 8;

	return swapUInt16(value);
}


quint32 StreamParser::parseUInt32(bool* ok)
{
	if (I + 3 >= length)
	{
		if (ok != nullptr)
			*ok = false;
		return -1;
	}

	if (ok != nullptr)
		*ok = true;

	quint32 value = streamData[I++];
	value |= streamData[I++] << 8;
	value |= streamData[I++] << 16;
	value |= streamData[I++] << 24;

	return swapUInt32(value);
}


quint64 StreamParser::parseUInt64(bool* ok)
{
	if (I + 7 >= length)
	{
		if (ok != nullptr)
			*ok = false;
		return -1;
	}

	if (ok != nullptr)
		*ok = true;

	quint64 value = streamData[I++];
	value |= streamData[I++] << 8;
	value |= streamData[I++] << 16;
	value |= streamData[I++] << 24;
	value |= streamData[I++] << 32;
	value |= streamData[I++] << 40;
	value |= streamData[I++] << 48;
	value |= streamData[I++] << 56;

	return swapUInt64(value);
}


QString StreamParser::parseString(int length, bool* ok)
{
	if (I + length * 2 > this->length)
	{
		if (ok != nullptr)
			*ok = false;
		return QString();
	}

	if (ok != nullptr)
		*ok = true;

	QByteArray data = parseData(length, 2, ok);
	return QString::fromUtf16(reinterpret_cast<const unsigned short*>(data.constData()), length);
}


QByteArray StreamParser::parseData(int length, int size, bool* ok)
{
	if (I + length * size > this->length)
	{
		if (ok != nullptr)
			*ok = false;
		return QByteArray();
	}

	if (ok != nullptr)
		*ok = true;

	QByteArray data = streamData.mid(I, length * size);
	I += length * 2;

	return (size == 1 ? swapData(data, size) : data);
}

void StreamParser::shift(int shift)
{
	I += shift;
}

void StreamParser::reset()
{
	I = 0;
	length = 0;
	streamData.clear();
}

void StreamParser::load(const QByteArray& data)
{
	I = 0;
	length = data.length();
	streamData = data;
}

QByteArray StreamParser::swapData(const QByteArray& data, int size)
{
	QByteArray result;
	for (int i = 0; i < data.length(); i += size)
	{
		result.append(data.at(i + size - 1));
		result.append(data.at(i));
	}
	return result;
}
