# pod/Makefile
# This file is derived from pod/Makefile.SH.  Any changes made here will
# be lost the next time you run Configure.

POD = perl5004delta.pod perl5005delta.pod perl5100delta.pod perl5101delta.pod perl5110delta.pod perl5111delta.pod perl5112delta.pod perl5113delta.pod perl5114delta.pod perl5115delta.pod perl5120delta.pod perl5121delta.pod perl5122delta.pod perl5123delta.pod perl5130delta.pod perl51310delta.pod perl51311delta.pod perl5131delta.pod perl5132delta.pod perl5133delta.pod perl5134delta.pod perl5135delta.pod perl5136delta.pod perl5137delta.pod perl5138delta.pod perl5139delta.pod perl5140delta.pod perl5141delta.pod perl5142delta.pod perl561delta.pod perl56delta.pod perl570delta.pod perl571delta.pod perl572delta.pod perl573delta.pod perl581delta.pod perl582delta.pod perl583delta.pod perl584delta.pod perl585delta.pod perl586delta.pod perl587delta.pod perl588delta.pod perl589delta.pod perl58delta.pod perl590delta.pod perl591delta.pod perl592delta.pod perl593delta.pod perl594delta.pod perl595delta.pod perlaix.pod perlamiga.pod perlapio.pod perlartistic.pod perlbeos.pod perlbook.pod perlboot.pod perlbot.pod perlbs2000.pod perlcall.pod perlce.pod perlcheat.pod perlclib.pod perlcn.pod perlcommunity.pod perlcompile.pod perlcygwin.pod perldata.pod perldbmfilter.pod perldebguts.pod perldebtut.pod perldebug.pod perldelta.pod perldgux.pod perldiag.pod perldoc.pod perldos.pod perldsc.pod perlebcdic.pod perlembed.pod perlepoc.pod perlfaq1.pod perlfaq2.pod perlfaq3.pod perlfaq4.pod perlfaq5.pod perlfaq6.pod perlfaq7.pod perlfaq8.pod perlfaq9.pod perlfaq.pod perlfilter.pod perlfork.pod perlform.pod perlfreebsd.pod perlfunc.pod perlgit.pod perlglossary.pod perlgpl.pod perlguts.pod perlhack.pod perlhacktips.pod perlhacktut.pod perlhaiku.pod perlhist.pod perlhpux.pod perlhurd.pod perlinterp.pod perlintro.pod perliol.pod perlipc.pod perlirix.pod perljp.pod perlko.pod perllexwarn.pod perllinux.pod perllocale.pod perllol.pod perlmacos.pod perlmacosx.pod perlmodinstall.pod perlmod.pod perlmodstyle.pod perlmpeix.pod perlmroapi.pod perlnetware.pod perlnewmod.pod perlnumber.pod perlobj.pod perlopenbsd.pod perlopentut.pod perlop.pod perlos2.pod perlos390.pod perlos400.pod perlpacktut.pod perlperf.pod perlplan9.pod perl.pod perlpod.pod perlpodspec.pod perlpodstyle.pod perlpolicy.pod perlport.pod perlpragma.pod perlqnx.pod perlreapi.pod perlrebackslash.pod perlrecharclass.pod perlref.pod perlreftut.pod perlreguts.pod perlre.pod perlrequick.pod perlreref.pod perlretut.pod perlriscos.pod perlrun.pod perlsec.pod perlsolaris.pod perlsource.pod perlstyle.pod perlsub.pod perlsymbian.pod perlsyn.pod perlthrtut.pod perltie.pod perltodo.pod perltooc.pod perltoot.pod perltrap.pod perltru64.pod perltw.pod perlunicode.pod perlunifaq.pod perluniintro.pod perlunitut.pod perlutil.pod perluts.pod perlvar.pod perlvmesa.pod perlvms.pod perlvos.pod perlwin32.pod perlxs.pod perlxstut.pod

