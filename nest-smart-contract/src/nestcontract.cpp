#include <nestcontract.hpp>




/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        nestcontract ACTIONS
//
/////////////////////////////////////////////////////////////////////////////////////////////////////


//action for creating new leader board
//owner - owner wax account
//boardname - name for nestcontract
//gameid - id of owner's game
//
void nestcontract::createlboard(eosio::name owner, string boardname, uint64_t gameid)
{
    require_auth(owner);

    lboards _lb(CONTRACTN, CONTRACTN.value);

    uint64_t boardid{0};

    auto itr = _lb.end();
    if (itr == _lb.begin())
    {
        boardid = 0;
    }
    else
    {
        boardid = itr->id;
        boardid++;
    }

    _lb.emplace(CONTRACTN, [&](auto &new_board) {
        new_board.id = boardid;
        new_board.owner = owner;
        new_board.boardname = boardname;
        new_board.gameid = gameid;
    });
}

//action for creating prize on nestcontract
//owner - owner wax account
//boardid - nestcontract id on table lboards
//mode - prize paments mode: 0 - absolute numbers, 1 - percents 
//values - string with payments values for top users on nestcontract
//vlues example - 100,50,40,
//
void nestcontract::createprize(eosio::name owner, uint64_t boardid, uint8_t mode, string values)
{
    require_auth(owner);

    lboards _lb(CONTRACTN, CONTRACTN.value);

    auto lb_itr = _lb.find(boardid);
    eosio::check(lb_itr != _lb.end(), "Leader board with " + to_string(boardid) + " id doesn't exist!");
    eosio::check(lb_itr->owner == owner, "You are not owner of current nestcontract.");

    vector<double> v; 
 
    while (!values.empty()) { 
        string substr; 
        size_t pos = values.find(',');
        substr = values.substr(0,pos);
        v.push_back(stoi(substr)); 
        values.erase(0,pos+1);
    } 
  

    _lb.modify(lb_itr, CONTRACTN, [&](auto &mod_lb) {
        for (int i = 0; i < v.size(); i++)
        {
            mod_lb.prize.values.push_back(v.at(i));
        }
        mod_lb.prize.mode = mode;
    });
}

//action for removing nestcontract from table
//owner - owner wax account
//boardid - nestcontract id on table lboards
//
void nestcontract::removelboard(uint64_t boardid, eosio::name owner)
{
    require_auth(owner);

    lboards _lb(CONTRACTN, CONTRACTN.value);

    auto lb_itr = _lb.find(boardid);
    eosio::check(lb_itr != _lb.end(), "Leader board with " + to_string(boardid) + " id doesn't exist!");
    eosio::check(lb_itr->owner == owner, "You are not owner of current nestcontract.");

    _lb.erase(lb_itr);
}

//action for reseting nestcontract
//owner - owner wax account
//boardid - nestcontract id on table lboards
//resetpool - 0: pot on nestcontract stay current, 1: pot on nestcontract change to 0
//
void nestcontract::resetlboard(eosio::name owner, uint64_t boardid, bool resetpool)
{
    require_auth(owner);

    lboards _lb(CONTRACTN, CONTRACTN.value);

    auto lb_itr = _lb.find(boardid);
    eosio::check(lb_itr != _lb.end(), "Leader board with " + to_string(boardid) + " id doesn't exist!");
    eosio::check(lb_itr->owner == owner, "You are not owner of current nestcontract.");

    _lb.modify(lb_itr, get_self(), [&](auto &mod_board) {
        mod_board.players.clear();
        if (resetpool == 1) 
        mod_board.pot = 0.0;
    });
}

