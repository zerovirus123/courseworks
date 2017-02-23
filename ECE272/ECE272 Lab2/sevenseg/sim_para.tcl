lappend auto_path "C:/lscc/diamond/2.2_x64/data/script"
package require simulation_generation
set ::bali::simulation::Para(PROJECT) {sevenseg}
set ::bali::simulation::Para(PROJECTPATH) {Z:/Windows.Documents/Desktop/ECE272 Lab2}
set ::bali::simulation::Para(FILELIST) {"Z:/Windows.Documents/Desktop/ECE272 Lab2/ECE272_Lab3/lab3_schematic.v" }
set ::bali::simulation::Para(GLBINCLIST) {}
set ::bali::simulation::Para(INCLIST) {"none"}
set ::bali::simulation::Para(WORKLIBLIST) {"" }
set ::bali::simulation::Para(COMPLIST) {"VERILOG" }
set ::bali::simulation::Para(SIMLIBLIST) {pmi_work ovi_machxo2}
set ::bali::simulation::Para(MACROLIST) {}
set ::bali::simulation::Para(SIMULATIONTOPMODULE) {lab3_schematic}
set ::bali::simulation::Para(SIMULATIONINSTANCE) {}
set ::bali::simulation::Para(LANGUAGE) {VERILOG}
set ::bali::simulation::Para(SDFPATH)  {}
set ::bali::simulation::Para(ADDTOPLEVELSIGNALSTOWAVEFORM)  {1}
set ::bali::simulation::Para(RUNSIMULATION)  {1}
set ::bali::simulation::Para(POJO2LIBREFRESH)    {}
set ::bali::simulation::Para(POJO2MODELSIMLIB)   {}
::bali::simulation::ActiveHDL_Run
