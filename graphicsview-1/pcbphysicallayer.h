#ifndef PCBPHYSICALLAYER_H
#define PCBPHYSICALLAYER_H

/* From altium 4 layers preset (thickness rounded)
 * solder mask: 10 um
 * top signal: 35 um
 * dielectric 1: core, 320 um
 * top plane: 35 um
 * dielectric 2: pre-reg, 130um
 * bot plane: 35 um
 * dielectric 3: core, 250 um
 * bot signal: 35 um
 * solder mask: 10 um
 *
 * From https://www.pcb-pool.com/download/spezifikation/eng_cmso004_ml4.pdf
 * solder mask: 10 um
 * top signal: 35 um
 * dielectric 1: core, 380 um
 * top plane: 35 um
 * dielectric 2: pre-reg, 710um
 * bot plane: 35 um
 * dielectric 3: core, 380 um
 * bot signal: 35 um
 * solder mask: 10 um
 *
 */

#include <QtGlobal>
#include <QString>


// FIXME: add color in here?

class PcbPhysicalLayer
{
public:
    PcbPhysicalLayer();
    PcbPhysicalLayer(const PcbPhysicalLayer &other);
    virtual ~PcbPhysicalLayer();

    enum {
        Type = 1,
        UserType = 65536
    };
    virtual int type() const { return UserType; }

    QString name() const;
    void setName(const QString &name);

    qreal thickness() const;
    void setThickness(const qreal &value);

    QString material() const;
    void setMaterial(const QString &value);

protected:
    QString m_name;
    qreal m_thickness;
    QString m_material;
};

class PcbPhysicalOverlayLayer: public PcbPhysicalLayer
{
public:
    PcbPhysicalOverlayLayer();
    PcbPhysicalOverlayLayer(const PcbPhysicalOverlayLayer &other);
    ~PcbPhysicalOverlayLayer();
    enum {
        Type = 2
    };
    int type() const { return Type; }
};


class PcbPhysicalConductorLayer: public PcbPhysicalLayer
{
public:
    PcbPhysicalConductorLayer();
    PcbPhysicalConductorLayer(const PcbPhysicalConductorLayer &other);
    ~PcbPhysicalConductorLayer();
    enum {
        Type = 3
    };
    int type() const { return Type; }

    qreal pullBackDistance() const; // TBD: more a keep out shape
    void setPullBackDistance(const qreal &value);

protected:
    qreal m_pullBackDistance;
};

class PcbPhysicalDielectricLayer: public PcbPhysicalLayer
{
public:
    PcbPhysicalDielectricLayer();
    PcbPhysicalDielectricLayer(const PcbPhysicalDielectricLayer &other);
    ~PcbPhysicalDielectricLayer();

    enum {
        Type = 4
    };
    int type() const { return Type; }

    QString dielectricMaterial() const;
    void setDielectricMaterial(const QString &value);

    QString dielectricConstant() const;
    void setDielectricConstant(const QString &value);

protected:
    QString m_dielectricMaterial;
    QString m_dielectricConstant;
};

// FIXME: derive from dielectric layer
// Components belong to the next (top) layer of a signal layer, which is always a dielectric
// Components belong to the prev (bot) layer of a signal layer, which is always a dielectric
// Or use a flag for surface vs internal?
// Surface layer have as well a dielectric material and constant
class PcbPhysicalSurfaceLayer: public PcbPhysicalDielectricLayer
{
public:
    PcbPhysicalSurfaceLayer();
    PcbPhysicalSurfaceLayer(const PcbPhysicalSurfaceLayer &other);
    ~PcbPhysicalSurfaceLayer();
    enum {
        Type = 3
    };
    int type() const { return Type; }
};

// FIXME: use flags as well: IsPlane

class PcbPhysicalSignalLayer: public PcbPhysicalConductorLayer
{
public:
    PcbPhysicalSignalLayer();
    PcbPhysicalSignalLayer(const PcbPhysicalSignalLayer &other);
    ~PcbPhysicalSignalLayer();

    enum ComponentPolicy {
        NoComponentAllowed,
        ComponentOnTop,
        ComponentOnBottom
    };
    enum {
        Type = 5
    };
    int type() const { return Type; }

    ComponentPolicy componentPolicy() const;
    void setComponentPolicy(const ComponentPolicy &value);

protected:
    ComponentPolicy m_componentPolicy;
};

class PcbPhysicalPlaneLayer: public PcbPhysicalConductorLayer
{
public:
    PcbPhysicalPlaneLayer();
    PcbPhysicalPlaneLayer(const PcbPhysicalPlaneLayer &other);
    ~PcbPhysicalPlaneLayer();

    enum {
        Type = 6
    };
    int type() const { return Type; }
};

#endif // PCBPHYSICALLAYER_H