//action for update/create user on nestcontract
//owner - owner wax account
//boardid - nestcontract id on table lboards
//username - user wax account
//point - number of points to add
//data - user data on JSON format
//
void nestcontract::update(eosio::name owner, uint64_t boardid, eosio::name username, double points, string data)
{
    require_auth(owner);

    lboards _lb(CONTRACTN, CONTRACTN.value);

    auto lb_itr = _lb.find(boardid);
    eosio::check(lb_itr != _lb.end(), "Leader board with " + to_string(boardid) + " id doesn't exist!");
    eosio::check(lb_itr->owner == owner, "You are not owner of current nestcontract.");

    uint64_t pos = finder(lb_itr->players, username);
    if (pos == -1)
    {
        player_s helper;
        pos = lb_itr->players.size();
        _lb.modify(lb_itr, get_self(), [&](auto &mod_board) {
            mod_board.players.push_back({
                username,
                points,
                data,
            });
            while (pos != 0)
            {
                if (mod_board.players.at(pos).points > mod_board.players.at(pos - 1).points)
                {
                    helper = mod_board.players.at(pos);
                    mod_board.players.at(pos) = mod_board.players.at(pos - 1);
                    mod_board.players.at(pos - 1) = helper;
                }
                else
                {
                    break;
                }
            }
        });
    }
    else
    {
        player_s helper;
        _lb.modify(lb_itr, get_self(), [&](auto &mod_board) {
            mod_board.players.at(pos).points += points;
            while (pos != 0)
            {
                if (mod_board.players.at(pos).points > mod_board.players.at(pos - 1).points)
                {
                    helper = mod_board.players.at(pos);
                    mod_board.players.at(pos) = mod_board.players.at(pos - 1);
                    mod_board.players.at(pos - 1) = helper;
                }
                else
                {
                    break;
                }
            }
        });
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        GAME ACTIONS
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

ACTION nestcontract::gameregister(eosio::name  owner, std::string gameurl, std::vector<std::string> keywords, std::string description, std::vector<std::string> screenshots, 
                    std::vector<std::string> videos, std::vector<std::string> promscrshots, eosio::asset price, std::string realisedate, std::string data)
{
    require_auth(owner);

    eosio::check(!screenshots.empty(),"Must be at least one game screenshot");


    apprgames _apprgames(CONTRACTN,CONTRACTN.value);

    _apprgames.emplace(CONTRACTN, [&](auto& new_game)
    {
        new_game.id = getgameid();
        new_game.owner = owner;
        new_game.gameurl = gameurl;
        new_game.keywords = keywords;
        new_game.description = description;
        new_game.screenshots = screenshots;
        new_game.videos = videos;
        new_game.promscrshots = promscrshots;
        new_game.price = price;
        new_game.realisedate = realisedate;
        new_game.data = data;
    });
}


ACTION nestcontract::gameupdate(uint64_t gameid, eosio::name  owner, std::string gameurl, std::vector<std::string> keywords, std::string description, std::vector<std::string> screenshots, 
                    std::vector<std::string> videos, std::vector<std::string> promscrshots, eosio::asset price, std::string realisedate, std::string data)
{
    require_auth(owner);

    apprgames _apprgames(CONTRACTN,CONTRACTN.value);
    auto app_itr = _apprgames.find(gameid);

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist");
    eosio::check(game_itr->owner == owner,"You are not OWNER of current game.");

    if(app_itr == _apprgames.end())
    {
        _apprgames.emplace(CONTRACTN, [&](auto& new_game)
        {
            new_game.id = gameid;
            new_game.owner = owner;
            new_game.gameurl = gameurl;
            new_game.keywords = keywords;
            new_game.description = description;
            new_game.screenshots = screenshots;
            new_game.videos = videos;
            new_game.promscrshots = promscrshots;
            new_game.price = price;
            new_game.realisedate = realisedate;
            new_game.data = data;
        });
    }
    else
    {
        _apprgames.modify(app_itr,CONTRACTN, [&](auto& mod_game)
        {
            mod_game.id = gameid;
            mod_game.owner = owner;
            mod_game.gameurl = gameurl;
            mod_game.keywords = keywords;
            mod_game.description = description;
            mod_game.screenshots = screenshots;
            mod_game.videos = videos;
            mod_game.promscrshots = promscrshots;
            mod_game.price = price;
            mod_game.realisedate = realisedate;
            mod_game.data = data;
        });
    }
}


ACTION nestcontract::gameapprove(uint64_t appgameid, bool approve)
{
    require_auth(CONTRACTN);

    apprgames _apprgames(CONTRACTN,CONTRACTN.value);
    auto app_itr = _apprgames.find(appgameid);
    eosio::check(app_itr != _apprgames.end(),"Request doesn't exist.");

    if(approve)
    {
        games _games(CONTRACTN,CONTRACTN.value);
        auto game_itr = _games.find(appgameid);
        if(game_itr == _games.end())
        {
            _games.emplace(CONTRACTN, [&](auto& new_game)
            {
                new_game.id = app_itr->id;
                new_game.owner = app_itr->owner;
                new_game.gameurl = app_itr->gameurl;
                new_game.keywords = app_itr->keywords;
                new_game.description = app_itr->description;
                new_game.screenshots = app_itr->screenshots;
                new_game.videos = app_itr->videos;
                new_game.promscrshots = app_itr->promscrshots;
                new_game.price = app_itr->price;
                new_game.realisedate = app_itr->realisedate;
                new_game.data = app_itr->data;
            });
        }
        else
        {
            _games.modify(game_itr, CONTRACTN, [&](auto& mod_game)
            {
                mod_game.id = app_itr->id;
                mod_game.owner = app_itr->owner;
                mod_game.gameurl = app_itr->gameurl;
                mod_game.keywords = app_itr->keywords;
                mod_game.description = app_itr->description;
                mod_game.screenshots = app_itr->screenshots;
                mod_game.videos = app_itr->videos;
                mod_game.promscrshots = app_itr->promscrshots;
                mod_game.price = app_itr->price;
                mod_game.realisedate = app_itr->realisedate;
                mod_game.data = app_itr->data;
            });
        }

    }
    _apprgames.erase(app_itr);
}






/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        USER ACTIONS
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

ACTION nestcontract::usercreate(eosio::name username)
{
    require_auth(CONTRACTN);

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr == _users.end(),"Current user exists.");

    _users.emplace(CONTRACTN,[&](auto& new_user){
        new_user.account = username;
    });
}

ACTION nestcontract::userregister(eosio::name username,std::string avatar, std::string nickname)
{
    require_auth(username);

    users _users(CONTRACTN,CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr == _users.end(),"Current user was registered.");

    _users.emplace(CONTRACTN,[&](auto& new_user){
        new_user.account = username;
    });

    apprusers _apprusers(CONTRACTN,CONTRACTN.value);
    auto appuser_itr = _apprusers.find(username.value);
    eosio::check(appuser_itr == _apprusers.end(),"Current user was registered.");

    _apprusers.emplace(CONTRACTN,[&](auto& new_user){
        new_user.account = username;
        new_user.avatar = avatar;
        new_user.nickname = nickname;
    });
}

ACTION nestcontract::userupdate(eosio::name username,std::string avatar, std::string nickname)
{
    require_auth(username);

    users _users(CONTRACTN,CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr != _users.end(),"Current user wasn't registered.");

    apprusers _apprusers(CONTRACTN,CONTRACTN.value);
    auto appuser_itr = _apprusers.find(username.value);
    if(appuser_itr == _apprusers.end())
    {
        _apprusers.emplace(CONTRACTN,[&](auto& new_appu)
        {
            new_appu.account = username;
            new_appu.avatar = avatar;
            new_appu.nickname = nickname;
        });
    }
    else
    {
        _apprusers.modify(appuser_itr,CONTRACTN,[&](auto& mod_appu)
        {
            mod_appu.account = username;
            mod_appu.avatar = avatar;
            mod_appu.nickname = nickname;
        });
    }
}

ACTION nestcontract::userapprove(eosio::name username, bool approve)
{
    require_auth(CONTRACTN);

    apprusers _apprusers(CONTRACTN,CONTRACTN.value);
    auto appuser_itr = _apprusers.find(username.value);
    eosio::check(appuser_itr == _apprusers.end(),"Current user has no data to approve.");
    
    users _users(CONTRACTN,CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    if(approve)
    {    
        if(user_itr == _users.end())
        {
            _users.emplace(CONTRACTN,[&](auto& new_user){
                new_user.account = appuser_itr->account;
                new_user.avatar = appuser_itr->avatar;
                new_user.nickname = appuser_itr->nickname;
            });
        }
        else
        {
            _users.modify(user_itr, CONTRACTN,[&](auto& mod_user){
                mod_user.avatar = appuser_itr->avatar;
                mod_user.nickname = appuser_itr->nickname;
            });
        }
    }

    _apprusers.erase(appuser_itr);

}

ACTION nestcontract::addfriend(eosio::name username, eosio::name newfriend)
{
    require_auth(username);

    users _users(CONTRACTN,CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr == _users.end(),"Current user doesn't exist.");
    eosio::check(!user_itr->nickname.empty() && !user_itr->avatar.empty(),"User must be setting up.");

    auto friend_itr = _users.find(newfriend.value);
    eosio::check(friend_itr == _users.end(),"Friend user doesn't exist.");

    int friend_pos = finder(user_itr->friends, newfriend);
    eosio::check(friend_pos == -1,"User is on your friends' list");
    _users.modify(user_itr, CONTRACTN, [&](auto& mod_user){
        mod_user.friends.push_back(newfriend);
    });
}

ACTION nestcontract::deletefriend(eosio::name username, eosio::name delfriend)
{
    require_auth(username);

    users _users(CONTRACTN,CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr == _users.end(),"Current user doesn't exist.");

    auto friend_itr = _users.find(delfriend.value);
    eosio::check(friend_itr == _users.end(),"Friend user doesn't exist.");

    int friend_pos = finder(user_itr->friends, delfriend);
    eosio::check(friend_pos != -1,"User isn't on your friends' list");
    _users.modify(user_itr, CONTRACTN, [&](auto& mod_user){
        mod_user.friends.erase(mod_user.friends.begin() + friend_pos);
    });
}



////////////////////////////////////////////////////////////////////////////////
//
//        CARDS ACTIONS
//
////////////////////////////////////////////////////////////////////////////////
ACTION nestcontract::cardcreate(eosio::name owner, uint64_t gameid, std::string cardname, std::string enableimg, std::string disableimg, std::string series)
{
    require_auth(owner);

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner,"You aren't the owner of the current game.");

    apprcards _apprcards(CONTRACTN,CONTRACTN.value);
    _apprcards.emplace(CONTRACTN,[&](auto& new_card){
        new_card.id = getcardid();
        new_card.gameid = gameid;
        new_card.cardname = cardname;
        new_card.enableimg = enableimg;
        new_card.disableimg = disableimg;
        new_card.series = series;
    });
}

ACTION nestcontract::carddelete(eosio::name owner, uint64_t cardid)
{
    require_auth(owner);

    cards _cards(CONTRACTN,CONTRACTN.value);
    auto card_itr = _cards.find(cardid);
    eosio::check(card_itr != _cards.end(),"Card with current id doesn't exist.");

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(card_itr->gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner,"You aren't the owner of current card.");

    _cards.erase(card_itr);
}

ACTION nestcontract::carddrop(eosio::name owner, uint64_t cardid, eosio::name username)
{
    require_auth(owner);

    cards _cards(CONTRACTN,CONTRACTN.value);
    auto card_itr = _cards.find(cardid);
    eosio::check(card_itr != _cards.end(),"Card with current id doesn't exist.");

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(card_itr->gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner,"You aren't the owner of current card.");

    users _users(CONTRACTN,CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr == _users.end(),"Current user doesn't exist.");

    usercards _usercards(CONTRACTN,CONTRACTN.value);
    auto usercard_itr = _usercards.find(username.value);
    if(usercard_itr == _usercards.end())
    {
        _usercards.emplace(CONTRACTN, [&](auto& new_record){
            new_record.username = username;
            new_record.cardids.push_back(cardid);
        });
    }
    else
    {
        _usercards.modify(usercard_itr, CONTRACTN, [&](auto& mod_record){
            mod_record.cardids.push_back(cardid);
        });
    }

    std::string idata = "{\"name\":\"" + card_itr->cardname + "\", \"gameid\":" + std::to_string(card_itr->gameid) +", \"series\":\"" + card_itr->series + "\" }";
    std::string mdata = "{\"img\": \""+ card_itr->enableimg +"\" }";

    eosio::action createAsset = eosio::action(
	    eosio::permission_level{CONTRACTN, eosio::name("active")},
	    SIMPLEASSETSCONTRACT,
	    eosio::name("create"),
	    std::make_tuple( CONTRACTN, eosio::name("card"), owner, idata, mdata,	0 )
    );
    createAsset.send();	
}

ACTION nestcontract::cardapprove(uint64_t cardid, bool approve)
{
    require_auth(CONTRACTN);

    apprcards _apprcards(CONTRACTN,CONTRACTN.value);
    auto apprcard_itr = _apprcards.find(cardid);
    eosio::check(apprcard_itr != _apprcards.end(),"Current card request doesn't exist");

    if(approve)
    {
        cards _cards(CONTRACTN,CONTRACTN.value);
        auto card_itr = _cards.find(cardid);
        eosio::check(card_itr == _cards.end(),"Current card exists.");
        _cards.emplace(CONTRACTN,[&](auto& new_card){
            new_card.id = apprcard_itr->id;
            new_card.gameid = apprcard_itr->gameid;
            new_card.cardname = apprcard_itr->cardname;
            new_card.enableimg = apprcard_itr->enableimg;
            new_card.disableimg = apprcard_itr->disableimg;
            new_card.series = apprcard_itr->series;
        });
        

        gamecards _gamecards(CONTRACTN,CONTRACTN.value);
        auto gamecard_itr = _gamecards.find(apprcard_itr->gameid);
        if(gamecard_itr == _gamecards.end())
        {
            _gamecards.emplace(CONTRACTN, [&](auto& new_record)
            {
                new_record.gameid = apprcard_itr->gameid;
                new_record.series.push_back({apprcard_itr->series,{apprcard_itr->id}});
            });
        }
        else
        {
            int pos = finder(gamecard_itr->series, apprcard_itr->series);
            _gamecards.modify(gamecard_itr, CONTRACTN, [&](auto& mod_record){
                if(pos == -1)
                {
                    mod_record.series.push_back({apprcard_itr->series,{apprcard_itr->id}});
                }
                else
                {
                    mod_record.series.at(pos).cards.push_back(apprcard_itr->id);
                }
            });
        }
    }
    _apprcards.erase(apprcard_itr);
}


////////////////////////////////////////////////////////////////////////////////
//
//        LEVELS ACTIONS
//
////////////////////////////////////////////////////////////////////////////////

ACTION nestcontract::lvlcreate(uint64_t level, uint64_t xp)
{
    require_auth(CONTRACTN);

    levels _levels(CONTRACTN,CONTRACTN.value);
    auto itr = _levels.find(level);
    eosio::check(itr == _levels.end(),"Level exists.");
    _levels.emplace(CONTRACTN, [&](auto& new_lvl){
        new_lvl.lvl = level;
        new_lvl.points = xp;
    });
}

ACTION nestcontract::lvlupdate(uint64_t level, uint64_t xp)
{
    require_auth(CONTRACTN);

    levels _levels(CONTRACTN,CONTRACTN.value);
    auto itr = _levels.find(level);
    eosio::check(itr != _levels.end(),"Level doesn't exist.");
    _levels.modify(itr, CONTRACTN, [&](auto& new_lvl){
        new_lvl.points = xp;
    });
}
    
ACTION nestcontract::lvlearn(eosio::name username)
{
    require_auth(CONTRACTN);

    users _users(CONTRACTN,CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr != _users.end(),"User doesn't exist.");

    levels _levels(CONTRACTN,CONTRACTN.value);
    auto lvl_itr = _levels.find(user_itr->level);
    eosio::check(lvl_itr != _levels.end(),"Level doesn't exist");
    uint64_t new_lvl;
    while(user_itr->xp >= lvl_itr->points)
    {
        new_lvl = lvl_itr->lvl;
        lvl_itr++;
        if(lvl_itr == _levels.end())
        {
            break;
        }
    }

    _users.modify(user_itr, CONTRACTN, [&](auto& mod_user)
    {
        mod_user.level = new_lvl;
    });
}

ACTION nestcontract::lvladdxp(eosio::name username, uint64_t xp)
{
    require_auth(CONTRACTN);

    users _users(CONTRACTN,CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr != _users.end(),"User doesn't exist.");

    levels _levels(CONTRACTN,CONTRACTN.value);
    auto lvl_itr = _levels.find(user_itr->level);
    eosio::check(lvl_itr != _levels.end(),"Level doesn't exist");

    uint64_t new_lvl, points = user_itr->xp;
    points += xp;
    while(points >= lvl_itr->points)
    {
        new_lvl = lvl_itr->lvl;
        lvl_itr++;
        if(lvl_itr == _levels.end())
        {
            break;
        }
    }

    _users.modify(user_itr, CONTRACTN, [&](auto& mod_user)
    {
        mod_user.level = new_lvl;
        mod_user.xp += xp;
    });
}


////////////////////////////////////////////////////////////////////////////////
//
//        BADGES ACTIONS
//
////////////////////////////////////////////////////////////////////////////////

ACTION nestcontract::badgecreate(eosio::name owner, uint64_t gameid, std::string badgename, std::vector<uint64_t> cardids)
{
    require_auth(owner);

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner,"You aren't the owner of current game.");

    cards _cards(CONTRACTN,CONTRACTN.value);
    for(uint8_t i=0;i<cardids.size();i++)
    {
        auto card_itr = _cards.find(cardids.at(i));
        eosio::check(card_itr != _cards.end(),"Card doesn't exist.");
    }

    apprbadges _apprbadge(CONTRACTN,CONTRACTN.value);
    _apprbadge.emplace(CONTRACTN, [&](auto& new_badge){
        new_badge.id = getbadgeid();
        new_badge.gameid = gameid;
        new_badge.badgename = badgename;
        new_badge.cardids = cardids;
    });
}

