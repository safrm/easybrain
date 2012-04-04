Easybrain 0.0.2-78-g779689f* (20120227_1146)
-------------------------------------------------------------------------------
Autor : Miroslav Safr
Contact: miroslav.safr@gmail.com
Web: http://safrm.net/

Introduction:
-------------------------------------------------------------------------------
How many times  you wanted to copy some text from web and had to open notepad or gedit and copy it it there and than again just to remove annoying formating which came with it..?
I did it many times :)
Also once I  opened the notepad I have been asking myself why this stupid editor does not have some more intuitive features.. Here is the reason simple but than when you use MS Office Word everything gets so complicated and ends up in never ending editing of never ending doc file. Specially style management grows like cancer.
There exists already millions of text editors and some of them have strong communities and very cool features.
I have been using many of them but in the end I realized that none of them suits me :(.
I am using quite a lot nowadays google docs - for example to store my todo list but I think I am not so much paranoid if I expect very soon to get some targeted advertisements since these "free" services are not "so free" how they try to look like..
I try to accent data portability and reusability in many different environments.

License
-------------------------------------------------------------------------------
available in license.txt
I choose Creative Commons — Attribution-NoDerivs 3.0 Unported because I would like to choose later if I want to opensouce it or reuse it in some commerical work later.
This project does not need source contribution at this moment and the result can be used as much as it's needed.


command line parameters:
-------------------------------------------------------------------------------
-h  --help      print command line usage
-t  --tray      start minimized in tray
-v  --version   print version


filesystem:
-------------------------------------------------------------------------------
local:
./easybrain.exe
./easybrain
./conf

data:
./data/<project_name>/tmp            # unpack synch
                     /new            # <project_name>_<TS>.eb
                     /archive        # <project_name>_<TS>.eb
                     /backup         # <project_name>_new_<TS>.tar.bz2
                                     # <project_name>_archive_<TS>.tar.bz2

remote filesystem:
ftp://easybrain/bin/version
ftp://easybrain/<passwd>/new

linux:
    /usr/bin/easybrain
    /usr/share/doc/easybrain/readme.txt
    /usr/share/doc/easybrain/todo.txt
    /usr/share/icons/hicolor/scalable/apps/easybrain.svg
    /usr/share/applications/easybrain.desktop
