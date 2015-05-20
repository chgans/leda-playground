schema=leda-xdl.xsd
files="models.xml symbols.xml netlist.xml schematic.xml simulation.xml"

echo "Validating $schema..."
xmllint --schema XMLSchema.xsd $schema >/dev/null

for file in $files; do
    echo "Validating $file..."
    xmllint --schema $schema $file > /dev/null
done
