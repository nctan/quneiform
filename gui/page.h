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
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QRect>
#include <QtCore/QVector>
#include <QtCore/QSize>
#include <QtGui/QColor>
#include <QTransform>

class QGraphicsScene;
class QDataStream;

namespace cf {
    class CRtfPage;
}

class Page: public QObject {
    Q_OBJECT
public:
    Page(const QString& image_path);

    typedef std::runtime_error Exception;
    typedef QVector<QRect> RectList;

    /**
      * Returns page rotation angle (0, 90, 180 or 270 degrees)
      */
    int angle() const;

    /**
      * Draws layout on given scene
      */
    void drawFormatLayout(QGraphicsScene * scene) const;

    /**
      * Returns image path
      */
    QString imagePath() const;

    /**
      * Returns image size
      */
    QSize imageSize() const;

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
      * Returns true if image already saved
      */
    bool isSaved() const;

    /**
      * Returns true if page is selected
      */
    bool isSelected() const;

    /**
      * Returns page number
      */
    unsigned int number() const;

    /**
      * Returns OCR text
      */
    QString ocrText() const;

    /**
      * Returns page area on image
      */
    const QRectF& pageArea() const;

    /**
      * Recognizes image
      * @throw Exception if page is not loaded
      */
    void recognize();

    /**
      * Resets page scaling, but saves rotate
      */
    void resetScale();

    /**
      * Rotates page
      */
    void rotate(int angle);

    /**
      * Saves ocr result
      * @throw Exception if page is not recognized
      */
    void save(const QString& file);

    /**
      * Scales page
      */
    void scale(qreal factor);

    /**
      * Sets recognition language
      * @param iso_code - language code, for ex. "ru"
      */
    void setLanguage(const QString& iso_code);

    /**
      * Sets page number
      */
    void setNumber(unsigned int number);

    /**
      * Sets page area on image
      */
    void setPageArea(const QRectF& area);

    /**
      * Selects page
      */
    void setSelected(bool value);

    /**
      * Sets page transform
      */
    void setTransform(const QTransform& t);

    /**
      * Returns page transform
      */
    QTransform transform() const;
signals:
    void changed();
    /**
      * Emmited then page is tranformed
      */
    void transformed();
    void rotated(int angle);
private:
    QString image_path_;
    QSize image_size_;
    QString ocr_text_;
    unsigned int number_;
    bool is_recognized_;
    bool is_saved_;
    bool is_selected_;
    RectList r_page_;
    RectList r_fragment_;
    QRectF page_area_;
    QString language_;
    QTransform transform_;
    bool is_null_;
private:
    void drawFormatPageLayout(QGraphicsScene * scene) const;
    void fillFormatLayout(const cf::CRtfPage * page);
    bool isFormatConvertionNeeded(int format) const;
private:
    static QColor format_page_color_;
public:
    friend QDataStream& operator<<(QDataStream& stream, const Page& page);
    friend QDataStream& operator>>(QDataStream& stream, Page& page);
};

QDataStream& operator<<(QDataStream& stream, const Page& page);
QDataStream& operator>>(QDataStream& stream, Page& page);

#endif /* PAGE_H_ */