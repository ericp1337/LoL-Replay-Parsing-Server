LoL-Replay-Parsing-Server
=========================
##Disclaimer##
Currently this project requires Qt >= 5.4
I have tried to recently build on Ubuntu 14.04 LTS, but it seems to throw an exception from one of the Qt libraries. I do successfully compile under ArchLinux, so I think that it just needs one of the newer libraries from Qt.

##Description##
This is a League of Legends Server that accepts a client to upload a replay, parses it, and then sends the client back a parsed version of the replay metadata.

* Dependencies are located in the `deps` folder. Make sure to do a git submodule update

##Build Instructions##
* Execute ```git submodule init && git submodule update```
* go to `deps/qhttp` and execute ```./update-dependencies.sh```
* edit src/src.pro and add ```CONFIG += staticlib``` This will make the library static. Just makes it easier.
* go back to ```deps/qhttp``` and execute ```qmake qhttp.pro``` then ```make -j 4```
* go to root of project folder, run ```qmake && make -j 4```

#Requests#
##Post##
|     Type    |      URL     |                       Description                   |
|-------------|--------------|-----------------------------------------------------|
| Upload Lrf  | /upload-lrf  | upload a lrf and get back parsed json               |
| Upload Rofl | /upload-rofl | upload official LoL replay and get back parsed json |

##Get##
|     Type        |             URL              |                       Description                 |
|-----------------|------------------------------|---------------------------------------------------|
| List Replays    | /list-replays                | returns json with all available matches on server |
| Download Replay | /replay/download/$(match_id) | downloads the replay file for the given match id  |
| Get Replay Info | /replay/info/$(match_id)     | Downloads the json for the specified match id     |

##Sample Replay Files##
You can find some replay files to try out here: [lrf replays](http://replays.computerfr33k.com/lol/lrf/)
