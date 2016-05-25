# Namiastka systemu NFS
Kompilacja:

`rpcgen nfs.x`

`make -f Makefile.nfs`

Uruchamianie serwera:

`./nfs_server`

Uruchamianie klienta:

`./nfs_client adres_serwera`

Plik *nfs_client.c* zawiera przykładowe wykorzystanie funkcji operujących na plikach znajdujących się na zdalnym serwerze.
W pliku *client_interface.c* znajdują się klienckie funkcje wywołujące zdalne procedury operujące na plikach.
Implementacja funkcji serwera znajduje się w pliku *nfs_server.c*