MAN = perl5004delta.man perl5005delta.man perl5100delta.man perl5101delta.man perl5110delta.man perl5111delta.man perl5112delta.man perl5113delta.man perl5114delta.man perl5115delta.man perl5120delta.man perl5121delta.man perl5122delta.man perl5123delta.man perl5130delta.man perl51310delta.man perl51311delta.man perl5131delta.man perl5132delta.man perl5133delta.man perl5134delta.man perl5135delta.man perl5136delta.man perl5137delta.man perl5138delta.man perl5139delta.man perl5140delta.man perl5141delta.man perl5142delta.man perl561delta.man perl56delta.man perl570delta.man perl571delta.man perl572delta.man perl573delta.man perl581delta.man perl582delta.man perl583delta.man perl584delta.man perl585delta.man perl586delta.man perl587delta.man perl588delta.man perl589delta.man perl58delta.man perl590delta.man perl591delta.man perl592delta.man perl593delta.man perl594delta.man perl595delta.man perlaix.man perlamiga.man perlapio.man perlartistic.man perlbeos.man perlbook.man perlboot.man perlbot.man perlbs2000.man perlcall.man perlce.man perlcheat.man perlclib.man perlcn.man perlcommunity.man perlcompile.man perlcygwin.man perldata.man perldbmfilter.man perldebguts.man perldebtut.man perldebug.man perldelta.man perldgux.man perldiag.man perldoc.man perldos.man perldsc.man perlebcdic.man perlembed.man perlepoc.man perlfaq1.man perlfaq2.man perlfaq3.man perlfaq4.man perlfaq5.man perlfaq6.man perlfaq7.man perlfaq8.man perlfaq9.man perlfaq.man perlfilter.man perlfork.man perlform.man perlfreebsd.man perlfunc.man perlgit.man perlglossary.man perlgpl.man perlguts.man perlhack.man perlhacktips.man perlhacktut.man perlhaiku.man perlhist.man perlhpux.man perlhurd.man perlinterp.man perlintro.man perliol.man perlipc.man perlirix.man perljp.man perlko.man perllexwarn.man perllinux.man perllocale.man perllol.man perlmacos.man perlmacosx.man perlmodinstall.man perlmod.man perlmodstyle.man perlmpeix.man perlmroapi.man perlnetware.man perlnewmod.man perlnumber.man perlobj.man perlopenbsd.man perlopentut.man perlop.man perlos2.man perlos390.man perlos400.man perlpacktut.man perlperf.man perlplan9.man perl.man perlpod.man perlpodspec.man perlpodstyle.man perlpolicy.man perlport.man perlpragma.man perlqnx.man perlreapi.man perlrebackslash.man perlrecharclass.man perlref.man perlreftut.man perlreguts.man perlre.man perlrequick.man perlreref.man perlretut.man perlriscos.man perlrun.man perlsec.man perlsolaris.man perlsource.man perlstyle.man perlsub.man perlsymbian.man perlsyn.man perlthrtut.man perltie.man perltodo.man perltooc.man perltoot.man perltrap.man perltru64.man perltw.man perlunicode.man perlunifaq.man perluniintro.man perlunitut.man perlutil.man perluts.man perlvar.man perlvmesa.man perlvms.man perlvos.man perlwin32.man perlxs.man perlxstut.man

