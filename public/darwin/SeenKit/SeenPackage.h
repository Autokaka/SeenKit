/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#if SEEN_BUILD_DARWIN

#include <Foundation/Foundation.h>
#include <Foundation/NSObject.h>

@interface SEENPackage : NSObject

@property(readonly, copy, nullable, nonatomic) NSDictionary* info;

@property(readonly, copy, nonnull, nonatomic) NSString* resourceDirectory;

@property(readonly, copy, nonnull, nonatomic) NSData* module;

- (nullable instancetype)initWithContentsOfFile:(nullable NSString*)path;

@end

#endif
