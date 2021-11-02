#include "ApplicationState.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#if QT_VERSION >= 0x060000
#include <QtWebEngineQuick/qtwebenginequickglobal.h>
#else
#include <qtwebengineglobal.h>
#endif

int main(int argc, char * argv[]) {
#if QT_VERSION >= 0x060000
  QCoreApplication::setOrganizationName("TurtleBrowser");
#else
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

#if QT_VERSION >= 0x060000
  QtWebEngineQuick::initialize();
  QGuiApplication app(argc, argv);
#else
  QApplication app(argc, argv);

  QtWebEngine::initialize();
#endif

  turtle_browser::ApplicationState applicationState;

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("licenseModelWebView", applicationState.licenseState().searchModelWebLicenses());
  engine.rootContext()->setContextProperty("licenseModelToolkit", applicationState.licenseState().searchModelToolkitLicenses());
  engine.rootContext()->setContextProperty("licenseModelPlatform", applicationState.licenseState().searchModelPlatformLicenses());
  engine.rootContext()->setContextProperty("licenseModelAll", applicationState.licenseState().searchModelAllLicenses());
  engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
  QMetaObject::invokeMethod(engine.rootObjects().first(), "start");

  return app.exec();
}
