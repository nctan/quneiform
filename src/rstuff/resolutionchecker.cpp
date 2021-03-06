/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <string.h>
#include <iostream>

#include "resolutionchecker.h"
#include "componenthistogram.h"
#include "common/log.h"
#include "cpage/cpage.h"

namespace cf {

static inline bool isValidResolution(const PAGEINFO& info)
{
    static const size_t MINIMAL_RESOLUTION = 99;
    return info.DPIX > MINIMAL_RESOLUTION && info.DPIY > MINIMAL_RESOLUTION;
}

ResolutionChecker::HistogramCallback ResolutionChecker::hist_height_callback_;
ResolutionChecker::HistogramCallback ResolutionChecker::hist_width_callback_;

ResolutionChecker::ResolutionChecker(CCOM_handle ccom, Handle cpage) :
    ccom_(ccom), cpage_(cpage)
{
}

void ResolutionChecker::check()
{
    PAGEINFO page_info;
    if(!GetPageInfo(cpage_, &page_info))
        return;

    if(isValidResolution(page_info)) {
        cfDebug(cf::MODULE_RSTUFF) << "no resolution correction needed: "
                                   << page_info.DPIX << "x" << page_info.DPIY;
        return;
    }

    ComponentHistogram hist;
    hist.fill(ccom_);
    hist.calculate();

    bool changed = false;

    if(hist.isYCorrectionNeeded(page_info)) {
        page_info.DPIY = hist.yDpi();
        changed = true;
    }

    if(hist.isXCorrectionNeeded(page_info)){
        page_info.DPIX = hist.xDpi();
        changed = true;
    }

    if(changed) {
        SetPageInfo(cpage_, page_info);
        cfDebug(cf::MODULE_RSTUFF) << "new resolution: "
                                   << page_info.DPIX << "x" << page_info.DPIY;
    }

    if(hist_height_callback_)
        hist_height_callback_(hist.heightHistogram());

    if(hist_width_callback_)
        hist_width_callback_(hist.widthHistogram());
}

void ResolutionChecker::setHistogramHeightCallback(const HistogramCallback& clb)
{
    hist_height_callback_ = clb;
}

void ResolutionChecker::setHistogramWidthCallback(const HistogramCallback& clb)
{
    hist_width_callback_ = clb;
}

}
