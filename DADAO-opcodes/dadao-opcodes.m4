#	$1		$2	$3	$4	$5	$6	$7	$8	$9
#	name		op-type	maj-op	min-op	fa	fb	fc	fd	insn-type
divert(0)dnl

insn(	swym,		zzzz,	0x00,	0,	-,	-,	-,	-,	normal)

insn(	and,		orrr,	0x10,	0x08,	op,	rg,	rg,	rg,	normal)
insn(	orr,		orrr,	0x10,	0x09,	op,	rg,	rg,	rg,	normal)
insn(	xor,		orrr,	0x10,	0x0A,	op,	rg,	rg,	rg,	normal)

insn(	shlu,		orrr,	0x10,	0x11,	op,	rg,	rg,	rg,	normal)
insn(	shrs,		orrr,	0x10,	0x12,	op,	rg,	rg,	rg,	normal)
insn(	shru,		orrr,	0x10,	0x13,	op,	rg,	rg,	rg,	normal)
insn(	exts,		orrr,	0x10,	0x14,	op,	rg,	rg,	rg,	normal)
insn(	extz,		orrr,	0x10,	0x15,	op,	rg,	rg,	rg,	normal)
insn(	not,		orrr,	0x10,	0x16,	op,	rg,	rg,	rg,	normal)
insn(  _shlu,		orri,	0x10,	0x19,	op,	rg,	rg,	i6,	normal)
insn(  _shrs,		orri,	0x10,	0x1A,	op,	rg,	rg,	i6,	normal)
insn(  _shru,		orri,	0x10,	0x1B,	op,	rg,	rg,	i6,	normal)
insn(  _exts,		orri,	0x10,	0x1C,	op,	rg,	rg,	i6,	normal)
insn(  _extz,		orri,	0x10,	0x1D,	op,	rg,	rg,	i6,	normal)
insn(  _not,		orri,	0x10,	0x1E,	op,	rg,	rg,	i6,	normal)

insn(	cmps,		orrr,	0x10,	0x24,	op,	rg,	rg,	rg,	normal)
insn(	cmpu,		orrr,	0x10,	0x25,	op,	rg,	rg,	rg,	normal)

insn(	rg2rg,		orri,	0x10,	0x30,	op,	rg,	rg,	i6,	normal)
insn(	rg2rp,		orri,	0x10,	0x31,	op,	rp,	rg,	i6,	normal)
insn(	rg2rf,		orri,	0x10,	0x32,	op,	rf,	rg,	i6,	normal)
insn(	rp2rg,		orri,	0x10,	0x34,	op,	rg,	rp,	i6,	normal)
insn(	rp2rp,		orri,	0x10,	0x35,	op,	rp,	rp,	i6,	normal)
insn(	rf2rg,		orri,	0x10,	0x38,	op,	rg,	rf,	i6,	normal)
insn(	rf2rf,		orri,	0x10,	0x3A,	op,	rf,	rf,	i6,	normal)

insn(	add,		riii,	0x11,	0,	rg,	s18,	-,	-,	normal)
insn(  _add,		rrrr,	0x12,	0,	rg,	rg,	rg,	rg,	normal)
insn(	sub,		rrrr,	0x13,	0,	rg,	rg,	rg,	rg,	normal)
insn(	muls,		rrrr,	0x14,	0,	rg,	rg,	rg,	rg,	normal)
insn(	mulu,		rrrr,	0x15,	0,	rg,	rg,	rg,	rg,	normal)
insn(	divs,		rrrr,	0x16,	0,	rg,	rg,	rg,	rg,	normal)
insn(	divu,		rrrr,	0x17,	0,	rg,	rg,	rg,	rg,	normal)

insn(  _cmps,		rrii,	0x1A,	0,	rg,	rg,	s12,	-,	normal)
insn(  _cmpu,		rrii,	0x1B,	0,	rg,	rg,	u12,	-,	normal)

insn(	setwl,		rjii,	0x1C,	0,	rg,	w16,	-,	-,	normal)
insn(	setwk,		rjii,	0x1C,	1,	rg,	w16,	-,	-,	normal)
insn(	setwj,		rjii,	0x1C,	2,	rg,	w16,	-,	-,	normal)
insn(	setwh,		rjii,	0x1C,	3,	rg,	w16,	-,	-,	normal)
insn(	orwl,		rjii,	0x1D,	0,	rg,	w16,	-,	-,	normal)
insn(	orwk,		rjii,	0x1D,	1,	rg,	w16,	-,	-,	normal)
insn(	orwj,		rjii,	0x1D,	2,	rg,	w16,	-,	-,	normal)
insn(	orwh,		rjii,	0x1D,	3,	rg,	w16,	-,	-,	normal)
insn(	andnwl,		rjii,	0x1E,	0,	rg,	w16,	-,	-,	normal)
insn(	andnwk,		rjii,	0x1E,	1,	rg,	w16,	-,	-,	normal)
insn(	andnwj,		rjii,	0x1E,	2,	rg,	w16,	-,	-,	normal)
insn(	andnwh,		rjii,	0x1E,	3,	rg,	w16,	-,	-,	normal)
insn(	movzwl,		rjii,	0x1F,	0,	rg,	w16,	-,	-,	normal)
insn(	movzwk,		rjii,	0x1F,	1,	rg,	w16,	-,	-,	normal)
insn(	movzwj,		rjii,	0x1F,	2,	rg,	w16,	-,	-,	normal)
insn(	movzwh,		rjii,	0x1F,	3,	rg,	w16,	-,	-,	normal)

