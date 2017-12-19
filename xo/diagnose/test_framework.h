#pragma once

#include "xo/system/platform.h"
#include "xo/utility/types.h"

#define XO_TEST( operation ) xo::test_framework::get_instance().test( #operation, operation )
#define XO_TEST_MSG( operation, message ) xo::test_framework::get_instance().test( #operation, operation, message )
#define XO_TEST_REPORT xo::test_framework::get_instance().report

namespace xo
{
	class XO_API test_framework
	{
	public:
		test_framework();
		bool test( const char* name, bool result, const string& message = "" );
		void reset();
		int report();

		int num_tests;
		int num_failed;
		int num_passed;

		bool show_passed = false;

		static test_framework& get_instance();
		static test_framework test_;
	};

}