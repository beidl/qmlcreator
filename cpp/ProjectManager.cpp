/****************************************************************************
**
** Copyright (C) 2013-2015 Oleg Yadrov
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
** http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
****************************************************************************/

#include "ProjectManager.h"

#include <QCoreApplication>
#include <QDebug>
#include <QQmlContext>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

ProjectManager::ProjectManager(QObject *parent, ExternalProjectPicker* importer) :
    QObject(parent)
{
    connect(importer, &ExternalProjectPicker::documentSelected, this, &ProjectManager::importProject);

    QDir().mkpath(baseFolderPath(Projects));
    QDir().mkpath(baseFolderPath(Examples));

    const QStandardPaths::StandardLocation location = QStandardPaths::ConfigLocation;
    const QString dbFilePath =
            QStandardPaths::writableLocation(location)
            + QStringLiteral("/%1/projects.db").arg(qApp->applicationName());
    const QString dbName = QStringLiteral("importedProjects");

    const QFileInfo dbFileInfo(dbFilePath);
    const QDir dbDir = dbFileInfo.absoluteDir();

    if (!(dbDir.exists() || dbDir.mkpath(dbDir.absolutePath()))) {
        qWarning() << "Failed to create necessary directory" << dbDir.absolutePath();
        return;
    }

    if (QSqlDatabase::contains(dbName))
        this->m_importedProjectDb = QSqlDatabase::database(dbName);
    else
        this->m_importedProjectDb = QSqlDatabase::addDatabase("QSQLITE", dbName);
    this->m_importedProjectDb.setDatabaseName(dbFilePath);

    this->createImportedProjectsDb();

    this->m_importer = new ExternalProjectPicker(nullptr);
}

ProjectManager::BaseFolder ProjectManager::baseFolder()
{
    return m_baseFolder;
}

void ProjectManager::setBaseFolder(BaseFolder baseFolder)
{
    if (m_baseFolder != baseFolder)
    {
        m_baseFolder = baseFolder;
        emit baseFolderChanged();
    }
}

QStringList ProjectManager::projects()
{
    QDir dir(baseFolderPath(m_baseFolder));
    QStringList projects;
    QFileInfoList folders = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);

    foreach(QFileInfo folder, folders) {
        QString folderName = folder.fileName();
        projects.push_back(folderName);
    }

    for (const QString& url : this->importedProjects()) {
        projects.push_back(url);
    }

    return projects;
}

void ProjectManager::createProject(QString projectName)
{
    QDir dir(baseFolderPath(Projects));
    if (dir.mkpath(projectName))
    {
        QFile file(baseFolderPath(Projects) + QDir::separator() + projectName + QDir::separator() + "main.qml");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString fileContent = "// Project \"" + projectName + "\"\n" + newFileContent("main");
            QTextStream textStream(&file);
            textStream<<fileContent;
        }
        else
        {
            qWarning() << "Unable to create file \"main.qml\"";
            emit error(QString("Unable to create file \"main.qml\""));
        }
    }
    else
    {
        qWarning() << "Failed to create folder" << dir.absolutePath();
        emit error(QString("Unable to create folder \"%1\".").arg(projectName));
    }
}

void ProjectManager::removeProject(QString projectName)
{
    QDir dir(baseFolderPath(m_baseFolder) + QDir::separator() + projectName);
    dir.removeRecursively();
}

bool ProjectManager::projectExists(QString projectName)
{
    QFileInfo checkFile(baseFolderPath(Projects) + QDir::separator() + projectName);
    return checkFile.exists();
}

void ProjectManager::restoreExamples()
{
    QDir deviceExamplesDir(baseFolderPath(Examples));
    deviceExamplesDir.removeRecursively();
    deviceExamplesDir.mkpath(baseFolderPath(Examples));

    QDir qrcExamplesDir(":/qml/examples");
    QFileInfoList folders = qrcExamplesDir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);

    foreach(QFileInfo folder, folders) {
        QString folderName = folder.fileName();
        deviceExamplesDir.mkpath(folderName);

        QDir qrcExampleDir(":/qml/examples/" + folderName);

        QFileInfoList files = qrcExampleDir.entryInfoList(QDir::Files);

        foreach(QFileInfo file, files) {
            QString fileName = file.fileName();
            QFile::copy(file.absoluteFilePath(), baseFolderPath(Examples) + QDir::separator() + folderName + QDir::separator() + fileName);

            QFile::setPermissions(baseFolderPath(Examples) + QDir::separator() + folderName + QDir::separator() + fileName,
                                  QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                                  QFileDevice::ReadUser  | QFileDevice::WriteUser  |
                                  QFileDevice::ReadGroup | QFileDevice::WriteGroup |
                                  QFileDevice::ReadOther | QFileDevice::WriteOther
                                  );
        }
    }
}