insn(	csn,		rrrr,	0x20,	0,	rg,	rg,	rg,	rg,	normal)
insn(	csnn,		rrrr,	0x21,	0,	rg,	rg,	rg,	rg,	normal)
insn(	csz,		rrrr,	0x22,	0,	rg,	rg,	rg,	rg,	normal)
insn(	csnz,		rrrr,	0x23,	0,	rg,	rg,	rg,	rg,	normal)
insn(	csp,		rrrr,	0x24,	0,	rg,	rg,	rg,	rg,	normal)
insn(	csnp,		rrrr,	0x25,	0,	rg,	rg,	rg,	rg,	normal)
insn(	csod,		rrrr,	0x26,	0,	rg,	rg,	rg,	rg,	normal)
insn(	csev,		rrrr,	0x27,	0,	rg,	rg,	rg,	rg,	normal)

insn(	brn,		riii,	0x28,	0,	rg,	s18,	-,	-,	condbranch)
insn(	brnn,		riii,	0x29,	0,	rg,	s18,	-,	-,	condbranch)
insn(	brz,		riii,	0x2A,	0,	rg,	s18,	-,	-,	condbranch)
insn(	brnz,		riii,	0x2B,	0,	rg,	s18,	-,	-,	condbranch)
insn(	brp,		riii,	0x2C,	0,	rg,	s18,	-,	-,	condbranch)
insn(	brnp,		riii,	0x2D,	0,	rg,	s18,	-,	-,	condbranch)
insn(	brod,		riii,	0x2E,	0,	rg,	s18,	-,	-,	condbranch)
insn(	brev,		riii,	0x2F,	0,	rg,	s18,	-,	-,	condbranch)

insn(	ldbs,		rrii,	0x30,	0,	rg,	rp,	s12,	-,	dref)
insn(	ldws,		rrii,	0x31,	0,	rg,	rp,	s12,	-,	dref)
insn(	ldts,		rrii,	0x32,	0,	rg,	rp,	s12,	-,	dref)
insn(	ldo,		rrii,	0x33,	0,	rg,	rp,	s12,	-,	dref)

insn(	ldmbs,		rrri,	0x34,	0,	rg,	rp,	rg,	i6,	dref)
insn(	ldmws,		rrri,	0x35,	0,	rg,	rp,	rg,	i6,	dref)
insn(	ldmts,		rrri,	0x36,	0,	rg,	rp,	rg,	i6,	dref)
insn(	ldmo,		rrri,	0x37,	0,	rg,	rp,	rg,	i6,	dref)

insn(	stb,		rrii,	0x38,	0,	rg,	rp,	s12,	-,	dref)
insn(	stw,		rrii,	0x39,	0,	rg,	rp,	s12,	-,	dref)
insn(	stt,		rrii,	0x3A,	0,	rg,	rp,	s12,	-,	dref)
insn(	sto,		rrii,	0x3B,	0,	rg,	rp,	s12,	-,	dref)

insn(	stmb,		rrri,	0x3C,	0,	rg,	rp,	rg,	i6,	dref)
insn(	stmw,		rrri,	0x3D,	0,	rg,	rp,	rg,	i6,	dref)
insn(	stmt,		rrri,	0x3E,	0,	rg,	rp,	rg,	i6,	dref)
insn(	stmo,		rrri,	0x3F,	0,	rg,	rp,	rg,	i6,	dref)

insn(	ldbu,		rrii,	0x40,	0,	rg,	rp,	s12,	-,	dref)
insn(	ldwu,		rrii,	0x41,	0,	rg,	rp,	s12,	-,	dref)
insn(	ldtu,		rrii,	0x42,	0,	rg,	rp,	s12,	-,	dref)
insn(	ldrp,		rrii,	0x43,	0,	rp,	rp,	s12,	-,	dref)

insn(	ldmbu,		rrri,	0x44,	0,	rg,	rp,	rg,	i6,	dref)
insn(	ldmwu,		rrri,	0x45,	0,	rg,	rp,	rg,	i6,	dref)
insn(	ldmtu,		rrri,	0x46,	0,	rg,	rp,	rg,	i6,	dref)
insn(	ldmrp,		rrri,	0x47,	0,	rp,	rp,	rg,	i6,	dref)

