Robust Maliit Framework with crash fixes
=========================================

This project is co‑authored by Qwen (mostly) and Gemini as the code reviewer.
However, this is not *vibe coding* in any sense. Here is my workflow with the AIs:

1. AI analyze existing code
2. AI summarize its understanding
3. I verify or correct that understanding
4. I define the requirements or the bug
5. AI implements the solution
6. I get another AI (in this case Gemini) to review the changes, and I repeat this step back and forth several times
7. Finally, I make the final decision

However, if this AI‑assisted workflow isn't something you're comfortable with, you're welcome to skip it.

---

**Above is the ROBUST fork information. Below is the original README content:**

---

Maliit
======

Maliit provides a flexible and cross-platform input method framework. It has a
plugin-based client-server architecture where applications act as clients and
communicate with the Maliit server via input context plugins. The communication
link currently uses D-Bus. Maliit is an open source framework (LGPL 2.1) with
a production-quality [keyboard plugin](https://github.com/maliit/keyboard) (LGPL 3.0).

Installing
----------

Qt 5 must be installed to build the Maliit framework. At a terminal, run:

```
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr ..
make
make install
```

Running
-------

Set Maliit as the Qt and GTK+ input context:

```
export QT_IM_MODULE=Maliit
export GTK_IM_MODULE=Maliit
```

Start the server:

```
maliit-server
```

Note that a compositing window manager and a D-Bus session bus are required to
use Maliit.

Test with the provided example applications:

```
maliit-exampleapp-plainqt # for Qt
maliit-exampleapp-gtk2 # for Gtk2
maliit-exampleapp-gtk3 # for Gtk3
```

Double-tap on the input field, and an input method (usually a virtual keyboard)
should be shown. Note that an input method plugin, such as the Maliit keyboard
from the maliit-plugins package, must be installed for the example applications
to work.


NETWORK TRANSPARENCY

On one computer run:
```
maliit-server -allow-anonymous -override-address tcp:host=xxx.xxx.xxx.xxx,port=yyyyy
```

Any valid dbus address is supported. Using -allow-anonymous must only be done on
a trusted network. If using a method with authentication, the -allow-anonymous flag
may be dropped.

On another computer:
```
MALIIT_SERVER_ADDRESS=tcp:host=xxx.xxx.xxx.xxx,port=yyyyy
export MALIIT_SERVER_ADDRESS
maliit-exampleapp-plainqt (or maliit-exampleapp-gtk{2,3})
```

Where xxx.xxx.xxx.xxx is IP address of computer where maliit-server is ran
and yyyyy is port number < 65536.
