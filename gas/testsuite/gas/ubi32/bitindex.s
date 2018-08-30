# Passing tests
	bset	d0,d1,#0
	bset	d0,d1,#5
	bset	d0,d1,#31
	bset	d0,d1,#%bit(0x8)
	bset	d0,d1,#%msbbit(0x12)
	bset	d0,d1,#%lsbbit(0x12)
# Failing tests
	bset	d0,d1,#-1
	bset	d0,d1,#33
	bset	d0,d1,#%bit(5)
	bset	d0,d1,#%bit(0)
	bset	d0,d1,#%bit(0x100000000)
	bset	d0,d1,#%msbbit(0)
	bset	d0,d1,#%lsbbit(0)
