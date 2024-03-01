// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

import SwiftUI
import SeenKit

struct ContentView: View {
    var body: some View {
        SeenSwiftUIView()
    }
}

struct ContentPreviewProvider: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
