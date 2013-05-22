/**
	@file
	@brief Implementation

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifdef OSGLUA_HAVE_BOOST

// Internal Includes
#include "RegisterMathMetamethods.h"
#include "UnaryOperatorDispatch.h"
#include "BinaryOperatorDispatch.h"
#include "OperatorMetamethodTraits.h"
#include "PrintInfoFunctor.h"
#include "StatefulTypeVisitFunctor.h"
#include <osgLua/LuaInclude>
#include <osgLua/Value>

#include <osgTraits/Operators.h>
#include <osgTraits/OperatorArity.h>
#include <osgTraits/IsOperatorAvailable.h>
#include <osgTraits/MathAndArithmeticTypes.h>

// Library/third-party includes
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/identity.hpp>

// Standard includes
// - none

namespace osgLua {
	//typedef boost::mpl::list3<osgTraits::Addition, osgTraits::Subtraction, osgTraits::Equality> SpecializedOperators;
	typedef osgTraits::MathOperators SpecializedOperators;
	//typedef boost::mpl::single_view<osg::Vec3d> SpecializedTypes;
	typedef osgTraits::math_types SpecializedTypes;

	template<typename T, typename Operator>
	inline void reportRegistration(bool applicable) {
		std::cerr << (applicable ? "Registering " : "Skipping ") << getTypeName<T>() << " metamethod " << MetamethodName<Operator>::get() << " (" << MetamethodName<Operator>::getSymbol() << ")" << std::flush << std::endl;
	}

	struct RegistrationData {
		RegistrationData(lua_State * _L, introspection::Type const& t) : L(_L), metatableType(t), foundOurType(false) {}
		lua_State * L;
		introspection::Type const& metatableType;
		bool foundOurType;
	};
	namespace detail {
		namespace mpl = boost::mpl;
		using namespace boost::mpl::placeholders;
		using boost::enable_if;
		using boost::disable_if;
		template<typename T>
		struct type_visitor {
			/// @brief Nested visitor template: for a given type, this is
			/// applied with each of the operators, and the lone useful function
			/// registers that operator if applicable.
			template<typename Operator>
			struct operator_visitor {
				static void visit(RegistrationData const& d) {
					reportRegistration<T, Operator>(true);
					lua_pushcfunction(d.L, &(AttemptOperator<Operator, T>::attempt));
					lua_setfield(d.L, -2, MetamethodName<Operator>::get());
				}
			};

			/// @brief Function called for each known math type: if it matches
			/// the type we're registering, we proceed on to visit all operators.
			static void visit(RegistrationData & d) {
				std::cerr << "Trying " << getTypeName<T>() << std::flush << std::endl;
				if (!d.foundOurType && introspection::Reflection::getType(extended_typeid<T>()) == d.metatableType) {
					d.foundOurType = true;
					typedef typename osgTraits::get_applicable_operators<T, SpecializedOperators>::type ApplicableOperators;
					boost::mpl::for_each<ApplicableOperators, operator_visitor<boost::mpl::_1> >(util::visitorState(d));
				}
			}
		};
	} // end of namespace detail

	bool registerMathMetamethods(lua_State * L, introspection::Type const& t) {
		RegistrationData data(L, t);
		std::cerr << "Attempting to register " << t.getQualifiedName() << std::flush << std::endl;
		boost::mpl::for_each<SpecializedTypes, detail::type_visitor<boost::mpl::_1> >(util::visitorState(data));
		return data.foundOurType;
	}

} // end of namespace osgLua

#endif
