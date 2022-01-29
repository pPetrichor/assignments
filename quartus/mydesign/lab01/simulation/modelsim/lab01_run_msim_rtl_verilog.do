transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/quarters/mydesign/lab01 {D:/quarters/mydesign/lab01/lab01.v}

vlog -vlog01compat -work work +incdir+D:/quarters/mydesign/lab01/simulation/modelsim {D:/quarters/mydesign/lab01/simulation/modelsim/lab01.vt}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  lab01_vlg_tst

add wave *
view structure
view signals
run -all
