transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+D:/quartus/mydesign/lab04_1_1 {D:/quartus/mydesign/lab04_1_1/lab04_1_1.v}

