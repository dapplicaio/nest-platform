# NEST

Nest - game platform for products and gamers.

--------------------------
# Example game: 2048
##### https://nest.dapplica.io/2048 
### Register game
-----------------
##### JS Sample
```js
const NESTCONTRACT = "nestplatform";
const author = "nestgameshub";
var game_url = "https://nest.dapplica.io/games/2048";
var keywords = ["1024","2048","numbers"];
var description = "It is a spinoff of popular 1024 game, but in this case you have to get 2048 in order to win the game";
var price = "0.00000000 WAX";
var releasedate = "10 September 2020";

try {
  const result = await WAX.transact(
    {
        actions: [
          {
            account: NESTCONTRACT,
            name: 'gameregister',
            authorization: [{
              actor: author,
              permission: 'active',
            }],
            data: {
              owner: author,
              gameurl: game_url,
              keywords: keywords,
              description: description,
              screenshots: [""],
              videos: [""],
              promscrshots: [],
              price: price,
              releasedata: releasedata,
              data: ""
            },
          }]
         }, 
        {
          blocksBehind: 3,
          expireSeconds: 30,
    });         
} catch (error) {
  console.log(error);
}
```
##### Contract Sample
```c++
name NESTCONTRACT = name("nestplatform");

name author = get_self();
string game_url = "https://nest.dapplica.io/games/2048";
vector<string> keywords[] = {"1024","2048","numbers"};
string description = "It is a spinoff of popular 1024 game, but in this case you have to get 2048 in order to win the game";
vector<string> empty_string_vector[] = {""};
vector<string> empty_vector[];
asset price = asset(0, eosio::symbol("WAX", 8));
string releasedate = "10 September 2020";

action createGame = action(
	permission_level{author, name("active")},
	NESTCONTRACT,
	name("gameregister"),
	std::make_tuple( author, game_url, keywords, description, empty_string_vector, empty_string_vector, empty_string_vector, price, releasedate, string(""))
);
createGame.send();	
```
-------------------------------
### Create achievement
-------------------------------
##### JS Sample
```js
const NESTCONTRACT = "nestplatform";
const author = "nestgameshub";
var game_url = "https://nest.dapplica.io/games/2048";
var gameid = 3;
var achiev_name = "Played total 1 game";
var description = "Played total one 2048 game round";
var image = "https://nest.dapplica.io/games/2048/images/achievement1.png";
var xp = 40;
var rarity = "Common";

try {
  const result = await WAX.transact(
    {
        actions: [
          {
            account: NESTCONTRACT,
            name: 'achievcreate',
            authorization: [{
              actor: author,
              permission: 'active',
            }],
            data: {
              owner: author,
              gameid: gameid,
              achievname: achiev_name,
              xp: xp,
              description: description,
              image: image,
              rarity: rarity
            },
          }]
         }, 
        {
          blocksBehind: 3,
          expireSeconds: 30,
    });         
} catch (error) {
  console.log(error);
}
```
##### Contract Sample
```c++
name NESTCONTRACT = name("nestplatform");

name author = get_self();
string game_url = "https://nest.dapplica.io/games/2048";
uint64_t gameid = 3;
string achiev_name = "Played total 1 game";
string description = "Played total one 2048 game round";
string image = "https://nest.dapplica.io/games/2048/images/achievement1.png";
uint64_t xp = 40;
string rarity = "Common";

action createAchieve = action(
	permission_level{author, name("active")},
	NESTCONTRACT,
	name("achievcreate"),
	std::make_tuple( author, gameid, achiev_name, xp, description, image, rarity)
);
createAchieve.send();	
```
-------------------------------
### Create leader board
-------------------------------
##### JS Sample
```js
const NESTCONTRACT = "nestplatform";
const author = "nestgameshub";
var boardname = "General";
var gameid = 3;

try {
  const result = await WAX.transact(
    {
        actions: [
          {
            account: NESTCONTRACT,
            name: 'createlboard',
            authorization: [{
              actor: author,
              permission: 'active',
            }],
            data: {
              owner: author,
              boardname: boardname,
              gameid: gameid
            },
          }]
         }, 
        {
          blocksBehind: 3,
          expireSeconds: 30,
    });         
} catch (error) {
  console.log(error);
}
```
##### Contract Sample
```c++
name NESTCONTRACT = name("nestplatform");

name author = get_self();
string boardname = "General";
uint64_t gameid = 3;

action createLBoard = action(
	permission_level{author, name("active")},
	NESTCONTRACT,
	name("createlboard"),
	std::make_tuple( author, boardname, gameid)
);
createLBoard.send();	
```
----------------------------
### Create leader board prize
-------------------------------
##### JS Sample
```js
const NESTCONTRACT = "nestplatform";
var boardid = 0;

try {
  const result = await WAX.transact(
    {
        actions: [
          {
            account: NESTCONTRACT,
            name: 'createprize',
            authorization: [{
              actor: author,
              permission: 'active',
            }],
            data: {
              boardid: boardid,
              mode: 0,
              values: ""
            },
          }]
         }, 
        {
          blocksBehind: 3,
          expireSeconds: 30,
    });         
} catch (error) {
  console.log(error);
}
```
##### Contract Sample
```c++
name NESTCONTRACT = name("nestplatform");
uint64_t boardid = 0;

//mode - prize paments mode: 0 - absolute numbers, 1 - percents
//values - string with payments values for top users on nestplatform
//vlues example - 100,50,40,
action createLBoard = action(
	permission_level{author, name("active")},
	NESTCONTRACT,
	name("createprize"),
	std::make_tuple( boardid, 0, string(0))
);
createLBoard.send();	
```
----------------------------
### Add user to leader board
You can use leader board for storing data in JSON format about user like in example:
-------------------------------
##### JS Sample
```js
const NESTCONTRACT = "nestplatform";
var boardid = 0;
var username = "someusername";
var points = 52012.0;
//User data example
var data = "{\"totalGames\":72,\"totalPoints\":42636,\"flippedTotalSquares\":92948,\"won2048\":0}";

try {
  const result = await WAX.transact(
    {
        actions: [
          {
            account: NESTCONTRACT,
            name: 'update',
            authorization: [{
              actor: author,
              permission: 'active',
            }],
            data: {
              boardid: boardid,
              username: username,
              points: points,
              data: data
            },
          }]
         }, 
        {
          blocksBehind: 3,
          expireSeconds: 30,
    });         
} catch (error) {
  console.log(error);
}
```
##### Contract Sample
```c++
name NESTCONTRACT = name("nestplatform");
uint64_t boardid = 0;
name username = name("someusername");
double points = 52012.0;
//User data example
string data = "{\"totalGames\":72,\"totalPoints\":42636,\"flippedTotalSquares\":92948,\"won2048\":0}";

//mode - prize paments mode: 0 - absolute numbers, 1 - percents
//values - string with payments values for top users on nestplatform
//vlues example - 100,50,40,
action updateLBoard = action(
	permission_level{author, name("active")},
	NESTCONTRACT,
	name("update"),
	std::make_tuple( boardid, username, points, data)
);
updateLBoard.send();	
```
------------------------------
## Contract actions
```bash
#-- For game---
gameregister            (name owner, string gameurl, vector<string> keywords, string description, 
                        vector<string> screenshots, vector<string> videos, vector<string> promscrshots, 
                        asset price, string releasedate, string data)

gameupdate              (uint64 gameid, name new_owner, string gameurl, vector<string> keywords, 
                        string description, vector<string> screenshots, vector<string> videos, 
                        vector<string> promscrshots, asset price, string releasedate, string data)
    
#-- For leader board
createlboard            (owner, boardname, gameid)
createprize             (boardid, mode, value)
resetlboard             (uint64_t boardid, bool resetpool)
removelboard            (uint64_t boardid)
update                  (uint64_t boardid, eosio::name username, double points, string data)

#-- For users
usercreate              (name author,name username)
userregister            (name username, string avatar, string nickname)
userupdate              (name username, string avatar, string nickname)
addfriend               (name username, name newfriend)
deletefriend            (name username, name delfriend)
applyfriend             (name username, name newfriend)
    
#-- For achievements
achievcreate            (name owner, uint64_t gameid, string achievname, uint64_t xp, string description, 
                        string image, string rarity)
achievdelete            (uint64_t achieveid)
achievearn              (name username, uint64_t achieveid)

#-- For cards
cardcreate              (name owner, uint64_t gameid, string cardname, string enableimg, 
                        string disableimg, string series)
carddelete              (uint64_t cardid)
carddrop                (uint64_t cardid, name username)

#For badges
badgecreate             (name owner, uint64_t gameid, string badgename, vector<uint64_t> cardids)
badgedelete             (uint64_t badgeid)
badgeearn               (name username, uint64_t badgeid)
```
--------------------
# Data Structures
### Games
```c++
gametbl {
      uint64_t                  id;                 //game id on games table
      name                      owner;              //owner WAX account
      string                    gameurl;            //game URL
      vector<string>            keywords;           //array of game keywords
      string                    description;        //game description
      vector<string>            screenshots;        //array of game screenshots URLs 
      vector<string>            videos;             //array of game videos URLs 
      vector<string>            promscrshots;       //array of promotional screenshots URLs 
      asset                     price;              //game price on WAX
      vector<string>            customtitles;       //array of users custom titles
      vector<uint64_t>          achievements;       //array of achievements IDs on TABLE achievements
      vector<uint64_t>          badges;             //array of badges IDs on TAVLE badges
      string                    releasedate;        //realise date on string format
      string                    data;               //game data on JSON format
    }
```
### Users
```c++
user{
      name                      account;            //user's WAX account
      string                    nickname;           //user's nickname
      string                    avatar;             //user's avarat URL
      uint64_t                  level;              //current user's level
      uint64_t                  xp;                 //current user's points
      vector<achiev_s>          achievements;       //array of user's achievements
      vector<name>              friends;            //array of user's friends(WAX accounts)
    }
```
### Achievements
```c++
achiev{
      uint64_t                  id;                 //achievement's id on TABLE
      uint64_t                  gameid;             //card's owner game id
      string                    achievname;         //achievement's name
      string                    description;        //achievement's description
      string                    image;              //achievement's image URL
      uint64_t                  xp;                 //achievement's xp
      string                    rarity;             //achievement's rarity
      bool                      candelete;          //achievement can be deleted only if nobody has it
    }
```
### Cards
```c++
card {
      uint64_t                  id;                 //card's TABLE id
      uint64_t                  gameid;             //card's owner game id
      string                    cardname;           //card's name
      string                    enableimg;          //card's enable image URL
      string                    disableimg;         //card's disable image URL
      string                    series;             //card's series name
      bool                      candelete;          //card can be deleted only if nobody has it
    }
``` 
### Badges
```c++
badge {
      uint64_t                  id;                 //badge TABLE id
      uint64_t                  gameid;             //badge's owner id
      string                    badgename;          //badge's name
      vector<uint64_t>          cardids;            //array of cards which includes on badge
      bool                      candelete;          //badge can be deleted only if nobody has it

```
--------------------------
# Example game: 2048
##### https://nest.dapplica.io/2048 
### Register game
-----------------
##### Contract Sample
```c++
name NESTCONTRACT = name("nestplatform");

name author = get_self();
string game_url = "https://nest.dapplica.io/games/2048";
vector<string> keywords[] = {"1024","2048","numbers"};
string description = "It is a spinoff of popular 1024 game, but in this case you have to get 2048 in order to win the game";
vector<string> empty_string_vector[] = {""};
vector<string> empty_vector[];
asset price = asset(0, eosio::symbol("WAX", 8));
string releasedate = "10 September 2020";

action createGame = action(
	permission_level{author, name("active")},
	NESTCONTRACT,
	name("gameregister"),
	std::make_tuple( author, game_url, keywords, description, empty_string_vector, empty_string_vector, empty_string_vector, price, releasedate, string(""))
);
createGame.send();	
```
##### JS Sample
```js
const NESTCONTRACT = "nestplatform";
const author = "nestgameshub";
var game_url = "https://nest.dapplica.io/games/2048";
var keywords = ["1024","2048","numbers"];
var description = "It is a spinoff of popular 1024 game, but in this case you have to get 2048 in order to win the game";
var price = "0.00000000 WAX";
var releasedate = "10 September 2020";

try {
  const result = await WAX.transact(
    {
        actions: [
          {
            account: NESTCONTRACT,
            name: 'gameregister',
            authorization: [{
              actor: author,
              permission: 'active',
            }],
            data: {
              owner: author,
              gameurl: game_url,
              keywords: keywords,
              description: description,
              screenshots: [""],
              videos: [""],
              promscrshots: [],
              price: price,
              releasedata: releasedata,
              data: ""
            },
          }]
         }, 
        {
          blocksBehind: 3,
          expireSeconds: 30,
    });         
} catch (error) {
  console.log(error);
}
```
-------------------------------
### Create achievement
-------------------------------
##### Contract Sample
```c++
name NESTCONTRACT = name("nestplatform");

name author = get_self();
string game_url = "https://nest.dapplica.io/games/2048";
uint64_t gameid = 3;
string achiev_name = "Played total 1 game";
string description = "Played total one 2048 game round";
string image = "https://nest.dapplica.io/games/2048/images/achievement1.png";
uint64_t xp = 40;
string rarity = "Common";

action createAchieve = action(
	permission_level{author, name("active")},
	NESTCONTRACT,
	name("achievcreate"),
	std::make_tuple( author, gameid, achiev_name, xp, description, image, rarity)
);
createAchieve.send();	
```
##### JS Sample
```js
const NESTCONTRACT = "nestplatform";
const author = "nestgameshub";
var game_url = "https://nest.dapplica.io/games/2048";
var gameid = 3;
var achiev_name = "Played total 1 game";
var description = "Played total one 2048 game round";
var image = "https://nest.dapplica.io/games/2048/images/achievement1.png";
var xp = 40;
var rarity = "Common";

try {
  const result = await WAX.transact(
    {
        actions: [
          {
            account: NESTCONTRACT,
            name: 'achievcreate',
            authorization: [{
              actor: author,
              permission: 'active',
            }],
            data: {
              owner: author,
              gameid: gameid,
              achievname: achiev_name,
              xp: xp,
              description: description,
              image: image,
              rarity: rarity
            },
          }]
         }, 
        {
          blocksBehind: 3,
          expireSeconds: 30,
    });         
} catch (error) {
  console.log(error);
}
```
-------------------------------
### Create leader board
-------------------------------
##### Contract Sample
```c++
name NESTCONTRACT = name("nestplatform");

name author = get_self();
string boardname = "General";
uint64_t gameid = 3;

action createLBoard = action(
	permission_level{author, name("active")},
	NESTCONTRACT,
	name("createlboard"),
	std::make_tuple( author, boardname, gameid)
);
createLBoard.send();	
```
##### JS Sample
```js
const NESTCONTRACT = "nestplatform";
const author = "nestgameshub";
var boardname = "General";
var gameid = 3;

try {
  const result = await WAX.transact(
    {
        actions: [
          {
            account: NESTCONTRACT,
            name: 'createlboard',
            authorization: [{
              actor: author,
              permission: 'active',
            }],
            data: {
              owner: author,
              boardname: boardname,
              gameid: gameid
            },
          }]
         }, 
        {
          blocksBehind: 3,
          expireSeconds: 30,
    });         
} catch (error) {
  console.log(error);
}
```
----------------------------
### Create leader board prize
-------------------------------
##### Contract Sample
```c++
name NESTCONTRACT = name("nestplatform");
uint64_t boardid = 0;

//mode - prize paments mode: 0 - absolute numbers, 1 - percents
//values - string with payments values for top users on nestplatform
//vlues example - 100,50,40,
action createLBoard = action(
	permission_level{author, name("active")},
	NESTCONTRACT,
	name("createprize"),
	std::make_tuple( boardid, 0, string(0))
);
createLBoard.send();	
```
##### JS Sample
```js
const NESTCONTRACT = "nestplatform";
var boardid = 0;

try {
  const result = await WAX.transact(
    {
        actions: [
          {
            account: NESTCONTRACT,
            name: 'createprize',
            authorization: [{
              actor: author,
              permission: 'active',
            }],
            data: {
              boardid: boardid,
              mode: 0,
              values: ""
            },
          }]
         }, 
        {
          blocksBehind: 3,
          expireSeconds: 30,
    });         
} catch (error) {
  console.log(error);
}
```
----------------------------
### Add user to leader board
You can use leader board for storing data in JSON format about user like in example:
-------------------------------
##### Contract Sample
```c++
name NESTCONTRACT = name("nestplatform");
uint64_t boardid = 0;
name username = name("someusername");
double points = 52012.0;
//User data example
string data = "{\"totalGames\":72,\"totalPoints\":42636,\"flippedTotalSquares\":92948,\"won2048\":0}";

//mode - prize paments mode: 0 - absolute numbers, 1 - percents
//values - string with payments values for top users on nestplatform
//vlues example - 100,50,40,
action updateLBoard = action(
	permission_level{author, name("active")},
	NESTCONTRACT,
	name("update"),
	std::make_tuple( boardid, username, points, data)
);
updateLBoard.send();	
```
##### JS Sample
```js
const NESTCONTRACT = "nestplatform";
var boardid = 0;
var username = "someusername";
var points = 52012.0;
//User data example
var data = "{\"totalGames\":72,\"totalPoints\":42636,\"flippedTotalSquares\":92948,\"won2048\":0}";

try {
  const result = await WAX.transact(
    {
        actions: [
          {
            account: NESTCONTRACT,
            name: 'update',
            authorization: [{
              actor: author,
              permission: 'active',
            }],
            data: {
              boardid: boardid,
              username: username,
              points: points,
              data: data
            },
          }]
         }, 
        {
          blocksBehind: 3,
          expireSeconds: 30,
    });         
} catch (error) {
  console.log(error);
}
```

 
