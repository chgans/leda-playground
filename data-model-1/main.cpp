#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "entity.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //MainWindow w;
    //w.show();

    EdlPort p;
    EdlAttribute a;
    EdlSignal sig;

    /*
     * Components from library
     */
    EdlEntity res;
    res.id = "Res";
    p.id = "1";
    p.direction = EdlPort::InOutPort;
    res.portList << p;
    p.id = "2";
    p.direction = EdlPort::InOutPort;
    res.portList << p;
    a.id = "resistance";
    res.attributeList << a;
    a.id = "tolerance";
    res.attributeList << a;
    a.id = "power";
    res.attributeList << a;

    EdlEntity cap;
    cap.id = "Cap";
    p.id = "1";
    p.direction = EdlPort::InOutPort;
    cap.portList << p;
    p.id = "2";
    p.direction = EdlPort::InOutPort;
    cap.portList << p;
    a.id = "capacitance";
    cap.attributeList << a;
    a.id = "voltage-rating";
    cap.attributeList << a;
    a.id = "tolerance";
    res.attributeList << a;
    a.id = "esr";
    res.attributeList << a;


    EdlEntity conn;
    conn.id = "Conn";
    p.id = "1";
    p.direction = EdlPort::InOutPort;
    conn.portList << p;
    p.id = "2";
    p.direction = EdlPort::InOutPort;
    conn.portList << p;


    /*
     * Sheet symbol
     */
    EdlEntity bridge;
    bridge.id = "Bridge";
    p.id = "In+";
    p.direction = EdlPort::InOutPort;
    bridge.portList << p;
    p.id = "In-";
    p.direction = EdlPort::InOutPort;
    bridge.portList << p;
    p.id = "Out+";
    p.direction = EdlPort::InOutPort;
    bridge.portList << p;
    p.id = "Out-";
    p.direction = EdlPort::InOutPort;
    bridge.portList << p;

    /*
     * Sheet netlist
     */
    EdlArchitecture arch; // Structure
    EdlInstance inst;
    arch.entityIdRef = "Bridge";
    sig.Id = "Net1";
    sig.SignalTypeId = 0; // TBD
    arch.signalList << sig;
    sig.Id = "Net2";
    sig.SignalTypeId = 0; // TBD
    arch.signalList << sig;
    sig.Id = "Net3";
    sig.SignalTypeId = 0; // TBD
    arch.signalList << sig;
    sig.Id = "Net4";
    sig.SignalTypeId = 0; // TBD
    arch.signalList << sig;
    arch.portMapping["In+"] = "Net1";
    arch.portMapping["In-"] = "Net4";
    arch.portMapping["Out+"] = "Net3";
    arch.portMapping["Out-"] = "Net4";
    inst.entityIdRef = "Res";
    inst.instanceId = "R1";
    inst.portMapping["1"] = "Net1";
    inst.portMapping["2"] = "Net2";
    arch.instanceList << inst;
    inst.entityIdRef = "Res";
    inst.instanceId = "R2";
    inst.portMapping["1"] = "Net2";
    inst.portMapping["2"] = "Net3";
    arch.instanceList << inst;
    inst.entityIdRef = "Cap";
    inst.instanceId = "C1";
    inst.portMapping["1"] = "Net2";
    inst.portMapping["2"] = "Net4";
    arch.instanceList << inst;

    // Top level sheet: the project
    EdlEntity projectEntity;
    projectEntity.id = "ProjectFoo";
    EdlArchitecture projectArch;
    projectArch.entityIdRef = "ProjectFoo";
    /*
    projectArch.signalList
            << EdlSignal("N1") << EdlSignal("N2") << EdlSignal("N3")
            << EdlSignal("N4") << EdlSignal("N5") << EdlSignal("N6");
    */
    inst.entityIdRef = "Conn";
    inst.instanceId = "P1";
    inst.portMapping["1"] = "N1";
    inst.portMapping["2"] = "N2";
    arch.instanceList << inst;
    inst.entityIdRef = "Bridge";
    inst.instanceId = "U1";
    inst.portMapping["In+"] = "N1";
    inst.portMapping["In-"] = "N2";
    inst.portMapping["Out+"] = "N3";
    inst.portMapping["Out-"] = "N4";
    arch.instanceList << inst;
    inst.entityIdRef = "Bridge";
    inst.instanceId = "U2";
    inst.portMapping["In+"] = "N3";
    inst.portMapping["In-"] = "N4";
    inst.portMapping["Out+"] = "N5";
    inst.portMapping["Out-"] = "N6";
    arch.instanceList << inst;
    inst.entityIdRef = "Conn";
    inst.instanceId = "P2";
    inst.portMapping["1"] = "N5";
    inst.portMapping["2"] = "N6";
    arch.instanceList << inst;

    qDebug() << "No stuff!";
    return 1;
}
