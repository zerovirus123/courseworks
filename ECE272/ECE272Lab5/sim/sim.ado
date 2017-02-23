setenv SIM_WORKING_FOLDER .
set newDesign 0
if {![file exists "Z:/Windows.Documents/Desktop/ECE272Lab5/sim/sim.adf"]} { 
	design create sim "Z:/Windows.Documents/Desktop/ECE272Lab5"
  set newDesign 1
}
design open "Z:/Windows.Documents/Desktop/ECE272Lab5/sim"
cd "Z:/Windows.Documents/Desktop/ECE272Lab5"
designverincludedir -clear
designverlibrarysim -PL -clear
designverlibrarysim -L -clear
designverlibrarysim -PL pmi_work
designverlibrarysim ovi_machxo2
designverdefinemacro -clear
if {$newDesign == 0} { 
  removefile -Y -D *
}
addfile "Z:/Windows.Documents/Desktop/ECE272Lab5/digital_analog.v"
	vlib "Z:/Windows.Documents/Desktop/ECE272Lab5/sim/work"
set worklib work
adel -all
vlog -dbg "Z:/Windows.Documents/Desktop/ECE272Lab5/digital_analog.v"
module DigitalAnalog
vsim +access +r DigitalAnalog   -PL pmi_work -L ovi_machxo2
add wave *
run 1000ns
