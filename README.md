
KLEK: Kyryl's Linux Configuration Kit

A bunch of personal config settings, feel free
to copy the idea / settings / etc 


Forks and custom changes overview:

- dwm - 
1. Added lots of cutom key bindings that do not
conflict with any other program in this repository
2. The bar was heavily modified to be more minimal and 
take up less space, with 100% mouse support as well.
3. Added window hide/show functionality that can be 
activated using mouse clicks on appropriate bar cell.
When window is hidden it is shown with darker color on the
bar, and you can't switch to it when cycling windows.
4. There are many extra window layouts that can be cycled

- st -
1. Not many changes here besides auto copy to system 
clipboard, and keyboard select patch. All colors are 
set to standard and the font used is terminus-ttf-4.47.0
located in this repository

- slstatus -
1. The only interesting part is the config.h where
I chose to display only the useful info

- GoldenShimmer -
1. This is a custom X11 cursor that is animated 
using bitmaps that I think looks amazing

- fastcd -
1. A server-client program, I only fixed some buggy
code that made it break when there are too many dirs
2. I also have a custom invoking script in ./scripts 
called x.sh

- grabc - 
1. No differences to original program, but it's too
useful to not include

- ./scripts -
1. Right now contains python scripts that I used to
download pics from pixiv.com using pixivpy

- xkeyboard -
1. This is a onscreen keyboard program heavily modified by me
to be usable with touchscreens.
2. Added font support
3. Added move, resize, hide buttons.
4. Added two different input modes, one via XTest extension
and the other via SendEvent. Different programs usually respond
differently to those.

- utils -
1. tinycc.c is a special version of tcc that tries to almaginate 
C source files. It has correct preprocessing of #include and 
the rest of the spec. So to use it pass in -E option.  
2. cembed.c & tinypgm.c a utility that I made to work with 
pgm images and convert them to C hex arrays. I mainly used 
when developing xkeyboard.
3. amal.c attempt to do what tinycc.c does but it is not accurate, 
don't use it, I keep it just as a template code for when I want to
build another utility program.

- neatvi -
1. Added unindent keybind: ^q 
2. Added quickexit to insert mode keybind: kj   
3. Added linenumbers, keybind: # 
4. Added invisible character view, keybind: V   
5. Added regex for changing spaces to tabs, keybind: ^i   
6. Added regex for removing /r in dos files, keybind: ^ii 
7. Changed behavior of ^a to change search direction when no more match 
8. Added fssearch, searches what is under the cursor 
in every file in the opened directory keybind: ^]   
Right now it only checks for .c and .h files as I use this
for code navigation between files, jump to definitions / etc.
9. fssearch but going in reverse keybind: ^p
10. Added key to change the mark origin which ^p will return to keybind: ^t
11. Added ex command "ea" which opens file case insensitive and ignores the path,
setting an extra number parameter after filename will skip that many matches with
similar path. So for example file might be named "file123.c" but you can open it
just by "ea fi" if this is a uniquely matched filename
12. Added ex command "dir" to set the directory for fssearch or "ea" ex command
13. Added numbered buffers to vi, max 8 and ex "b" command to switch
14. Added key to show buffers and switch buffer keybind: ^s 
15. Added key to exit vi keybind: qq 
16. Added key to goto line 0 keybind: gg 
17. Added key to delete everything inside (cursor outside) "" keybind: di" 
18. Added key to delete everything inside () keybind: di) 
19. Added key to delete everything inside (cursor outside) () keybind: di( 
20. Added key to change . to -> on cursor line keybind: vh
21. Added key to change -> to . on cursor line keybind: vg
22. Added ex command "z" it does the same thing "!" does but the shell is
launched in interactive mode, so stuff like aliases work. But it will cause
vi to suspend.
23. Changed the colors for C language in conf.h to be sane
24. Added extra highlight keywords for C
25. Added key that splits the line (opposite of J) keybind: K
26. Added key that line wraps text to 80 line limit keybind: gq 
27. Added key that deletes relative line 
without moving the cursor (down) keybind: arg+I (up) keybind arg+R
28. Added ability to view the numbers for arguments that keys e,w,E,W,b,B
may take. keybind: ^v Pressing again will change the key mode, specifying
any arguments will exit the mode. This is a major step up to how navigation
works in vi, it makes it so much easier to use because now you can see where 
you are going. If not anything else I suggest you to try out this distro
just to see it in action.
29. Added ability to change highlight dynamically.
30. New ex option "hww" which highlights every instance of word under the
cursor when enabled. Useful for when studying source code, currently only
works in C files, but can be made to work in other places too.

- surf -
1. I took a fork from Francesco as a base, see the Readme in
surf directory. This will get updated if I decide to change anything.

