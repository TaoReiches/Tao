#pragma once

struct BeMainPtr
{
	BeMainPtr(void) :pkAttachMain(nullptr)
	{
	}

	void AttachMain(void* pkMain)
	{
		pkAttachMain = pkMain;
	}

	void* pkAttachMain;
};
