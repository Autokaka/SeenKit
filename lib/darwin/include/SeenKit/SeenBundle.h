// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/07.

#import <Foundation/Foundation.h>
#import <SeenKit/SeenMacros.h>

NS_ASSUME_NONNULL_BEGIN

SEEN_EXPORT @interface SeenBundle : NSObject

@property(nonatomic, readonly, copy) NSString* entryPath;
@property(nonatomic, readonly, copy) NSString* assetsDirectory;

- (nullable instancetype)initWithPath:(NSString*)path NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END
