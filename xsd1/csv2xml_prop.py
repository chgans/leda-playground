#!/usr/bin/env python3

import sys
from xml.sax.saxutils import escape

class Generic:
    def __init__(self):
        self.name="<NAME>"
        self.unit=""
        self.dataType="string"
        self.defaultValue=""

    # id;value;displayed;description
    def fromCsv(self, tokens):
        name, value, _, _ = tokens
        self.name = name.strip()
        self.defaultValue = value.replace('"', '').strip()

    def toXml(self):
        print('<generic id="{:s}">'.format(self.name))
        if len(self.unit) != 0:
            print(' <unit>{:s}</unit>'.format(self.unit))
        if len(self.dataType) != 0:
            print(' <type>{:s}</type>'.format(self.dataType))
        if len(self.defaultValue) != 0:
            print(' <value>{:s}</value>'.format(self.defaultValue))
        print('</generic>')

class Port:
    def __init__(self, name):
        self.name=name
        self.mode="inout"
        self.signalType="real"
        self.initialValue=""

    def toXml(self):
        print('<port id="{:s}">'.format(self.name))
        print('</port>')

class Entity:
    def __init__(self, caption, identifier):
        self.name = identifier;
        self.generics=[]
        self.ports=[]

    def toXml(self):
        print('<entity id="{:s}">'.format(self.name))
        for generic in self.generics:
            generic.toXml()
        for port in self.ports:
            port.toXml()
        print("</entity>")

    # text;x;y;content;color;size;sin;cos;over;under
    def fromCsv(self, tokens):
        pass

f = open(sys.argv[1])
lines = f.readlines()
caption = lines[1].split(";")[1].strip()
description = lines[2].split(";")[1].strip()
identifier = lines[3].split(";")[1].strip().replace("`", "")
nbProps = int(lines[7].split(";")[1].strip())
nbPorts = int(lines[9].split(";")[1].strip())

f = open(sys.argv[2])
lines = f.readlines()

entity = Entity(caption, identifier)
for i in range(nbPorts):
    entity.ports.append(Port(str(i)))

for line in lines:
    if line[0] == "#":
        continue
    prop = Generic()
    prop.fromCsv(line.strip().split(";"))
    entity.generics.append(prop)

entity.toXml()

sys.exit(0)
