#include <nestplatform.hpp>
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        nestplatform ACTIONS
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

//action for creating new leader board
//owner - owner wax account
//boardname - name for nestplatform
//gameid - id of owner's game
//
void nestplatform::createlboard(eosio::name owner, string boardname, uint64_t gameid, double startpoints)
{
    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist");

    require_auth(game_itr->owner);



    lboards _lb(CONTRACTN, CONTRACTN.value);

    uint64_t boardid{0};

    auto itr = _lb.end();
    if (itr == _lb.begin())
    {
        boardid = 0;
    }
    else
    {
        itr--;
        boardid = itr->id;
        boardid++;
    }

    _lb.emplace(CONTRACTN, [&](auto &new_board) {
        new_board.id = boardid;
        new_board.owner = owner;
        new_board.boardname = boardname;
        new_board.gameid = gameid;
    });

    if(startpoints != 0.0)
    {
        startlpoints _startpoints(CONTRACTN, CONTRACTN.value);
        auto points_itr = _startpoints.find(boardid);
        eosio::check(points_itr == _startpoints.end(),"Points for this board exists.");
        
        _startpoints.emplace(CONTRACTN, [&](auto& new_points){
            new_points.boardid = boardid;
            new_points.startpoints = startpoints;
        });
    }
}

void nestplatform::changebpoint(uint64_t boardid, double startpoints)
{

    lboards _lb(CONTRACTN, CONTRACTN.value);

    auto lb_itr = _lb.find(boardid);
    eosio::check(lb_itr != _lb.end(), "Leader board with " + to_string(boardid) + " id doesn't exist!");

    require_auth(lb_itr->owner);

    startlpoints _startpoints(CONTRACTN, CONTRACTN.value);
    auto points_itr = _startpoints.find(boardid);
    if(points_itr == _startpoints.end())
    {    
        _startpoints.emplace(CONTRACTN, [&](auto& new_points){
            new_points.boardid = boardid;
            new_points.startpoints = startpoints;
        });
    }
    else
    {
        _startpoints.modify(points_itr, CONTRACTN, [&](auto& mod_points){
            mod_points.startpoints = startpoints;
        });
    }
}

//action for creating prize on nestplatform
//owner - owner wax account
//boardid - nestplatform id on table lboards
//mode - prize paments mode: 0 - absolute numbers, 1 - percents
//values - string with payments values for top users on nestplatform
//vlues example - 100,50,40,
//
void nestplatform::createprize(uint64_t boardid, uint8_t mode, string values)
{

    lboards _lb(CONTRACTN, CONTRACTN.value);

    auto lb_itr = _lb.find(boardid);
    eosio::check(lb_itr != _lb.end(), "Leader board with " + to_string(boardid) + " id doesn't exist!");

    require_auth(lb_itr->owner);

    vector<double> v;

    while (!values.empty())
    {
        string substr;
        size_t pos = values.find(',');
        substr = values.substr(0, pos);
        v.push_back(stoi(substr));
        values.erase(0, pos + 1);
    }

    _lb.modify(lb_itr, CONTRACTN, [&](auto &mod_lb) {
        for (int i = 0; i < v.size(); i++)
        {
            mod_lb.prize.values.push_back(v.at(i));
        }
        mod_lb.prize.mode = mode;
    });
}

//action for removing nestplatform from table
//owner - owner wax account
//boardid - nestplatform id on table lboards
//
void nestplatform::removelboard(uint64_t boardid)
{
    lboards _lb(CONTRACTN, CONTRACTN.value);

    auto lb_itr = _lb.find(boardid);
    eosio::check(lb_itr != _lb.end(), "Leader board with " + to_string(boardid) + " id doesn't exist!");

    require_auth(lb_itr->owner);

    _lb.erase(lb_itr);
}

//action for reseting nestplatform
//owner - owner wax account
//boardid - nestplatform id on table lboards
//resetpool - 0: pot on nestplatform stay current, 1: pot on nestplatform change to 0
//
void nestplatform::resetlboard(uint64_t boardid, bool resetpool)
{
    lboards _lb(CONTRACTN, CONTRACTN.value);

    auto lb_itr = _lb.find(boardid);
    eosio::check(lb_itr != _lb.end(), "Leader board with " + to_string(boardid) + " id doesn't exist!");

    require_auth(lb_itr->owner);

    _lb.modify(lb_itr, get_self(), [&](auto &mod_board) {
        mod_board.players.clear();
        if (resetpool == 1)
            mod_board.pot = 0.0;
    });
}