ACTION nestcontract::badgedelete(eosio::name owner, uint64_t badgeid)
{
    require_auth(owner);

    badges _badges(CONTRACTN,CONTRACTN.value);
    auto badge_itr = _badges.find(badgeid);
    eosio::check(badge_itr != _badges.end(),"Badge with current id doesn't exist.");
    eosio::check(badge_itr->candelete == 1,"Badge can't be deleted.");

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(badge_itr->gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner,"You aren't the owner of current game.");

    _badges.erase(badge_itr);
}

ACTION nestcontract::badgeapprove(uint64_t badgeid, bool approve)
{
    require_auth(CONTRACTN);

    apprbadges _apprbadge(CONTRACTN,CONTRACTN.value);
    auto apprbadge_itr = _apprbadge.find(badgeid);
    eosio::check(apprbadge_itr != _apprbadge.end(),"Current badge request don't exist");

    if(approve)
    {
        badges _badges(CONTRACTN,CONTRACTN.value);
        auto badge_itr = _badges.find(badgeid);
        eosio::check(badge_itr == _badges.end(),"Badge with current id doesn't exist.");
        _badges.emplace(CONTRACTN,[&](auto& new_badge){
            new_badge.id = badgeid;
            new_badge.gameid = apprbadge_itr->gameid;
            new_badge.badgename = apprbadge_itr->badgename;
            new_badge.cardids = apprbadge_itr->cardids;
        });
        

        games _games(CONTRACTN,CONTRACTN.value);
        auto game_itr = _games.find(apprbadge_itr->gameid);
        eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");

        _games.modify(game_itr,CONTRACTN,[&](auto& mod_game){
            mod_game.badges.push_back(badgeid);
        });

    }
    _apprbadge.erase(apprbadge_itr);
}
  
