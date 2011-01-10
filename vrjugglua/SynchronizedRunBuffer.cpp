/**	@file	SynchronizedRunBuffer.cpp
	@brief	implementation

	@date
	2009-2010

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

// Local includes
#define NEED_RUNBUFFER_LUABIND_OBJECT
#include "SynchronizedRunBuffer.h"

// Library/third-party includes
// - none

// Standard includes
#include <stdexcept>

namespace vrjLua {

	SynchronizedRunBuffer::SynchronizedRunBuffer(luabind::object const& delegate) :
			_init(false) {
		_state = delegate.interpreter();
	}


	SynchronizedRunBuffer::SynchronizedRunBuffer(LuaStatePtr const& state) :
			_init(false) {
		/// @todo own this state? How?
		_state = state.get();
	}

	SynchronizedRunBuffer::SynchronizedRunBuffer(LuaScript const& script) :
			_init(false) {	
		/// @todo own this state? How?
		_state = script.getLuaState().lock().get();
	}
	
	void SynchronizedRunBuffer::init() {
		if (_init) {
			throw std::logic_error("SynchronizedRunBuffer already initialized!");
		}
		
		/// Initialize run buffer's shared data ID
		_runBuf.init(LuaRunBuffer::getGUID());
		// Tell the run buffer what our state pointer is.
		_runBuf->initLua(_state);
		
		_init = true;
	}

	bool SynchronizedRunBuffer::isLocal() {
		_checkInit();
		return _runBuf.isLocal();
	}

	bool SynchronizedRunBuffer::addFile(std::string const& filename, bool blocking) {
		if (_runBuf.isLocal()) {
			_checkInit();
			return _runBuf->addFile(filename, blocking);
		} else {
			return false;
		}
	}

	bool SynchronizedRunBuffer::addString(std::string const& str, bool blocking) {
		if (_runBuf.isLocal()) {
			_checkInit();
			return _runBuf->addString(str, blocking);
		} else {
			return false;
		}
	}

	unsigned int SynchronizedRunBuffer::runBuffer() {
		_checkInit();
		return _runBuf->runBuffer();
	}
	
	void SynchronizedRunBuffer::_checkInit() {
		if (!_init) {
			std::cerr << "WARNING: init was not called on the run buffer during init/initScene - cluster support won't work" << std::endl;
			init();
		}
	}

}// end of vrjLua namespace
