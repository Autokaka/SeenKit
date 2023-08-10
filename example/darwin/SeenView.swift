// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/02.

import SwiftUI

#if os(iOS) || os(tvOS) || os(watchOS)
#else
struct SeenSwiftUIView: NSViewRepresentable {

  typealias NSViewType = NSView

  func makeNSView(context: Context) -> NSViewType {
    SeenEngine()
    return NSView()
  }
  
  func updateNSView(_ nsView: NSViewType, context: Context) {
  }
}
#endif
