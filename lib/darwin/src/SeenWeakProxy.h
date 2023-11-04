// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/03.

#import <Foundation/Foundation.h>

@interface SeenWeakProxy : NSObject

- (instancetype)initWithId:(id)ptr NS_DESIGNATED_INITIALIZER;

- (nullable id)lock;

@end
