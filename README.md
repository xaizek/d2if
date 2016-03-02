dzen2 input formatter
=====================

Copyright (C) 2012 xaizek <xaizek@openmailbox.org>

Licensed under GNU/GPL v2.

Brief description
-----------------

This is a simple dzen2 input formatter, which is intender to omit using of
scripts to do this job.  Compilable language is better for this purpose, since
it will require less resources and work faster than scripts written in
interpretable languages.  The amount of memory and CPU time should be considered
since status bar should be updated regularly.

Configuration
-------------

None (at least yet).  Body of `main()` configures everything.

Fields
------

* Date and time.
* Current desktop number.
* Active keyboard layout.
* Memory usage.
* CPU usage.
* Display brightness.
* Current audio level.
* Battery status.
* Network interfaces status.
* MPD status and currently playing song.
