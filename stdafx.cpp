#include "stdafx.h"
#include "Log.h"

using namespace D3D11Framework;

int main()
{
	Log* log = Log::Get();

	log->Print("%s", "kek");
}