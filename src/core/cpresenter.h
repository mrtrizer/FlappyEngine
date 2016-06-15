#ifndef CPRESENTER_H
#define CPRESENTER_H

#include <memory>

#include "gpresenter.h"

class CPresenter
{
public:
    CPresenter();
    inline void setPresenter(std::shared_ptr<GPresenter> presenter) {
        this->presenter = presenter;
    }
    inline std::shared_ptr<GPresenter> getPresenter() const {
        return presenter;
    }

private:
    std::shared_ptr<GPresenter> presenter;
};

#endif // CPRESENTER_H
