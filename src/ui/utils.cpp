#include "utils.h"
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

auto createRoundedLogo(const QPixmap &pixmap, int size) -> QPixmap
{
    if (pixmap.isNull())
    {
        qWarning() << "createRoundedLogo: Input pixmap is null.";
        return QPixmap();
    }

    QPixmap scaled = pixmap.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap rounded(size, size);
    rounded.fill(Qt::transparent);

    QPainter painter(&rounded);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, scaled);

    return rounded;
}
