GTU:
	bison -d .\gpp_parser.y
	flex .\gpp_lexer.l
	gcc lex.yy.c gpp_parser.tab.c -o deneme
	./deneme.exe