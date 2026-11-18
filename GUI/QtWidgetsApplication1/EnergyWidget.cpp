#include "EnergyWidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QFont>
#include <algorithm>
#include <cmath>

EnergyWidget::EnergyWidget(QWidget* parent)
    : QWidget(parent) {
    setMinimumHeight(32);
}

void EnergyWidget::setEnergy(double current, double max) {
    const double safeMax = std::max(1.0, max);
    const double safeCurrent = std::clamp(current, 0.0, safeMax);

    if (!qFuzzyCompare(currentEnergy_, safeCurrent) || !qFuzzyCompare(maxEnergy_, safeMax)) {
        currentEnergy_ = safeCurrent;
        maxEnergy_ = safeMax;
        update();
    }
}

QSize EnergyWidget::sizeHint() const {
    return QSize(320, 36);
}

QSize EnergyWidget::minimumSizeHint() const {
    return QSize(180, 28);
}

void EnergyWidget::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRectF barRect = rect().adjusted(2, 2, -2, -2);
    const double radius = 8.0;

   
    painter.setPen(QPen(QColor(45, 45, 55), 2.0));
    painter.setBrush(QColor(28, 30, 38));
    painter.drawRoundedRect(barRect, radius, radius);

   
    const double ratio = currentEnergy_ / maxEnergy_;
    const QRectF innerRect = barRect.adjusted(2, 2, -2, -2);

    if (ratio > 0.0) {
        painter.save();

        QPainterPath clipPath;
        clipPath.addRoundedRect(innerRect, radius - 2.0, radius - 2.0);
        painter.setClipPath(clipPath);

        QRectF fillRect = innerRect;
        fillRect.setWidth(innerRect.width() * ratio);

        QLinearGradient gradient(innerRect.topLeft(), innerRect.bottomLeft());
        gradient.setColorAt(0.0, QColor(186, 85, 211));
        gradient.setColorAt(1.0, QColor(123, 31, 162));

        painter.fillRect(fillRect, gradient);
        painter.restore();
    }

    
    const int totalSegments = static_cast<int>(std::round(maxEnergy_));
    if (totalSegments > 1) {
        painter.setPen(QPen(QColor(20, 20, 28, 160), 1.5));
        const double stepWidth = innerRect.width() / static_cast<double>(totalSegments);
        for (int i = 1; i < totalSegments; ++i) {
            const double x = innerRect.left() + i * stepWidth;
            painter.drawLine(QPointF(x, innerRect.top()), QPointF(x, innerRect.bottom()));
        }
    }

   
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(11);
    painter.setFont(font);

    const int wholeMax = static_cast<int>(std::round(maxEnergy_));
    const QString text = QString("%1 / %2")
        .arg(currentEnergy_, 0, 'f', 1)
        .arg(wholeMax);

    painter.setPen(QColor(0, 0, 0, 180));
    painter.drawText(barRect.translated(1, 1), Qt::AlignCenter, text);

    painter.setPen(Qt::white);
    painter.drawText(barRect, Qt::AlignCenter, text);
}