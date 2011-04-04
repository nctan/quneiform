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

#ifndef PAGE_H_
#define PAGE_H_

#include <stdexcept>
#include <QString>
#include <QList>
#include <QObject>
#include <QRect>
#include <QSize>
#include <QColor>
#include <QTransform>
#include <QMutex>
#include <QFlags>
#include "recognitionsettings.h"
#include "formatsettings.h"

class QDataStream;
class QTextDocument;

class Page: public QObject {
    Q_OBJECT
public:
    Page(const QString& image_path);

    enum PageFlag {
        NONE = 0,
        RECOGNIZED = 1,
        RECOGNITION_FAILED = (1 << 1),
        EXPORTED = (1 << 2),
        EXPORT_FAILED = (1 << 3)
    };

    Q_DECLARE_FLAGS(PageFlags, PageFlag);

    class Exception {
        QString msg_;
    public:
        Exception(const QString& msg) : msg_(msg) {}
        QString message() const {
            return msg_;
        }
    };

    typedef QList<QRect> Rectangles;
    typedef QList<Rectangles> RectList;

    /* do no change values! */
    enum BlockType {
        PICTURE = 0,
        CHAR = 1,
        LINE = 2,
        PARAGRAPH = 3,
        COLUMN = 4,
        SECTION = 5
    };

    /**
      * Returns page rotation angle (0, 90, 180 or 270 degrees)
      * @see rotate()
      */
    int angle() const;

    /**
      * Append block bounding rectangle
      * @see clearBlocks(), blocks(), setBlocks()
      */
    void appendBlock(const QRect& rect, BlockType type);

    /**
      * Returns list of blocks rectangles
      * @see setBlocks(), blocksCount()
      */
    const Rectangles& blocks(BlockType t) const;

    /**
      * Returns number of blocks by given type
      * @see blocks(), setBlocks()
      */
    int blocksCount(BlockType t) const;

    /**
      * Clears block bounding rectangles
      * @see blocks(), setBlocks(), appendBlock()
      */
    void clearBlocks(BlockType type);

    /**
      * Returns pointer to page document
      */
    QTextDocument * document();

    /**
      * Saves ocr result
      * @throw Exception if page is not recognized
      * Emits signal exported()
      */
    void exportTo(const QString& file);

    /**
      * Returns page state flags
      * @see hasFlag(), setFlag(), unsetFlag(), setFlags()
      */
    PageFlags flags() const;

    /**
      * Returns page format settings
      * @see setFormatSettings()
      */
    const FormatSettings& formatSettings() const;

    /**
      * Returns true if page have flag
      * @see flags(), setFlag(), unsetFlag(), setFlags()
      */
    bool hasFlag(PageFlag flag);

    /**
      * Returns image path
      */
    QString imagePath() const;

    /**
      * Returns image size
      */
    QSize imageSize() const;

    /**
      * Returns true if page already exported
      */
    bool isExported() const;

    /**
      * Returns true if points to valid image
      * otherwise returns false
      */
    bool isNull() const;

    /**
      * Returns true if image recognized
      */
    bool isRecognized() const;

    /**
      * Returns true if page is selected
      */
    bool isSelected() const;

    /**
      * Returns page name - filename of page path
      */
    QString name() const;

    /**
      * Returns page number
      * @see setNumber()
      */
    unsigned int number() const;

    /**
      * Returns OCR text
      * @see setOcrText()
      */
    QString ocrText() const;

    /**
      * Returns page area on image
      * @see setPageArea()
      */
    const QRect& pageArea() const;

    /**
      * Returns page recognize settings
      * @see setRecognitionSettings()
      */
    const RecognitionSettings& recognitionSettings() const;

    /**
      * Resets page scaling, but saves rotate
      * Emits signals changed() and transformed()
      * @see scale(), resetTransform()
      */
    void resetScale();

    /**
      * Resets page transform
      * @see resetScale()
      */
    void resetTransform();

    /**
      * Rotates page
      * Emits signals changed() and rotated()
      */
    void rotate(int angle);

    /**
      * Scales page
      * Emits signals changed() and transformed()
      */
    void scale(qreal factor);

    /**
      * Sets page state flag
      * @see setFlags(), unsetFlag()
      * emits signal changed()
      */
    void setFlag(PageFlag flag);

    /**
      * Sets page state flags
      * All previous flags are reset
      * emits signal changed()
      * @see setFlag(), unsetFlag()
      */
    void setFlags(PageFlags flags);

    /**
      * Sets format settings
      * @see formatSettings()
      */
    void setFormatSettings(const FormatSettings& settings);

    /**
      * Sets page number
      * Emits signal changed()
      * @see number()
      */
    void setNumber(unsigned int number);

    /**
      * Sets page ocr text
      * Emits signal changed() and recognized()
      * @see ocrText()
      */
    void setOcrText(const QString& text);

    /**
      * Sets page area on image
      * Emits signal changed()
      * @see pageArea()
      */
    void setPageArea(const QRect& area);

    /**
      * Sets page recognize settings
      * emits signal changed()
      * @see recognitionSettings()
      */
    void setRecognitionSettings(const RecognitionSettings& opts);

    /**
      * Sets page blocks
      * emits signal changed()
      * @see blocks(), appendBlock()
      */
    void setBlocks(const Rectangles& rects, BlockType type);

    /**
      * Selects page
      * Emits signal changed()
      * @see isSelected()
      */
    void setSelected(bool value);

    /**
      * Sets page transform
      * Emits signals changed() and transformed()
      * @see transform()
      */
    void setTransform(const QTransform& t);

    /**
      * Sets page view scroll
      * @see viewScroll()
      */
    void setViewScroll(const QPoint& pt);

    /**
      * Returns page transform
      * @see setTransform()
      */
    QTransform transform() const;

    /**
      * Unsets page state flag
      * @see setFlag(), flags()
      */
    void unsetFlag(PageFlag flag);

    /**
      * Returns page view scroll
      * @see setViewScroll()
      */
    QPoint viewScroll() const;
signals:
    /**
      * Emmitted when some page data changed, such as
      * scale, angle, flags, selection etc.
      */
    void changed();

    /**
      * Emitted when page exported
      * @see exportTo()
      */
    void exported();

    /**
      * Emmited when page is transformed
      */
    void transformed();

    /**
      * Emitted when page is recognized
      */
    void recognized();

    /**
      * Emmitted when page is rotated
      */
    void rotated(int angle);
private:
    void initRects();
private:
    QString image_path_;
    QSize image_size_;
    QString ocr_text_;
    unsigned int number_;
    PageFlags state_flags_;
    bool is_selected_;
    QRect page_area_;
    QTransform transform_;
    QPoint view_scroll_;
    bool is_null_;
    mutable QMutex mutex_;
    RecognitionSettings rec_settings_;
    RectList blocks_;
    QTextDocument * doc_;
    FormatSettings format_settings_;
public:
    friend QDataStream& operator<<(QDataStream& stream, const Page& page);
    friend QDataStream& operator>>(QDataStream& stream, Page& page);
    friend class PageRecognizer;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Page::PageFlags);

QDataStream& operator<<(QDataStream& stream, const Page& page);
QDataStream& operator>>(QDataStream& stream, Page& page);
QDataStream& operator<<(QDataStream& stream, const Page::PageFlags& flags);
QDataStream& operator>>(QDataStream& stream, Page::PageFlags& flags);

#endif /* PAGE_H_ */
