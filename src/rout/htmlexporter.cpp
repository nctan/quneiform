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

#include <cassert>
#include "htmlexporter.h"
#include "ced/cedint.h"
#include "rout_own.h"

#ifdef USE_ICONV
#include "common/iconv_local.h"
#endif

namespace CIF
{

HtmlExporter::HtmlExporter(CEDPage * page, const FormatOptions& opts) :
    GenericExporter(page, opts), converter_(0), lines_left_(0), current_font_style_(0) {

    setEncodings();

#ifdef USE_ICONV
    if(isCharsetConversionNeeded()) {
        converter_ = new Iconv(inputEncoding(), outputEncoding());
    }
#endif

    setSkipEmptyLines(true);
    setSkipEmptyParagraphs(true);
}

HtmlExporter::~HtmlExporter() {
#ifdef USE_ICONV
    delete converter_;
#endif
}

std::string HtmlExporter::escapeHtmlSpecialChar(uchar code) {
    switch (code) {
    case '>':
        return "&gt;";
    case '<':
        return "&lt;";
    case '&':
        return "&amp;";
    case '"':
        return "&quot;";
    default:
        return std::string(1, code);
    }
}

void HtmlExporter::setEncodings() {
    switch (formatOptions().language()) {
    case LANGUAGE_RUSSIAN:
    case LANGUAGE_RUS_ENG:
    case LANGUAGE_UKRAINIAN:
        setInputEncoding("cp1251");
        setOutputEncoding("utf-8");
    default:
        ;
    }
}

void HtmlExporter::writeCharacter(std::ostream& os, CEDChar * chr) {
    assert(chr && chr->alternatives);

    writeFontStyle(os, chr->fontAttribs);

#ifdef USE_ICONV
    if(isCharsetConversionNeeded())
    os << converter_->convert(escapeHtmlSpecialChar(chr->alternatives->alternative));
    else
    os << escapeHtmlSpecialChar(chr->alternatives->alternative);
#else
    os << escapeHtmlSpecialChar(chr->alternatives->alternative);
#endif
}

void HtmlExporter::writeDoctype(std::ostream& os) {
    os << "<!DOCTYPE html PUBLIC "
        "\"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
        "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
}

void HtmlExporter::writeFontStyle(std::ostream& os, long newStyle) {
    if ((newStyle & FONT_BOLD) && (!(current_font_style_ & FONT_BOLD) || (current_font_style_
            & FONT_LIGHT))) {
        os << "<b>";
    } else if ((current_font_style_ & FONT_BOLD) && (!(newStyle & FONT_BOLD) || (newStyle
            & FONT_LIGHT))) {
        os << "</b>";
    }

    if ((newStyle & FONT_ITALIC) && (!(current_font_style_ & FONT_ITALIC))) {
        os << "<i>";
    } else if ((current_font_style_ & FONT_ITALIC) && (!(newStyle & FONT_ITALIC))) {
        os << "</i>";
    }

    if ((newStyle & FONT_UNDERLINE) && !(current_font_style_ & FONT_UNDERLINE)) {
        os << "<u>";
    } else if ((current_font_style_ & FONT_UNDERLINE) && !(newStyle & FONT_UNDERLINE)) {
        os << "</u>";
    }

    // save font style
    current_font_style_ = newStyle;
}

void HtmlExporter::writeLineEnd(std::ostream& os, CEDLine * line) {
    lines_left_--;
    if (lines_left_ < 1)
        return;

    if (formatOptions().preserveLineBreaks() || line->hardBreak)
        os << "<br/>";
    os << "\n";
}

void HtmlExporter::writeMeta(std::ostream& os) {
    Attributes attrs;
    attrs["name"] = "Generator";
    attrs["content"] = "Cuneiform";
    writeSingleTag(os, "meta", attrs);
    os << "\n";

#ifdef USE_ICONV
    if(isCharsetConversionNeeded()) {
        attrs["name"] = "Content-Type";
        attrs["content"] = "text/html;charset=" + outputEncoding();
        writeSingleTag(os, "meta", attrs);
        os << "\n";
    }
#endif
}

void HtmlExporter::writePageBegin(std::ostream& os) {
    writeDoctype(os);
    os << "<html>\n<head>\n";
    writeTitle(os);
    writeMeta(os);
    os << "</head>\n<body>\n";
    writeFontStyle(os, 0);

    if (os.fail())
        throw Exception("HtmlExporter failed");
}

void HtmlExporter::writePageEnd(std::ostream& os) {
    writeFontStyle(os, 0);
    os << "</body>\n</html>\n";
}

void HtmlExporter::writeParagraphBegin(std::ostream& os, CEDParagraph * par) {
    assert(par);

    Attributes attrs;
    switch (par->alignment & ALIGN_MASK) {
    case ALIGN_CENTER:
        attrs["align"] = "center";
        break;
    case (ALIGN_LEFT | ALIGN_RIGHT):
        attrs["align"] = "justify";
        break;
    case ALIGN_RIGHT:
        attrs["align"] = "right";
    default:
        // "left" by default
        break;
    }

    writeFontStyle(os, 0);
    writeStartTag(os, "p", attrs);

    lines_left_ = par->GetCountLine();
}

void HtmlExporter::writeParagraphEnd(std::ostream& os, CEDParagraph * /*par*/) {
    writeFontStyle(os, 0);
    os << "</p>\n";
}

void HtmlExporter::writePicture(std::ostream& os, CEDChar * picture) {
    std::string path = savePicture(picture);
    Attributes attrs;
    attrs["src"] = path;
    attrs["alt"] = "";
    writeSingleTag(os, "img", attrs);
}

void HtmlExporter::writeTableBegin(std::ostream& os, CEDParagraph * /*table*/) {
    writeFontStyle(os, 0);
    os << "<table>";
}

void HtmlExporter::writeTableEnd(std::ostream& os, CEDParagraph * /*table*/) {
    os << "</table>";
}

void HtmlExporter::writeAttributes(std::ostream& os, const Attributes& attrs) {
    for (Attributes::const_iterator it = attrs.begin(), end = attrs.end(); it != end; ++it)
        os << " " << it->first << "=\"" << it->second << "\"";
}

void HtmlExporter::writeSingleTag(std::ostream& os, const std::string& tagName,
        const Attributes& attrs) {
    os << "<" << tagName;
    writeAttributes(os, attrs);
    os << "/>";
}

void HtmlExporter::writeStartTag(std::ostream& os, const std::string& tagName) {
    os << "<" << tagName << ">";
}

void HtmlExporter::writeStartTag(std::ostream& os, const std::string& tagName,
        const Attributes& attrs) {
    os << "<" << tagName;
    writeAttributes(os, attrs);
    os << ">";
}

void HtmlExporter::writeTitle(std::ostream& os) {
    os << "<title>Cuneiform output</title>\n";
}

}
