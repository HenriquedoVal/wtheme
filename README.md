## wtheme

Simple CLI tool for changing Windows theme (light, dark, and others) using the undocumented IThemeManager2 COM.  

~~~
Usage: wtheme <cmd> [idx]
        Commands: dump: print theme names to stdout with its index
                  set: set current theme by index

`wtheme dump` or `wtheme set <idx>`
~~~

#### Building
~~~Console
cl main.c
~~~

#### Warning
IThemeManager2 has changed on previous versions of Windows and might change again, but
to the extent of our use in this simple program, it seems pretty stable.
Anyway, be careful when using different versions of Windows.  
  
Tested on:
- Win10 22h2 - 10.0.19045
- Win11 22h2 - 10.0.22621
