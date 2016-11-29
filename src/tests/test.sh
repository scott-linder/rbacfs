#!/bin/bash

for dir in $(find . -maxdepth 1 -type d -not -name '.*'); do
	echo "[+] Running test $dir"
	cd $dir
	mkdir mount
	sudo rbacfs -o allow_other mount root rbac.defs
	./test.sh \
		&& echo -e "\e[0;32m[+] Test $dir passed\e[0m" \
		|| echo -e "\e[0;31m[-] Test $dir failed\e[0m";
	sudo fusermount -u mount
	rmdir mount
	cd ..
done
