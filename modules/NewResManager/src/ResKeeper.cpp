#include "ResKeeper.h"

#include <Entity.h>
#include <IFileMonitorManager.h>

#include "Res.h"
#include "IResFactory.h"
#include "ResManager.h"

namespace flappy {

ResKeeper::ResKeeper(SafePtr<IResFactory> resFactory, std::string name)
    : m_resFactory (resFactory)
    , m_name(name)
{
    try { // catch all possible exceptions while resource creating
        m_res = m_resFactory->create(m_name);
        LOGI("Resource %s created",  m_name.c_str());
    } catch (std::exception& e) {
        LOGE("Can't create resource %s", name.c_str());
        throw std::runtime_error(std::string("Default resource create error.\nDescription:\n") + e.what());
    }
    catch (...) {
        LOGE("Can't create resource %s", name.c_str());
        throw std::runtime_error("Default resource create error.");
    }
}


bool ResKeeper::needRemove()
{
    return (m_res == m_res->nextResBase()) && (m_res.use_count() == 1);
}

void ResKeeper::cleanUpRes()
{
    while ((m_res != m_res->nextResBase()) && (m_res.use_count() == 1))
        m_res = m_res->nextResBase();
}

bool ResKeeper::dependencyChanged()
{
    for (auto dependency: actualRes(ExecType::ASYNC)->dependencyList())
        if ((dependency != nullptr) && dependency->resUpdated())
            return true;
    return false;
}

std::shared_ptr<ResBase> ResKeeper::actualRes(ExecType execType)
{
    if (execType == ExecType::SYNC) {
        auto loadedRes = m_resFactory->load(m_name, ExecType::SYNC);
        if (loadedRes != nullptr) {
            m_res->pushRes(loadedRes);
        }
    }
    return m_res->lastResBase();
}

void ResKeeper::updateRes()
{
    // check res or dependencies changed
    bool changed = m_resFactory->changed(m_name) || dependencyChanged() || !m_loaded;

    // reload if anything is changed
    if (changed) {
        try { // swallow all possible exceptions while resource loading
            auto loadedRes = m_resFactory->load(m_name, ExecType::ASYNC);
            if (loadedRes != nullptr) {
                m_res->pushRes(loadedRes);
                m_loaded = true;
                LOGI("Resource %s updated",  m_name.c_str());
            } else {
                LOGW("Resource %s update is rejected by factory",  m_name.c_str());
            }
        }
        catch (std::exception& e) {
            LOGE("Unexpected error during resource loading.\nDescription:\n%s", e.what());
        }
        catch (...) {
            LOGE("Unexpected error during resource loading.");
        }
    }
}


} // flappy
