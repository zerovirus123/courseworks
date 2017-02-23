lappend auto_path "C:/lscc/diamond/2.2_x64/data/script"
package require tbtemplate_generation

set ::bali::Para(MODNAME) RemoteControllerSchematic
set ::bali::Para(PACKAGE) {"C:/lscc/diamond/2.2_x64/cae_library/vhdl_packages/vdbs"}
set ::bali::Para(PRIMITIVEFILE) {"C:/lscc/diamond/2.2_x64/cae_library/synthesis/verilog/machxo2.v"}
set ::bali::Para(TFT) {"C:/lscc/diamond/2.2_x64/data/templates/plsitft.tft"}
set ::bali::Para(OUTNAME) RemoteControllerSchematic_tf
set ::bali::Para(EXT) .v
set ::bali::Para(FILELIST) {"Z:/Windows.Documents/Desktop/ECE272 Lab2/ece272_lab2/RemoteControllerSchematic.v=work" }
set ::bali::Para(INCLUDEPATH) {"Z:/Windows.Documents/Desktop/ECE272 Lab2/ece272_lab2" }
puts "set parameters done"
::bali::GenerateTbTemplate
