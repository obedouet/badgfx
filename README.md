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
#define MAIN
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

`main.cpp` must define `MAIN` before including `badgfx.hpp` (it instantiates a debug symbol).

## Building

Requirements: X11 development headers (`libx11-dev` on Debian/Ubuntu, `xorg-x11-proto-devel` + `libX11-devel` on Fedora) and `g++`.

Compile the library sources, then the example:

```sh
# from the repository root
g++ -Wall -O -I. -c fenetre.cpp bouton.cpp composant.cpp message.cpp

cd examples/HelloWorld
make
./helloworld
```

> Note: the example `Makefile` dates from ~2002. It assumes the repository directory is named `BadGFX` (`BADGFXDIR=../BadGFX`) and hardcodes `/usr/X11R6` paths. On modern systems you may need to adapt those paths (on most distros plain `-I. -lX11` is enough).

## Heads-up: this is 20-year-old code

The code is published as-is, for historical and fun purposes. Expect pre-modern-C++ habits (`void` parameters, `(void *)` component lists, fixed 64-byte debug buffers, `strcpy`/`malloc` usage, French identifiers: *fenêtre*, *bouton*, *composant*, *message*) and see the source files for a few known quirks. Patches to modernize it are welcome — see `LICENSE`.

## License

Apache License 2.0 — see [LICENSE](LICENSE).