ACTION nestcontract::badgeearn(eosio::name username, uint64_t badgeid)
{
    require_auth(CONTRACTN);

    badges _badges(CONTRACTN,CONTRACTN.value);
    auto badge_itr = _badges.find(badgeid);
    eosio::check(badge_itr != _badges.end(),"Badge with current id doesn't exist.");

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(badge_itr->gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr != _users.end(),"User doesn't exist.");
    eosio::check(!user_itr->nickname.empty() && !user_itr->avatar.empty(),"User must be setting up.");

    usercards _usercards(CONTRACTN, CONTRACTN.value);
    auto usercard_itr = _usercards.find(username.value);
    eosio::check(usercard_itr != _usercards.end(),"User doesn't exist.");

    bool is_badge = true;
    for(uint8_t i; i < badge_itr->cardids.size(); i++)
    {
        int pos = finder(usercard_itr->cardids,  badge_itr->cardids.at(i));
        if(pos == -1)
        {
            is_badge = false;
            break;
        }
    }

    eosio::check(is_badge == true,"User hasn't all cards from this badge.");

    levels _levels(CONTRACTN,CONTRACTN.value);
    auto lvl_itr = _levels.find(user_itr->level);
    eosio::check(lvl_itr != _levels.end(),"Level doesn't exist");

    uint64_t xp = 100;

    uint64_t new_lvl, points = user_itr->xp;
    points += xp;
    while(points >= lvl_itr->points)
    {
        new_lvl = lvl_itr->lvl;
        lvl_itr++;
        if(lvl_itr == _levels.end())
        {
            break;
        }
    }

    _users.modify(user_itr, CONTRACTN,[&](auto& mod_user){
        mod_user.level = new_lvl;
        mod_user.xp += xp;
    });
}




