
# add here all files
data_DATA = index.html index-1.html index-2.html index-3.html logotp3.gif

# this is the directory, where all datas are installed
# you have given the datas in data_DATA
datadir = $(kde_htmldir)/en/konquest

EXTRA_DIST = $(data_DATA) index.sgml index.nif

doc: index.sgml index.nif
	ksgml2html index en
