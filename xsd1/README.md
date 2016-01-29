# XDL <= XML and VHDL

## XML format

The XML file-format has been defined using XSD (XML Schema Definition), see the directory "eclipse/xsd"

### VHDL-like hierarchical design

The idea is to use the VHDL approach when it comes to hierarchical design, namely:
- design unit 
- entity
- architecture (structural)
- port, generic and signals
- port and generic maps

### Simulation

To define the implementation needed for the simulation, one just need
to create architectures associated to entities, to get started the
idea is to allow a templated architecture.

Let say that we have 2 entities defined in a library:
- Resistor: 
- Capacitor

Then we can define 2 implementations as follow:
- Architecture Qucs of Resistor is:
  - Ports: [1, 2]
  - Generics: [R: real, Temp: real, ...]
  - Template: 'R: %refdes% R=%R% Temp=%Temp% ...'

### Graphical representation


## Using https://github.com/Qucs as a use case

Qucs have been slightly hacked to generate property files (see https://github.com/chgans/qucs)
The drawing files and the property files (CSV) have been converted to a custom defined xml file using the script "convertall.sh".
These CSV files have been converted to XML files using the script "csv2xml.py"
These XML files have been converted to SVG using the command "./xsd1 --icons"

## Viewing Qucs components (XML files)

```
./xsd1
```

Then navigate symbols by clicking on any xml file on the left pane.

## Build instructions
```
sudo apt-get install xsdcxx
make -f xsd.make
qmake
make -j10
```

