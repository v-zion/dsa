# Cache Timing using DSA Algorithm
### Team Members
<ul>
	<li>Animesh Bohara</li>
	<li>Ankit</li>
	<li>Anmol Singh</li>
	<li>Eashan Gupta</li>
	<li>Shourya Pandey</li>
</ul>

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

### Resources
<ol>
	<li>Cache-Timing Techniques: Exploiting the DSA Algorithm - https://aaltodoc.aalto.fi/bitstream/handle/123456789/21579/master_Pereida_Garcia_Cesar_2016.pdf?sequence=2&isAllowed=y</li>
	<li>Mastik - https://cs.adelaide.edu.au/~yval/Mastik/?fbclid=IwAR2hGZZjn1zFULJPkjfsrJZlfN23pcYQg1M1I-gShnoQdaN5BzinsMM4Ow0</li>
	<li>Mastik Tutorial - https://cs.adelaide.edu.au/~yval/CHES16/?fbclid=IwAR0iWFJXs7-JhOXMH_jTsQTzrQ91VOMt4OoFbN3pgbzT-Ha5Uy0dwV7-S1k</li>
	<li>Flush Reload Attacks (Github Repo) - https://github.com/defuse/flush-reload-attacks</li>
	<li>Side Channel Attack Tutorial - https://cryptologie.net/article/367/ches-2016-tutorial-part-2-micro-architectural-side-channel-attacks/?fbclid=IwAR0Kyk1BpvkNUbZBGlAa5Vtc2fWj8C1ydHcgFd25CbGGglafSsp1nMu7XH8</li>
</ol>