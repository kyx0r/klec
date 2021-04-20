heirloom-mailx
==============

This is version 12.4 of "heirloom-mailx"

History
-------

The mailx program began at Berkely around 1978. The BSD version
continued to be updated until 1993. That code was later forked
(around the year 2000) into the `nail` project, which added
first the MIME changes to email from the 1990s and then
additional features like reading remote mail stores (POP and
IMAP). 

In 2006 Gunnar Ritter adopted it into the "heirloom" project
and changed the name back to `mailx`. The last update was
apparently in 2008, version 12.4. This version comes from
[nail-12.4-14.mga6.src.rpm](http://distrib-coffee.ipsl.jussieu.fr/pub/linux/Mageia/distrib/6/SRPMS/core/release/nail-12.4-14.mga6.src.rpm)

It has the ten patches from that RPM applied, and the ChangeLog
has been updated with the changes listed in `nail.spec`. The
patches include fixing CVE-2004-2771 and killing off SSLv2.

