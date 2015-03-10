
- get rid of iteminterface:
 - make a GraphicsObjetc based QGraphicsObject
 - don't try to reuse Qt line, rect, etc, ... implement our own
 - for complex one (Svg, Text & widget) use a parent wrapper
- document objects:
 - QObject based
- GraphicsObjects monitor DocumentGraphicsObjects
