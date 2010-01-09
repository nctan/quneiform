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
#include <iomanip>

#include "formatoptions.h"

namespace CIF
{

FormatOptions::FormatOptions() :
    serif_name_("Times New Roman"), sans_serif_name_("Arial"), monospace_name_("Courier New"),
            use_bold_(true), use_italic_(true), use_font_size_(true), preserve_line_breaks_(false),
            format_mode_(PUMA_FORMAT_ALL), unrecognized_char_('~'), language_(LANGUAGE_RUS_ENG)
{
}

FormatOptions::~FormatOptions()
{
}

puma_format_mode_t FormatOptions::formatMode() const
{
    return format_mode_;
}

bool FormatOptions::isBoldUsed() const
{
    return use_bold_;
}

bool FormatOptions::isFontSizeUsed() const
{
    return use_font_size_;
}

bool FormatOptions::isItalicUsed() const
{
    return use_italic_;
}

language_t FormatOptions::language() const
{
    return language_;
}

std::string FormatOptions::monospaceName() const
{
    return monospace_name_;
}

bool FormatOptions::preserveLineBreaks() const
{
    return preserve_line_breaks_;
}

std::string FormatOptions::sansSerifName() const
{
    return sans_serif_name_;
}

std::string FormatOptions::serifName() const
{
    return serif_name_;
}

void FormatOptions::setFormatMode(puma_format_mode_t format)
{
    format_mode_ = format;
}

void FormatOptions::setLanguage(language_t lang)
{
    language_ = lang;
}

void FormatOptions::setMonospaceName(const std::string & name)
{
    monospace_name_ = name;
}

void FormatOptions::setPreserveLineBreaks(bool val) {
    preserve_line_breaks_ = val;
}

void FormatOptions::setSansSerifName(const std::string & name)
{
    sans_serif_name_ = name;
}

void FormatOptions::setSerifName(const std::string & name)
{
    serif_name_ = name;
}

void FormatOptions::setUnrecognizedChar(wchar_t ch)
{
    unrecognized_char_ = ch;
}

void FormatOptions::setUnrecognizedChar(char ch)
{
    unrecognized_char_ = ch;
}

wchar_t FormatOptions::unrecognizedChar() const
{
    return unrecognized_char_;
}

void FormatOptions::useBold(bool val)
{
    use_bold_ = val;
}

void FormatOptions::useFontSize(bool val)
{
    use_font_size_ = val;
}

void FormatOptions::useItalic(bool val)
{
    use_italic_ = val;
}

std::ostream & operator <<(std::ostream & os, const FormatOptions & fmt)
{
    using namespace std;
    os << "FormatOptions:\n";
    os << setw(25) << "   SerifName:" << fmt.serifName() << "\n";
    os << setw(25) << "   SansSerifName:" << fmt.sansSerifName() << "\n";
    os << setw(25) << "   Monospace Name:" << fmt.monospaceName() << "\n";
    os << setw(25) << "   Use bold:" << fmt.isBoldUsed() << "\n";
    os << setw(25) << "   Use Italic: " << fmt.isItalicUsed() << "\n";
    os << setw(25) << "   Use font size: " << fmt.isFontSizeUsed() << "\n";
    os << setw(25) << "   Unrecognized char: " << (char) (fmt.unrecognizedChar()) << "\n";
    os << setw(25) << "   Line breaks: " << fmt.preserveLineBreaks() << "\n";
    return os;
}

}