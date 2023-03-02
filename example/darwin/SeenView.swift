/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

import SwiftUI

#if os(iOS) || os(tvOS) || os(watchOS)
#else
struct SeenSwiftUIView: NSViewRepresentable {

  typealias NSViewType = SeenView

  func makeNSView(context: Context) -> SeenView {
    SeenView()
  }
  
  func updateNSView(_ nsView: SeenView, context: Context) {
  }
}
#endif
