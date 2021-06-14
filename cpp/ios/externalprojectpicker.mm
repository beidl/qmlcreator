#include "externalprojectpicker.h"

#import <MobileCoreServices/MobileCoreServices.h>

#include <QDir>
#include <QGuiApplication>
#include <QWindow>
#include <QDebug>

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface DocumentPickerDelegate : NSObject<UINavigationControllerDelegate, UIDocumentPickerDelegate>
{
                                        ExternalProjectPicker *m_DocumentPicker;
}
@end

@implementation DocumentPickerDelegate

- (id) initWithObject:(ExternalProjectPicker *)documentPicker
{
    self = [super init];
    if (self) {
        m_DocumentPicker = documentPicker;
    }
    return self;
}

- (void)documentPicker:(UIDocumentPickerViewController *)controller
  didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls {
    qWarning() << "DERE:" << controller.documentPickerMode;
    if (controller.documentPickerMode == UIDocumentPickerModeOpen)
    {
        for (NSURL* obj in urls)
        {
            BOOL isAccess = [obj startAccessingSecurityScopedResource];
            if (!isAccess) {
                qWarning() << "Failed to access security-scoped resource";
                continue;
            }

            NSError *error = nil;
            NSData *bookmarkData = [obj bookmarkDataWithOptions:NSURLBookmarkCreationSuitableForBookmarkFile includingResourceValuesForKeys:nil relativeToURL:nil error:&error];
            if (error) {
                qWarning() << "Getting bookmark data failed:" << error;
                [obj stopAccessingSecurityScopedResource];
                continue;
            }

            QByteArray qBookmarkData = QByteArray::fromNSData(bookmarkData);
            Q_EMIT self->m_DocumentPicker->documentSelected(qBookmarkData);
            [obj stopAccessingSecurityScopedResource];
        }
    }
}

- (void)documentPickerWasCancelled:(UIDocumentPickerViewController *)controller
{
    qWarning() << "Project import cancelled";
}

@end

static DocumentPickerDelegate* pickerDelegate = nil;
static NSArray *utis = @[(NSString *)kUTTypeFolder];

ExternalProjectPicker::ExternalProjectPicker(QObject *parent) : QObject(parent)
{
    if (pickerDelegate)
        return;

    pickerDelegate = [[DocumentPickerDelegate alloc] initWithObject:this];
}

ExternalProjectPicker::~ExternalProjectPicker()
{
    if (pickerDelegate == nil)
        return;

    [pickerDelegate dealloc];
    pickerDelegate = nil;
}

void ExternalProjectPicker::startImport()
{
    UIView *view = reinterpret_cast<UIView*>(QGuiApplication::focusWindow()->winId());
    UIViewController *qtController = [[view window] rootViewController];

    UIDocumentPickerViewController *documentPicker = [[UIDocumentPickerViewController alloc]
            initWithDocumentTypes:utis inMode:UIDocumentPickerModeOpen];

    documentPicker.delegate = pickerDelegate;
    [qtController presentViewController:documentPicker animated:YES completion:nil];
}

// Open a file using the given sandbox data
QString ExternalProjectPicker::openFileOrPath(const QByteArray &encodedData)
{
    const QByteArray sandboxData = QByteArray::fromBase64(encodedData);
    NSData* bookmarkData = sandboxData.toNSData();
    NSError *error = 0;
    BOOL stale = false;

    NSURL* url = [NSURL URLByResolvingBookmarkData:bookmarkData options:0 relativeToURL:nil bookmarkDataIsStale:&stale error:&error];
    if (!error && url)
    {
        [url startAccessingSecurityScopedResource];
        return QUrl::fromNSURL(url).toLocalFile();
    }

    qWarning() << "Error occured opening file or path:" << error << sandboxData;
    return QString();
}

// Stop sandbox access
void ExternalProjectPicker::closeFile(QUrl url)
{
    NSURL* nsurl = url.toNSURL();
    if (nsurl)
    {
        [nsurl stopAccessingSecurityScopedResource];
        return;
    }

    qWarning() << "Fell through during closing of external project";
}

QString ExternalProjectPicker::getDirNameForBookmark(const QByteArray& encodedData)
{
    QString path = openFileOrPath(encodedData);
    if (path.isEmpty()) {
        qWarning() << "Failed to get name: path is empty";
        return QString();
    }

    QStringList parts = path.split(QDir::separator(), QString::SkipEmptyParts);
    if (parts.isEmpty()) {
        qWarning() << "Failed to get name: path must have been empty";
        closeFile(QUrl(path));
        return QString();
    }

    closeFile(QUrl(path));
    return parts.takeLast();
}
