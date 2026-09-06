#ifndef PROFILE_AVATAR_WIDGET_H
#define PROFILE_AVATAR_WIDGET_H

#include "themed_widget.h"
#include <QImage>

class ProfileAvatarWidget : public ThemedWidget
{
	Q_OBJECT

public:
	explicit ProfileAvatarWidget(QWidget *parent = nullptr);

	QImage image() const;
	void setImage(const QImage &newImage);

signals:
	void imageChanged();
	void clicked();

protected:
	virtual void paintEvent(QPaintEvent*) override;
	virtual void mousePressEvent(QMouseEvent*) override;
	virtual void enterEvent(QEvent*) override;
	virtual void leaveEvent(QEvent*) override;
	void onThemeChanged(const ThemeData& theme) override;

private:
	QImage m_image;
	Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged FINAL)

	bool m_isHovered = false;
};

#endif // PROFILE_AVATAR_WIDGET_H
