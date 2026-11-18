#pragma once

#include <QWidget>

class EnergyWidget : public QWidget {
    Q_OBJECT

private:
    double currentEnergy_{ 5.0 };
    double maxEnergy_{ 10.0 };

public:
    explicit EnergyWidget(QWidget* parent = nullptr);
    ~EnergyWidget() override = default;

    void setEnergy(double current, double max);

    [[nodiscard]] QSize sizeHint() const override;
    [[nodiscard]] QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
};