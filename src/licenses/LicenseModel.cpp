#include "LicenseModel.h"

#include "LicenseCategory.h"
#include "LicenseItem.h"

#include <QDirIterator>

namespace turtle_browser::licenses {

  namespace {
    LicenseItem * getItem(const QModelIndex & index) {
      return static_cast<LicenseItem *>(index.internalPointer());
    }

    bool isValidRole(int role) {
      switch (role) {
        case static_cast<int>(LicenseRole::LicenseFileName):
        case static_cast<int>(LicenseRole::LicenseFilePath):
        case static_cast<int>(LicenseRole::LicenseCategories):
          return true;
        default:
          return false;
      }
    }

    QVariant asVariant(const LicenseCategory & category) {
      return { static_cast<int>(category) };
    }
  }

  LicenseModel::LicenseModel(QString platformRootPath, QString qtRootPath, QString webviewRootPath, QObject * parentObject)
    : QAbstractItemModel(parentObject),
      m_platformRootPath(std::move(platformRootPath)),
      m_qtRootPath(std::move(qtRootPath)),
      m_webviewRootPath(std::move(webviewRootPath)),
      m_dir(m_platformRootPath) {
    populate();
  }

  LicenseModel::~LicenseModel() = default;

  void LicenseModel::populate() {
    QList<QVariant> root_categories;
    m_rootItem = std::make_unique<LicenseItem>(QString("Licenses"), m_platformRootPath, root_categories);

    QHash<QString, LicenseItem *> map;
    map[m_rootItem->path()] = m_rootItem.get();

    QDirIterator it(m_dir, QDirIterator::Subdirectories);

    while (it.hasNext()) {
      QString path = it.next();

      bool inWebview = path.startsWith(m_webviewRootPath);
      bool inQt = path.startsWith(m_qtRootPath);

      QList<QVariant> categories;
      categories.append(asVariant(LicenseCategory::All));

      if (inWebview)
        categories.append(asVariant(LicenseCategory::WebView));
      else if (inQt)
        categories.append(asVariant(LicenseCategory::Toolkit));
      else
        categories.append(asVariant(LicenseCategory::Platform));

      const QFileInfo & fileInfo = it.fileInfo();
      LicenseItem * parentItem = map[fileInfo.dir().path()];

      auto * child = new LicenseItem(fileInfo.fileName(), fileInfo.absoluteFilePath(), categories, parentItem);

      if (fileInfo.isDir())
        map[child->path()] = child;
    }
  }

  QModelIndex LicenseModel::index(int row, int column, const QModelIndex & parentIndex) const {
    if (!hasIndex(row, column, parentIndex))
      return {};

    LicenseItem * parentItem = !parentIndex.isValid() ? m_rootItem.get() : getItem(parentIndex);

    LicenseItem * childItem = parentItem->child(row);
    if (childItem)
      return createIndex(row, column, childItem);
    return {};
  }

  QModelIndex LicenseModel::parent(const QModelIndex & childIndex) const {
    if (!childIndex.isValid())
      return {};

    auto * childItem = getItem(childIndex);
    LicenseItem * parentItem = childItem->parentItem();

    if (parentItem == m_rootItem.get())
      return {};

    return createIndex(parentItem->row(), 0, parentItem);
  }

  int LicenseModel::rowCount(const QModelIndex & parentIndex) const {
    if (parentIndex.column() > 0)
      return 0;

    LicenseItem * parentItem = !parentIndex.isValid() ? m_rootItem.get() : getItem(parentIndex);

    return parentItem->childCount();
  }

  int LicenseModel::columnCount(const QModelIndex & parentIndex) const {
    return 1;
  }

  QVariant LicenseModel::data(const QModelIndex & index, int role) const {
    if (!index.isValid())
      return {};

    if (!isValidRole(role))
      return {};

    auto * item = getItem(index);

    return item->data(static_cast<LicenseRole>(role));
  }

}