//action for update/create user on nestplatform
//owner - owner wax account
//boardid - nestplatform id on table lboards
//username - user wax account
//point - number of points to add
//data - user data on JSON format
//
void nestplatform::update(uint64_t boardid, eosio::name username, double points, string data)
{
    lboards _lb(CONTRACTN, CONTRACTN.value);

    auto lb_itr = _lb.find(boardid);
    eosio::check(lb_itr != _lb.end(), "Leader board with " + to_string(boardid) + " id doesn't exist!");

    require_auth(lb_itr->owner);

    double totalpoints = 0.0;

    startlpoints _startpoints(CONTRACTN, CONTRACTN.value);
    auto points_itr = _startpoints.find(boardid);
    if(points_itr != _startpoints.end())
    totalpoints = points_itr->startpoints;

    uint64_t pos = finder(lb_itr->players, username);
    if (pos == -1)
    {
        player_s helper;
        pos = lb_itr->players.size();
        _lb.modify(lb_itr, get_self(), [&](auto &mod_board) {
            mod_board.players.push_back({
                username,
                (points+totalpoints),
                data,
            });
            while (pos != 0)
            {
                if (mod_board.players.at(pos).points >= mod_board.players.at(pos - 1).points)
                {
                    helper = mod_board.players.at(pos);
                    mod_board.players.at(pos) = mod_board.players.at(pos - 1);
                    mod_board.players.at(pos - 1) = helper;
                }
                else
                {
                    break;
                }
                pos--;
            }
        });
    }
    else
    {
        player_s helper;
        _lb.modify(lb_itr, get_self(), [&](auto &mod_board) {
            mod_board.players.at(pos).points += (points);
            mod_board.players.at(pos).data = data;
            while (pos != 0)
            {
                if (mod_board.players.at(pos).points >= mod_board.players.at(pos - 1).points)
                {
                    helper = mod_board.players.at(pos);
                    mod_board.players.at(pos) = mod_board.players.at(pos - 1);
                    mod_board.players.at(pos - 1) = helper;
                }
                else
                {
                    break;
                }
                pos--;
            }
        });
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        GAME ACTIONS
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

ACTION nestplatform::gameregister(eosio::name owner, std::string gameurl, std::vector<std::string> keywords, std::string description, std::vector<std::string> screenshots,
                                  std::vector<std::string> videos, std::vector<std::string> promscrshots, eosio::asset price, std::string releasedate, std::string data)
{
    require_auth(owner);

    eosio::check(!screenshots.empty(), "Must be at least one game screenshot");

    apprgames _apprgames(CONTRACTN, CONTRACTN.value);

    _apprgames.emplace(CONTRACTN, [&](auto &new_game) {
        new_game.id = getgameid();
        new_game.owner = owner;
        new_game.gameurl = gameurl;
        new_game.keywords = keywords;
        new_game.description = description;
        new_game.screenshots = screenshots;
        new_game.videos = videos;
        new_game.promscrshots = promscrshots;
        new_game.price = price;
        new_game.releasedate = releasedate;
        new_game.data = data;
    });
}

ACTION nestplatform::gameupdate(uint64_t gameid, eosio::name new_owner, std::string gameurl, std::vector<std::string> keywords, std::string description, std::vector<std::string> screenshots,
                                std::vector<std::string> videos, std::vector<std::string> promscrshots, eosio::asset price, std::string releasedate, std::string data)
{
    apprgames _apprgames(CONTRACTN, CONTRACTN.value);
    auto app_itr = _apprgames.find(gameid);

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist");

    (has_auth(game_itr->owner)) ? require_auth(game_itr->owner) : require_auth(CONTRACTN);

    std::vector<std::string> help_array;

    if (app_itr == _apprgames.end())
    {
        _apprgames.emplace(CONTRACTN, [&](auto &new_game) {
            new_game.id = gameid;
            if(game_itr->owner != new_owner && !new_owner.to_string().empty())
                new_game.owner = new_owner;
            else
                new_game.owner = game_itr->owner;
            if(game_itr->gameurl != gameurl)
                new_game.gameurl = gameurl;
            else
                new_game.gameurl = game_itr->gameurl;
            if(!keywords.empty())
                new_game.keywords = keywords;
            else
                new_game.keywords = game_itr->keywords;
            if(game_itr->description != description && !description.empty())
                new_game.description = description;
            else
                new_game.description = game_itr->description;
            if(!screenshots.empty())
            {
                help_array = game_itr->screenshots;
                help_array.assign(screenshots.begin(), screenshots.end());
                new_game.screenshots = help_array;
            }
            else
                new_game.screenshots = game_itr->screenshots;
            if(!videos.empty())
            {
                help_array = game_itr->videos;
                help_array.assign(videos.begin(), videos.end());
                new_game.videos= help_array;
            }
            else
                new_game.videos = game_itr->videos;
            if(!promscrshots.empty())
            {
                help_array = game_itr->promscrshots;
                help_array.assign(promscrshots.begin(), promscrshots.end());
                new_game.promscrshots = help_array;
            }
            else
                new_game.promscrshots = game_itr->promscrshots;
            if(game_itr->price.amount != price.amount)
                new_game.price = price;
            else
                new_game.price = game_itr->price;
            if(game_itr->releasedate != releasedate && !releasedate.empty())
                new_game.releasedate = releasedate;
            else
                new_game.releasedate = game_itr->releasedate;
            if(game_itr->data != data && !data.empty())
                new_game.data = data;
            else
                new_game.data = game_itr->data;
        });
    }
    else
    {
        _apprgames.modify(app_itr, CONTRACTN, [&](auto &new_game) {
            new_game.id = gameid;
            if(game_itr->owner != new_owner && !new_owner.to_string().empty())
                new_game.owner = new_owner;
            else
                new_game.owner = game_itr->owner;
            if(game_itr->gameurl != gameurl)
                new_game.gameurl = gameurl;
            else
                new_game.gameurl = game_itr->gameurl;
            if(!keywords.empty())
                new_game.keywords = keywords;
            else
                new_game.keywords = game_itr->keywords;
            if(game_itr->description != description && !description.empty())
                new_game.description = description;
            else
                new_game.description = game_itr->description;
            if(!screenshots.empty())
            {
                help_array = game_itr->screenshots;
                help_array.assign(screenshots.begin(), screenshots.end());
                new_game.screenshots = help_array;
            }
            else
                new_game.screenshots = game_itr->screenshots;
            if(!videos.empty())
            {
                help_array = game_itr->videos;
                help_array.assign(videos.begin(), videos.end());
                new_game.videos= help_array;
            }
            else
                new_game.videos = game_itr->videos;
            if(!promscrshots.empty())
            {
                help_array = game_itr->promscrshots;
                help_array.assign(promscrshots.begin(), promscrshots.end());
                new_game.promscrshots = help_array;
            }
            else
                new_game.promscrshots = game_itr->promscrshots;
            if(game_itr->price.amount != price.amount)
                new_game.price = price;
            else
                new_game.price = game_itr->price;
            if(game_itr->releasedate != releasedate && !releasedate.empty())
                new_game.releasedate = releasedate;
            else
                new_game.releasedate = game_itr->releasedate;
            if(game_itr->data != data && !data.empty())
                new_game.data = data;
            else
                new_game.data = game_itr->data;
        });
    }
}

ACTION nestplatform::gameapprove(uint64_t appgameid, bool approve)
{
    require_auth(CONTRACTN);

    apprgames _apprgames(CONTRACTN, CONTRACTN.value);
    auto app_itr = _apprgames.find(appgameid);
    eosio::check(app_itr != _apprgames.end(), "Request doesn't exist.");

    if (approve)
    {
        games _games(CONTRACTN, CONTRACTN.value);
        auto game_itr = _games.find(appgameid);
        if (game_itr == _games.end())
        {
            _games.emplace(CONTRACTN, [&](auto &new_game) {
                new_game.id = app_itr->id;
                new_game.owner = app_itr->owner;
                new_game.gameurl = app_itr->gameurl;
                new_game.keywords = app_itr->keywords;
                new_game.description = app_itr->description;
                new_game.screenshots = app_itr->screenshots;
                new_game.videos = app_itr->videos;
                new_game.promscrshots = app_itr->promscrshots;
                new_game.price = app_itr->price;
                new_game.releasedate = app_itr->releasedate;
                new_game.data = app_itr->data;
            });
        }
        else
        {
            _games.modify(game_itr, CONTRACTN, [&](auto &mod_game) {
                mod_game.owner = app_itr->owner;
                mod_game.gameurl = app_itr->gameurl;
                mod_game.keywords = app_itr->keywords;
                mod_game.description = app_itr->description;
                mod_game.screenshots = app_itr->screenshots;
                mod_game.videos = app_itr->videos;
                mod_game.promscrshots = app_itr->promscrshots;
                mod_game.price = app_itr->price;
                mod_game.releasedate = app_itr->releasedate;
                mod_game.data = app_itr->data;
            });
        }
    }
    _apprgames.erase(app_itr);
}


ACTION nestplatform::gamerate(eosio::name username, uint64_t gameid, double rate, std::string comment)
{
    require_auth(username);

    comentapprs _comentaproves(CONTRACTN, CONTRACTN.value);

    uint64_t id = 0;

    auto rate_itr = _comentaproves.end();
    if(rate_itr != _comentaproves.begin())
    {
        rate_itr--;
        id = rate_itr->id;
    }
    id++;

    _comentaproves.emplace(CONTRACTN, [&](auto& new_rate){
        new_rate.id = id;
        new_rate.gameid = gameid;
        new_rate.username = username;
        new_rate.rate = rate;
        new_rate.comment = comment;
    });
}

ACTION nestplatform::apprcomment(uint64_t id, bool accept)
{
    require_auth(CONTRACTN);


    comentapprs _comentaproves(CONTRACTN, CONTRACTN.value);
    auto appr_itr = _comentaproves.find(id);
    eosio::check(appr_itr != _comentaproves.end(),"Record was created.");

    if(accept)
    {
        rategames _rategames(CONTRACTN, CONTRACTN.value);
        auto rate_itr = _rategames.find(appr_itr->gameid);
        
        if(rate_itr != _rategames.end())
        {
            _rategames.modify(rate_itr, CONTRACTN, [&](auto& mod_rate){
                mod_rate.totalstars += appr_itr->rate;
                mod_rate.totalvotes++;
                mod_rate.rate = (double)(mod_rate.totalstars / mod_rate.totalvotes);
                mod_rate.records.push_back({appr_itr->username,appr_itr->rate, appr_itr->comment});
            });
        }
        else
        {
            _rategames.emplace(CONTRACTN, [&](auto& new_record){
                new_record.gameid = appr_itr->gameid;
                new_record.totalstars = appr_itr->rate;
                new_record.totalvotes = 1;
                new_record.rate = appr_itr->rate;
                new_record.records.push_back({appr_itr->username,appr_itr->rate, appr_itr->comment});
            });
        }


    }
    
    _comentaproves.erase(appr_itr);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//        USER ACTIONS
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

ACTION nestplatform::usercreate(eosio::name author, eosio::name username)
{
    require_auth(author);

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    if(user_itr == _users.end())
    {
        _users.emplace(CONTRACTN, [&](auto &new_user) {
            new_user.account = username;
        });
    }
}

ACTION nestplatform::userregister(eosio::name username, std::string avatar, std::string nickname, eosio::name referrer)
{
    require_auth(username);

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr == _users.end(), "Current user was registered.");

    _users.emplace(CONTRACTN, [&](auto &new_user) {
        new_user.account = username;
    });

    apprusers _apprusers(CONTRACTN, CONTRACTN.value);
    auto appuser_itr = _apprusers.find(username.value);
    eosio::check(appuser_itr == _apprusers.end(), "Current user was registered.");

    _apprusers.emplace(CONTRACTN, [&](auto &new_user) {
        new_user.account = username;
        new_user.avatar = avatar;
        new_user.nickname = nickname;
    });

    eosio::action addRef = eosio::action(
            eosio::permission_level{CONTRACTN, eosio::name("active")},
            CONTRACTN,
            eosio::name("addreferral"),
            std::make_tuple(referrer, username));
        addRef.send();
}

ACTION nestplatform::addreferral(eosio::name referrer, eosio::name referee)
{
    if(eosio::has_auth(referrer))
    {
        require_auth(referrer);
    }
    else
    {
        require_auth(CONTRACTN);    
    }
    
    referrals _refs(CONTRACTN,CONTRACTN.value);
    auto referrer_itr = _refs.find(referrer.value);
    auto referee_itr = _refs.find(referee.value);

    if (referrer_itr != _refs.end())
    {
        _refs.modify(referrer_itr, CONTRACTN, [&](auto& mod_record){
            mod_record.referee.push_back(referee);
        });
    }
    else
    {
        _refs.emplace(CONTRACTN, [&](auto& new_record){
            new_record.account = referrer;
            new_record.referee = {referee};
        });
    }
    
    
    if (referee_itr != _refs.end())
    {
        _refs.modify(referee_itr, CONTRACTN, [&](auto& mod_record){
            mod_record.referrer = referrer;
        });
    }
    else
    {
        _refs.emplace(CONTRACTN, [&](auto& new_record){
            new_record.account = referee;
            new_record.referrer = referrer;
        });
    }
    
}

ACTION nestplatform::userupdate(eosio::name username, std::string avatar, std::string nickname)
{
    require_auth(username);

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr != _users.end(), "Current user wasn't registered.");

    apprusers _apprusers(CONTRACTN, CONTRACTN.value);
    auto appuser_itr = _apprusers.find(username.value);
    if (appuser_itr == _apprusers.end())
    {
        _apprusers.emplace(CONTRACTN, [&](auto &new_appu) {
            new_appu.account = username;
            if(user_itr->avatar != avatar && !avatar.empty())
                new_appu.avatar = avatar;
            if(user_itr->nickname != nickname && !nickname.empty())
                new_appu.nickname = nickname;
        });
    }
    else
    {
        _apprusers.modify(appuser_itr, CONTRACTN, [&](auto &mod_appu) {
            if(mod_appu.avatar != avatar && !avatar.empty())
                mod_appu.avatar = avatar;
            if(mod_appu.nickname != nickname && !nickname.empty())
                mod_appu.nickname = nickname;
        });
    }
}

ACTION nestplatform::userapprove(eosio::name username, bool approve)
{
    require_auth(CONTRACTN);

    apprusers _apprusers(CONTRACTN, CONTRACTN.value);
    auto appuser_itr = _apprusers.find(username.value);
    eosio::check(appuser_itr == _apprusers.end(), "Current user hasn't data to approve.");

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    if (approve)
    {
        if (user_itr == _users.end())
        {
            _users.emplace(CONTRACTN, [&](auto &new_user) {
                new_user.account = appuser_itr->account;
                new_user.avatar = appuser_itr->avatar;
                new_user.nickname = appuser_itr->nickname;
            });
        }
        else
        {
            _users.modify(user_itr, CONTRACTN, [&](auto &mod_user) {
                mod_user.avatar = appuser_itr->avatar;
                mod_user.nickname = appuser_itr->nickname;
            });
        }
    }

    _apprusers.erase(appuser_itr);
}

ACTION nestplatform::addfriend(eosio::name username, eosio::name newfriend)
{
    require_auth(username);

    eosio::check(is_account(newfriend),"Friend account doesn't exist on WAX");

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr == _users.end(), "Current user doesn't exist.");
    eosio::check(!user_itr->nickname.empty() && !user_itr->avatar.empty(), "User must be setting up.");

    auto friend_itr = _users.find(newfriend.value);
    eosio::check(friend_itr == _users.end(), "Friend user doesn't exist.");

    int friend_pos = finder(user_itr->friends, newfriend);
    eosio::check(friend_pos == -1, "User is on your friends' list");

    friendapplys _friend(CONTRACTN,CONTRACTN.value);
    auto apply_itr = _friend.find(username.value);
    if(apply_itr == _friend.end())
    {
        _friend.emplace(CONTRACTN, [&](auto& new_apply){
            new_apply.account = newfriend;
            new_apply.friends.push_back(username);
        });
    }
}

