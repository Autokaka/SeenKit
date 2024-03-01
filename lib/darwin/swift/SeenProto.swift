// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import SwiftUI

#if os(iOS) || os(tvOS)
public typealias SeenOSView = UIView
public typealias SeenOSViewRepresentable = UIViewRepresentable
#elseif os(macOS)
public typealias SeenOSView = NSView
public typealias SeenOSViewRepresentable = NSViewRepresentable
#endif

public protocol SeenOSViewProto: SeenOSViewRepresentable {
    associatedtype SeenOSViewType

    func makeSeenOSView(context: Context) -> SeenOSViewType
    func updateSeenOSView(_ seenOSView: SeenOSViewType, context: Context)
}

#if os(iOS) || os(tvOS)
public extension SeenOSViewProto where UIViewType == SeenOSViewType {
    func makeUIView(context: Context) -> UIViewType {
        makeSeenOSView(context: context)
    }

    func updateUIView(_ uiView: UIViewType, context: Context) {
        updateSeenOSView(uiView, context: context)
    }
}
#elseif os(macOS)
public extension SeenOSViewProto where NSViewType == SeenOSViewType {
    func makeNSView(context: Context) -> NSViewType {
        makeSeenOSView(context: context)
    }

    func updateNSView(_ nsView: NSViewType, context: Context) {
        updateSeenOSView(nsView, context: context)
    }
}
#endif
