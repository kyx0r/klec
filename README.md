
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
(see submodule readme)

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
(see submodule readme)

- surf -
1. I took a fork from Francesco as a base, see the Readme in
surf directory. This will get updated if I decide to change anything.

