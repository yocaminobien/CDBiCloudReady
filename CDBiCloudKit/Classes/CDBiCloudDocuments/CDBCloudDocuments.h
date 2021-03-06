

#import <Foundation/Foundation.h>
#import <CDBKit/CDBKitCore.h>


#import "CDBDocument.h"

typedef void (^CDBiCloudDocumentCompletion) (CDBDocument * _Nullable document, NSError * _Nullable error);


@protocol CDBCloudDocumentsDelegate;


@interface CDBCloudDocuments : NSObject
<
CDBDocumentDelegate
>

/**
 @brief: Use this option to enable verbose logging
**/

@property (assign, nonatomic) BOOL verbose;
@property (assign, nonatomic, readonly) BOOL ubiquitosActive;
@property (assign, nonatomic) BOOL appGroupsActive;
@property (assign, nonatomic) BOOL metadataQueryShouldStopAfterFinishGathering;

/**
 This is the URL where documents container points (where files stored)
 ubiquitosActive => ubiquityDocumentsURL
    appGroupsActive => appGroupDocumentsURL
        appGroupsActive == NO => localDocumentsURL
 App makes attempt to create this path if it doesn't exist
 **/

@property (copy, nonatomic, readonly, nullable) NSURL * currentDocumentsURL;

/**
 This is the URL where cloud documents container points (where cloud files stored)
 By default it points to cloud Documents directory
 App makes attempt to create this path if it doesn't exist
 **/

@property (copy, nonatomic, readonly, nullable) NSURL * ubiquityDocumentsURL;

/**
 This is the URL where app groups documents container points (where local files stored)
 By default it points to app groups container directory
 App makes attempt to create this path if it doesn't exist
 You could change it at any time you want
 **/

@property (strong, nonatomic, readonly, nullable) NSURL * appGroupDocumentsURL;

/**
 This is the URL where local documents container points (where local files stored)
 By default it points to local Documents directory
 App makes attempt to create this path if it doesn't exist
 You could change it at any time you want
**/

@property (copy, nonatomic, nullable) NSURL * localDocumentsURL;



/**
 Contains all documents that present in container
**/

@property (strong, nonatomic, readonly, nullable) NSArray<NSURL *> * cloudDocumentURLs;

/**
 This is for cloudConnection only
**/
- (void) initiateUsingContainerPathComponent: (NSString * _Nullable) pathComponent
                          appGroupIdentifier: (NSString * _Nullable) appGroupID;

/**
 This is for cloudConnection only
 **/

- (void) updateForUbiquityActive: (BOOL) active
       usingUbiquityContainerURL: (NSURL * _Nullable) ubiquityContainerURL;

/**
 return all URLs in specified directory
 **/
- (NSArray * _Nullable)URLsForItemsInsideUbiquitosDirectory:(NSURL * _Nonnull)directory;

/**
 return full document alias for storage
 @example:
 path: /storage/images/document.txt
 alias: images/document.txt
**/

- (NSString * _Nullable)documentAliasUsingItURL:(NSURL *_Nonnull)URL;

/**
 return document name
 @example:
 path: /storage/images/document.txt
 name: document.txt
 **/

- (NSString * _Nullable)documentNameUsingItURL:(NSURL * _Nonnull)URL;

/**
 Add delegate to notify changes
 **/

- (void)addDelegate:(id<CDBCloudDocumentsDelegate> _Nonnull)delegate;

/**
 Remove delegate
 **/

- (void)removeDelegate:(id<CDBCloudDocumentsDelegate> _Nonnull)delegate;


/**
 To move document to the cloud, use YES, otherwise NO
**/

- (void)makeDocument:(CDBDocument * _Nonnull)document
          ubiquitous:(BOOL)ubiquitous
          completion:(CDBErrorCompletion _Nonnull)completion;
/** 
 @brief: method using currentDocumentsURL like base URL to generate 
         relative path components and ubiquityDocumentsURL to apply this components
         to mirror the file position in source
**/

- (NSURL * _Nullable)ubiquityDocumentFileURLUsingLocalURL:(NSURL * _Nonnull)localURL;

/**
 @brief: method using ubiquityDocumentsURL like base URL to generate
         relative path components and currentDocumentsURL to apply this components
         to mirror the file position in source
 **/
- (NSURL * _Nullable)localDocumentFileURLUsingUbiquityURL:(NSURL * _Nonnull)ubiquitousURL;

