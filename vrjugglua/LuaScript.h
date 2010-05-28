/**	@file	LuaScript.h
	@brief	header

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/
#pragma once
#ifndef INCLUDED_vrjugglua_LuaScript_h
#define INCLUDED_vrjugglua_LuaScript_h

#if defined(__cplusplus) && !defined(BUILD_LUA_AS_CPP)
extern "C" {
#endif // defined(__cplusplus) && !defined(BUILD_LUA_AS_CPP)

#include "lua.h"
void setInteractiveInterpreter(lua_State * state);
int luaopen_vrjugglua(lua_State *L);
int luaopen_libvrjugglua(lua_State *L);

#if defined(__cplusplus) && !defined(BUILD_LUA_AS_CPP)
}
#endif

#if defined(__cplusplus)
// Local includes
// - none

// Library/third-party includes

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

// Standard includes
#include <string>
#include <map>

namespace vrjLua {

typedef boost::shared_ptr<lua_State> LuaStatePtr;
typedef boost::weak_ptr<lua_State> LuaStateWeakPtr;


void setInteractiveInterpreter(LuaStatePtr * state);
LuaStatePtr getInteractiveInterpreter();

class LuaScript {
	public:
		LuaScript();

		/// @brief constructor from an externally-allocated state
		explicit LuaScript(lua_State * state, bool bind = false);

		/// @brief copy constructor - doesn't re-bind
		LuaScript(const LuaScript & other);

		/// @brief copy constructor - doesn't re-bind
		LuaScript(const LuaStatePtr & other);

		LuaScript & operator=(const LuaScript & other);

		bool runFile(const std::string & fn);
		bool runString(const std::string & fn);
		bool call(const std::string & func);

		LuaStateWeakPtr getLuaState() const;

		bool isValid() const;

	protected:
		void _applyBindings();
		LuaStatePtr _state;
		static std::map<lua_State*, LuaStateWeakPtr> _allStates;
};

// -- inline implementations -- /

inline bool LuaScript::isValid() const {
	return (_state);
}

}// end of vrjLua namespace
#endif // defined(__cplusplus)
#endif // INCLUDED_vrjugglua_LuaScript_h