////////////////////////////////////////////////////////////////////////////////
//
//        ACHIEVEMENTS ACTIONS
//
////////////////////////////////////////////////////////////////////////////////

ACTION nestcontract::achievcreate(eosio::name owner, uint64_t gameid, std::string achievname, std::string description, std::string image, std::string rarity)
{
    require_auth(owner);

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner,"You aren't the owner of current game.");

    appracheivs _appracheivs(CONTRACTN,CONTRACTN.value);
    _appracheivs.emplace(CONTRACTN, [&](auto& new_achiev){
        new_achiev.id = getachievid();
        new_achiev.gameid = gameid;
        new_achiev.achievname = achievname;
        new_achiev.description = description;
        new_achiev.image = image;
        new_achiev.rarity = rarity;
    });
}

ACTION nestcontract::achievdelete(eosio::name owner, uint64_t achieveid)
{
    require_auth(owner);

    achievements _achievements(CONTRACTN,CONTRACTN.value);
    auto achiev_itr = _achievements.find(achieveid);
    eosio::check(achiev_itr == _achievements.end(),"Achievement with current id exists.");
    eosio::check(achiev_itr->candelete == 1,"Achievement can't be deleted.");

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(achiev_itr->gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner,"You aren't the owner of current game.");

    _achievements.erase(achiev_itr);
}

