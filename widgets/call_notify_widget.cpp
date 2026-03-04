#include "call_notify_widget.h"
#include "../utils/image_utils.h"
#include <QToolButton>
#include <QVariant>
#include <QHBoxLayout>
#include <QPainter>



CallNotifyWidget::CallNotifyWidget(QWidget *parent)
	: QWidget{parent}
{
	acceptButton = new QToolButton(this);
	rejectButton = new QToolButton(this);

	acceptButton->setFixedSize(30, 30);
	rejectButton->setFixedSize(30, 30);

	acceptButton->setProperty("role", QVariant("call"));
	rejectButton->setProperty("role", QVariant("call"));

	acceptButton->setIcon(QIcon(":/r/resources/images/call.png"));
	rejectButton->setIcon(QIcon(":/r/resources/images/close.png"));
	acceptButton->setIconSize({ 24, 24 });
	rejectButton->setIconSize({ 24, 24 });

	acceptButton->setCursor(Qt::PointingHandCursor);
	rejectButton->setCursor(Qt::PointingHandCursor);

	setFixedSize(300, 80);
	setAttribute(Qt::WA_TranslucentBackground);

	acceptButton->setFixedSize(40, 40);
	rejectButton->setFixedSize(40, 40);

	acceptButton->hide();
	rejectButton->hide();

	connect(acceptButton, &QAbstractButton::clicked, this, [this]{
		setCallState(RESET);
	});

	connect(rejectButton, &QAbstractButton::clicked, this, [this]{
		setCallState(RESET);
	});
}

void CallNotifyWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QFont nameFont("Roboto", 12, QFont::Bold);
	QFont stateFont("Roboto", 10);

	QFontMetrics nameMetrics(nameFont);
	QFontMetrics stateMetrics(stateFont);

	QString stateString;
	switch (m_callState)
	{
		case INCOMING:
			stateString = tr("Incoming");
			break;
		case OUTGOING:
			stateString = tr("Outgoing");
			break;
		case RESET:
			stateString = tr("Reset");
			break;
	}

	//draw background
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(0, 0, 0, 100));
	painter.drawRoundedRect(0, 0, width(), height(), 10, 10);

	//draw image
	QImage image;
	if (m_callerImage.isNull())
		setCallerImage(ImageUtils::GetImageFromName(m_callerName));
	image = ImageUtils::CropImageToCircle(m_callerImage, 40);
	painter.drawImage(QRect(10, height() / 2 - 20, 40, 40), image);

	//draw name
	painter.setPen(QColor(40, 100, 250));
	painter.setFont(nameFont);
	painter.drawText(60, height() / 2 - 5, m_callerName);

	//draw state
	painter.setPen(Qt::white);
	painter.setFont(stateFont);
	painter.drawText(60, height() / 2 + stateMetrics.height(), stateString);
}

void CallNotifyWidget::resizeEvent(QResizeEvent *)
{
	acceptButton->move(width() - acceptButton->width() - rejectButton->width() - 20, height() / 2 - acceptButton->height() / 2);
	rejectButton->move(width() - rejectButton->width() - 10, height() / 2 - rejectButton->height() / 2);
}

CallNotifyWidget::CallState CallNotifyWidget::callState() const
{
	return m_callState;
}

void CallNotifyWidget::setCallState(CallState newCallState)
{
	if (m_callState == newCallState)
		return;
	m_callState = newCallState;

	acceptButton->setVisible(m_callState == INCOMING);
	rejectButton->setVisible(m_callState == INCOMING || m_callState == OUTGOING);

	emit callStateChanged();
	update();
}

QString CallNotifyWidget::callerName() const
{
	return m_callerName;
}

void CallNotifyWidget::setCallerName(const QString &newCallerName)
{
	if (m_callerName == newCallerName)
		return;
	m_callerName = newCallerName;
	emit callerNameChanged();
	update();
}

QImage CallNotifyWidget::callerImage() const
{
	return m_callerImage;
}

void CallNotifyWidget::setCallerImage(const QImage &newCallerImage)
{
	if (m_callerImage == newCallerImage)
		return;
	m_callerImage = newCallerImage;
	emit callerImageChanged();
	update();
}
