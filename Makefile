ABP: 
	gcc -g project2.c student2A_ABP.c student2B_ABP.c student_common_ABP.c -o ABP

GBN:
	gcc -g project2.c student2A_GBN.c student2B_GBN.c student_common_GBN.c -o GBN

clean:
	rm -f ABP GBN
