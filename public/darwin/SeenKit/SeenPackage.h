/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#include <Foundation/Foundation.h>

@interface SeenPackage : NSObject

@property(readonly, copy, nullable, nonatomic) NSDictionary* info;

@property(readonly, copy, nonnull, nonatomic) NSData* module;

- (nullable instancetype)initWithContentsOfFile:(nonnull NSString*)path;

// clang-format off
- (void)saveFile:(nonnull NSData*)data toSandbox:(nonnull NSString*)relativePath withCompletionHandler:(nonnull void (^)(BOOL))handler;
- (void)getFileFromSandbox:(nonnull NSString*)relativePath withCompletionHandler:(nonnull void (^)(NSData* _Nonnull))handler;
// clang-format on

@end
