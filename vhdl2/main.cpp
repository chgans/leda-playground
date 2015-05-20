#include "mainwindow.h"
#include <QApplication>

#include <entity.h>
#include <architecture.h>
#include <configuration.h>

Entity *createResistorEntity()
{
    Entity *e = new Entity("entity.resistor");
    e->generics.append(new Generic("R", "Resistance", "Ω",
                                   QMetaType::Double, true));
    e->generics.append(new Generic("Temp", "Simulation temperature", "°C",
                                   QMetaType::Double, false, 26.85));
    e->generics.append(new Generic("Tc1", "First order temperature coefficient", "Ω⋅°C⁻¹",
                                   QMetaType::Double, false, 0.0));
    e->generics.append(new Generic("Tc2", "Second order temperature coefficient", "Ω⋅°C⁻²",
                                   QMetaType::Double, false, 0.0));
    e->generics.append(new Generic("Tnom", "Temperature at which parameters were extracted", "°C",
                                   QMetaType::Double, false, 26.85));
    e->ports.append(new Port("1", QMetaType::Double));
    e->ports.append(new Port("2", QMetaType::Double));
    return e;
}

Entity *createCapacitorEntity()
{
    Entity *e = new Entity("entity.capacitor");
    e->generics.append(new Generic("C", "Capacitance", "F",
                                   QMetaType::Double, true));
    e->generics.append(new Generic("V", "Initial voltage (transient simulation)", "V",
                                   QMetaType::Double, true));
    e->ports.append(new Port("1", QMetaType::Double));
    e->ports.append(new Port("2", QMetaType::Double));
    return e;
}

Entity *createVacEntity()
{
    Entity *e = new Entity("entity.ac_voltage_source");
    e->generics.append(new Generic("U", "Peak voltage", "V",
                                   QMetaType::Double, true));
    e->generics.append(new Generic("f", "Frequency", "Hz",
                                   QMetaType::Double, true));
    e->generics.append(new Generic("Phase", "Initial phase", "°",
                                   QMetaType::Double, false, 0.0));
    e->generics.append(new Generic("Theta", "Damping factor (transient simulation)", "s⁻¹",
                                   QMetaType::Double, false, 0.0));
    e->ports.append(new Port("1", QMetaType::Double));
    e->ports.append(new Port("2", QMetaType::Double));
    return e;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Entity *resEntity = createResistorEntity();
    Entity *capEntity = createCapacitorEntity();
    Entity *vacEntity = createVacEntity();

    auto circuitEntity = new Entity("entity.circuit");
    auto circuit = new StructuralArchitecture("struct.arch.circuit", "entity.circuit");

    auto gnd = new Signal("gnd", QMetaType::Double);
    circuit->signalList.append(gnd);
    auto in = new Signal("in", QMetaType::Double);
    circuit->signalList.append(in);
    auto out = new Signal("out", QMetaType::Double);
    circuit->signalList.append(out);

    auto r1 = new Component("R1", "entity.resistor");
    r1->portMap.insert("in", "1");
    r1->portMap.insert("out", "2");
    circuit->componentList.append(r1);

    auto c1 = new Component("C1", "entity.capacitor");
    c1->portMap.insert("out", "1");
    c1->portMap.insert("gnd", "2");
    circuit->componentList.append(c1);

    auto v1 = new Component("V1", "entity.ac_voltage_source");
    v1->portMap.insert("gnd", "1");
    v1->portMap.insert("in", "2");
    circuit->componentList.append(v1);

    auto r1Config = new ComponentConfiguration("r1Config", "component.resistor", "lib");
    r1Config->genericMap.insert("R", "120e3");
    r1Config->genericMap.insert("Tc1", "1e-3");
    r1Config->genericMap.insert("Tc2", "2e-3");

    auto c1Config = new ComponentConfiguration("c1Config", "component.capacitor", "lib");
    c1Config->genericMap.insert("C", "100e-9");

    auto v1Config = new ComponentConfiguration("v1Config", "component.ac_source_voltage", "lib");
    v1Config->genericMap.insert("V", "10.0");

    auto circuitConfig = new ArchitectureConfiguration("circuitConfig");
    circuitConfig->compConfigList.append(r1Config);
    circuitConfig->compConfigList.append(c1Config);
    circuitConfig->compConfigList.append(v1Config);

    auto simuConfig = new Configuration("simuConfig", "entity.circuit");
    simuConfig->archConfigList.append(circuitConfig);

    return a.exec();
}
