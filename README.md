# flash-cardc
Terminal-based graphical flash card app

### Build
Just uses CMake, which will fetch the main dependency [ftxui](https://github.com/ArthurSonzogni/FTXUI) itself when you start building.

From root directory:
```sh
mkdir build
cd build
cmake ../        # generate build files
cmake --build    # compile/link
./flash-cardc    # run program
```

> [!NOTE]
> If you want to build from vscode like I like to, it works best if your build/ folder is empty.
> If you've already built the project from the command line, just delete everything from build/
> and then build it all from vscode and it will work from then on. Also I use g++ on linux, in case
> it asks and affects you.
