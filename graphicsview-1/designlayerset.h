#ifndef DESIGNLAYERSET_H
#define DESIGNLAYERSET_H


class DesignLayerSet : public List<const DesignLayer *>
{
public:
    DesignLayerSet();
    ~DesignLayerSet();
};

#endif // DESIGNLAYERSET_H
