Tepl - Text editor product line
===============================

![folding test app screenshot](etc/test-fold-actions.png "folding test app")

This is version 3.1.0 of Tepl.

Tepl is a library that eases the development of GtkSourceView-based text
editors and IDEs.

Tepl was previously named Gtef (GTK+ text editor framework). The project has
been renamed in June 2017 to have a more beautiful name. The end of Tepl is
pronounced like in “apple”.

The Tepl library is free software and is released under the terms of the GNU
Lesser General Public License, see the 'COPYING' file for more information.

The Tepl web page:

    https://wiki.gnome.org/Projects/Tepl

Dependencies
------------

- GLib >= 2.52
- GTK+ >= 3.20
- GtkSourceView >= 3.22
- libxml2 >= 2.5
- uchardet

Since uchardet is a fairly new library, here is the URL:
https://www.freedesktop.org/wiki/Software/uchardet/

Installation
------------

Simple install procedure:

  $ ./configure
  $ make
  [ Become root if necessary ]
  $ make install

See the file 'INSTALL' for more detailed information.

From the Git repository, the 'configure' script and the 'INSTALL' file are not
yet generated, so you need to run 'autogen.sh' instead, which takes the same
arguments as 'configure'.

To build the latest version of Tepl plus its dependencies from Git, Jhbuild is
recommended:

    https://wiki.gnome.org/Projects/Jhbuild

How to contribute
-----------------

See the file 'HACKING'.
