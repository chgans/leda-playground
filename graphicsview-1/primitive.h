#ifndef PRIMITIVE_H
#define PRIMITIVE_H


class Primitive
{
public:
    enum Type {
        Track = 0,
        Arc,
        Pad,
        Via,
        Polygon,
        Region,
        Fill,
        String,
        Room,
        Body,
        Dimension,
        Coordinate,

        _BeginType = Track,
        _EndType = Coordinate
    };
    Primitive();
    ~Primitive();
};

#endif // PRIMITIVE_H
