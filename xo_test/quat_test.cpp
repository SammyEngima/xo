#include "xo/system/test_case.h"
#include "xo/geometry/quat.h"
#include "xo/geometry/vec3.h"
#include "xo/numerical/random.h"
#include "test_tools.h"

namespace xo
{
	template< typename T > quat_<T> quat_from_directions_old( const vec3_<T>& source, const vec3_<T>& target ) {
		vec3_<T> s = normalized( source );
		vec3_<T> t = normalized( target );
		vec3_<T> c = cross_product( s, t );
		T d = dot_product( s, t );

		if ( equal( d, T( 1 ) ) ) // check if vectors are the same
			return quat_<T>::identity();

		auto clen = length( c );
		if ( equal( clen, T( 0 ) ) ) // check if vectors are 180 deg apart
			return quat_<T>( 0, 1, 0, 0 ); // this doesn't work if source is unit_x

		c /= clen;
		auto a = std::acos( d ) * T( 0.5 );
		auto sa = std::sin( a );

		return quat_<T>( std::cos( a ), c.x * sa, c.y * sa, c.z * sa );
	}

	XO_TEST_CASE( xo_quat_axes_test )
	{
		auto q = quat_from_euler( 20_deg, -30_deg, 40_deg );
		auto qx = local_x_axis( q );
		auto qy = local_y_axis( q );
		auto qz = local_z_axis( q );
		auto qx1 = q * vec3d::unit_x();
		auto qy1 = q * vec3d::unit_y();
		auto qz1 = q * vec3d::unit_z();
		auto qxyz = axes_from_quat( q );
		XO_CHECK( equal( qx, qx1 ) );
		XO_CHECK( equal( qy, qy1 ) );
		XO_CHECK( equal( qz, qz1 ) );
		XO_CHECK( qxyz.x == qx );
		XO_CHECK( qxyz.y == qy );
		XO_CHECK( qxyz.z == qz );
	}

	XO_TEST_CASE( xo_quat_from_directions )
	{
		random_number_generator rng;
		for ( index_t i = 0; i < 10; ++i )
		{
			auto v1 = normalized( rand_vec3<float>( -1, 1, rng ) );
			auto v2 = normalized( rand_vec3<float>( -1, 1, rng ) );
			auto q1 = quat_from_directions( v1, v2 );
			auto q2 = quat_from_directions( v2, v1 );
			auto q3 = quat_from_directions( v2, -v2 );
			auto q4 = quat_from_directions( v1, v1 );
			auto q1o = quat_from_directions_old( v1, v2 );
			auto q2o = quat_from_directions_old( v2, v1 );
			auto q3o = quat_from_directions_old( v2, -v2 );
			auto q4o = quat_from_directions_old( v1, v1 );
			XO_CHECK( equal( q1, q1o ) );
			XO_CHECK( equal( q2, q2o ) );
			XO_CHECK( equal( q3, q3o ) );
			XO_CHECK( equal( q4, q4o ) );
		}
	}
}