# no perltoc.html
HTML = perl5004delta.html perl5005delta.html perl5100delta.html perl5101delta.html perl5110delta.html perl5111delta.html perl5112delta.html perl5113delta.html perl5114delta.html perl5115delta.html perl5120delta.html perl5121delta.html perl5122delta.html perl5123delta.html perl5130delta.html perl51310delta.html perl51311delta.html perl5131delta.html perl5132delta.html perl5133delta.html perl5134delta.html perl5135delta.html perl5136delta.html perl5137delta.html perl5138delta.html perl5139delta.html perl5140delta.html perl5141delta.html perl5142delta.html perl561delta.html perl56delta.html perl570delta.html perl571delta.html perl572delta.html perl573delta.html perl581delta.html perl582delta.html perl583delta.html perl584delta.html perl585delta.html perl586delta.html perl587delta.html perl588delta.html perl589delta.html perl58delta.html perl590delta.html perl591delta.html perl592delta.html perl593delta.html perl594delta.html perl595delta.html perlaix.html perlamiga.html perlapio.html perlartistic.html perlbeos.html perlbook.html perlboot.html perlbot.html perlbs2000.html perlcall.html perlce.html perlcheat.html perlclib.html perlcn.html perlcommunity.html perlcompile.html perlcygwin.html perldata.html perldbmfilter.html perldebguts.html perldebtut.html perldebug.html perldelta.html perldgux.html perldiag.html perldoc.html perldos.html perldsc.html perlebcdic.html perlembed.html perlepoc.html perlfaq1.html perlfaq2.html perlfaq3.html perlfaq4.html perlfaq5.html perlfaq6.html perlfaq7.html perlfaq8.html perlfaq9.html perlfaq.html perlfilter.html perlfork.html perlform.html perlfreebsd.html perlfunc.html perlgit.html perlglossary.html perlgpl.html perlguts.html perlhack.html perlhacktips.html perlhacktut.html perlhaiku.html perlhist.html perlhpux.html perlhurd.html perlinterp.html perlintro.html perliol.html perlipc.html perlirix.html perljp.html perlko.html perllexwarn.html perllinux.html perllocale.html perllol.html perlmacos.html perlmacosx.html perlmodinstall.html perlmod.html perlmodstyle.html perlmpeix.html perlmroapi.html perlnetware.html perlnewmod.html perlnumber.html perlobj.html perlopenbsd.html perlopentut.html perlop.html perlos2.html perlos390.html perlos400.html perlpacktut.html perlperf.html perlplan9.html perl.html perlpod.html perlpodspec.html perlpodstyle.html perlpolicy.html perlport.html perlpragma.html perlqnx.html perlreapi.html perlrebackslash.html perlrecharclass.html perlref.html perlreftut.html perlreguts.html perlre.html perlrequick.html perlreref.html perlretut.html perlriscos.html perlrun.html perlsec.html perlsolaris.html perlsource.html perlstyle.html perlsub.html perlsymbian.html perlsyn.html perlthrtut.html perltie.html perltodo.html perltooc.html perltoot.html perltrap.html perltru64.html perltw.html perlunicode.html perlunifaq.html perluniintro.html perlunitut.html perlutil.html perluts.html perlvar.html perlvmesa.html perlvms.html perlvos.html perlwin32.html perlxs.html perlxstut.html

TEX  = perl5004delta.tex perl5005delta.tex perl5100delta.tex perl5101delta.tex perl5110delta.tex perl5111delta.tex perl5112delta.tex perl5113delta.tex perl5114delta.tex perl5115delta.tex perl5120delta.tex perl5121delta.tex perl5122delta.tex perl5123delta.tex perl5130delta.tex perl51310delta.tex perl51311delta.tex perl5131delta.tex perl5132delta.tex perl5133delta.tex perl5134delta.tex perl5135delta.tex perl5136delta.tex perl5137delta.tex perl5138delta.tex perl5139delta.tex perl5140delta.tex perl5141delta.tex perl5142delta.tex perl561delta.tex perl56delta.tex perl570delta.tex perl571delta.tex perl572delta.tex perl573delta.tex perl581delta.tex perl582delta.tex perl583delta.tex perl584delta.tex perl585delta.tex perl586delta.tex perl587delta.tex perl588delta.tex perl589delta.tex perl58delta.tex perl590delta.tex perl591delta.tex perl592delta.tex perl593delta.tex perl594delta.tex perl595delta.tex perlaix.tex perlamiga.tex perlapio.tex perlartistic.tex perlbeos.tex perlbook.tex perlboot.tex perlbot.tex perlbs2000.tex perlcall.tex perlce.tex perlcheat.tex perlclib.tex perlcn.tex perlcommunity.tex perlcompile.tex perlcygwin.tex perldata.tex perldbmfilter.tex perldebguts.tex perldebtut.tex perldebug.tex perldelta.tex perldgux.tex perldiag.tex perldoc.tex perldos.tex perldsc.tex perlebcdic.tex perlembed.tex perlepoc.tex perlfaq1.tex perlfaq2.tex perlfaq3.tex perlfaq4.tex perlfaq5.tex perlfaq6.tex perlfaq7.tex perlfaq8.tex perlfaq9.tex perlfaq.tex perlfilter.tex perlfork.tex perlform.tex perlfreebsd.tex perlfunc.tex perlgit.tex perlglossary.tex perlgpl.tex perlguts.tex perlhack.tex perlhacktips.tex perlhacktut.tex perlhaiku.tex perlhist.tex perlhpux.tex perlhurd.tex perlinterp.tex perlintro.tex perliol.tex perlipc.tex perlirix.tex perljp.tex perlko.tex perllexwarn.tex perllinux.tex perllocale.tex perllol.tex perlmacos.tex perlmacosx.tex perlmodinstall.tex perlmod.tex perlmodstyle.tex perlmpeix.tex perlmroapi.tex perlnetware.tex perlnewmod.tex perlnumber.tex perlobj.tex perlopenbsd.tex perlopentut.tex perlop.tex perlos2.tex perlos390.tex perlos400.tex perlpacktut.tex perlperf.tex perlplan9.tex perl.tex perlpod.tex perlpodspec.tex perlpodstyle.tex perlpolicy.tex perlport.tex perlpragma.tex perlqnx.tex perlreapi.tex perlrebackslash.tex perlrecharclass.tex perlref.tex perlreftut.tex perlreguts.tex perlre.tex perlrequick.tex perlreref.tex perlretut.tex perlriscos.tex perlrun.tex perlsec.tex perlsolaris.tex perlsource.tex perlstyle.tex perlsub.tex perlsymbian.tex perlsyn.tex perlthrtut.tex perltie.tex perltodo.tex perltooc.tex perltoot.tex perltrap.tex perltru64.tex perltw.tex perlunicode.tex perlunifaq.tex perluniintro.tex perlunitut.tex perlutil.tex perluts.tex perlvar.tex perlvmesa.tex perlvms.tex perlvos.tex perlwin32.tex perlxs.tex perlxstut.tex  

