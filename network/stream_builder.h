#ifndef STREAMBUILDER_H
#define STREAMBUILDER_H

#include <QByteArray>
#include <vector>
#include <QVector>
#include <QList>

class StreamBuilder
{
	QByteArray streamData;

public:
	StreamBuilder();
	StreamBuilder(const StreamBuilder&) = delete;
	StreamBuilder(StreamBuilder&&) noexcept = delete;
	StreamBuilder(const QByteArray& streamData);

	void addBool(bool value);
	void addInt8(int8_t value);
	void addInt16(int16_t value);
	void addInt32(int32_t value);
	void addInt64(int64_t value);
	void addUInt8(quint8 value);
	void addUInt16(quint16 value);
	void addUInt32(quint32 value);
	void addUInt64(quint64 value);
	void addString(const QString& str);
	void addData(const QByteArray& data);

	void addBoolArray(const std::vector<bool>& array);
	void addInt8Array(const QVector<int8_t>& array);
	void addInt16Array(const QVector<int16_t>& array);
	void addInt32Array(const QVector<int32_t>& array);
	void addInt64Array(const QVector<int64_t>& array);
	void addUInt8Array(const QVector<quint8>& array);
	void addUInt16Array(const QVector<quint16>& array);
	void addUInt32Array(const QVector<quint32>& array);
	void addUInt64Array(const QVector<quint64>& array);
	void addStringArray(const QStringList& array);
	void addDataArray(const QVector<QByteArray>& array);

	void reset();
	void setData(const QByteArray& data);
	QByteArray build();

private:
	inline int16_t swapInt16(int16_t value) { return (value << 8) | (value >> 8); }
	inline int32_t swapInt32(int32_t value) { return (value << 24) | ((value << 8) & 0xFF0000) | ((value >> 8) & 0xFF00) | (value >> 24); };
	inline int64_t swapInt64(int64_t value) { return (value << 56) | ((value << 40) & 0xFF000000) | ((value << 24) & 0xFF0000) | ((value << 8) & 0xFF00) | (value >> 56); };
	inline quint16 swapUInt16(quint16 value) { return (value << 8) | (value >> 8); }
	inline quint32 swapUInt32(quint32 value) { return (value << 24) | ((value << 8) & 0xFF0000) | ((value >> 8) & 0xFF00) | (value >> 24); }
	inline quint64 swapUInt64(quint64 value) { return (value << 56) | ((value << 40) & 0xFF000000) | ((value << 24) & 0xFF0000) | ((value << 8) & 0xFF00) | (value >> 56); }
	QByteArray swapString(const QString& str);
	QByteArray swapData(const QByteArray& data);
};

#endif // STREAMBUILDER_H