bool ProjectManager::importProject(QByteArray url)
{
    if (url.isEmpty())
        return false;

    QString encodedUrl = url.toBase64();

    if (!this->m_importedProjectDb.open()) {
        qWarning() << "Failed to open imported projects database:"
                       << this->m_importedProjectDb.lastError().text();
        return false;
    }

    const QString importQuery =
            QStringLiteral("INSERT INTO projects"
                           " (url) "
                           "values(:url);");

    QSqlQuery query(this->m_importedProjectDb);
    query.prepare(importQuery);
    query.bindValue(":url", encodedUrl);

    const bool querySuccess = query.exec();
    if (!querySuccess) {
        qWarning() << "Failed to insert project:" << query.lastError().text();
        return false;
    }

    qDebug() << url;
    qDebug() << "Imported project:" << query.executedQuery();
    emit projectListChanged();
    return true;
}

void ProjectManager::createImportedProjectsDb()
{
    if (!this->m_importedProjectDb.open()) {
        qWarning() << "Failed to open imported projects database:"
                       << this->m_importedProjectDb.lastError().text();
        return;
    }

    const QString version =
            QStringLiteral("CREATE table version (versionNumber INTEGER, "
                               "PRIMARY KEY(versionNumber));");
    const QString versionInsert =
            QStringLiteral("INSERT or REPLACE INTO version "
                               "values(1);");
    const QString projects =
            QStringLiteral("CREATE table projects ("
                               "url TEXT, PRIMARY KEY(url));");

    const QStringList existingTables = this->m_importedProjectDb.tables();
    qDebug() << "existing tables:" << existingTables;

    if (!existingTables.contains("version")) {
        const QSqlQuery versionCreateQuery = this->m_importedProjectDb.exec(version);
        if (versionCreateQuery.lastError().type() != QSqlError::NoError) {
            qWarning() << "Failed to create version table, error:"
                           << versionCreateQuery.lastError().text();
            return;
        }
        qDebug() << versionCreateQuery.executedQuery();

        const QSqlQuery versionInsertQuery = this->m_importedProjectDb.exec(versionInsert);
        if (versionInsertQuery.lastError().type() != QSqlError::NoError) {
            qWarning() << "Failed to create version table, error:"
                           << versionInsertQuery.lastError().text();
            return;
        }
        qDebug() << versionInsertQuery.executedQuery();
    }

    if (!existingTables.contains("projects")) {
        const QSqlQuery accountsCreateQuery = this->m_importedProjectDb.exec(projects);
        if (accountsCreateQuery.lastError().type() != QSqlError::NoError) {
            qWarning() << "Failed to create projects table, error:"
                           << accountsCreateQuery.lastError().text();
            return;
        }
        qDebug() << accountsCreateQuery.executedQuery();
    }
}

bool ProjectManager::removeImportedProject(QString url)
{
    const QString deleteString =
            QStringLiteral("DELETE from projects"
                           " WHERE url=:url");

    QSqlQuery deleteQuery(this->m_importedProjectDb);
    deleteQuery.prepare(deleteString);
    deleteQuery.bindValue(":url", url);

    const bool deleteSuccess = deleteQuery.exec();
    if (!deleteSuccess) {
        qWarning() << "Failed to delete project:"
                       << deleteQuery.lastError().text();
        return false;
    }

    Q_EMIT projectListChanged();

    qDebug() << deleteQuery.executedQuery();

    return true;
}

