redirect_rewrite: redirect_rewrite.c urldecode.c
	gcc -o redirect_rewrite -Os redirect_rewrite.c urldecode.c -I.
clean:
	-$(RM) redirect_rewrite
	-@echo 'Cleanup complete'
