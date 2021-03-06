/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#include <fstream>
#include <limits>
#include <algorithm>
#include <cassert>

#include "imageloaderfactory.h"
#include "imageloader.h"
#include "rdib_debug.h"
#include "common/imageurl.h"
#include "imageformatdetector.h"
#include "cfcompat.h"

namespace cf
{

ImageLoaderFactory::ImageLoaderFactory()
{
}

ImageLoaderFactory& ImageLoaderFactory::instance()
{
    static ImageLoaderFactory factory;
    return factory;
}

ImagePtr ImageLoaderFactory::load(const ImageURL& url)
{
    if(!url.exists())
        throw ImageLoader::Exception("ImageLoaderFactory: file not exists:") << url;

    image_format_t format = ImageFormatDetector::instance().detect(url);
    ImagePtr ret(loader(format).load(url));
    assert(ret.get());
    ret->setFileName(url.path());
    return ret;
}

ImagePtr ImageLoaderFactory::load(std::istream& stream)
{
    image_format_t format = ImageFormatDetector::instance().detect(stream);
    return loader(format).load(stream);
}

ImageLoader& ImageLoaderFactory::loader(image_format_t format)
{
    if (loader_map_.find(format) == loader_map_.end()) {
        if (format == FORMAT_UNKNOWN)
            throw std::runtime_error("ImageLoaderFactory:: no default loader registered");

        RDIB_WARNING_FUNC << "ImageLoaderFactory: loader not registered for format:" << format
                          << "trying default...";

        return unknownLoader();
    }
    
#ifdef _WIN32
#undef min
#endif

    std::pair<LoaderMap::iterator, LoaderMap::iterator> loaders = loader_map_.equal_range(format);
    int priority = std::numeric_limits<int>::min();
    LoaderMap::iterator loader_ptr = loader_map_.end();
    for (LoaderMap::iterator it = loaders.first; it != loaders.second; ++it) {
        if (it->second.first > priority) {
            priority = it->second.first;
            loader_ptr = it;
        }
    }
    assert(loader_ptr != loader_map_.end());
    loaders_list_.push_back(ImageLoaderPtr(loader_ptr->second.second()));
    return *loaders_list_.back().get();
}

bool ImageLoaderFactory::registerCreator(image_format_t format, int priority, loaderCreate creator)
{
    loader_map_.insert(LoaderMap::value_type(format, std::make_pair(priority, creator)));

    RDIB_TRACE_FUNC << "format:" << imageFormatToString(format)
                    << "with priority:" << priority << creator;

    return true;
}

ImageFormatList ImageLoaderFactory::supportedFormats() const {
    ImageFormatList res;

    for(LoaderMap::const_iterator it = loader_map_.begin(), end = loader_map_.end(); it != end; ++it) {
        // skip unknown
        if(it->first == FORMAT_UNKNOWN)
            continue;

        res.push_back(it->first);
    }

    std::sort(res.begin(), res.end());

    ImageFormatList::iterator last = std::unique(res.begin(), res.end());
    res.erase(last, res.end());

    return res;
}

ImageLoader& ImageLoaderFactory::unknownLoader()
{
    return loader(FORMAT_UNKNOWN);
}

}
