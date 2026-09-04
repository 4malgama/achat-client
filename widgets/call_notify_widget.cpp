#include "call_notify_widget.h"
#include "../utils/image_utils.h"
#include <QToolButton>
#include <QVariant>
#include <QHBoxLayout>
#include <QPainter>



CallNotifyWidget::CallNotifyWidget(QWidget *parent)
	: ThemedWidget{parent}
{
	acceptButton = new QToolButton(this);
	rejectButton = new QToolButton(this);

	acceptButton->setFixedSize(30, 30);
	rejectButton->setFixedSize(30, 30);

	acceptButton->setProperty("callAction", QVariant("accept"));
	rejectButton->setProperty("callAction", QVariant("reject"));

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

	onThemeChanged(theme());
}

void CallNotifyWidget::paintEvent(QPaintEvent *)
{
	const ThemeData& t = theme();

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QFont nameFont = t.fonts.button;
	QFont stateFont = t.fonts.small;

	QFontMetrics nameMetrics(nameFont);

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

	QRectF backgroundRect = rect();
	backgroundRect.adjust(
		t.metrics.borderWidth / 2.0,
		t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0,
		-t.metrics.borderWidth / 2.0
	);
	painter.setPen(QPen(t.colors.borderStrong, t.metrics.borderWidth));
	painter.setBrush(t.colors.panelBackground);
	painter.drawRoundedRect(backgroundRect, t.radii.large, t.radii.large);

	//draw image
	const QImage sourceImage = m_callerImage.isNull()
		? ImageUtils::makeImageFromName(m_callerName)
		: m_callerImage;
	const QImage image = ImageUtils::makeCircularAvatar(sourceImage, 40);
	painter.drawImage(QRect(10, height() / 2 - 20, 40, 40), image);

	//draw name
	painter.setPen(t.colors.accentHover);
	painter.setFont(nameFont);
	const int controlsWidth = acceptButton->isVisible() ? 100 : 55;
	const QRect nameRect(60, 8, width() - 60 - controlsWidth, height() / 2);
	painter.drawText(
		nameRect,
		Qt::AlignLeft | Qt::AlignVCenter,
		nameMetrics.elidedText(m_callerName, Qt::ElideRight, nameRect.width())
	);

	//draw state
	painter.setPen(t.colors.textSecondary);
	painter.setFont(stateFont);
	painter.drawText(QRect(60, height() / 2, width() - 120, height() / 2 - 8), Qt::AlignLeft | Qt::AlignVCenter, stateString);
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

void CallNotifyWidget::onThemeChanged(const ThemeData &theme)
{
	setFont(theme.fonts.base);
	updateGeometry();
	update();
}
