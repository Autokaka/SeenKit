/**
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/14.
 */

import SwiftUI

struct ContentView: View {
    var body: some View {
        VStack {
            Image(systemName: "globe")
                .imageScale(.large)
                .foregroundColor(.accentColor)
            Text("Hello, world!")
        }
        .padding()
    }
}

struct ContentViewPreviewProvider: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
