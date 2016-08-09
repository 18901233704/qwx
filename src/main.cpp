// Copyright (C) 2014 - 2016 Leslie Zhai <xiang.zhai@i-soft.com.cn>

#include "globaldeclarations.h"
#include "kwxapp.h"

#include <KLocalizedString>
#include <KAboutData>

#include <QCommandLineParser>
#include <QDebug>

int main(int argc, char* argv[]) 
{                                                                        
    KwxApp app(argc, argv);

    KLocalizedString::setApplicationDomain(CODE_NAME.toStdString().c_str());

    KAboutData aboutData(CODE_NAME, i18n("WeChat KDE frontend"), PROJECT_VERSION, i18n("WeChat GUI frontend for KDE."), KAboutLicense::GPL, i18n("(c) 2014 - 2016, Leslie Zhai"));
    aboutData.addAuthor(i18n("Leslie Zhai"), i18n("Maintainer"), "xiang.zhai@i-soft.com.cn");
    aboutData.addAuthor(i18n("Leslie Zhai"), i18n("Developer"), "xiang.zhai@i-soft.com.cn");
    aboutData.addAuthor(i18n("Mingcong Bai"), i18n("Developer"), "jeffbai@aosc.xyz");
#if KF5PRISON_FOUND
    aboutData.addAuthor(i18n("Leslie Zhai"), i18n("KF5 Prison Maintainer"), "xiang.zhai@i-soft.com.cn");
#endif
    KAboutData::setApplicationData(aboutData);

    app.setApplicationName(CODE_NAME);
    app.setApplicationVersion(PROJECT_VERSION);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    KwxWin* window = new KwxWin;
    window->show();
    app.setMainWindow(window);

    return app.exec();
}