bool ProjectManager::importedProjectExists(QString url)
{
    if (url.isEmpty())
        return false;

    if (!this->m_importedProjectDb.open()) {
        qWarning() << "Failed to open imported projects database:"
                       << this->m_importedProjectDb.lastError().text();
        return false;
    }

    const QString existanceCheck =
            QStringLiteral("SELECT url from projects"
                           " WHERE url=:url");

    QSqlQuery existanceQuery(this->m_importedProjectDb);
    existanceQuery.prepare(existanceCheck);
    existanceQuery.bindValue(":url", QVariant(url));

    const bool existanceSuccess = existanceQuery.exec();
    if (!existanceSuccess) {
        qWarning() << "Failed to query for existing project:"
                       << existanceQuery.lastError().text();
        return false;
    }

    return existanceQuery.next();
}

QStringList ProjectManager::importedProjects()
{
    QStringList ret;

    if (!this->m_importedProjectDb.open()) {
        qWarning() << "Failed to open imported projects database:"
                       << this->m_importedProjectDb.lastError().text();
        return QStringList();
    }

    const QString existanceCheck =
            QStringLiteral("SELECT url from projects;");

    QSqlQuery existanceQuery(this->m_importedProjectDb);
    existanceQuery.prepare(existanceCheck);

    const bool existanceSuccess = existanceQuery.exec();
    if (!existanceSuccess) {
        qWarning() << "Failed to query for existing project:"
                       << existanceQuery.lastError().text();
        return QStringList();
    }

    qDebug() << "Number of imported projects:" << existanceQuery.record().count();
    qDebug() << existanceQuery.record();
    while(existanceQuery.next()) {
        ret.push_back(existanceQuery.value(0).toByteArray());
    }

    return ret;
}

QString ProjectManager::projectName()
{
    return m_projectName;
}

void ProjectManager::setProjectName(QString projectName)
{
    if (m_projectName != projectName)
    {
        m_projectName = projectName;
        emit projectNameChanged();
    }
}

QString ProjectManager::subDir()
{
    return m_subdir;
}

void ProjectManager::setSubDir(QString dir)
{
    if (m_subdir != dir)
    {
        m_subdir = dir;
        emit subDirChanged();
    }
}

QVariantList ProjectManager::files()
{
    QDir dir;
    if (this->isImported())
    {
        dir = QDir(m_projectName + QDir::separator() + m_subdir);
    } else {
        dir = QDir(baseFolderPath(m_baseFolder) + QDir::separator() + m_projectName + QDir::separator() + m_subdir);
    }

    qDebug() << Q_FUNC_INFO << dir;

    QVariantList projectFiles;
    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    foreach(QFileInfo file, files) {
        qDebug() << file.absolutePath();
        if (!file.isDir() && (file.suffix() == "qml" || file.suffix() == "js"))
        {
            QVariantMap fileEntry;
            QString fileName = file.fileName();
            fileEntry.insert("name", fileName);
            fileEntry.insert("isDir", false);
            projectFiles.push_back(fileEntry);
        } else if (file.isDir()) {
            QVariantMap dirEntry;
            QString fileName = file.fileName();
            dirEntry.insert("name", fileName);
            dirEntry.insert("isDir", true);
            projectFiles.push_back(dirEntry);
        }
    }

    return projectFiles;
}

void ProjectManager::createFile(QString fileName, QString fileExtension)
{
    QString pathPrefix;
    if (!isImported())
        pathPrefix = baseFolderPath(m_baseFolder) + QDir::separator();

    QFile file(pathPrefix + m_projectName +
               QDir::separator() + m_subdir +
               QDir::separator() + fileName + "." + fileExtension);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);
        textStream<<newFileContent(fileExtension);
    }
    else
    {
        emit error(QString("Unable to create file \"%1.%2\"").arg(fileName, fileExtension));
    }
}

void ProjectManager::removeFile(QString fileName)
{
    QString pathPrefix;
    if (!isImported())
        pathPrefix = baseFolderPath(m_baseFolder) + QDir::separator();

    const QString filePath =
            pathPrefix + m_projectName +
            QDir::separator() + m_subdir +
            QDir::separator() + fileName;
    qDebug() << "Removing" << filePath;

    const bool isDir = QFileInfo(filePath).isDir();
    if (isDir) {
        QDir(filePath).removeRecursively();
    } else {
        QDir().remove(filePath);
    }
}