/**
 Get document from local documents using file name
 Return nil if no file with such name exists or if it is a directory
 Local documents directory is defined by localDocumentsURL property
**/

- (CDBDocument * _Nullable)localDocumentWithFileName:(NSString * _Nonnull)fileName
                                               error:(NSError *_Nullable __autoreleasing * _Nullable)error;

/**
 Get document from iCloud documents using file name
 Return nil if no file with such name exists or if it is a directory
 iCloud documents directory is defined by documentsDirectoryPath variable passed on initalization
 **/

- (CDBDocument * _Nullable)ubiquitousDocumentWithFileName:(NSString * _Nonnull)fileName
                                                    error:(NSError *_Nullable __autoreleasing * _Nullable)error;

/**
 Create document in localDocumentsURL directory using file name and content
 Be aware - by defaullt it owerrides any existing document with such name
 If you aren't shure use localDocumentWithFileName: to check if any exist
**/

- (void)createClosedLocalDocumentUsingFileName:(NSString * _Nonnull)fileName
                               documentContent:(NSData * _Nullable)content
                                    completion:(CDBiCloudDocumentCompletion _Nonnull)completion;

- (void)createClosedLocalDocumentAtURL:(NSURL * _Nonnull)URL
                           withContent:(NSData * _Nullable)content
                            completion:(CDBiCloudDocumentCompletion _Nonnull)completion;

- (void)readContentOfDocumentAtURL:(NSURL * _Nonnull)URL
                        completion:(nonnull void(^)(NSData * _Nullable data, NSError * _Nullable error))completion;
/**
 Process block called only once for every single revision of the document with unique name
 if file modified date doesn't change processing block doesn't call
**/
- (void)processDocumentWithName:(NSString * _Nonnull)name
                          atURL:(NSURL * _Nonnull)URL
                processingBlock:(nonnull void(^) (NSData * _Nullable documentData, NSError * _Nullable error))processingBlock;

- (void)processStringsDocumentWithName:(NSString * _Nonnull)name
                                 atURL:(NSURL * _Nonnull)URL
               separationCharactersSet:(NSCharacterSet * _Nonnull)separators
                     onlyUniqueStrings:(BOOL)unique
                processingStringsBlock:(nonnull void(^) (NSArray * _Nullable documentStrings, NSError * _Nullable error))stringsProcessingBlock;

/**
 Delete document in localDocumentsURL directory (for local documents)
 Could delete both local and ubiquitous iCloud documents;
 Behaviour based on deleting a file and setting empty fileURL for a document
 It resolves issue with deleted document wich stored somewhere in a property
**/

- (void)deleteDocument:(CDBDocument * _Nonnull)document
            completion:(CDBErrorCompletion _Nonnull)completion;

- (void)copyDocument:(CDBDocument * _Nonnull)document
               toURL:(NSURL * _Nonnull)destinationURL
             replace:(BOOL)replace
          completion:(CDBErrorCompletion _Nonnull)completion;

- (void)moveDocument:(CDBDocument * _Nonnull)document
               toURL:(NSURL * _Nonnull)destinationURL
          completion:(CDBErrorCompletion _Nonnull)completion;

+ (BOOL)ensureThatDirectoryPresentsAtURL: (NSURL * _Nonnull)URL
                        usingFileManager: (NSFileManager * _Nullable)fileManager
                                   error: (NSError * _Nullable * _Nullable)error;

@end


@protocol CDBCloudDocumentsDelegate <NSObject>

@optional

- (void)didChangeCloudStateOfCDBCloudDocuments:(CDBCloudDocuments * _Nonnull)documents;

- (void)CDBCloudDocuments:(CDBCloudDocuments * _Nonnull)documents
   didChangeMetadataQuery:(NSMetadataQuery * _Nonnull)query;

- (void)CDBCloudDocuments:(CDBCloudDocuments * _Nonnull)documents
    didAutoresolveConflictInCDBDocument:(CDBDocument * _Nonnull)document;

- (void)CDBCloudDocuments:(CDBCloudDocuments * _Nonnull)documents
    didChangeUbiquitosDocumentAtURL:(NSURL * _Nullable)URL;
    
- (void)CDBCloudDocuments:(CDBCloudDocuments * _Nonnull)documents
    didRemoveUbiquitosDocumentAtURL:(NSURL * _Nullable)URL;

@end
