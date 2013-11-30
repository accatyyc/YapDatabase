#import <Foundation/Foundation.h>

#import "YapAbstractDatabasePrivate.h"

#import "YapDatabase.h"
#import "YapDatabaseConnection.h"
#import "YapDatabaseTransaction.h"

#import "sqlite3.h"


@interface YapDatabase () {
@public
	
	YapDatabaseSerializer objectSerializer;       // Read-only by transactions
	YapDatabaseDeserializer objectDeserializer;   // Read-only by transactions
	
	YapDatabaseSerializer metadataSerializer;     // Read-only by transactions
	YapDatabaseDeserializer metadataDeserializer; // Read-only by transactions
	
	YapDatabaseSanitizer objectSanitizer;         // Read-only by transactions
	YapDatabaseSanitizer metadataSanitizer;       // Read-only by transactions
}

@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

@interface YapDatabaseConnection () {
@public
	
	__strong YapDatabase *database;
	
	NSMutableDictionary *objectChanges;
	NSMutableDictionary *metadataChanges;
	NSMutableSet *removedKeys;
	NSMutableSet *removedCollections;
	BOOL allKeysRemoved;
}

- (sqlite3_stmt *)getCollectionCountStatement;
- (sqlite3_stmt *)getKeyCountForCollectionStatement;
- (sqlite3_stmt *)getKeyCountForAllStatement;
- (sqlite3_stmt *)getCountForRowidStatement;
- (sqlite3_stmt *)getRowidForKeyStatement;
- (sqlite3_stmt *)getKeyForRowidStatement;
- (sqlite3_stmt *)getKeyDataForRowidStatement;
- (sqlite3_stmt *)getKeyMetadataForRowidStatement;
- (sqlite3_stmt *)getDataForRowidStatement;
- (sqlite3_stmt *)getAllForRowidStatement;
- (sqlite3_stmt *)getDataForKeyStatement;
- (sqlite3_stmt *)getMetadataForKeyStatement;
- (sqlite3_stmt *)getAllForKeyStatement;
- (sqlite3_stmt *)insertForRowidStatement;
- (sqlite3_stmt *)updateAllForRowidStatement;
- (sqlite3_stmt *)updateMetadataForRowidStatement;
- (sqlite3_stmt *)removeForRowidStatement;
- (sqlite3_stmt *)removeCollectionStatement;
- (sqlite3_stmt *)removeAllStatement;
- (sqlite3_stmt *)enumerateCollectionsStatement;
- (sqlite3_stmt *)enumerateCollectionsForKeyStatement;
- (sqlite3_stmt *)enumerateKeysInCollectionStatement;
- (sqlite3_stmt *)enumerateKeysInAllCollectionsStatement;
- (sqlite3_stmt *)enumerateKeysAndMetadataInCollectionStatement;
- (sqlite3_stmt *)enumerateKeysAndMetadataInAllCollectionsStatement;
- (sqlite3_stmt *)enumerateKeysAndObjectsInCollectionStatement;
- (sqlite3_stmt *)enumerateKeysAndObjectsInAllCollectionsStatement;
- (sqlite3_stmt *)enumerateRowsInCollectionStatement;
- (sqlite3_stmt *)enumerateRowsInAllCollectionsStatement;

@end

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

@interface YapDatabaseReadTransaction () {
@public
	__unsafe_unretained YapDatabaseConnection *connection;
}

- (BOOL)getRowid:(int64_t *)rowidPtr forKey:(NSString *)key inCollection:(NSString *)collection;

- (BOOL)getKey:(NSString **)keyPtr collection:(NSString **)collectionPtr forRowid:(int64_t)rowid;

- (BOOL)getKey:(NSString **)keyPtr
    collection:(NSString **)collectionPtr
        object:(id *)objectPtr
      forRowid:(int64_t)rowid;

- (BOOL)getKey:(NSString **)keyPtr
    collection:(NSString **)collectionPtr
      metadata:(id *)metadataPtr
      forRowid:(int64_t)rowid;

- (BOOL)getKey:(NSString **)keyPtr
    collection:(NSString **)collectionPtr
        object:(id *)objectPtr
      metadata:(id *)metadataPtr
      forRowid:(int64_t)rowid;

- (BOOL)hasRowForRowid:(int64_t)rowid;

- (id)objectForKey:(NSString *)key inCollection:(NSString *)collection withRowid:(int64_t)rowid;

- (void)_enumerateKeysInCollection:(NSString *)collection
                        usingBlock:(void (^)(int64_t rowid, NSString *key, BOOL *stop))block;

