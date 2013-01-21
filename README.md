Kqueue notifier
===============

Supported operating systems
---------------------------

Should run fine on Macs and FreeBSD. Although I never tried it, Linux might work
if kqueue is available, other BSDs should be supported, too.

Compilation
-----------

Type `make`.

Usage
-----

`./kqueue-notify <directory>`

TODO
----

* Watch directories recursively.
* Accept multiple directories as arguments.
* Fix memory leaks :)
