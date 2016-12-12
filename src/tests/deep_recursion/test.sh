#!/bin/bash

. ../funcs.sh

readas user1 /etc/dir/bar succeed
writeas user1 /etc/dir/bar succeed
readas user1 /etc/dir/foo/dv3 succeed
writeas user1 /etc/dir/foo/dv3 fail
readas user1 /test/bot/tom fail
writeas user1 /test/bot/tom fail
readas user2 /test/bot/tom succeed
writeas user2 /test/bot/tom succeed
readas user2 /test/bot/top/per succeed
writeas user2 /test/bot/top/per succeed
readas user3 /var/bool/ean succeed
writeas user3 /var/bool/ean fail
readas user3 /var/int/eger succeed
writeas user3 /var/int/eger succeed
readas user3 /var/int/float succeed
writeas user3 /var/int/float succeed
