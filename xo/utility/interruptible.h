#pragma once

#include "xo/system/platform.h"

namespace xo
{
	class XO_API interruptible
	{
	public:
		interruptible();
		virtual ~interruptible();

		virtual void interrupt() const;
		bool test_interrupt_flag() const;

	private:
		mutable bool interrupt_flag_;
	};
}