insn(	strp,		rrii,	0x4B,	0,	rp,	rp,	s12,	-,	dref)
insn(	stmrp,		rrri,	0x4F,	0,	rp,	rp,	rg,	i6,	dref)

insn(	ft2fo,		orri,	0x50,	0x01,	op,	rf,	rf,	i6,	normal)
insn(	fo2ft,		orri,	0x50,	0x04,	op,	rf,	rf,	i6,	normal)

insn(	ftadd,		orrr,	0x50,	0x10,	op,	rf,	rf,	rf,	normal)
insn(	ftsub,		orrr,	0x50,	0x11,	op,	rf,	rf,	rf,	normal)
insn(	ftmul,		orrr,	0x50,	0x12,	op,	rf,	rf,	rf,	normal)
insn(	ftdiv,		orrr,	0x50,	0x13,	op,	rf,	rf,	rf,	normal)

insn(	ftcun,		orrr,	0x50,	0x18,	op,	rg,	rf,	rf,	normal)
insn(	ftcor,		orrr,	0x50,	0x19,	op,	rg,	rf,	rf,	normal)
insn(	ftcne,		orrr,	0x50,	0x1A,	op,	rg,	rf,	rf,	normal)
insn(	ftceq,		orrr,	0x50,	0x1B,	op,	rg,	rf,	rf,	normal)
insn(	ftclt,		orrr,	0x50,	0x1C,	op,	rg,	rf,	rf,	normal)
insn(	ftcge,		orrr,	0x50,	0x1D,	op,	rg,	rf,	rf,	normal)
insn(	ftcgt,		orrr,	0x50,	0x1E,	op,	rg,	rf,	rf,	normal)
insn(	ftcle,		orrr,	0x50,	0x1F,	op,	rg,	rf,	rf,	normal)

insn(	foadd,		orrr,	0x50,	0x20,	op,	rf,	rf,	rf,	normal)
insn(	fosub,		orrr,	0x50,	0x21,	op,	rf,	rf,	rf,	normal)
insn(	fomul,		orrr,	0x50,	0x22,	op,	rf,	rf,	rf,	normal)
insn(	fodiv,		orrr,	0x50,	0x23,	op,	rf,	rf,	rf,	normal)

insn(	focun,		orrr,	0x50,	0x28,	op,	rg,	rf,	rf,	normal)
insn(	focor,		orrr,	0x50,	0x29,	op,	rg,	rf,	rf,	normal)
insn(	focne,		orrr,	0x50,	0x2A,	op,	rg,	rf,	rf,	normal)
insn(	foceq,		orrr,	0x50,	0x2B,	op,	rg,	rf,	rf,	normal)
insn(	foclt,		orrr,	0x50,	0x2C,	op,	rg,	rf,	rf,	normal)
insn(	focge,		orrr,	0x50,	0x2D,	op,	rg,	rf,	rf,	normal)
insn(	focgt,		orrr,	0x50,	0x2E,	op,	rg,	rf,	rf,	normal)
insn(	focle,		orrr,	0x50,	0x2F,	op,	rg,	rf,	rf,	normal)

insn(	ldft,		rrii,	0x52,	0,	rf,	rp,	s12,	-,	dref)
insn(	ldfo,		rrii,	0x53,	0,	rf,	rp,	s12,	-,	dref)
insn(	ldmft,		rrri,	0x56,	0,	rf,	rp,	rg,	i6,	dref)
insn(	ldmfo,		rrri,	0x57,	0,	rf,	rp,	rg,	i6,	dref)

insn(	stft,		rrii,	0x5A,	0,	rf,	rp,	s12,	-,	dref)
insn(	stfo,		rrii,	0x5B,	0,	rf,	rp,	s12,	-,	dref)
insn(	stmft,		rrri,	0x5E,	0,	rf,	rp,	rg,	i6,	dref)
insn(	stmfo,		rrri,	0x5F,	0,	rf,	rp,	rg,	i6,	dref)

insn(	jump,		iiii,	0x64,	0,	s24,	-,	-,	-,	branch)
insn(  _jump,		rrii,	0x65,	0,	rg,	rp,	s12,	-,	branch)

insn(	ldmrr,		rrri,	0x67,	0,	rr,	rp,	rg,	i6,	dref)

insn(	call,		iiii,	0x6C,	0,	s24,	-,	-,	-,	jsr)
insn(  _call,		rrii,	0x6D,	0,	rg,	rp,	s12,	-,	jsr)

insn(	ret,		zzzz,	0x6E,	0,	-,	-,	-,	-,	jsr)
insn(	stmrr,		rrri,	0x6F,	0,	rr,	rp,	rg,	i6,	dref)

insn(	trap,		oiii,	0x70,	0x00,	op,	u18,	-,	-,	jsr)

