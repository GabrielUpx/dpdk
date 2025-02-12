Compile: 

gcc -o dpdk_sniffer dpdk_sniffer.c $(pkg-config --cflags --libs libdpdk)

Excecute o programa:

sudo ./dpdk_sniffer -- -l 0 -n 4 -- -p 0x1

Documentação:

https://doc.dpdk.org/guides/prog_guide/pdump_lib.html