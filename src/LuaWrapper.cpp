#include "LuaWrapper.h"

extern "C" {

    static int lua_wrapper_delay(lua_State *lua) {
        int a = luaL_checkinteger(lua, 1);
        delay(a);
        return 0;
    }

  static int lua_wrapper_millis(lua_State *lua) {
    lua_pushnumber(lua, (lua_Number) millis());
    return 1;
  }
}

LuaWrapper::LuaWrapper() {
  _state = luaL_newstate();

  luaopen_base(_state);
  luaopen_table(_state);
  luaopen_string(_state);
  luaopen_math(_state);

  lua_register(_state, "delay", lua_wrapper_delay);
  lua_register(_state, "millis", lua_wrapper_millis);
}

String LuaWrapper::Lua_dostring(const String *script) {
  String scriptWithConstants = addConstants() + *script;
  String result;
  if (luaL_dostring(_state, scriptWithConstants.c_str())) {
    result += "# lua error:\n" + String(lua_tostring(_state, -1));
    lua_pop(_state, 1);
  }
  return result;
}

void LuaWrapper::Lua_register(const String name, const lua_CFunction function) {
  lua_register(_state, name.c_str(), function);
}

String LuaWrapper::addConstants() {
  String constants = "INPUT = " + String(INPUT) + "\r\n";
  constants += "OUTPUT = " + String(OUTPUT) + "\r\n";
  constants += "LOW = " + String(LOW) + "\r\n";
  constants += "HIGH = " + String(HIGH) + "\r\n";
  return constants;
}