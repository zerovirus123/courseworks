setenv SIM_WORKING_FOLDER .
set newDesign 0
if {![file exists "Z:/Windows.Documents/Desktop/ECE272Lab4/smart_remote/smart_remote.adf"]} { 
	design create smart_remote "Z:/Windows.Documents/Desktop/ECE272Lab4"
  set newDesign 1
}
design open "Z:/Windows.Documents/Desktop/ECE272Lab4/smart_remote"
cd "Z:/Windows.Documents/Desktop/ECE272Lab4"
designverincludedir -clear
designverlibrarysim -PL -clear
designverlibrarysim -L -clear
designverlibrarysim -PL pmi_work
designverlibrarysim ovi_machxo2
designverdefinemacro -clear
if {$newDesign == 0} { 
  removefile -Y -D *
}
addfile "Z:/Windows.Documents/Desktop/ECE272Lab4/Lab4_Remote/RemoteControllerSchematic.v"
addfile "Z:/Windows.Documents/Desktop/ECE272Lab4/Smart_Remote.v"
	vlib "Z:/Windows.Documents/Desktop/ECE272Lab4/smart_remote/work"
set worklib work
adel -all
vlog -dbg "Z:/Windows.Documents/Desktop/ECE272Lab4/Lab4_Remote/RemoteControllerSchematic.v"
vlog -dbg "Z:/Windows.Documents/Desktop/ECE272Lab4/Smart_Remote.v"
module SmartRemote
vsim +access +r SmartRemote   -PL pmi_work -L ovi_machxo2
add wave *
run 1000ns
