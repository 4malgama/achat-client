#include "stream_builder.h"


StreamBuilder::StreamBuilder()
    : streamData{}
{ }

StreamBuilder::StreamBuilder(const QByteArray& streamData)
    : streamData{streamData}
{ }

void StreamBuilder::addBool(bool value)
{
    addInt8(value ? 1 : 0);
}

void StreamBuilder::addInt8(int8_t value)
{
    streamData.append((char*)&value, sizeof(int8_t));
}

void StreamBuilder::addInt16(int16_t value)
{
    int16_t swap_value = swapInt16(value);
    streamData.append((char*)&swap_value, sizeof(int16_t));
}

void StreamBuilder::addInt32(int32_t value)
{
    int32_t swap_value = swapInt32(value);
    streamData.append((char*)&swap_value, sizeof(int32_t));
}

void StreamBuilder::addInt64(int64_t value)
{
    int64_t swap_value = swapInt64(value);
    streamData.append((char*)&swap_value, sizeof(int64_t));
}

void StreamBuilder::addUInt8(quint8 value)
{
    streamData.append((char*)&value, sizeof(quint8));
}

void StreamBuilder::addUInt16(quint16 value)
{
    quint16 swap_value = swapUInt16(value);
    streamData.append((char*)&swap_value, sizeof(quint16));
}

void StreamBuilder::addUInt32(quint32 value)
{
    quint32 swap_value = swapUInt32(value);
    streamData.append((char*)&swap_value, sizeof(quint32));
}

void StreamBuilder::addUInt64(quint64 value)
{
    quint64 swap_value = swapUInt64(value);
    streamData.append((char*)&swap_value, sizeof(quint64));
}

void StreamBuilder::addString(const QString& str)
{
    streamData.append(swapString(str));
}

void StreamBuilder::addData(const QByteArray& data)
{
    streamData.append(swapData(data));
}

void StreamBuilder::addBoolArray(const std::vector<bool>& array)
{
    for (bool value : array)
        addBool(value);
}

void StreamBuilder::addInt8Array(const QVector<int8_t>& array)
{
    for (int8_t value : array)
        addInt8(value);
}

void StreamBuilder::addInt16Array(const QVector<int16_t>& array)
{
    for (int16_t value : array)
        addInt16(value);
}

void StreamBuilder::addInt32Array(const QVector<int32_t>& array)
{
    for (int32_t value : array)
        addInt32(value);
}

void StreamBuilder::addInt64Array(const QVector<int64_t>& array)
{
    for (int64_t value : array)
        addInt64(value);
}

void StreamBuilder::addUInt8Array(const QVector<quint8>& array)
{
    for (quint8 value : array)
        addUInt8(value);
}

void StreamBuilder::addUInt16Array(const QVector<quint16>& array)
{
    for (quint16 value : array)
        addUInt16(value);
}

void StreamBuilder::addUInt32Array(const QVector<quint32>& array)
{
    for (quint32 value : array)
        addUInt32(value);
}

void StreamBuilder::addUInt64Array(const QVector<quint64>& array)
{
    for (quint64 value : array)
        addUInt64(value);
}

void StreamBuilder::addStringArray(const QStringList& array)
{
    for (QString value : array)
        addString(value);
}

void StreamBuilder::addDataArray(const QVector<QByteArray>& array)
{
    for (QByteArray value : array)
        addData(value);
}

void StreamBuilder::reset()
{
    streamData.clear();
}

void StreamBuilder::setData(const QByteArray& data)
{
    streamData = data;
}

QByteArray StreamBuilder::build()
{
    return streamData;
}

QByteArray StreamBuilder::swapString(const QString& str)
{
    int length = str.length();
	const ushort* data = str.utf16();
	
	char16_t* swap_buffer = new char16_t[length];
	for (int i = 0; i < length; i++)
		swap_buffer[i] = swapUInt16(data[i]);

	QByteArray result((char*)swap_buffer, length * sizeof(char16_t));

	delete[] swap_buffer;
	return result;
}

QByteArray StreamBuilder::swapData(const QByteArray& data)
{
    return swapString(QString(data));
}