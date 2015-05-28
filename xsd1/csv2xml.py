#!/usr/bin/env python3

import sys
from xml.sax.saxutils import escape

class Item:
    def __init__(self):
        self.x = 0.0
        self.y = 0.0
        self.z = 0.0
        self.rotation = 0.0
        self.opacity = 0.0
        self.xMirrored = False
        self.yMirrored = False

    def toXml(self):
        print('<position><x>{:e}</x><y>{:e}</y></position>'.format(self.x, self.y))
        # print('<z-value>{:e}</z-value>'.format(self.z))
        # print('<rotation>{:e}</rotation>'.format(self.rotation))
        # print('<opacity>{:e}</opacity>'.format(self.opacity))
        # print('<x-mirrored>{:s}</x-mirrored>'.format("true" if self.xMirrored else "false"))
        # print('<y-mirrored>{:s}</y-mirrored>'.format("true" if self.yMirrored else "false"))
        
class Shape(Item):
    def __init__(self):
        Item.__init__(self)
        self.pen = {}
        self.pen["width"] = 0.0
        self.pen["color"] = "#000000"
        self.pen["style"] = "SolidLine"
        self.brush = {}
        self.brush["style"] = "Solid"
        self.brush["color"] = "#000000"

    def toXml(self):
        Item.toXml(self)
        print('<pen><width>{:e}</width><color>{:s}</color></pen>'.format(self.pen["width"], self.pen["color"]))
        print('<brush><color>{:s}</color><style>{:s}</style></brush>'.format(self.brush["color"], self.brush["style"]))

class Ellipse(Shape):
    def __init__(self):
        Shape.__init__(self)
        self.xCenter = 0.0
        self.yCenter = 0.0
        self.xRadius = 0.0
        self.yRadius = 0.0
        self.startAngle = 0.0
        self.spanAngle = 360.0

    def toXml(self):
        print("<ellipse>")
        Shape.toXml(self)
        print('<center><x>{:e}</x><y>{:e}</y></center>'.format(self.xCenter, self.yCenter))
        print('<x-radius>{:e}</x-radius><y-radius>{:e}</y-radius>'.format(self.xRadius, self.yRadius))
        print('<start-angle>{:e}</start-angle><span-angle>{:e}</span-angle>'.format(self.startAngle, self.spanAngle))
        print("</ellipse>")

    # ellips;x;y;width;height;pen.width;pen.color;pen.style;brush.color;brush.style
    def fromCsv(self, tokens):
        _,x,y,w,h,pw,pc,ps,bc,bs = tokens
        self.xCenter = int(x) + int(w)/2.0
        self.yCenter = int(y) + int(h)/2.0
        self.xRadius = int(w)/2.0
        self.yRadius = int(h)/2.0
        self.pen["width"] = int(pw)/5.0
        self.pen["color"] = pc
        self.brush["color"] = bc

class Rect(Shape):
    def __init__(self):
        Shape.__init__(self)
        self.xTopLeft = 0.0
        self.yTopLeft = 0.0
        self.xBottomRight = 0.0
        self.yBottomRight = 0.0
        
    def toXml(self):
        print("<rectangle>")
        Shape.toXml(self)
        print('<top-left><x>{:e}</x><y>{:e}</y></top-left>'.format(self.xTopLeft, self.yTopLeft))
        print('<bottom-right><x>{:e}</x><y>{:e}</y></bottom-right>'.format(self.xBottomRight, self.yBottomRight))
        print("</rectangle>")

    # rect;x;y;width;height;pen.width;pen.color;pen.style;brush.color;brush.style
    def fromCsv(self, tokens):
        _,x,y,w,h,pw,pc,ps,bc,bs = tokens
        self.x = int(x)
        self.y = int(y)
        self.xTopLeft = 0
        self.yTopLeft = 0
        self.xBottomRight = int(w)
        self.yBottomRight = int(h)
        self.pen["width"] = int(pw)/5.0
        self.pen["color"] = pc
        self.brush["color"] = bc

class Line(Item):
    def __init__(self):
        Item.__init__(self)
        self.x1 = 0.0
        self.y1 = 0.0
        self.x2 = 0.0
        self.y2 = 0.0
        self.pen = {}
        self.pen["width"] = 0.0
        self.pen["color"] = "#000000"
        self.pen["style"] = "SolidLine"

    def toXml(self):
        print("<line>")
        Item.toXml(self)
        print('<points>'.format())
        print('<point><x>{:e}</x><y>{:e}</y></point>'.format(self.x1, self.y1))
        print('<point><x>{:e}</x><y>{:e}</y></point>'.format(self.x2, self.y2))
        print('</points>'.format())
        print('<pen><width>{:e}</width><color>{:s}</color></pen>'.format(self.pen["width"], self.pen["color"]))
        print("</line>")

    # line;x1;y1;x2;y2;pen.width;pen.color;pen.style
    def fromCsv(self, tokens):
        self.x1 = int(tokens[1])
        self.y1 = int(tokens[2])
        self.x2 = int(tokens[3])
        self.y2 = int(tokens[4])
        self.pen["width"] = int(tokens[5])/5.0
        self.pen["color"] = tokens[6]
        
