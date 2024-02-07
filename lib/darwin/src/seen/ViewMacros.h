// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/28.

#define SEEN_VIEW_INIT_COMMON_DECL -(void)initCommon

#define SEEN_VIEW_INIT_COMMON_IMPL                   \
  -(instancetype)initWithFrame : (CGRect)frameRect { \
    self = [super initWithFrame:frameRect];          \
    if (self != nil) {                               \
      [self initCommon];                             \
    }                                                \
    return self;                                     \
  }                                                  \
  -(instancetype)initWithCoder : (NSCoder*)coder {   \
    self = [super initWithCoder:coder];              \
    if (self != nil) {                               \
      [self initCommon];                             \
    }                                                \
    return self;                                     \
  }                                                  \
  -(void)initCommon
