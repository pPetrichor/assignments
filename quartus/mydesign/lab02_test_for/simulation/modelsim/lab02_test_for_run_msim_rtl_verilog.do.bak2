transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/quartus/mydesign/lab02_test_for {D:/quartus/mydesign/lab02_test_for/lab02_test_for.v}

vlog -vlog01compat -work work +incdir+D:/quartus/mydesign/lab02_test_for/simulation/modelsim {D:/quartus/mydesign/lab02_test_for/simulation/modelsim/lab02_test_for.vt}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  lab02_test_for_vlg_tst

add wave *
view structure
view signals
run -all
