// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/02.

import SwiftUI

public struct SeenSwiftUIView: SeenOSViewProto {
  public init() {}

  public func makeSeenOSView(context: Context) -> SeenView {
    return SeenView()
  }

  public func updateSeenOSView(_ view: SeenView, context: Context) {

  }
}