void ProjectManager::createDir(QString dirName)
{
    QString pathPrefix;
    if (!isImported())
        pathPrefix = baseFolderPath(m_baseFolder) + QDir::separator();

    const QString fullPath =
            pathPrefix + m_projectName +
            QDir::separator() + m_subdir +
            QDir::separator() + dirName;
    qDebug() << "Creating dir" << fullPath;
    QDir().mkpath(fullPath);
}

bool ProjectManager::fileExists(QString fileName)
{
    QString pathPrefix;
    if (!isImported())
        pathPrefix = baseFolderPath(m_baseFolder) + QDir::separator();

    QFileInfo checkFile(pathPrefix + m_projectName +
                        QDir::separator() + m_subdir +
                        QDir::separator() + fileName);
    return checkFile.exists();
}

QString ProjectManager::fileName()
{
    return m_fileName;
}

QString ProjectManager::fileFormat()
{
    return m_fileFormat;
}

void ProjectManager::setFileName(QString fileName)
{
    if (m_fileName != fileName)
    {
        QString pathPrefix;
        if (!isImported())
            pathPrefix = baseFolderPath(m_baseFolder) + QDir::separator();

        QFileInfo fileInfo(pathPrefix + m_projectName +
                           QDir::separator() + m_subdir +
                           QDir::separator() + fileName);

        m_fileName = fileName;
        m_fileFormat = fileInfo.suffix();

        emit fileNameChanged();
        emit fileFormatChanged();
    }
}

QString ProjectManager::getFilePath()
{
    QString pathPrefix;
    if (!isImported())
        pathPrefix = baseFolderPath(m_baseFolder) + QDir::separator();

    return "file:///" + pathPrefix + m_projectName +
            QDir::separator() + m_subdir +
            QDir::separator() + m_fileName;
}

QString ProjectManager::getFileContent()
{
    QString pathPrefix;
    if (!isImported())
        pathPrefix = baseFolderPath(m_baseFolder) + QDir::separator();

    QFile file(pathPrefix + m_projectName +
               QDir::separator() + m_subdir +
               QDir::separator() + m_fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream textStream(&file);
    QString fileContent = textStream.readAll().trimmed();
    return fileContent;
}

void ProjectManager::saveFileContent(QString content)
{
    QString pathPrefix;
    if (!isImported())
        pathPrefix = baseFolderPath(m_baseFolder) + QDir::separator();

    QFile file(pathPrefix + m_projectName +
               QDir::separator() + m_subdir +
               QDir::separator() + m_fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream textStream(&file);
    textStream<<content;
}

QQmlApplicationEngine *ProjectManager::m_qmlEngine = Q_NULLPTR;

void ProjectManager::setQmlEngine(QQmlApplicationEngine *engine)
{
    ProjectManager::m_qmlEngine = engine;
}

void ProjectManager::clearComponentCache()
{
    ProjectManager::m_qmlEngine->clearComponentCache();
}

bool ProjectManager::isImported()
{
    return this->m_isImported;
}

void ProjectManager::setIsImported(bool value)
{
    if (this->m_isImported == value)
        return;

    this->m_isImported = value;
    emit isImportedChanged();
}

QObject *ProjectManager::projectManagerProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    ProjectManager *projectManager = new ProjectManager(nullptr, qvariant_cast<ExternalProjectPicker*>(engine->rootContext()->contextProperty("externalPicker")));
    return projectManager;
}

QString ProjectManager::baseFolderPath(BaseFolder folder)
{
    QString folderName;

    switch (folder)
    {
    case Projects:
        folderName = "Projects";
        break;
    case Examples:
        folderName = "Examples";
        break;
    }

#ifndef UBUNTU_CLICK
    QString folderPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
#else
    QString folderPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
#endif
        QDir::separator() + "QML Projects";

    if (!folderName.isEmpty())
    {
        folderPath += QDir::separator() + folderName;
    }

    return folderPath;
}

QString ProjectManager::newFileContent(QString fileType)
{
    QString fileName;

    if (fileType == "main")
        fileName = "MainFile.qml";
    else if (fileType == "qml")
        fileName = "QmlFile.qml";
    else if (fileType == "js")
        fileName = "JsFile.js";

    QString fileContent;

    if (!fileName.isEmpty())
    {
        QFile file(":/resources/templates/" + fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream textStream(&file);
        fileContent = textStream.readAll().trimmed();
    }

    return fileContent;
}