class Arc(Shape):
    def __init__(self):
        Shape.__init__(self)
        self.xCenter = 0.0
        self.yCenter = 0.0
        self.xRadius = 0.0
        self.yRadius = 0.0
        self.startAngle = 0.0
        self.spanAngle = 360.0

    def toXml(self):
        print("<arc>")
        Shape.toXml(self)
        print('<center><x>{:e}</x><y>{:e}</y></center>'.format(self.xCenter, self.yCenter))
        print('<x-radius>{:e}</x-radius><y-radius>{:e}</y-radius>'.format(self.xRadius, self.yRadius))
        print('<start-angle>{:e}</start-angle><span-angle>{:e}</span-angle>'.format(self.startAngle, self.spanAngle))
        print("</arc>")

    # arc;x;y;width;height;angle;arclen;pen.width;pen.color;pen.style
    def fromCsv(self, tokens):
        _,x,y,w,h,sa,spa,pw,pc,ps = tokens
        self.xCenter = int(x) + int(w)/2.0
        self.yCenter = int(y) + int(h)/2.0
        self.xRadius = int(w)/2.0
        self.yRadius = int(h)/2.0
        self.startAngle = int(sa)/16.0
        self.spanAngle = int(spa)/16.0
        self.pen["width"] = int(pw)/5.0
        self.pen["color"] = pc
        self.brush["style"] = "NoBrush"
        
class Port(Item):
    def __init__(self):
        Item.__init__(self)
        self.name = ""
        self.description=""

    def toXml(self):
        print('<pin>')
        Item.toXml(self)
        print('<designator>{:s}</designator>'.format(self.name))
        print("</pin>")
        
    # port;seq;x;y;avail;type
    def fromCsv(self, tokens):
        self.name = tokens[1]
        self.x = int(tokens[2])
        self.y = int(tokens[3])

class Text(Shape):
    def __init__(self):
        Shape.__init__(self)
        self.content = ""
        
    def toXml(self):
        print('<label>')
        Shape.toXml(self)
        print('<text>{:s}</text>'.format(escape(self.content)));        
        print("</label>")
        
    # text;x;y;content;color;size;sin;cos;over;under
    def fromCsv(self, tokens):
        self.x = int(tokens[1])
        self.y = int(tokens[2])
        self.content = tokens[3]
        self.size = int(tokens[5])
        self.brush["style"] = "Solid"
        self.brush["color"] = tokens[4]
        if tokens[6] != "0" and tokens[7] != "1":
            raise Exception("Orientation not supported")
        if tokens[8] != "0" and tokens[9] != "0":
            raise Exception("Text under/over line not supported")
    
# component;flags;cx;cy;tx;ty;x1;y1;x2;y2
class Symbol():
    def __init__(self, caption, description):
        self.caption = caption
        self.description = description
        self.items = []
        
    def toXml(self):
        print('<symbol xmlns="http://www.leda.org/xdl">')
        print('<name>{:s}</name>'.format(self.caption))
        print('<label>{:s}</label>'.format(self.description));
        print('<drawing>')
        for item in self.items:
            item.toXml()
        print('</drawing>')
        print("</symbol>")        
                      
    def fromCsv(self, lines):
        for line in lines:
            line = line.strip()
            item = None
            if line.startswith('#'):
                continue
            elif line.startswith('component;'):
                continue
            elif line.startswith('line;'):
                item = Line()
            elif line.startswith('arc;'):
                item = Arc()
            elif line.startswith('rect;'):
                item = Rect()
            elif line.startswith('ellips;'):
                item = Ellipse()
            elif line.startswith('port;'):
                item = Port()
            elif line.startswith('text;'):
                item = Text()
            else:
                raise Exception('Unsupported primitive: {:s}'.format(l))
            item.fromCsv(line.split(';'))
            self.items.append(item)
                      
f = open(sys.argv[1])
lines = f.readlines()
caption = lines[1].split(";")[1].strip()
description = lines[2].split(";")[1].strip()

f = open(sys.argv[2])
lines = f.readlines()

sym = Symbol(caption, description)
sym.fromCsv(lines)
sym.toXml()
sys.exit(0)