ACTION nestcontract::achievearn(eosio::name owner, eosio::name username, uint64_t achieveid)
{
    require_auth(owner);

    achievements _achievements(CONTRACTN,CONTRACTN.value);
    auto achiev_itr = _achievements.find(achieveid);
    eosio::check(achiev_itr != _achievements.end(),"Achievement with current id exists.");

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(achiev_itr->gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner,"You aren't the owner of current game.");

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr != _users.end(),"User doesn't exist.");
    eosio::check(!user_itr->nickname.empty() && !user_itr->avatar.empty(),"User must be setting up.");

    int game_pos = finder(user_itr->achievements, achiev_itr->gameid);

    levels _levels(CONTRACTN,CONTRACTN.value);
    auto lvl_itr = _levels.find(user_itr->level);
    eosio::check(lvl_itr != _levels.end(),"Level doesn't exist");

    uint64_t xp = get_achiev_points(achiev_itr->rarity);

    uint64_t new_lvl, points = user_itr->xp;
    points += xp;
    while(points >= lvl_itr->points)
    {
        new_lvl = lvl_itr->lvl;
        lvl_itr++;
        if(lvl_itr == _levels.end())
        {
            break;
        }
    }

    _users.modify(user_itr, CONTRACTN,[&](auto& mod_user){
        if(game_pos == -1)
        {
            mod_user.achievements.push_back({achiev_itr->gameid,static_cast<uint8_t>(game_itr->achievements.size()),{achieveid}});
        }
        else
        {
            eosio::check(finder(mod_user.achievements,achieveid)==-1,"User has current achievement.");
            mod_user.achievements.at(static_cast<uint64_t>(game_pos)).achievs.push_back(achieveid);
        }
        mod_user.level = new_lvl;
        mod_user.xp += xp;
    });
}

