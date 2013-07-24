#!/bin/sh

PROJECT=$(cd $(dirname $0) && pwd)
includes="Callback Config.in LoadWrapper LuaInclude Script Value"
astyle -n "${PROJECT}/src/osgLua/*.cpp" "${PROJECT}/src/osgLua/*.h"
for fn in ${includes}; do
	astyle -n "${PROJECT}/include/osgLua/${fn}"
done
