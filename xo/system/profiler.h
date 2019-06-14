#pragma once

#include "xo/xo_types.h"
#include "xo/system/system_tools.h"
#include "xo/time/timer.h"
#include <thread>
#include <vector>

namespace xo
{
	class XO_API profiler
	{
	public:
		profiler( bool auto_start = false );
		~profiler();

		void start( const char* label = "TOTAL" );
		void stop();
		prop_node report( double minimum_expand_percentage = 0.5 );

		bool enabled() const { return enabled_; }

		static profiler& instance();

		friend struct scope_profiler;

	private:
		struct section
		{
			section( const char* n, size_t i, size_t pi ) :
				name( n ), id( i ), parent_id( pi ), total_time( 0 ), overhead( 0 ), count( 0 ) {}
			const char* name;
			size_t id;
			size_t parent_id;
			time total_time;
			time overhead;
			size_t count;
			time epoch;
		};

		time now() const { return timer_(); }
		void init_overhead_estimate();
		section* start_section( const char* name );
		void end_section();
		void report_section( section* s, prop_node& pn, double minimum_expand_percentage );
		time exclusive_time( section* s );
		time total_overhead( section* s );
		section* root() { return &sections_.front(); }
		section* find_section( size_t id );
		section* find_section( const char* name, size_t parent_id );
		section* acquire_section( const char* name, size_t parent_id );
		section* add_section( const char* name, size_t parent_id );
		std::vector< section* > get_children( size_t parent_id );

	private:
		std::vector< section > sections_;
		timer timer_;
		bool enabled_;
		section* current_section_;
		time overhead_estimate;
		std::thread::id instance_thread_;
	};

	struct scope_profiler
	{
		scope_profiler( const char* name, profiler& p = profiler::instance() ) : profiler_( p )
		{ if ( profiler_.enabled() ) profiler_.start_section( name ); }
		~scope_profiler()
		{ if ( profiler_.enabled() ) profiler_.end_section(); }
		profiler& profiler_;
	};
}