ACTION nestcontract::achievapprove(uint64_t achieveid, bool approve)
{
    require_auth(CONTRACTN);

    appracheivs _appracheivs(CONTRACTN,CONTRACTN.value);
    auto appracheiv_itr = _appracheivs.find(achieveid);
    eosio::check(appracheiv_itr != _appracheivs.end(),"Achievement request doesn't exist.");

    games _games(CONTRACTN,CONTRACTN.value);
    auto game_itr = _games.find(appracheiv_itr->gameid);
    eosio::check(game_itr != _games.end(),"Game with current id doesn't exist.");

    if(approve)
    {
        achievements _achievements(CONTRACTN,CONTRACTN.value);
        auto achiev_itr = _achievements.find(achieveid);
        eosio::check(achiev_itr == _achievements.end(),"Achievement with current id exists.");

        _achievements.emplace(CONTRACTN, [&](auto& new_achiev){
            new_achiev.id = appracheiv_itr->id;
            new_achiev.gameid = appracheiv_itr->gameid;
            new_achiev.achievname = appracheiv_itr->achievname;
            new_achiev.description = appracheiv_itr->description;
            new_achiev.image = appracheiv_itr->image;
            new_achiev.rarity = appracheiv_itr->rarity;
        });
    }

    _appracheivs.erase(appracheiv_itr);
}



