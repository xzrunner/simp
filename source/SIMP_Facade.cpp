#include "SIMP_Facade.h"
#include "NodeFactory.h"
#include "PageAlloc.h"
#include "PkgIDMgr.h"
#include "RelocateTexcoords.h"

namespace simp
{

void Facade::Clear()
{
	NodeFactory::Instance()->Clear();
	PageAlloc::Instance()->Clear();
	PkgIDMgr::Instance()->Clear();
	RelocateTexcoords::Instance()->Clear();
}

}