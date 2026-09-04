#ifndef RIPPLEBUTTONWIDGET_H
#define RIPPLEBUTTONWIDGET_H

#include <QPushButton>
#include <QColor>

class RippleButtonWidget : public QPushButton
{
	Q_OBJECT

	QRect m_rippleRect;
	int m_opacity = 0;
	QColor m_rippleColor;
	int m_cornerRadius = 0;

public:
	explicit RippleButtonWidget(QWidget* parent = nullptr);

	void setRippleRect(const QRect& rect);
	void setOpacity(int opacity);

	QRect rippleRect() const;
	int opacity() const;

	Q_PROPERTY(QRect rippleRect READ rippleRect WRITE setRippleRect)
	Q_PROPERTY(int opacity READ opacity WRITE setOpacity)

protected:
	void paintEvent(QPaintEvent* event);

private slots:
	void startRippleAnimation();
};

#endif // RIPPLEBUTTONWIDGET_H
