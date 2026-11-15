# BadGFX

A tiny, vintage C++ library for building simple graphical user interfaces directly on top of **X11/Xlib**.

Written between 1999 and 2004 as a personal project, BadGFX wraps raw Xlib calls in a handful of small classes so that a working GUI application can be written in a few lines of code. It has no dependency other than `libX11` and a C++ compiler.

## What it gives you

| Class | Role |
|---|---|
| `Fenetre` | Top-level X11 window: creation, main event loop, colour handling, component list |
| `Composant` | Abstract base class for widgets (`Rafraichir`, `OnClic`, `OnMove`, `OnKey`) |
| `Bouton` | Push button (`NORMAL`) or toggleable checkbox (`CHECKBOX`), with a C callback per click |
| `MessageBox` | Bordered message/log zone pinned to the bottom of the window, with a small history |

Events from Xlib are translated into a generic `Evenement` struct (keyboard buffer, mouse button, coordinates) and dispatched to every component registered on the window.

## Hello World

```cpp
#include <badgfx.hpp>

Fenetre Fen1(200, 80, "Hello World");

void HelloWorld(Composant *B)
{
    ((Bouton *)B)->ChgTexte("Hello World");
    B->Rafraichir();
}

int main(void)
{
    Coord pos = { 5, 5 };

    Bouton Hello(&Fen1, &pos, "Click here", NORMAL);
    Hello.ChgAction(HelloWorld);
    Fen1.AjouteComposant(&Hello);

    Fen1.MainLoop();
    return 0;
}
```

No special macro is required before including `badgfx.hpp`.

## Building

Requirements: X11 development headers (`libx11-dev` on Debian/Ubuntu, `xorg-x11-proto-devel` + `libX11-devel` on Fedora) and `g++`.

From the example directory:

```sh
cd examples/HelloWorld
make
./helloworld
```

The `Makefile` also compiles the library sources from the repository root. Closing the window through the window manager now exits `MainLoop()` instead of leaving the program waiting forever.

## Heads-up: this is 20-year-old code

The code keeps its original French identifiers and simple callback style: *fenêtre*, *bouton*, *composant*, *message*. Recent cleanups fixed const-correctness, unsafe `strcpy` use, uninitialized pointer risks, and missing X11 window-close handling without trying to rewrite BadGFX into a modern GUI framework.

## License

Apache License 2.0 — see [LICENSE](LICENSE).
