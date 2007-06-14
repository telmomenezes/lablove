/*
 * Based on Lunar, modified by Telmo Menezes
 */

#if !defined(__INCLUDE_ORBIT_H)
#define __INCLUDE_ORBIT_H

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}

template <typename T> class Orbit
{
	typedef struct {T* pT;} userdataType;

public:
	typedef int (T::*mfp)(lua_State* luaState);
	typedef struct {const char* name; mfp mfunc;} MethodType;
	typedef struct {const char* name; float value;} NumberGlobalType;

	static void orbitRegister(lua_State* luaState)
	{
		lua_newtable(luaState);
		int class_table = lua_gettop(luaState);

		luaL_newmetatable(luaState, T::mClassName);
		int metatable = lua_gettop(luaState);

		// store method table in globals so that
		// scripts can add functions written in Lua.
		lua_pushvalue(luaState, class_table);
		set(luaState, LUA_GLOBALSINDEX, T::mClassName);

		// hide metatable from Lua getmetatable()
		lua_pushvalue(luaState, class_table);
		set(luaState, metatable, "__metatable");

		lua_pushvalue(luaState, class_table);
		set(luaState, metatable, "__index");

		lua_pushcfunction(luaState, tostring_T);
		set(luaState, metatable, "__tostring");

		lua_pushcfunction(luaState, gc_T);
		set(luaState, metatable, "__gc");

		lua_newtable(luaState);                // mt for method table
		lua_pushcfunction(luaState, new_T);
		lua_pushvalue(luaState, -1);           // dup new_T function
		set(luaState, class_table, "new");         // add new_T to method table
		set(luaState, -3, "__call");           // mt.__call = new_T

		lua_setmetatable(luaState, class_table);

		// fill class table with methods from class T
		for (MethodType *l = T::mMethods; l->name; l++)
		{
			lua_pushstring(luaState, l->name);
			lua_pushlightuserdata(luaState, (void*)l);
			lua_pushcclosure(luaState, thunk, 1);
			lua_settable(luaState, class_table);
		}

		// fill class table with number globals from class T
		for (NumberGlobalType *l = T::mNumberGlobals; l->name; l++)
		{
			lua_pushstring(luaState, l->name);
			lua_pushnumber(luaState, l->value);
			lua_settable(luaState, class_table);
		}

		lua_pop(luaState, 2);  // drop metatable and method table
	}

	// call named lua method from userdata method table
	static int call(lua_State* luaState,
			const char *method,
			int nargs=0,
			int nresults=LUA_MULTRET,
			int errfunc=0)
	{
		int base = lua_gettop(luaState) - nargs;  // userdata index
		if (!luaL_checkudata(luaState, base, T::mClassName))
		{
			lua_settop(luaState, base-1);           // drop userdata and args
			lua_pushfstring(luaState, "not a valid %s userdata", T::mClassName);
			return -1;
		}

		lua_pushstring(luaState, method);         // method name
		lua_gettable(luaState, base);             // get method from userdata
		if (lua_isnil(luaState, -1))
		{            // no method?
			lua_settop(luaState, base-1);           // drop userdata and args
			lua_pushfstring(luaState, "%s missing method '%s'", T::mClassName, method);
			return -1;
		}
		lua_insert(luaState, base);               // put method under userdata, args

		int status = lua_pcall(luaState, 1+nargs, nresults, errfunc);  // call method
		if (status)
		{
			const char *msg = lua_tostring(luaState, -1);
			if (msg == NULL)
			{
				msg = "(error with no message)";
			}
			lua_pushfstring(luaState,
					"%s:%s status = %d\n%s",
					T::mClassName,
					method,
					status,
					msg);
			lua_remove(luaState, base);             // remove old message
			return -1;
		}
		return lua_gettop(luaState) - base + 1;   // number of results
	}

	// push onto the Lua stack a userdata containing a pointer to T object
	static int push(lua_State* luaState, T* obj, bool gc=false)
	{
		if (!obj)
		{
			lua_pushnil(luaState);
			return 0;
		}
		luaL_getmetatable(luaState, T::mClassName);  // lookup metatable in Lua registry
		if (lua_isnil(luaState, -1))
		{
			luaL_error(luaState, "%s missing metatable", T::mClassName);
		}
		int mt = lua_gettop(luaState);
    		subtable(luaState, mt, "userdata", "v");
    		userdataType *ud =
			static_cast<userdataType*>(pushuserdata(luaState, obj, sizeof(userdataType)));
		if (ud)
		{
			ud->pT = obj;  // store pointer to object in userdata
			lua_pushvalue(luaState, mt);
			lua_setmetatable(luaState, -2);
			if (gc == false)
			{
				lua_checkstack(luaState, 3);
				subtable(luaState, mt, "do not trash", "k");
				lua_pushvalue(luaState, -2);
				lua_pushboolean(luaState, 1);
				lua_settable(luaState, -3);
				lua_pop(luaState, 1);
			}
		}
		lua_replace(luaState, mt);
		lua_settop(luaState, mt);
		return mt;  // index of userdata containing pointer to T object
	}

	// get userdata from Lua stack and return pointer to T object
	static T* check(lua_State* luaState, int narg)
	{
		userdataType *ud =
			static_cast<userdataType*>(luaL_checkudata(luaState, narg, T::mClassName));
		if(!ud)
		{
			luaL_typerror(luaState, narg, T::mClassName);
		}
		return ud->pT;  // pointer to T object
	}

	// Added by Telmo Menezes
	// get userdata from Lua stack and return it as a void* pointer
	static void* pointer(lua_State* luaState, int narg)
	{
		userdataType* ud = static_cast<userdataType*>(lua_touserdata(luaState, narg));
		if(!ud) luaL_typerror(luaState, narg, T::mClassName);
		return (void*)ud->pT;  // pointer to T object
	}

	

private:
	Orbit();  // hide default constructor

	// member function dispatcher
	static int thunk(lua_State* luaState)
	{
		// stack has userdata, followed by method args
		T *obj = check(luaState, 1);  // get 'self', or if you prefer, 'this'
		lua_remove(luaState, 1);  // remove self so member function args start at index 1
		// get member function from upvalue
		MethodType *l = static_cast<MethodType*>(lua_touserdata(luaState, lua_upvalueindex(1)));
		return (obj->*(l->mfunc))(luaState);  // call member function
	}

	// create a new T object and
	// push onto the Lua stack a userdata containing a pointer to T object
	static int new_T(lua_State* luaState)
	{
		lua_remove(luaState, 1);   // use classname:new(), instead of classname.new()
		T* obj = new T(luaState);  // call constructor for T objects
		push(luaState, obj, true); // gc_T will delete this object
		return 1;           // userdata containing pointer to T object
	}

	// garbage collection metamethod
	static int gc_T(lua_State* luaState)
	{
		if (luaL_getmetafield(luaState, 1, "do not trash"))
		{
			lua_pushvalue(luaState, 1);  // dup userdata
			lua_gettable(luaState, -2);
			if (!lua_isnil(luaState, -1))
			{
				return 0;  // do not delete object
			}
		}
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(luaState, 1));
		T* obj = ud->pT;
		if (obj)
		{
			delete obj;  // call destructor for T objects
		}
		return 0;
	}

	static int tostring_T (lua_State* luaState)
	{
		char buff[32];
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(luaState, 1));
		T *obj = ud->pT;
		sprintf(buff, "%p", obj);
		lua_pushfstring(luaState, "%s (%s)", T::mClassName, buff);
		return 1;
	}

	static void set(lua_State* luaState, int table_index, const char *key)
	{
		lua_pushstring(luaState, key);
		lua_insert(luaState, -2);  // swap value and key
		lua_settable(luaState, table_index);
	}

	static void weaktable(lua_State* luaState, const char* mode)
	{
		lua_newtable(luaState);
		lua_pushvalue(luaState, -1);  // table is its own metatable
		lua_setmetatable(luaState, -2);
		lua_pushliteral(luaState, "__mode");
		lua_pushstring(luaState, mode);
		lua_settable(luaState, -3);   // metatable.__mode = mode
	}

	static void subtable(lua_State* luaState, int tindex, const char* name, const char* mode)
	{
		lua_pushstring(luaState, name);
		lua_gettable(luaState, tindex);

		if (lua_isnil(luaState, -1))
		{
			lua_pop(luaState, 1);
			lua_checkstack(luaState, 3);
			weaktable(luaState, mode);
			lua_pushstring(luaState, name);
			lua_pushvalue(luaState, -2);
			lua_settable(luaState, tindex);
		}
	}

	static void* pushuserdata(lua_State* luaState, void* key, size_t sz)
	{
		void* ud = 0;
		lua_pushlightuserdata(luaState, key);
		lua_gettable(luaState, -2);     // lookup[key]

		if (lua_isnil(luaState, -1))
		{
			lua_pop(luaState, 1);         // drop nil
			lua_checkstack(luaState, 3);
			ud = lua_newuserdata(luaState, sz);  // create new userdata
			lua_pushlightuserdata(luaState, key);
			lua_pushvalue(luaState, -2);  // dup userdata
			lua_settable(luaState, -4);   // lookup[key] = userdata
		}
		return ud;
	}
};

#endif

