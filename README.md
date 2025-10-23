# printfˣ
Full compile-time printf without Unicode bloat.

### Demo

![Demo Video](demo.gif)

### FAQs
#### My compiler already catches this - what's the point?

This is portable to any compiler that supports the C++20 standard or greater
and is always a hard error, as it should be. It doesn't depend on an LSP or
external lint tool. If your IDE/LSP can evaluate templates and macros,
this will work in the editor, too.

#### Should I use this?

Maybe? It's the first "real" C++ library I've made and for 99% of cases you
should probably use libfmt.