ACTION nestplatform::deletefriend(eosio::name username, eosio::name delfriend)
{
    require_auth(username);

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr == _users.end(), "Current user doesn't exist.");

    auto friend_itr = _users.find(delfriend.value);
    eosio::check(friend_itr == _users.end(), "Friend user doesn't exist.");

    int friend_pos = finder(user_itr->friends, delfriend);
    eosio::check(friend_pos != -1, "User isn't on your friends' list");
    _users.modify(user_itr, CONTRACTN, [&](auto &mod_user) {
        mod_user.friends.erase(mod_user.friends.begin() + friend_pos);
    });
}

ACTION nestplatform::applyfriend(eosio::name username, eosio::name newfriend)
{
    require_auth(username);

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr == _users.end(), "Current user doesn't exist.");
    int friend_pos = finder(user_itr->friends, newfriend);
    eosio::check(friend_pos == -1, "User is on your friends' list");

    auto baseuser_itr = _users.find(newfriend.value);
    eosio::check(baseuser_itr == _users.end(), "New friend user doesn't exist.");
    friend_pos = finder(baseuser_itr->friends, username);
    eosio::check(friend_pos == -1, "User is on your friends' list");

    friendapplys _friend(CONTRACTN,CONTRACTN.value);
    auto friend_itr = _friend.find(username.value);
    eosio::check(friend_itr != _friend.end(),"User hasn't friends to apply");

    int pos = finder(friend_itr->friends, newfriend);
    eosio::check(pos != -1,"You haven't this friend");

    _friend.modify(friend_itr, CONTRACTN,[&](auto& mod_record){
        mod_record.friends.erase(mod_record.friends.begin()+pos);
    });

    _users.modify(user_itr, CONTRACTN, [&](auto &mod_user) {
        mod_user.friends.push_back(newfriend);
    });

    _users.modify(baseuser_itr, CONTRACTN, [&](auto &mod_user) {
        mod_user.friends.push_back(newfriend);
    });
}


