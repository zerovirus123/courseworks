setenv SIM_WORKING_FOLDER .
set newDesign 0
if {![file exists "Z:/Windows.Documents/Desktop/ECE272 Lab2/sevenseg/sevenseg.adf"]} { 
	design create sevenseg "Z:/Windows.Documents/Desktop/ECE272 Lab2"
  set newDesign 1
}
design open "Z:/Windows.Documents/Desktop/ECE272 Lab2/sevenseg"
cd "Z:/Windows.Documents/Desktop/ECE272 Lab2"
designverincludedir -clear
designverlibrarysim -PL -clear
designverlibrarysim -L -clear
designverlibrarysim -PL pmi_work
designverlibrarysim ovi_machxo2
designverdefinemacro -clear
if {$newDesign == 0} { 
  removefile -Y -D *
}
addfile "Z:/Windows.Documents/Desktop/ECE272 Lab2/ECE272_Lab3/lab3_schematic.v"
	vlib "Z:/Windows.Documents/Desktop/ECE272 Lab2/sevenseg/work"
set worklib work
adel -all
vlog -dbg "Z:/Windows.Documents/Desktop/ECE272 Lab2/ECE272_Lab3/lab3_schematic.v"
module lab3_schematic
vsim +access +r lab3_schematic   -PL pmi_work -L ovi_machxo2
add wave *
run 1000ns
