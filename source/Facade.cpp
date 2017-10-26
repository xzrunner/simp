#include "simp/Facade.h"
#include "simp/NodeFactory.h"
#include "simp/PageAlloc.h"
#include "simp/PkgIDMgr.h"
#include "simp/RelocateTexcoords.h"

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