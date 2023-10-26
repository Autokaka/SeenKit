// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/02.

import SwiftUI

#if os(iOS) || os(tvOS) || os(watchOS)
#else
struct SeenView: NSViewRepresentable {
  private let _engine: SeenEngine = SeenEngine()

  func makeNSView(context: Context) -> NSView {
    _engine.update(withTimeDelta: 16)
    return NSView()
  }
  
  func updateNSView(_ nsView: NSView, context: Context) {
  }
}
#endif