- (void)_enumerateKeysInCollections:(NSArray *)collections
                         usingBlock:(void (^)(int64_t rowid, NSString *collection, NSString *key, BOOL *stop))block;

- (void)_enumerateKeysInAllCollectionsUsingBlock:
                            (void (^)(int64_t rowid, NSString *collection, NSString *key, BOOL *stop))block;

- (void)_enumerateKeysAndMetadataInCollection:(NSString *)collection
                                   usingBlock:(void (^)(int64_t rowid, NSString *key, id metadata, BOOL *stop))block;
- (void)_enumerateKeysAndMetadataInCollection:(NSString *)collection
                                   usingBlock:(void (^)(int64_t rowid, NSString *key, id metadata, BOOL *stop))block
                                   withFilter:(BOOL (^)(int64_t rowid, NSString *key))filter;

- (void)_enumerateKeysAndMetadataInCollections:(NSArray *)collections
                usingBlock:(void (^)(int64_t rowid, NSString *collection, NSString *key, id metadata, BOOL *stop))block;
- (void)_enumerateKeysAndMetadataInCollections:(NSArray *)collections
                usingBlock:(void (^)(int64_t rowid, NSString *collection, NSString *key, id metadata, BOOL *stop))block
                withFilter:(BOOL (^)(int64_t rowid, NSString *collection, NSString *key))filter;

- (void)_enumerateKeysAndMetadataInAllCollectionsUsingBlock:
                        (void (^)(int64_t rowid, NSString *collection, NSString *key, id metadata, BOOL *stop))block;
- (void)_enumerateKeysAndMetadataInAllCollectionsUsingBlock:
                        (void (^)(int64_t rowid, NSString *collection, NSString *key, id metadata, BOOL *stop))block
             withFilter:(BOOL (^)(int64_t rowid, NSString *collection, NSString *key))filter;

- (void)_enumerateKeysAndObjectsInCollection:(NSString *)collection
                                  usingBlock:(void (^)(int64_t rowid, NSString *key, id object, BOOL *stop))block;
- (void)_enumerateKeysAndObjectsInCollection:(NSString *)collection
                                  usingBlock:(void (^)(int64_t rowid, NSString *key, id object, BOOL *stop))block
                                  withFilter:(BOOL (^)(int64_t rowid, NSString *key))filter;

- (void)_enumerateKeysAndObjectsInCollections:(NSArray *)collections
                 usingBlock:(void (^)(int64_t rowid, NSString *collection, NSString *key, id object, BOOL *stop))block;
- (void)_enumerateKeysAndObjectsInCollections:(NSArray *)collections
                 usingBlock:(void (^)(int64_t rowid, NSString *collection, NSString *key, id object, BOOL *stop))block
                 withFilter:(BOOL (^)(int64_t rowid, NSString *collection, NSString *key))filter;

- (void)_enumerateKeysAndObjectsInAllCollectionsUsingBlock:
                            (void (^)(int64_t rowid, NSString *collection, NSString *key, id object, BOOL *stop))block;
- (void)_enumerateKeysAndObjectsInAllCollectionsUsingBlock:
                            (void (^)(int64_t rowid, NSString *collection, NSString *key, id object, BOOL *stop))block
                 withFilter:(BOOL (^)(int64_t rowid, NSString *collection, NSString *key))filter;

- (void)_enumerateRowsInCollection:(NSString *)collection
                        usingBlock:(void (^)(int64_t rowid, NSString *key, id object, id metadata, BOOL *stop))block;
- (void)_enumerateRowsInCollection:(NSString *)collection
                        usingBlock:(void (^)(int64_t rowid, NSString *key, id object, id metadata, BOOL *stop))block
                        withFilter:(BOOL (^)(int64_t rowid, NSString *key))filter;

- (void)_enumerateRowsInCollections:(NSArray *)collections
     usingBlock:(void (^)(int64_t rowid, NSString *collection, NSString *key, id object, id metadata, BOOL *stop))block;
- (void)_enumerateRowsInCollections:(NSArray *)collections
     usingBlock:(void (^)(int64_t rowid, NSString *collection, NSString *key, id object, id metadata, BOOL *stop))block
     withFilter:(BOOL (^)(int64_t rowid, NSString *collection, NSString *key))filter;

- (void)_enumerateRowsInAllCollectionsUsingBlock:
                (void (^)(int64_t rowid, NSString *collection, NSString *key, id object, id metadata, BOOL *stop))block;
- (void)_enumerateRowsInAllCollectionsUsingBlock:
                (void (^)(int64_t rowid, NSString *collection, NSString *key, id object, id metadata, BOOL *stop))block
     withFilter:(BOOL (^)(int64_t rowid, NSString *collection, NSString *key))filter;

@end