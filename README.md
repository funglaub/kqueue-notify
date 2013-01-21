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

Documentation
-------------

* [Kqueue whitepaper][]
* [Kqueue(2)][]

TODO
----

* Watch directories recursively.
* Accept multiple directories as arguments.
* Fix memory leaks :)

[Kqueue whitepaper]: http://people.freebsd.org/~jlemon/papers/kqueue.pdf
[Kqueue(2)]: http://www.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2