# The following is used to include the current directory in
# the dynamic loader path you are building a shared libperl.
LDLIBPTH = 


HTMLROOT = /	# Change this to fix cross-references in HTML
POD2HTML_ARGS = --htmlroot=$(HTMLROOT) \
	    --podroot=.. --podpath=pod:lib:ext:vms \
	    --libpods=perlfunc:perlguts:perlvar:perlrun:perlop
POD2HTML = ../ext/Pod-Html/pod2html
POD2MAN = ../cpan/podlators/pod2man
POD2LATEX = ../cpan/Pod-LaTeX/pod2latex
PODCHECKER =  ../cpan/Pod-Parser/podchecker

PERL = ../miniperl
PERLILIB = $(PERL) -I../lib
REALPERL = ../perl

all: man

man:	$(POD2MAN) $(MAN)

html:	$(POD2HTML) $(HTML)

tex:	$(POD2LATEX) $(TEX)

toc perltoc.pod:	buildtoc
	$(PERLILIB) buildtoc --build-toc

.SUFFIXES: .pm .pod

.SUFFIXES: .man

.pm.man:     $(POD2MAN)
	$(REALPERL) -I../lib $(POD2MAN) $*.pm >$*.man

.pod.man:     $(POD2MAN)
	$(REALPERL) -I../lib $(POD2MAN) $*.pod >$*.man

.SUFFIXES: .html

.pm.html:    $(POD2HTML)
	$(PERL) -I../lib $(POD2HTML) $(POD2HTML_ARGS) --infile=$*.pm --outfile=$*.html

.pod.html:    $(POD2HTML)
	$(PERL) -I../lib $(POD2HTML) $(POD2HTML_ARGS) --infile=$*.pod --outfile=$*.html

.SUFFIXES: .tex

.pm.tex: $(POD2LATEX)
	$(PERL) -I../lib $(POD2LATEX) $*.pm

.pod.tex: $(POD2LATEX)
	$(PERL) -I../lib $(POD2LATEX) $*.pod

clean:
	rm -f $(MAN)
	rm -f $(HTML)
	rm -f $(TEX)
	rm -f pod2html-*cache
	rm -f *.aux *.log *.exe

realclean:	clean

distclean:	realclean

veryclean:	distclean
	-rm -f *~ *.orig

check:	$(PODCHECKER)
	@echo "checking..."; \
	$(PERL) -I../lib $(PODCHECKER) $(POD)
