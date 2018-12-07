top_down: reg_alloc.c merge_sort.c list.c
	gcc reg_alloc.c merge_sort.c list.c -o top_down

clean: 
	rm -rf  top_down top_dwn_reg_alloc_report.txt

