#!/bin/bash

. ../funcs.sh

readas user1 /file1.txt fail
writeas user1 /file1.txt fail
readas user1 /file2.txt fail
writeas user1 /file2.txt fail
readas user1 /dev/dv0 fail
writeas user1 /dev/dv0 fail
readas user1 /temp/tp2 fail
writeas user1 /temp/tp2 fail
readas user1 /boot/config.txt fail
writeas user1 /boot/config.txt fail

readas user2 /file1.txt fail
writeas user2 /file1.txt fail
readas user2 /file2.txt fail
writeas user2 /file2.txt fail
readas user2 /dev/dv0 fail
writeas user2 /dev/dv0 fail
readas user2 /temp/tp2 fail
writeas user2 /temp/tp2 fail
readas user2 /boot/config.txt fail
writeas user2 /boot/config.txt fail



readas user3 /file1.txt fail
writeas user3 /file1.txt fail
readas user3 /file2.txt fail
writeas user3 /file2.txt fail
readas user3 /dev/dv0 fail
writeas user3 /dev/dv0 fail
readas user3 /temp/tp2 fail
writeas user3 /temp/tp2 fail
readas user3 /boot/config.txt fail
writeas user3 /boot/config.txt fail
