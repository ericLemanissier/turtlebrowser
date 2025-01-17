#ifndef TURTLEBROWSER_LICENSEITEM_H
#define TURTLEBROWSER_LICENSEITEM_H

#include "LicenseRole.h"

#include <QObject>

namespace turtle_browser::licenses {

  class LicenseItem : public QObject {

    Q_OBJECT

  public:
    LicenseItem(QString file_name, QString file_path, QList<QVariant> categories, LicenseItem * parentItem = nullptr);

    LicenseItem * child(int row);

    int childCount() const;

    QVariant data(LicenseRole role) const;

    int row() const;

    QString path() const;

    LicenseItem * parentItem() const;

  private:
    QString m_file_name;
    QString m_file_path;
    QList<QVariant> m_categories;
  };

}

#endif //TURTLEBROWSER_LICENSEITEM_H
