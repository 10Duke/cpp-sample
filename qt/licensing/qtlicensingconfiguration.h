#ifndef QTLICENSINGCONFIGURATION_H
#define QTLICENSINGCONFIGURATION_H

#include <QString>
#include <QUrl>

namespace tenduke { namespace qt { namespace licensing {


/** QT-specific licensing configuration.
 *
 */
class QtLicensingConfiguration
{
public:
    QtLicensingConfiguration(
            const QUrl endpoint,
            const QString hardwareId
    ) : endpoint(endpoint), hardwareId(hardwareId)
    {}

    const QUrl getEndpoint() const {return endpoint;}
    const QString getHardwareId() const {return hardwareId;}

private:
    const QUrl endpoint;
    const QString hardwareId;
};


}}}

#endif // QTLICENSINGCONFIGURATION_H
