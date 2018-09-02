# PASS
	move.4 4(a0)++,8(a1)++
	move.4 (a2)8++,(a3)12++
# FAIL
	move.4 4(a1)++,8(a1)++
	move.4 (a2)8++,(a2)12++
