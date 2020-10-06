#pragma once

#include <QObject>
#include "cache/cache.h"
#include "directorymanager/directorymanager.h"
#include "scaler/scaler.h"
#include "loader/loader.h"
#include "utils/fileoperations.h"

enum FileListSource { // rename? wip
    SOURCE_DIRECTORY,
    SOURCE_LIST
};

class DirectoryModel : public QObject {
    Q_OBJECT
public:
    explicit DirectoryModel(QObject *parent = nullptr);
    ~DirectoryModel();

    Scaler *scaler;

    void load(QString filePath, bool asyncHint);
    void preload(QString filePath);

    int fileCount() const;
    int dirCount() const;
    int indexOfFile(QString filePath) const;
    int indexOfDir(QString filePath) const;
    QString fileNameAt(int index) const;
    bool containsFile(QString filePath) const;
    bool isEmpty() const;
    QString nextOf(QString filePath) const;
    QString prevOf(QString filePath) const;
    QString first() const;
    QString last() const;
    QDateTime lastModified(QString filePath) const;

    bool forceInsert(QString filePath);
    void copyTo(const QString destDirPath, const QFileInfo srcFile, FileOpResult &result);
    void moveTo(const QString destDirPath, const QFileInfo srcFile, FileOpResult &result);
    void renameFile(const QString &oldFilePath, const QString &newName, FileOpResult &result);
    void removeFile(const QString &filePath, bool trash, FileOpResult &result);

    void setDirectory(QString);

    void unload(int index);

    bool loaderBusy() const;

    std::shared_ptr<Image> getImageAt(int index);
    std::shared_ptr<Image> getImage(QString filePath);

    void updateImage(QString filePath, std::shared_ptr<Image> img);

    void setSortingMode(SortingMode mode);
    SortingMode sortingMode() const;

    QString directoryPath() const;
    void unload(QString filePath);
    bool isLoaded(int index) const;
    bool isLoaded(QString filePath) const;
    void reload(QString filePath);
    QString filePathAt(int index) const;
    void unloadExcept(QString filePath, bool keepNearby);
    const FSEntry &entryAt(int index) const;

    int totalCount() const;
    QString dirNameAt(int index) const;
    QString dirPathAt(int index) const;

    bool autoRefresh();

    bool saveFile(const QString &filePath);
    bool saveFile(const QString &filePath, const QString &destPath);

    bool containsDir(QString dirPath) const;
signals:
    void fileRemoved(QString filePath, int index);
    void fileRenamed(QString fromPath, int indexFrom, QString toPath, int indexTo);
    void fileAdded(QString filePath);
    void fileModified(QString filePath);
    void loaded(QString filePath);
    void loadFailed(const QString &path);
    void sortingChanged(SortingMode);
    void indexChanged(int oldIndex, int index);
    void imageReady(std::shared_ptr<Image> img, const QString&);
    void imageUpdated(QString filePath);

private:
    DirectoryManager dirManager;
    Loader loader;
    Cache cache;
    FileListSource fileListSource;

private slots:
    void onImageReady(std::shared_ptr<Image> img, const QString &path);
    void onSortingChanged();
    void onFileAdded(QString filePath);
    void onFileRemoved(QString filePath, int index);
    void onFileRenamed(QString fromPath, int indexFrom, QString toPath, int indexTo);
    void onFileModified(QString filePath);
};
