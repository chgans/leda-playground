= Good architecture, Grail Quest vs reality ==

This playground is part of a series dealing with finding a good document object model for LibreEDA.

== Document centric design ==

- Any design modification has to be done through the document model
- Data exists only in one place: the document model
- Reapeatability, through Undo/Redo of document operations executed through the document model itself
- Data: Using the Qt's model/view framework (http://doc.qt.io/qt-5/model-view-programming.html)
- Graphical tools: Using Qt's Graphics View Framework (http://doc.qt.io/qt-5/graphicsview.html)
- But decoupled: Using TASKMASTER architecture pattern (http://www.objectmentor.com/resources/articles/taskmast.pdf)
- Still allow for non-graphical document too (generated or user written)

Extra:
- Collaborative work (local or distant)

== Document Object Model ==

In a document, objects are property aggregations. There are 4 categories of properties:
- Simple property: Properties which type and value can be easily manipulated through QVariant
- List property: A list of properties values, all of the same type
- Object property: Aggregation of more properties: recursion of Document Object
- Object link/reference: A link to an object within the document model or from another document model

Notes:
- make it easy to use TreeView, TableView and ListView
- TBD: QObject vs QStandardItem vs custom Item
- TBD: Tree model vs database model
- Application/Graphical/User properties

== Data Storage ==

- Decoupling Document Model from storage (local disk, cloud, database, ...)
- Document can be read from and written to different file format.
- Concept of reader/writer, formatter, import/export, ...

== Other interesting approaches ==

=== FreeCAD ===

CAD has different needs, FreeCAD uses a concept of feature that can be executed.

TODO: quick explanation of features and execution

