LoL-Replay-Parsing-Server
=========================
##Description##
This is a League of Legends Server that accepts a client to upload a replay, parses it, and then sends the client back a parsed version of the replay metadata.

* Dependencies are located in the `deps` folder. Make sure to do a git submodule update

Build Instructions
==================
* go to `deps/qhttp` and execute ```./update-dependencies.sh```
* edit src/src.pro and add ```CONFIG += staticlib``` This will make the library static. Just makes it easier.
* go back to ```deps/qhttp``` and execute ```qmake qhttp.pro``` then ```make -j 4```
* go to root of project folder and then build ```src``` from Qt and run
