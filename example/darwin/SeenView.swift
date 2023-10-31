// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/02.

import SwiftUI

#if os(iOS) || os(tvOS) || os(watchOS)
#else
struct SeenSwiftUIView: NSViewRepresentable {
  func makeNSView(context: Context) -> NSView {
    return SeenNSView()
  }
  
  func updateNSView(_ view: NSView, context: Context) {
      
  }
}
#endif