////////////////////////////////////////////////////////////////////////////////
//
//        CARDS ACTIONS
//
////////////////////////////////////////////////////////////////////////////////
ACTION nestplatform::setupdrop(uint8_t maxdrops, uint8_t maxonseries)
{
    carddrset config(CONTRACTN, CONTRACTN.value);
    _cdrops = config.exists() ? config.get() : carddropset{};
    _cdrops.maxdrop = maxdrops;
    _cdrops.maxonseries = maxonseries;
    config.set(_cdrops, CONTRACTN);
}

ACTION nestplatform::cardcreate(eosio::name owner, uint64_t gameid, std::string cardname, std::string enableimg, std::string disableimg, std::string series, eosio::name schema_name)
{
    require_auth(owner);

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner, "You aren't the owner of current game.");



    apprcards _apprcards(CONTRACTN, CONTRACTN.value);
    _apprcards.emplace(CONTRACTN, [&](auto &new_card) {
        new_card.id = getcardid();
        new_card.gameid = gameid;
        new_card.cardname = cardname;
        new_card.enableimg = enableimg;
        new_card.schema_name = schema_name;
        new_card.disableimg = disableimg;
        new_card.series = series;
    });
}

ACTION nestplatform::carddelete(uint64_t cardid)
{
    cards _cards(CONTRACTN, CONTRACTN.value);
    auto card_itr = _cards.find(cardid);
    eosio::check(card_itr != _cards.end(), "Card with current id doesn't exist.");

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(card_itr->gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");

    require_auth(game_itr->owner);

    _cards.erase(card_itr);
}

ACTION nestplatform::carddrop(uint64_t cardid, eosio::name username, uint8_t nftstandard)
{

    cards _cards(CONTRACTN, CONTRACTN.value);
    auto card_itr = _cards.find(cardid);
    eosio::check(card_itr != _cards.end(), "Card with current id doesn't exist.");

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(card_itr->gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");

    require_auth(game_itr->owner);

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    if(user_itr == _users.end())
    {
        eosio::action createUser = eosio::action(
            eosio::permission_level{CONTRACTN, eosio::name("active")},
            CONTRACTN,
            eosio::name("usercreate"),
            std::make_tuple(game_itr->owner, username));
        createUser.send();
    }

    usercards _usercards(CONTRACTN, CONTRACTN.value);
    auto usercard_itr = _usercards.find(username.value);
    if (usercard_itr == _usercards.end())
    {
        _usercards.emplace(CONTRACTN, [&](auto &new_record) {
            new_record.username = username;
            new_record.cardids.push_back({cardid,1});
        });
    }
    else
    {
        int card_vec_id = finder(usercard_itr->cardids,cardid);
        _usercards.modify(usercard_itr, CONTRACTN, [&](auto &mod_record) {
            if(card_vec_id == -1)
            {
                mod_record.cardids.push_back({cardid,1});
            }
            else
            {
                mod_record.cardids.at(card_vec_id).amount++;
            }
        });
    }

    if(nftstandard == 1)
    {

        std::string idata = "{\"name\":\"" + card_itr->cardname + "\", \"gameid\":" + std::to_string(card_itr->gameid) + ", \"series\":\"" + card_itr->series + "\" }";
        std::string mdata = "{\"img\": \"" + card_itr->enableimg + "\" }";

        eosio::action createAsset = eosio::action(
            eosio::permission_level{CONTRACTN, eosio::name("active")},
            SIMPLEASSETSCONTRACT,
            eosio::name("create"),
            std::make_tuple(CONTRACTN, eosio::name("card"), game_itr->owner, idata, mdata, 0));
        createAsset.send();
    }
    else if(nftstandard == 2)
    {

        ATTRIBUTE_MAP empty_atr = {};
        std::vector<eosio::asset> nullvec{};
        
        eosio::action mintAsset = eosio::action(
            eosio::permission_level{CONTRACTN, eosio::name("active")},
            eosio::name("atomicassets"),
            eosio::name("mintasset"),
            std::make_tuple(CONTRACTN, CONTRACTN, card_itr->schema_name, card_itr->templateid, username, empty_atr, empty_atr, nullvec)
        );
    }
}

ACTION nestplatform::cardapprove(uint64_t cardid, bool approve, int32_t templateid)
{
    require_auth(CONTRACTN);

    apprcards _apprcards(CONTRACTN, CONTRACTN.value);
    auto apprcard_itr = _apprcards.find(cardid);
    eosio::check(apprcard_itr != _apprcards.end(), "Current card request don't exist");

    if (approve)
    {
        cards _cards(CONTRACTN, CONTRACTN.value);
        auto card_itr = _cards.find(cardid);
        eosio::check(card_itr == _cards.end(), "Current card exists.");
        _cards.emplace(CONTRACTN, [&](auto &new_card) {
            new_card.id = apprcard_itr->id;
            new_card.gameid = apprcard_itr->gameid;
            new_card.cardname = apprcard_itr->cardname;
            new_card.enableimg = apprcard_itr->enableimg;
            new_card.disableimg = apprcard_itr->disableimg;
            new_card.schema_name = apprcard_itr->schema_name;
            new_card.templateid = templateid;
            new_card.series = apprcard_itr->series;
        });

        gamecards _gamecards(CONTRACTN, CONTRACTN.value);
        auto gamecard_itr = _gamecards.find(apprcard_itr->gameid);
        if (gamecard_itr == _gamecards.end())
        {
            _gamecards.emplace(CONTRACTN, [&](auto &new_record) {
                new_record.gameid = apprcard_itr->gameid;
                new_record.series.push_back({apprcard_itr->series, {apprcard_itr->id}});
            });
        }
        else
        {
            int pos = finder(gamecard_itr->series, apprcard_itr->series);
            _gamecards.modify(gamecard_itr, CONTRACTN, [&](auto &mod_record) {
                if (pos == -1)
                {
                    mod_record.series.push_back({apprcard_itr->series, {apprcard_itr->id}});
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

ACTION nestplatform::lvlcreate(uint64_t level, uint64_t xp)
{
    require_auth(CONTRACTN);

    levels _levels(CONTRACTN, CONTRACTN.value);
    auto itr = _levels.find(level);
    eosio::check(itr == _levels.end(), "Level exists.");
    _levels.emplace(CONTRACTN, [&](auto &new_lvl) {
        new_lvl.lvl = level;
        new_lvl.points = xp;
    });
}

ACTION nestplatform::lvlupdate(uint64_t level, uint64_t xp)
{
    require_auth(CONTRACTN);

    levels _levels(CONTRACTN, CONTRACTN.value);
    auto itr = _levels.find(level);
    eosio::check(itr != _levels.end(), "Level doesn't exist.");
    _levels.modify(itr, CONTRACTN, [&](auto &new_lvl) {
        new_lvl.points = xp;
    });
}

ACTION nestplatform::lvlearn(eosio::name username)
{
    require_auth(CONTRACTN);

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr != _users.end(), "User doesn't exist.");

    levels _levels(CONTRACTN, CONTRACTN.value);
    auto lvl_itr = _levels.find(user_itr->level);
    eosio::check(lvl_itr != _levels.end(), "Level doesn't exist");
    uint64_t new_lvl;
    while (user_itr->xp >= lvl_itr->points)
    {
        new_lvl = lvl_itr->lvl;
        lvl_itr++;
        if (lvl_itr == _levels.end())
        {
            break;
        }
    }

    _users.modify(user_itr, CONTRACTN, [&](auto &mod_user) {
        mod_user.level = new_lvl;
    });
}

ACTION nestplatform::lvladdxp(eosio::name username, uint64_t xp)
{
    require_auth(CONTRACTN);

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    eosio::check(user_itr != _users.end(), "User doesn't exist.");

    levels _levels(CONTRACTN, CONTRACTN.value);
    auto lvl_itr = _levels.find(user_itr->level);
    eosio::check(lvl_itr != _levels.end(), "Level doesn't exist");

    uint64_t new_lvl, points = user_itr->xp;
    points += xp;
    while (points >= lvl_itr->points)
    {
        new_lvl = lvl_itr->lvl;
        lvl_itr++;
        if (lvl_itr == _levels.end())
        {
            break;
        }
    }

    _users.modify(user_itr, CONTRACTN, [&](auto &mod_user) {
        mod_user.level = new_lvl;
        mod_user.xp += xp;
    });
}

////////////////////////////////////////////////////////////////////////////////
//
//        BADGES ACTIONS
//
////////////////////////////////////////////////////////////////////////////////

ACTION nestplatform::badgecreate(eosio::name owner, uint64_t gameid, std::string badgename, std::vector<uint64_t> cardids)
{
    require_auth(owner);

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner, "You aren't the owner of current game.");

    cards _cards(CONTRACTN, CONTRACTN.value);
    for (uint8_t i = 0; i < cardids.size(); i++)
    {
        auto card_itr = _cards.find(cardids.at(i));
        eosio::check(card_itr != _cards.end(), "Card doesn't exist.");
    }

    apprbadges _apprbadge(CONTRACTN, CONTRACTN.value);
    _apprbadge.emplace(CONTRACTN, [&](auto &new_badge) {
        new_badge.id = getbadgeid();
        new_badge.gameid = gameid;
        new_badge.badgename = badgename;
        new_badge.cardids = cardids;
    });
}

ACTION nestplatform::badgedelete(uint64_t badgeid)
{

    badges _badges(CONTRACTN, CONTRACTN.value);
    auto badge_itr = _badges.find(badgeid);
    eosio::check(badge_itr != _badges.end(), "Badge with current id doesn't exist.");
    eosio::check(badge_itr->candelete == 1, "Badge can't be deleted.");

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(badge_itr->gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");

    require_auth(game_itr->owner);

    _badges.erase(badge_itr);
}

ACTION nestplatform::badgeapprove(uint64_t badgeid, bool approve)
{
    require_auth(CONTRACTN);

    apprbadges _apprbadge(CONTRACTN, CONTRACTN.value);
    auto apprbadge_itr = _apprbadge.find(badgeid);
    eosio::check(apprbadge_itr != _apprbadge.end(), "Current badge request don't exist");

    if (approve)
    {
        badges _badges(CONTRACTN, CONTRACTN.value);
        auto badge_itr = _badges.find(badgeid);
        eosio::check(badge_itr == _badges.end(), "Badge with current id doesn't exist.");
        _badges.emplace(CONTRACTN, [&](auto &new_badge) {
            new_badge.id = badgeid;
            new_badge.gameid = apprbadge_itr->gameid;
            new_badge.badgename = apprbadge_itr->badgename;
            new_badge.cardids = apprbadge_itr->cardids;
        });

        games _games(CONTRACTN, CONTRACTN.value);
        auto game_itr = _games.find(apprbadge_itr->gameid);
        eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");

        _games.modify(game_itr, CONTRACTN, [&](auto &mod_game) {
            mod_game.badges.push_back(badgeid);
        });
    }
    _apprbadge.erase(apprbadge_itr);
}

ACTION nestplatform::badgeearn(eosio::name username, uint64_t badgeid)
{
    require_auth(CONTRACTN);

    badges _badges(CONTRACTN, CONTRACTN.value);
    auto badge_itr = _badges.find(badgeid);
    eosio::check(badge_itr != _badges.end(), "Badge with current id doesn't exist.");

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(badge_itr->gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    if(user_itr == _users.end())
    {
        eosio::action createUser = eosio::action(
            eosio::permission_level{CONTRACTN, eosio::name("active")},
            CONTRACTN,
            eosio::name("usercreate"),
            std::make_tuple(game_itr->owner,username));
        createUser.send();
    }

    usercards _usercards(CONTRACTN, CONTRACTN.value);
    auto usercard_itr = _usercards.find(username.value);
    eosio::check(usercard_itr != _usercards.end(), "User doesn't exist.");

    bool is_badge = true;
    for (uint8_t i; i < badge_itr->cardids.size(); i++)
    {
        int pos = finder(usercard_itr->cardids, badge_itr->cardids.at(i));
        if (pos == -1)
        {
            is_badge = false;
            break;
        }
    }

    eosio::check(is_badge == true, "User hasn't all cards from this badge.");

    userbadges _userbadges(CONTRACTN,CONTRACTN.value);
    auto badge_usr_itr = _userbadges.find(username.value);
    if(badge_usr_itr == _userbadges.end())
    {
        _userbadges.emplace(CONTRACTN,[&](auto& new_badge_rec){
            new_badge_rec.username = username;
            new_badge_rec.badge_id.push_back(badgeid);
        });
    }
    else
    {
        int badge_arr_id = finder(badge_usr_itr->badge_id, badgeid);
        eosio::check(badge_arr_id == -1,"User had current badge.");
        _userbadges.modify(badge_usr_itr,CONTRACTN,[&](auto& mod_badge_rec){
            mod_badge_rec.badge_id.push_back(badgeid);
        });
    }

    levels _levels(CONTRACTN, CONTRACTN.value);
    auto lvl_itr = _levels.find(user_itr->level);
    eosio::check(lvl_itr != _levels.end(), "Level doesn't exist");

    uint64_t xp = 100;

    uint64_t new_lvl, points = user_itr->xp;
    points += xp;
    while (points >= lvl_itr->points)
    {
        new_lvl = lvl_itr->lvl;
        lvl_itr++;
        if (lvl_itr == _levels.end())
        {
            break;
        }
    }

    _users.modify(user_itr, CONTRACTN, [&](auto &mod_user) {
        mod_user.level = new_lvl;
        mod_user.xp += xp;
    });
}

////////////////////////////////////////////////////////////////////////////////
//
//        ACHIEVEMENTS ACTIONS
//
////////////////////////////////////////////////////////////////////////////////

ACTION nestplatform::achievcreate(eosio::name owner, uint64_t gameid, std::string achievname, uint64_t xp, std::string description, std::string image, std::string rarity)
{
    require_auth(owner);

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");
    eosio::check(game_itr->owner == owner, "You aren't the owner of current game.");

    appracheivs _appracheivs(CONTRACTN, CONTRACTN.value);
    _appracheivs.emplace(CONTRACTN, [&](auto &new_achiev) {
        new_achiev.id = getachievid();
        new_achiev.gameid = gameid;
        new_achiev.achievname = achievname;
        new_achiev.description = description;
        new_achiev.image = image;
        new_achiev.xp = xp;
        new_achiev.rarity = rarity;
    });
}

ACTION nestplatform::achievdelete(uint64_t achieveid)
{
    achievements _achievements(CONTRACTN, CONTRACTN.value);
    auto achiev_itr = _achievements.find(achieveid);
    eosio::check(achiev_itr == _achievements.end(), "Achievement with current id exists.");
    eosio::check(achiev_itr->candelete == 1, "Achievement can't be deleted.");

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(achiev_itr->gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");

    require_auth(game_itr->owner);

    _achievements.erase(achiev_itr);
}

ACTION nestplatform::achievearn(eosio::name username, uint64_t achieveid)
{
    achievements _achievements(CONTRACTN, CONTRACTN.value);
    auto achiev_itr = _achievements.find(achieveid);
    eosio::check(achiev_itr != _achievements.end(), "Achievement with current id exists.");

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(achiev_itr->gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");

    require_auth(game_itr->owner);

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    if(user_itr == _users.end())
    {
        eosio::action createUser = eosio::action(
            eosio::permission_level{CONTRACTN, eosio::name("active")},
            CONTRACTN,
            eosio::name("usercreate"),
            std::make_tuple(game_itr->owner,username));
        createUser.send();
    }

    int game_pos = finder(user_itr->achievements, achiev_itr->gameid);

    levels _levels(CONTRACTN, CONTRACTN.value);
    auto lvl_itr = _levels.find(user_itr->level);
    eosio::check(lvl_itr != _levels.end(), "Level doesn't exist");

    uint64_t xp = achiev_itr->xp;

    uint64_t new_lvl, points = user_itr->xp;
    points += xp;
    while (points >= lvl_itr->points)
    {
        new_lvl = lvl_itr->lvl;
        lvl_itr++;
        if (lvl_itr == _levels.end())
        {
            break;
        }
    }

    _users.modify(user_itr, CONTRACTN, [&](auto &mod_user) {
        if (game_pos == -1)
        {
            mod_user.achievements.push_back({achiev_itr->gameid, static_cast<uint8_t>(game_itr->achievements.size()), {achieveid}});
        }
        else
        {
            eosio::check(finder(mod_user.achievements, achieveid) == -1, "User has current achievement.");
            mod_user.achievements.at(static_cast<uint64_t>(game_pos)).achievs.push_back(achieveid);
        }
        mod_user.level = new_lvl;
        mod_user.xp += xp;
    });
}

ACTION nestplatform::achievapprove(uint64_t achieveid, bool approve)
{
    require_auth(CONTRACTN);

    appracheivs _appracheivs(CONTRACTN, CONTRACTN.value);
    auto appracheiv_itr = _appracheivs.find(achieveid);
    eosio::check(appracheiv_itr != _appracheivs.end(), "Achievement request doesn't exist.");

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(appracheiv_itr->gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");

    if (approve)
    {
        achievements _achievements(CONTRACTN, CONTRACTN.value);
        auto achiev_itr = _achievements.find(achieveid);
        eosio::check(achiev_itr == _achievements.end(), "Achievement with current id exists.");

        _achievements.emplace(CONTRACTN, [&](auto &new_achiev) {
            new_achiev.id = appracheiv_itr->id;
            new_achiev.gameid = appracheiv_itr->gameid;
            new_achiev.achievname = appracheiv_itr->achievname;
            new_achiev.description = appracheiv_itr->description;
            new_achiev.image = appracheiv_itr->image;
            new_achiev.xp = appracheiv_itr->xp;
            new_achiev.rarity = appracheiv_itr->rarity;
        });
    }

    _appracheivs.erase(appracheiv_itr);
}

ACTION nestplatform::achievxp(uint64_t id, uint64_t xp)
{
    require_auth(CONTRACTN);

    achievements _achievements(CONTRACTN, CONTRACTN.value);
    auto achiev_itr = _achievements.find(id);
    eosio::check(achiev_itr != _achievements.end(), "Achievement with current id exists.");

    _achievements.modify(achiev_itr, CONTRACTN, [&](auto& mod_ach){
        mod_ach.xp = xp;
    });
}


////////////////////////////////////////////////////////////////////////////////
//        OUR GAME FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

ACTION nestplatform::startround(eosio::name username, uint64_t gameid)
{
    require_auth(username);
    require_recipient(username);

    games _games(CONTRACTN, CONTRACTN.value);
    auto game_itr = _games.find(gameid);
    eosio::check(game_itr != _games.end(), "Game with current id doesn't exist.");

    users _users(CONTRACTN, CONTRACTN.value);
    auto user_itr = _users.find(username.value);
    if(user_itr == _users.end())
    {
        eosio::action createUser = eosio::action(
            eosio::permission_level{CONTRACTN, eosio::name("active")},
            CONTRACTN,
            eosio::name("usercreate"),
            std::make_tuple(game_itr->owner,username));
        createUser.send();
    }
}

////////////////////////////////////////////////////////////////////////////////
//        HELP FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

int nestplatform::finder(vector<player_s> players, eosio::name username)
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

int nestplatform::finder(vector<eosio::name> friends, eosio::name username)
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

int nestplatform::finder(std::vector<achiev_s> achieves, uint64_t gameid)
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

int nestplatform::finder(std::vector<card_s> cards, uint64_t card_id)
{
    for (uint64_t i = 0; i < cards.size(); i++)
    {
        if (cards.at(i).card_id == card_id)
        {
            return i;
        }
    }
    return -1;
}

int nestplatform::finder(std::vector<series_s> series, std::string ser_name)
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

int nestplatform::finder(std::vector<uint64_t> ids, uint64_t id)
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

uint64_t nestplatform::get_achiev_points(std::string rarity)
{
    return 100;
}

uint64_t nestplatform::getcardid()
{

    conf config(CONTRACTN, CONTRACTN.value);
    _cstate = config.exists() ? config.get() : global{};
    _cstate.cardid++;
    config.set(_cstate, CONTRACTN);

    return _cstate.cardid;
}
uint64_t nestplatform::getbadgeid()
{

    conf config(CONTRACTN, CONTRACTN.value);
    _cstate = config.exists() ? config.get() : global{};
    _cstate.badgeid++;
    config.set(_cstate, CONTRACTN);

    return _cstate.badgeid;
}
uint64_t nestplatform::getachievid()
{

    conf config(CONTRACTN, CONTRACTN.value);
    _cstate = config.exists() ? config.get() : global{};
    _cstate.achievid++;
    config.set(_cstate, CONTRACTN);

    return _cstate.achievid;
}
uint64_t nestplatform::getgameid()
{

    conf config(CONTRACTN, CONTRACTN.value);
    _cstate = config.exists() ? config.get() : global{};
    _cstate.gameid++;
    config.set(_cstate, CONTRACTN);

    return _cstate.gameid;
}
uint64_t nestplatform::getlboardid()
{

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
            EOSIO_DISPATCH_HELPER(nestplatform, (createlboard)(createprize)(resetlboard)(removelboard)(update)(changebpoint)
                                                (gameregister)(gameupdate)(gameapprove)
                                                (usercreate)(userregister)(userupdate)(userapprove)(addfriend)(deletefriend)
                                                (achievcreate)(achievdelete)(achievearn)(achievapprove)(achievxp)
                                                (cardcreate)(carddelete)(carddrop)(cardapprove)(setupdrop)
                                                (badgecreate)(badgedelete)(badgeapprove)(badgeearn)
                                                (lvlcreate)(lvlupdate)(lvlearn)(lvladdxp)
                                                (startround)(apprcomment)(gamerate))
        }
    }
    else
    {
        printf("Couldn't find an action.");
        return;
    }
}