////////////////////////////////////////////////////////////////////////////////
//        HELP FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

int nestcontract::finder(vector<player_s> players, eosio::name username)
{
    for (uint64_t i = 0; i < players.size(); i++)
    {
        if (players.at(i).account == username)
        {
            return i;
        }
    }
    return -1;
}


int nestcontract::finder(vector<eosio::name> friends, eosio::name username)
{
    for (uint64_t i = 0; i < friends.size(); i++)
    {
        if (friends.at(i) == username)
        {
            return i;
        }
    }
    return -1;
}

int nestcontract::finder(std::vector<achiev_s> achieves, uint64_t gameid)
{
    for (uint64_t i = 0; i < achieves.size(); i++)
    {
        if (achieves.at(i).gameid == gameid)
        {
            return i;
        }
    }
    return -1;
}

int nestcontract::finder(std::vector<uint64_t> ids, uint64_t id)
{
    for (uint64_t i = 0; i < ids.size(); i++)
    {
        if (ids.at(i) == id)
        {
            return i;
        }
    }
    return -1;
}

int nestcontract::finder(std::vector<series_s> series, std::string ser_name)
{
    for (uint64_t i = 0; i < series.size(); i++)
    {
        if (series.at(i).series == ser_name)
        {
            return i;
        }
    }
    return -1;
}

uint64_t nestcontract::get_achiev_points(std::string rarity)
{
    return 100;
}

uint64_t nestcontract::getcardid() {

	conf config(CONTRACTN, CONTRACTN.value);
	_cstate = config.exists() ? config.get() : global{};
	_cstate.cardid++;
	config.set(_cstate, CONTRACTN);

	return _cstate.cardid;
}
uint64_t nestcontract::getbadgeid() {

	conf config(CONTRACTN, CONTRACTN.value);
	_cstate = config.exists() ? config.get() : global{};
	_cstate.badgeid++;
	config.set(_cstate, CONTRACTN);

	return _cstate.badgeid;
}
uint64_t nestcontract::getachievid() {

	conf config(CONTRACTN, CONTRACTN.value);
	_cstate = config.exists() ? config.get() : global{};
	_cstate.achievid++;
	config.set(_cstate, CONTRACTN);

	return _cstate.achievid;
}
uint64_t nestcontract::getgameid() {

	conf config(CONTRACTN, CONTRACTN.value);
	_cstate = config.exists() ? config.get() : global{};
	_cstate.gameid++;
	config.set(_cstate, CONTRACTN);

	return _cstate.gameid;
}
uint64_t nestcontract::getlboardid() {

	conf config(CONTRACTN, CONTRACTN.value);
	_cstate = config.exists() ? config.get() : global{};
	_cstate.lboardid++;
	config.set(_cstate, CONTRACTN);

	return _cstate.lboardid;
}


extern "C" void apply(uint64_t receiver, uint64_t code, uint64_t action)
{
  if (code == receiver)
  {
    switch (action)
    {
      EOSIO_DISPATCH_HELPER(nestcontract, (createlboard)(createprize)(resetlboard)(removelboard)(update)
                                            (gameregister)(gameupdate)(gameapprove)
                                            (usercreate)(userregister)(userupdate)(userapprove)(addfriend)(deletefriend)
                                            (achievcreate)(achievdelete)(achievearn)(achievapprove)
                                            (cardcreate)(carddelete)(carddrop)(cardapprove)
                                            (badgecreate)(badgedelete)(badgeapprove)(badgeearn)
                                            (lvlcreate)(lvlupdate)(lvlearn)(lvladdxp))
    }
  }
  else
  {
    printf("Couldn't find an action.");
    return;
  }
}
