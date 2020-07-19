const globals = require("./global_constants");

//read user TABLE
async function get_users() {
  const user_rows = await globals.NEST_CONTRACT.getTableRows({
    code: globals.NEST_CONTRACT,
    scope: globals.NEST_CONTRACT,
    table: "users",
    json: true,
  });
  return user_rows;
}

//read game TABLE
async function get_games() {
  const user_rows = await globals.NEST_CONTRACT.getTableRows({
    code: globals.NEST_CONTRACT,
    scope: globals.NEST_CONTRACT,
    table: "games",
    json: true,
  });
  return user_rows;
}

//read leader board TABLE
async function get_users() {
  const user_rows = await globals.NEST_CONTRACT.getTableRows({
    code: globals.NEST_CONTRACT,
    scope: globals.NEST_CONTRACT,
    table: "lboards",
    json: true,
  });
  return user_rows;
}


