

== User stories ==

=== 20150215 ===

Given a simple JSON document file (contains 1 object, with 1 property), I want to be able to explore modify review my modifications and undo/redo them.

The GUI should be made of 3 widgets aligned horizontally:
* Document model explorer (Domex):
  Shows a Tree of object

* Document object property browser (Dowser):
  List the properties of the object selected in the Domex

* Document change order (Docor):
  Shows a list of pending modifications, that i can accept, revert or cancel


I should be able to load and save one file per run
