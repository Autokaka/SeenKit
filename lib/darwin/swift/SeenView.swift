// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import SwiftUI

public struct SeenSwiftUIView: SeenOSViewProto {
  public init() {}

  public func makeSeenOSView(context: Context) -> SeenView {
    return SeenView()
  }

  public func updateSeenOSView(_ view: SeenView, context: Context) {

  }
}
