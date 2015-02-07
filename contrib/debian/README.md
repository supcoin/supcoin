
Debian
====================
This directory contains files used to package supcoind/supcoin-qt
for Debian-based Linux systems. If you compile supcoind/supcoin-qt yourself, there are some useful files here.

## supcoin: URI support ##


supcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install supcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your supcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/supcoin128.png` to `/usr/share/pixmaps`

supcoin-qt.protocol (KDE)

