#include "ripple_button_widget.h"
#include "../../theme_manager/theme_binding.h"

#include <QPropertyAnimation>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>

RippleButtonWidget::RippleButtonWidget(QWidget *parent)
    : QPushButton{parent}
{
    theme::bind(this, [this] (const ThemeData& theme) {
        setFont(theme.fonts.button);
        m_rippleColor = theme.colors.textOnAccent;
        m_cornerRadius = theme.radii.medium;
    });
    connect(this, &QPushButton::pressed, this, &RippleButtonWidget::startRippleAnimation);
}

void RippleButtonWidget::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath clipPath;
    clipPath.addRoundedRect(rect(), m_cornerRadius, m_cornerRadius);
    painter.setClipPath(clipPath);
    painter.setPen(Qt::NoPen);
    QColor rippleColor = m_rippleColor;
    rippleColor.setAlpha(m_opacity);
    painter.setBrush(rippleColor);
    painter.drawEllipse(m_rippleRect.intersected(rect()));
}

void RippleButtonWidget::startRippleAnimation()
{
    m_opacity = 128;

    QPoint clickPoint = mapFromGlobal(QCursor::pos());
    if (clickPoint.x() == 0 && clickPoint.y() == 0)
        clickPoint = rect().center();

    int radius = qMin(width(), height()) / 2;

    m_rippleRect.setSize(QSize(0, 0));
    m_rippleRect.moveCenter(clickPoint);
    update();

    QPropertyAnimation *animation = new QPropertyAnimation(this, "rippleRect");
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setDuration(500);
    animation->setStartValue(m_rippleRect);
    animation->setEndValue(QRect(clickPoint.x() - radius * 2, clickPoint.y() - radius * 2, radius * 4, radius * 4));
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    connect(animation, &QPropertyAnimation::valueChanged, [=](const QVariant &value) {
        m_rippleRect = value.toRect();
        m_opacity = 128 - (128 * animation->currentTime() / animation->duration());
        update();
    });

    QTimer::singleShot(animation->duration(), [=]() {
        m_opacity = 0;
        update();
    });
}

void RippleButtonWidget::setRippleRect(const QRect &rect)
{
    m_rippleRect = rect;
    update();
}

QRect RippleButtonWidget::rippleRect() const
{
    return m_rippleRect;
}

int RippleButtonWidget::opacity() const
{
    return m_opacity;
}

void RippleButtonWidget::setOpacity(int opacity)
{
    m_opacity = opacity;
    update();
}
