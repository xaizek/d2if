dzen2 input formatter
=====================

Licensed under GNU/GPL v2+.

Brief description
-----------------

This is a simple dzen2 input formatter, which is intended to replace use of
scripts to do this job.  Compiled language is better for this purpose, because
native applications require less resources and work faster than scripts.  The
amount of memory and CPU time should be considered for obvious reason that
status bar is updated with relatively high rate.

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
* Current audio volume level.
* Battery status.
* Network interfaces status.
* MPD status and currently playing song.
