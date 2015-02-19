function func() {

}

/*
 * Models:
 * - graphical/drawing
 * - electrical/netlist
 * - signal integrity
 * - PCB (footprint land pattern)
 * - 2D/3D model
 * - simulation/spice
 * - Documentation, Manufacturer, provider
 */

parameter = {
    name, visible, locked,
    value, visible, locked,
    x, y, orientation, color, font, locked,
    autopos, justification,
    type /* string, bool, int, float */,
    /* + sync to db/lib, ... */
}

pin = {
    /* data model */
    label: "vcc", /* + visible (aka display name) */
    designator: "1", /* + visible */
    electricalType: "passive", /* in, out, io, passive, power, hiz, opencol, openemit */
    description: "blah",
    hidden: "false",
    net: "vcc", /* if hidden = true */
    partNumber: 1, /* if multi-part component */

    /* graphical */
    // symbols
    insideSymbol: "schmitt", /* inside, insideEdge, outsideEdge, outside */
                             /* enum: IEEE symbols */
    // graphical
    x: 30,
    y: 0,
    length: 30,
    orientation: 0, /* 0, 90, 180, 270 */
    color: 0xFFDDEE, /* standard vs custom (See geda file format) */
    // customised for label and designator:
    // pos, margin, orientation to pin or component, local font

    /* dynamic parameters */
}

component {
    /* properties */
    defaultDesignator: "U?", /* + visible, locked */
    defaultComment: "*", /* + can reference a parameter using =<paramName>, + visible */
    partNumber: 1, /* if multi-part + locked */
    description: "blah",
    type: "standard",

    /* Library link */
    symbolRef: "TLV3201",

    /* Graphical */
    mode: "Normal", /* http://techdocs.altium.com/display/ADOH/Component,+Model+and+Library+Concepts#Component,ModelandLibraryConcepts-Non-standardComponentTypes */
    lockedPins: "true",
    showHiddenPins: "true",
    // localColors, ...

    /* dynamic parameters */
    // TBD: supplier, manufacturer, ...

    /* model list (footprint, 3D, simu, signal integrity) w/ pin mapping */

}



