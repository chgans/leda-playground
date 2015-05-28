#include "symbolparser.h"
#include "leda-xdl-symbol-pimpl.h"

#include <sstream>

struct SymbolParserPrivate
{
    SymbolParserPrivate()
    {
        // Connect the parsers together.
        //
        Symbol_p.parsers (string_p,
                          string_p,
                          ItemList_p);

        ItemList_p.parsers (Ellipse_p,
                            Line_p,
                            Pin_p,
                            Rectangle_p,
                            ItemGroup_p,
                            Arc_p,
                            Label_p);

        Ellipse_p.parsers (Point_p,
                           double_p,
                           Angle_p,
                           Opacity_p,
                           boolean_p,
                           Mirroring_p,
                           boolean_p,
                           Pen_p,
                           Brush_p,
                           Point_p,
                           NonNegativeDouble_p,
                           NonNegativeDouble_p,
                           Angle_p,
                           Angle_p);

        Point_p.parsers (double_p,
                         double_p);

        Mirroring_p.parsers (boolean_p,
                             boolean_p);

        Pen_p.parsers (NonNegativeDouble_p,
                       Color_p,
                       PenStyle_p,
                       PenCapStyle_p,
                       PenJoinStyle_p);

        Brush_p.parsers (Color_p,
                         BrushStyle_p);

        Line_p.parsers (Point_p,
                        double_p,
                        Angle_p,
                        Opacity_p,
                        boolean_p,
                        Mirroring_p,
                        boolean_p,
                        PointList_p,
                        Pen_p);

        PointList_p.parsers (Point_p);

        Pin_p.parsers (Point_p,
                       double_p,
                       Angle_p,
                       Opacity_p,
                       boolean_p,
                       Mirroring_p,
                       boolean_p,
                       string_p);

        Rectangle_p.parsers (Point_p,
                             double_p,
                             Angle_p,
                             Opacity_p,
                             boolean_p,
                             Mirroring_p,
                             boolean_p,
                             Pen_p,
                             Brush_p,
                             Point_p,
                             Point_p);

        ItemGroup_p.parsers (Point_p,
                             double_p,
                             Angle_p,
                             Opacity_p,
                             boolean_p,
                             Mirroring_p,
                             boolean_p,
                             ItemList_p);

        Arc_p.parsers (Point_p,
                       double_p,
                       Angle_p,
                       Opacity_p,
                       boolean_p,
                       Mirroring_p,
                       boolean_p,
                       Pen_p,
                       Brush_p,
                       Point_p,
                       NonNegativeDouble_p,
                       NonNegativeDouble_p,
                       Angle_p,
                       Angle_p);

        Label_p.parsers (Point_p,
                         double_p,
                         Angle_p,
                         Opacity_p,
                         boolean_p,
                         Mirroring_p,
                         boolean_p,
                         Pen_p,
                         Brush_p,
                         string_p,
                         Font_p);

        Font_p.parsers (string_p,
                        non_negative_integer_p,
                        boolean_p,
                        boolean_p,
                        boolean_p,
                        boolean_p);
    }

    // Instantiate individual parsers.
    //
    ::xdl::Symbol_pimpl Symbol_p;
    ::xml_schema::string_pimpl string_p;
    ::xdl::ItemList_pimpl ItemList_p;
    ::xdl::Ellipse_pimpl Ellipse_p;
    ::xdl::Point_pimpl Point_p;
    ::xml_schema::double_pimpl double_p;
    ::xdl::Angle_pimpl Angle_p;
    ::xdl::Opacity_pimpl Opacity_p;
    ::xml_schema::boolean_pimpl boolean_p;
    ::xdl::Mirroring_pimpl Mirroring_p;
    ::xdl::Pen_pimpl Pen_p;
    ::xdl::NonNegativeDouble_pimpl NonNegativeDouble_p;
    ::xdl::Color_pimpl Color_p;
    ::xdl::PenStyle_pimpl PenStyle_p;
    ::xdl::PenCapStyle_pimpl PenCapStyle_p;
    ::xdl::PenJoinStyle_pimpl PenJoinStyle_p;
    ::xdl::Brush_pimpl Brush_p;
    ::xdl::BrushStyle_pimpl BrushStyle_p;
    ::xdl::Line_pimpl Line_p;
    ::xdl::PointList_pimpl PointList_p;
    ::xdl::Pin_pimpl Pin_p;
    ::xdl::PinSymbolList_pimpl PinSymbolList_p;
    ::xdl::PinSymbol_pimpl PinSymbol_p;
    ::xdl::PinSymbolEnum_pimpl PinSymbolEnum_p;
    ::xdl::Side_pimpl Side_p;
    ::xdl::Rectangle_pimpl Rectangle_p;
    ::xdl::ItemGroup_pimpl ItemGroup_p;
    ::xdl::Arc_pimpl Arc_p;
    ::xdl::Label_pimpl Label_p;
    ::xdl::Font_pimpl Font_p;
    ::xml_schema::non_negative_integer_pimpl non_negative_integer_p;
};

SymbolParser::SymbolParser():
    p(new SymbolParserPrivate)
{
}

Symbol *SymbolParser::parse(const QString &filename)
{
    m_errorString.clear();
    try
    {
        p->Symbol_p.pre();
        ::xml_schema::document doc_p(p->Symbol_p,
                                     "http://www.leda.org/xdl",
                                     "symbol");
        doc_p.parse(filename.toLocal8Bit().constData());
        return p->Symbol_p.post_Symbol();
    }
    catch (const ::xml_schema::parsing &e)
    {
        std::stringstream stream;
        stream << e;
        m_errorString = QString::fromStdString(stream.str());
        return nullptr;
    }
    catch (const std::ios_base::failure&)
    {
        m_errorString = QString("General I/O failure");
        return nullptr;
    }
}

