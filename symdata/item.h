#ifndef ITEM_H
#define ITEM_H

#include <cstdint>
#include <string>
#include <list>

typedef std::uint64_t UID;

typedef enum
{
    NoPen          = 0,
    SolidLine      = 1,
    DashLine       = 2,
    DotLine        = 3,
    DashDotLine    = 4,
    DashDotDotLine = 5
} PenStyle;

typedef enum
{
    FlatCap   = 0x00,
    SquareCap = 0x10,
    RoundCap  = 0x20
} PenCapStyle;

typedef enum
{
    MiterJoin = 0x00,
    BevelJoin = 0x40,
    RoundJoin = 0x80
} PenJoinStyle;

typedef enum
{
    NoBrush          = 0,
    SolidPattern     = 1,
    GridPattern      = 5,
    HorPattern       = 9,
    VerPattern       = 10,
    CrossPattern     = 11,
    BDiagPattern     = 12,
    FDiagPattern     = 13,
    DiagCrossPattern = 14,
} BrushStyle;

typedef enum
{
    AlignLeft    = 0x0001,
    AlignRight   = 0x0002,
    AlignHCenter = 0x0004,
} HorizontalAlignment;

typedef enum
{
    AlignTop     = 0x0020,
    AlignBottom  = 0x0040,
    AlignVCenter = 0x0080,
} VerticalAlignment;


/*
 * Color palette:
 *  - Emphasied content
 *  - Primary content
 *  - Secondary content
 *  - Background highlights
 *  - Background
 *  - Yellow accent
 *  - Orange accent
 *  - Red accent
 *  - Magenta accent
 *  - Violet accent
 *  - Blue accent
 *  - Cyan accent
 *  - Green accent
 */

class Color
{
public:
    Color();
    double red;
    double green;
    double blue;
};

class Pen
{
public:
    Pen();
    double width;
    Color color;
    PenStyle style;
    PenCapStyle capStyle;
    PenJoinStyle joinStyle;
};

class Brush
{
public:
    Brush();
    Color color;
    BrushStyle style;
};

class Item
{
public:
    Item();
    UID uid;
    double xPosition;
    double yPosition;
    double zValue;
    double rotation;
    double opacity;
    bool isLocked;
    bool isHidden;
    bool isMirroredHorizontally;
    bool isMirroredVertically;
};

class AbstractShapeItem: public Item
{
public:
    AbstractShapeItem();
    Pen pen;
    Brush brush;
};

class RectangleItem: public AbstractShapeItem
{
public:
    RectangleItem();
    double width;
    double height;
};

class EllipseItem: public AbstractShapeItem
{
public:
    EllipseItem();
    double xRadius;
    double yRadius;
};

class TextItem: public Item
{
public:
    TextItem();
    std::string htmlText;
    HorizontalAlignment horizontalAlignment;
    VerticalAlignment verticalAlignment;
};

class PinItem: public Item
{
public:
    PinItem();
    TextItem *designator;
    TextItem *label;
    double length;
};

class GroupItem: public Item
{
public:
    GroupItem();
    std::list<Item*> children;
};

#endif // ITEM_H
