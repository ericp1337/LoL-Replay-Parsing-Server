LoL-Replay-Parsing-Server
=========================
##Disclaimer##
* Currently this project requires Qt >= 5.4
* The Client Example is cross-platform including windows, you will have to run this manually instead of through QtCreator as it will try to build the entire project and throw and error since the server is not compatible with windows.

##Description##
This is a League of Legends Server that accepts a client to upload a replay, parses it, and then sends the client back a parsed version of the replay metadata.

* Dependencies are located in the `deps` folder. Make sure to do a git submodule update

##Build Instructions##
###Build from command-line###
* Execute ```git submodule init && git submodule update```
* go to `deps/qhttp` and execute ```./update-dependencies.sh```
* go back to ```deps/qhttp``` and execute ```qmake qhttp.pro``` then ```make -j 4```
* go to root of project folder, run ```qmake && make -j 4```
 
###Build using QtCreator###
* Execute ```git submodule init && git submodule update```
* go to `deps/qhttp` and execute ```./update-dependencies.sh```
* Select `src` from build menu and click run to run the server
* (Select `client` if you wish to run the example client that can interact with the server using a GUI app)

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
You can find some replay files to try out here: [league replays](http://www.leaguereplays.com/replays/)
