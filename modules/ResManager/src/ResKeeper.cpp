#include "ResKeeper.h"

#include <Entity.h>
#include <IFileMonitorManager.h>

#include "Res.h"
#include "IResFactory.h"
#include "ResManager.h"

namespace flappy {

using namespace std;

ResKeeper::ResKeeper(shared_ptr<Res> res, bool changed):
    m_changed(changed),
    m_res(res)
{
}

void ResKeeper::cleanUpRes()
{
    while ((m_res != m_res->nextRes()) && (m_res.use_count() == 1))
        m_res = m_res->nextRes();
}

bool ResKeeper::needRemove()
{
    return (m_res == m_res->nextRes()) && (m_res.use_count() == 1);
}

bool ResKeeper::dependencyChanged() const
{
    auto actualRes = m_res->nextRes();
    for (auto dependency: actualRes->dependencyList())
        if (dependency->resUpdated())
            return true;
    return false;
}

std::shared_ptr<Res> ResKeeper::actualRes()
{
    return m_res->nextRes();
}

void ResKeeper::updateRes(SafePtr<IResFactory> resFactory, const std::string& name, SafePtr<IFileMonitorManager> monitor)
{
    // check res changed
    m_changed = m_changed || resFactory->changed(name);
    // check dependencies changed
    m_changed = m_changed || dependencyChanged();
    // reload if anything is changed
    if (m_changed) {
        try { // catch all possible error while resource loading
            auto loadedRes = resFactory->load(name);
            if (loadedRes != nullptr) {
                m_res->pushRes(loadedRes);
                m_changed = false;
            }
        }
        catch (exception& e) {
            LOGE("Unexpected error during resource loading.\nDescription:\n%s", e.what());
            m_changed = false;
        }
        catch (...) {
            LOGE("Unexpected error during resource loading.");
            m_changed = false;
        }
    }
}


} // flappy
