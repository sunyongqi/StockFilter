#***************************************************************************
#                                  _   _ ____  _
#  Project                     ___| | | |  _ \| |
#                             / __| | | | |_) | |
#                            | (__| |_| |  _ <| |___
#                             \___|\___/|_| \_\_____|
#
# Copyright (C) 1998 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.
#
# This software is licensed as described in the file COPYING, which
# you should have received as part of this distribution. The terms
# are also available at http://curl.haxx.se/docs/copyright.html.
#
# You may opt to use, copy, modify, merge, publish, distribute and/or sell
# copies of the Software, and permit persons to whom the Software is
# furnished to do so, under the terms of the COPYING file.
#
# This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
# KIND, either express or implied.
#
###########################################################################
ZSH_FUNCTIONS_DIR = @ZSH_FUNCTIONS_DIR@
PERL = @PERL@

ZSH_COMPLETION_FUNCTION_FILENAME = _curl

all-local: $(ZSH_COMPLETION_FUNCTION_FILENAME)

$(ZSH_COMPLETION_FUNCTION_FILENAME): zsh.pl
if CROSSCOMPILING
	@echo "NOTICE: we can't generate zsh completion when cross-compiling!"
else # if not cross-compiling:
	@if ! test -x "$(PERL)"; then echo "No perl: can't install zsh.pl"; exit 0; fi
	$(PERL) $(srcdir)/zsh.pl $(top_builddir)/src/curl > $@
endif

install-data-local:
if CROSSCOMPILING
	@echo "NOTICE: we can't install zsh completion when cross-compiling!"
else # if not cross-compiling:
	$(MKDIR_P) $(DESTDIR)$(ZSH_FUNCTIONS_DIR)
	$(INSTALL_DATA) $(ZSH_COMPLETION_FUNCTION_FILENAME) $(DESTDIR)$(ZSH_FUNCTIONS_DIR)/$(ZSH_COMPLETION_FUNCTION_FILENAME)
endif
