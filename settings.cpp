#include "settings.h"

#include <QCoreApplication>

Settings::Settings(QObject *parent)
        : QSettings(QSettings::IniFormat,
                    QSettings::UserScope,
                    QCoreApplication::instance()->organizationName(),
                    QCoreApplication::instance()->applicationName(),
                    parent)
{
}

void Settings::setValue(const QString &key,
                        const QVariant &value)
{
    QSettings::setValue(key, value);
}

QVariant Settings::value(const QString &key,
                         const QVariant &defaultValue) const
{
    return QSettings::value(key, defaultValue);
}

Q_DECLARE_METATYPE(Settings*)
