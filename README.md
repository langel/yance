# yance

## compilation notes
### Makefile
If you want yance to launch without the console window add the following line:
`-Wl,-subsystem,windows \`
### Mac OS
Requires a brew installation of SDL2. Compile/Run with `./brew_build.sh`
### Windows
minGW is kind of a pain to setup but kick off a build with `make.bat`
