#include <QCoreApplication>
#include <QObject>
#include <QTimer>
#include <QCommandLineParser>
#include <QSocketNotifier>

#include "serverhandler.h"

void startServer(QObject* parent, const QString& dirPath)
{
    ServerHandler *netHandler = new ServerHandler(parent, dirPath);
    netHandler->init();
    QObject::connect(netHandler, &ServerHandler::finished,
                     parent, &QCoreApplication::quit);

    QSocketNotifier *input = new QSocketNotifier(STDIN_FILENO,
                                                 QSocketNotifier::Read,
                                                 netHandler);
    QObject::connect(input, &QSocketNotifier::activated,
                     netHandler, &ServerHandler::checkCommand);
    input->setEnabled(true);
    QTimer::singleShot(0, netHandler, SLOT(start()));
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("QtFileServer");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    const QCommandLineOption helpOption = parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();
    const QCommandLineOption serverOption(QStringList() << "s" << "server",
                                          "Start app as server");
    parser.addOption(serverOption);
    parser.addPositionalArgument("source", "Source dir to share files");

    parser.process(a);
    if (parser.isSet(helpOption))
        parser.showHelp();
    if (parser.isSet(versionOption))
        parser.showVersion();

    if (!parser.isSet(serverOption)) {
        QTextStream(stdout) << "No options are specified\n";
        parser.showHelp(1);
    }
    const QStringList args = parser.positionalArguments();
    if (args.size() == 0) {
        QTextStream(stdout) << "No arguments are specified\n";
        parser.showHelp(1);
    }

    QString dirPath = args[0];
    if (!QDir(dirPath).exists())
        dirPath = QDir::currentPath();
    startServer(&a, dirPath);
    return a.exec();
}
