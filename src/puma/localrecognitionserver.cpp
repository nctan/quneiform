/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <iostream>

#include "ced/cedpage.h"
#include "rdib/imageloaderfactory.h"
#include "localrecognitionserver.h"
#include "puma.h"
#include "common/cifconfig.h"
#include "common/percentcounter.h"
#include "common/recognitionstate.h"
#include "common/console_messages.h"

#define CF_ERROR std::cerr << cf::console::error

namespace cf {

LocalRecognitionServer::LocalRecognitionServer() {
}

LocalRecognitionServer::~LocalRecognitionServer() {
    if(isTextDebug())
        Puma::instance().close();
}

void LocalRecognitionServer::close(const RecognizeOptions& ropts)
{
    // for normal formatting - we have to close puma library,
    // except textdebug
    if(!isTextDebug() && !ropts.debugCleanupDelayed())
        Puma::instance().close();
}

CEDPagePtr LocalRecognitionServer::format() {
    if(!isTextDebug())
        Puma::instance().formatResult();

    if(counter_)
        counter_->add(10);

    if(state_)
        state_->set(RecognitionState::FORMATTED);

    return Puma::instance().cedPage();
}

void LocalRecognitionServer::handleExceptionCommon(std::exception& e,
                                                   const RecognizeOptions& ropts)
{
    CF_ERROR << e.what() << std::endl;

    if(!ropts.debugCleanupDelayed())
        Puma::instance().close();

    if(state_)
        state_->set(RecognitionState::FAILED);
}

void LocalRecognitionServer::open(ImagePtr image) {
    Puma::instance().open(image);

    if(counter_)
        counter_->add(20);

    if(state_)
        state_->set(RecognitionState::OPENED);
}

void LocalRecognitionServer::doRecognize() {
    Puma::instance().recognize();

    if(counter_)
        counter_->add(70);

    if(state_)
        state_->set(RecognitionState::RECOGNIZED);
}

CEDPagePtr LocalRecognitionServer::recognize(const std::string& imagePath,
                                             const RecognizeOptions& ropts,
                                             const FormatOptions& fopts)
{
    try {
        if(imagePath.empty()) {
            throw RecognitionException("LocalRecognitionServer::recognize() : empty image path",
                                       FILE_NOT_FOUND);
        }

        ImagePtr img = ImageLoaderFactory::instance().load(imagePath);
        return recognize(img, ropts, fopts);
    }
    catch(RecognitionException& e) {
        handleExceptionCommon(e, ropts);
        throw e;
    }
    catch(ImageLoader::Exception& e) {
        handleExceptionCommon(e, ropts);
        throw RecognitionException(e.what(), IMAGE_LOAD_ERROR);
    }
    catch(std::exception& e) {
        handleExceptionCommon(e, ropts);
        throw RecognitionException(e.what());
    }
}

CEDPagePtr LocalRecognitionServer::recognize(ImagePtr image,
                                             const RecognizeOptions& ropts,
                                             const FormatOptions& fopts)
{
    try {
        if (!image.get()) {
            throw RecognitionException("LocalRecognitionServer::recognize() : NULL image given",
                                       IMAGE_LOAD_ERROR);
        }

        if(counter_)
            counter_->reset();
        if(state_)
            state_->reset();

        setOptions(ropts, fopts);
        open(image);
        doRecognize();
        CEDPagePtr page = format();
        close(ropts);

        // set filename
        if(page)
            page->setImageName(image->fileName());

        return page;
    }
    catch(RecognitionException& e) {
        handleExceptionCommon(e, ropts);
        throw e;
    }
    catch(PumaException& e) {
        handleExceptionCommon(e, ropts);
        throw RecognitionException(e.what(), RECOGNITION_ERROR);
    }
    catch (std::exception& e) {
        handleExceptionCommon(e, ropts);
        throw RecognitionException(e.what(), UNKNOWN);
    }
}

void LocalRecognitionServer::setOptions(const RecognizeOptions& ropts, const FormatOptions& fopts)
{
    Puma::instance().setRecognizeOptions(ropts);
    Puma::instance().setFormatOptions(fopts);
}

}
