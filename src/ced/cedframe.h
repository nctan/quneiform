/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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

#ifndef CEDFRAME_H_
#define CEDFRAME_H_

#include "globus.h"
#include "blockelement.h"

namespace cf
{

class CEDParagraph;
class CEDPicture;

class CLA_EXPO CEDFrame: public BlockElement
{
    public:
        enum vposition_t
        {
            /** y position relating top page border */
            VPOS_PAGE = 0,
            /** y position relating top page margin */
            VPOS_MARGIN = 1,
            /** y position relating */
            VPOS_PARAGRAPH = 2
        };

        enum hposition_t
        {
            /** x position relating left page border */
            HPOS_PAGE = (0 << 4),
            /** x position relating left page margin */
            HPOS_MARGIN = (1 << 4),
            /** x position relating column left border */
            HPOS_COLUMN = (2 << 4)
        };

        CEDFrame(BlockElement * parent = NULL, hposition_t hpos = HPOS_COLUMN, vposition_t vpos =
                VPOS_PARAGRAPH);

        /**
         * Returns pointer to deep frame copy
         */
        CEDFrame * clone() const;

        void exportElement(CEDExporter& exp);

        int dxfrtextx;
        int dxfrtexty;

        void setBorderSpace(int borderSpace);

        /**
         * Sets horizontal frame snap position
         */
        void setHPosition(hposition_t pos);

        /**
         * Sets vertical frame snap position
         */
        void setVPosition(vposition_t pos);
    protected:
        CEDFrame(const CEDFrame& frame);
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar.template register_type<CEDParagraph>();
            ar.template register_type<CEDPicture>();
            ar & make_nvp("block-element", boost::serialization::base_object<BlockElement>(*this));
            ar & make_nvp("border-space", border_space_);
            ar & make_nvp("h-position", hpos_);
            ar & make_nvp("v-position", vpos_);
        }
#endif
    private:
        int border_space_;
        hposition_t hpos_;
        vposition_t vpos_;
};

}

#endif /* CEDFRAME_H_ */
