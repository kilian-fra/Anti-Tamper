#pragma once

#include "VMContext.hpp"

class VMDispatcher
{
	VMContext m_VmContext;

public:

	VMDispatcher(const VMContext& vmContext);

	void dispatch();
};